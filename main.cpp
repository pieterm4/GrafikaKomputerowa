#include <iostream>
#include <cstdint>
#include <memory>
#include "FileReader.h"
#include "FileWriter.h"
#include "Format.h"
#include "BMP.h"
#include "GK.h"
#include "Algorithm.h"
#include "Arithmetic.h"
#include "LZ77.h"
#include "Bit4.h"

const char * const COMRPESS_4BIT = "4bit";
const char * const COMRPESS_LZ77 = "lz77";
const char * const COMRPESS_ARITHMETIC = "arithmetic";
const char * const DECOMRPESS = "d";
const char * const GRAY_SCALE = "s";
const static uint8_t METHOD_DECOMRPESS = 1;
const static uint8_t METHOD_COMRPESS_4BIT = 2;
const static uint8_t METHOD_COMRPESS_LZ77 = 3;
const static uint8_t METHOD_COMRPESS_ARITHMETIC = 4;
const static uint8_t METHOD_DECOMRPESS_4BIT = 5;
const static uint8_t METHOD_DECOMRPESS_LZ77 = 6;
const static uint8_t METHOD_DECOMRPESS_ARITHMETIC = 7;
const static uint8_t ARGV_METHOD = 1;
const static uint8_t ARGV_PATH = 2;
const static uint8_t ARGV_OPTIONAL = 3;
const static uint32_t BMP_8BIT = 0;
const static uint32_t BMP_4BIT = 1;
const static uint32_t BMP_LZ77 = 2;
const static uint32_t BMP_ARITHMETIC = 3;

int main(int argc, char **argv)
{
	if (!(argc == 3 || argc == 4))
	{
		std::cerr << "Usage:\n";
        std::cerr << "To compress: bmp <compress method> file s // parameter s can be optional\n";
        std::cerr << "Available methods: 4bit, lz77, arithmetic\n";
		std::cerr << "To decompress: bmp d file\n";
		return 1;
	}

	uint8_t method = 0;
	bool grayScale = false;

    if (strcmp(argv[ARGV_METHOD], COMRPESS_4BIT) == 0)
	{
        method = METHOD_COMRPESS_4BIT;
	}
    else if (strcmp(argv[ARGV_METHOD], COMRPESS_ARITHMETIC) == 0)
    {
        method = METHOD_COMRPESS_ARITHMETIC;
    }
    else if (strcmp(argv[ARGV_METHOD], COMRPESS_LZ77) == 0)
    {
        method = METHOD_COMRPESS_LZ77;
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

    if (method != METHOD_DECOMRPESS && argc == 4 && (strcmp(argv[ARGV_OPTIONAL], GRAY_SCALE) == 0))
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

    if (method != METHOD_DECOMRPESS)
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

    if (method != METHOD_DECOMRPESS)
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

    if (method != METHOD_DECOMRPESS)
	{
		format->getBih().biBitCount = format->BYTES;

        if (method == METHOD_COMRPESS_4BIT)
        {
            format->getBih().biCompression = BMP_4BIT;
        }
        else if (method == METHOD_COMRPESS_ARITHMETIC)
        {
            format->getBih().biCompression = BMP_ARITHMETIC;
        }
        else if (method == METHOD_COMRPESS_LZ77)
        {
            format->getBih().biCompression = BMP_LZ77;
        }
	}
    else if (method == METHOD_DECOMRPESS)
	{
		format->getBih().biBitCount = 24;

        if (format->getBih().biCompression == BMP_4BIT)
        {
            method = METHOD_DECOMRPESS_4BIT;
        }
        else if (format->getBih().biCompression == BMP_ARITHMETIC)
        {
            method = METHOD_DECOMRPESS_ARITHMETIC;
        }
        else if (format->getBih().biCompression == BMP_LZ77)
        {
            method = METHOD_DECOMRPESS_LZ77;
        }

        format->getBih().biCompression = BMP_8BIT;
	}

	if (!writer->write<BITMAPINFOHEADER>(format->getBih()))
	{
		std::cerr << "Cannot write to a file\n";
		return 1;
	}

    std::unique_ptr<Algorithm> alg;

    if (method == METHOD_COMRPESS_4BIT)
    {
        alg = std::make_unique<Bit4>();
        alg->compressAndSave(*format, *writer, grayScale);
    }
    else if (method == METHOD_COMRPESS_ARITHMETIC)
    {
        alg = std::make_unique<Arithmetic>();
        alg->compressAndSave(*format, *writer, grayScale);
    }
    else if (method == METHOD_COMRPESS_LZ77)
    {
        alg = std::make_unique<LZ77>();
        alg->compressAndSave(*format, *writer, grayScale);
    }
    else if (method == METHOD_DECOMRPESS_4BIT)
    {
        alg = std::make_unique<Bit4>();
        alg->decompressAndSave(*format, *writer);
    }
    else if (method == METHOD_DECOMRPESS_ARITHMETIC)
    {
        alg = std::make_unique<Arithmetic>();
        alg->decompressAndSave(*format, *writer);
    }
    else if (method == METHOD_DECOMRPESS_LZ77)
    {
        alg = std::make_unique<LZ77>();
        alg->decompressAndSave(*format, *writer);
    }

	return 0;
}
