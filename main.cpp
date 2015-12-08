#include <iostream>
#include <cstdint>
#include <memory>
#include "FileReader.h"
#include "FileWriter.h"
#include "Format.h"
#include "BMP.h"
#include "GK.h"

const static uint8_t BMP_PADDING = 0x00;
const char * const COMRPESS = "c";
const char * const DECOMRPESS = "d";
const char * const GRAY_SCALE = "s";
const static uint8_t METHOD_COMRPESS = 1;
const static uint8_t METHOD_DECOMRPESS = 2;
const static uint8_t ARGV_METHOD = 1;
const static uint8_t ARGV_PATH = 2;
const static uint8_t ARGV_OPTIONAL = 3;

int main(int argc, char **argv)
{
	if (!(argc == 3 || argc == 4))
	{
		std::cerr << "Usage:\n";
		std::cerr << "To compress: bmp c file s // parameter s can be optional\n";
		std::cerr << "To decompress: bmp d file\n";
		return 1;
	}

	uint8_t method = 0;
	bool grayScale = false;

	if (strcmp(argv[ARGV_METHOD], COMRPESS) == 0)
	{
		method = METHOD_COMRPESS;
	}
	else if (strcmp(argv[ARGV_METHOD], DECOMRPESS) == 0)
	{
		method = METHOD_DECOMRPESS;
	}

	if (method == 0)
	{
		std::cerr << "Unrecognized method\n";
		return 1;
	}

	if (method == METHOD_COMRPESS && argc == 4 && (strcmp(argv[ARGV_OPTIONAL], GRAY_SCALE) == 0))
	{
		grayScale = true;
	}

	std::unique_ptr<Format> format;
	auto reader = std::make_unique<FileReader>(argv[ARGV_PATH]);
	std::unique_ptr<FileWriter> writer;

	if (!reader->open())
	{
		std::cerr << "Cannot open a file\n";
		return 1;
	}

	if (method == METHOD_COMRPESS)
	{
		format = std::make_unique<BMP>(*reader);
	}
	else if (method == METHOD_DECOMRPESS)
	{
		format = std::make_unique<GK>(*reader);
	}

	if (!format->verify())
	{
		std::cerr << "File is corrupted or not supported\n";
		return 1;
	}

	if (method == METHOD_COMRPESS)
	{
		writer = std::make_unique<FileWriter>("compressed.bmp");
	}
	else if (method == METHOD_DECOMRPESS)
	{
		writer = std::make_unique<FileWriter>("decompressed.bmp");
	}

	if (!writer->create())
	{
		std::cerr << "Cannot create a file\n";
		return 1;
	}

	if (!writer->write<BITMAPFILEHEADER>(format->getBfh()))
	{
		std::cerr << "Cannot write to a file\n";
		return 1;
	}

	if (method == METHOD_COMRPESS)
	{
		format->getBih().biBitCount = format->BYTES;
	}
	else if (method == METHOD_DECOMRPESS)
	{
		format->getBih().biBitCount = 24;
	}

	if (!writer->write<BITMAPINFOHEADER>(format->getBih()))
	{
		std::cerr << "Cannot write to a file\n";
		return 1;
	}

	if (method == METHOD_COMRPESS)
	{
		uint8_t red[2] = { 0 }, green[2] = { 0 }, blue[2] = { 0 };
		for (uint32_t y = 0; y < format->getHeight(); y++)
		{
			for (uint32_t x = 0; x < format->getWidth(); x++)
			{
				if (x % 2 == 0)
				{
					red[0] = format->getRed(x, y);
					green[0] = format->getGreen(x, y);
					blue[0] = format->getBlue(x, y);

					if (grayScale)
					{
						uint8_t gray = (red[0] + green[0] + blue[0]) / 3;
						red[0] = gray;
						green[0] = gray;
						blue[0] = gray;
					}

					red[0] >>= format->BYTES;
					green[0] >>= format->BYTES;
					blue[0] >>= format->BYTES;

					uint8_t data = 0;
					data |= green[0];
					data |= (red[0] << format->BYTES);

					if (!writer->write<uint8_t>(data))
					{
						std::cerr << "Cannot write to a file\n";
						return 1;
					}
				}
				else
				{
					red[1] = format->getRed(x, y);
					green[1] = format->getGreen(x, y);
					blue[1] = format->getBlue(x, y);

					if (grayScale)
					{
						uint8_t gray = (red[1] + green[1] + blue[1]) / 3;
						red[1] = gray;
						green[1] = gray;
						blue[1] = gray;
					}

					red[1] >>= format->BYTES;
					green[1] >>= format->BYTES;
					blue[1] >>= format->BYTES;

					uint8_t data = 0;
					data |= red[1];
					data |= (blue[0] << format->BYTES);

					if (!writer->write<uint8_t>(data))
					{
						std::cerr << "Cannot write to a file\n";
						return 1;
					}

					data = 0;
					data |= blue[1];
					data |= (green[1] << format->BYTES);

					if (!writer->write<uint8_t>(data))
					{
						std::cerr << "Cannot write to a file\n";
						return 1;
					}
				}
			}

			if (format->getWidth() % 2)
			{
				uint8_t data = 0;
				data |= (blue[0] << format->BYTES);

				if (!writer->write<uint8_t>(data))
				{
					std::cerr << "Cannot write to a file\n";
					return 1;
				}
			}
		}
	}
	else if (method == METHOD_DECOMRPESS)
	{
		for (uint32_t y = 0; y < format->getHeight(); y++)
		{
			for (uint32_t x = 0; x < format->getWidth(); x++)
			{
				uint8_t red = format->getRed(x, y);
				uint8_t green = format->getGreen(x, y);
				uint8_t blue = format->getBlue(x, y);

				if (!writer->write<uint8_t>(red))
				{
					std::cerr << "Cannot write to a file\n";
					return 1;
				}
				if (!writer->write<uint8_t>(green))
				{
					std::cerr << "Cannot write to a file\n";
					return 1;
				}
				if (!writer->write<uint8_t>(blue))
				{
					std::cerr << "Cannot write to a file\n";
					return 1;
				}
			}

			for (uint32_t i = 0; i < format->getPadding(); i++)
			{
				if (!writer->write<uint8_t>(BMP_PADDING))
				{
					std::cerr << "Cannot write to a file\n";
					return 1;
				}
			}
		}
	}

	return 0;
}
