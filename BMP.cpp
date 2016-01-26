#include "BMP.h"

BMP::BMP(FileReader& _reader) : Format(_reader)
{

}

bool BMP::verify()
{
	if (!reader.read<BITMAPFILEHEADER>(bfh))
	{
		return false;
	}

	if (bfh.bfType != BMP_MAGIC)
	{
		return false;
	}

	if (bfh.bfSize != reader.getSize())
	{
		return false;
	}

	if (!reader.read<BITMAPINFOHEADER>(bih))
	{
		return false;
	}

	width = bih.biWidth;
	height = bih.biHeight;
	bytesPerPixel = bih.biBitCount;

	if (bytesPerPixel != 24)
	{
		return false;
	}

	if (bih.biCompression != BI_RGB)
	{
		return false;
	}

	padding = width % 4;

	if (!reader.goToOffset(bfh.bfOffBits))
	{
		return false;
	}

	if (bih.biSizeImage == 0)
	{
		bih.biSizeImage = (width * 3 + padding) * height;
	}

    sizeOfBuffer = bih.biSizeImage;

	buffer = new (std::nothrow) int8_t[bih.biSizeImage];

	if (!buffer)
	{
		return false;
	}

	if (!reader.read<int8_t>(*buffer, bih.biSizeImage))
	{
		return false;
	}

	return true;
}

uint8_t BMP::getRed(uint32_t x, uint32_t y) const
{
	if (x >= width || y >= height)
	{
		return 0;
	}

	uint32_t offset = y * width * 3 + y * padding + x * 3;
	return reinterpret_cast<uint8_t *>(buffer)[offset + 2];
}

uint8_t BMP::getGreen(uint32_t x, uint32_t y) const
{
	if (x >= width || y >= height)
	{
		return 0;
	}

	uint32_t offset = y * width * 3 + y * padding + x * 3;
	return reinterpret_cast<uint8_t *>(buffer)[offset + 1];
}

uint8_t BMP::getBlue(uint32_t x, uint32_t y) const
{
	if (x >= width || y >= height)
	{
		return 0;
	}

	uint32_t offset = y * width * 3 + y * padding + x * 3;
	return reinterpret_cast<uint8_t *>(buffer)[offset];
}
