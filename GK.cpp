#include "GK.h"

GK::GK(FileReader& _reader) : Format(_reader)
{

}

bool GK::verify()
{
	if (!reader.read<BITMAPFILEHEADER>(bfh))
	{
		return false;
	}

	if (bfh.bfType != BMP_MAGIC)
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

	if (bytesPerPixel != BYTES)
	{
		return false;
	}

	padding = width % 4;

	if (!reader.goToOffset(bfh.bfOffBits))
	{
		return false;
	}

	bih.biSizeImage = width * 3 / 2;
	if ((width * 3) % 2)
	{
		bih.biSizeImage++;
	}
	bih.biSizeImage *= height;

	buffer = new (std::nothrow) int8_t[bih.biSizeImage];

	if (!buffer)
	{
		return false;
	}

	if (!reader.read<int8_t>(*buffer, bih.biSizeImage))
	{
		return false;
	}

	bih.biSizeImage = (width * 3 + padding) * height;

	return true;
}

uint8_t GK::getRed(uint32_t x, uint32_t y) const
{
	if (x >= width || y >= height)
	{
		return 0;
	}

	uint32_t offset = width * 3 / 2;
	if ((width * 3) % 2)
	{
		offset++;
	}
	offset *= y;
	offset += (x * 3 / 2);

	uint8_t color = reinterpret_cast<uint8_t *>(buffer)[offset + 1];

	if ((x * 3) % 2)
	{
		color &= 0x0F;
	}
	else
	{
		color &= 0xF0;
		color >>= BYTES;
	}

	color <<= BYTES;

	return color;
}

uint8_t GK::getGreen(uint32_t x, uint32_t y) const
{
	if (x >= width || y >= height)
	{
		return 0;
	}

	uint32_t offset = width * 3 / 2;
	if ((width * 3) % 2)
	{
		offset++;
	}
	offset *= y;
	offset += (x * 3 / 2);

	uint8_t color = 0;

	if ((x * 3) % 2)
	{
		color = reinterpret_cast<uint8_t *>(buffer)[offset + 1];
		color &= 0xF0;
		color >>= BYTES;
	}
	else
	{
		color = reinterpret_cast<uint8_t *>(buffer)[offset];
		color &= 0x0F;
	}

	color <<= BYTES;

	return color;
}

uint8_t GK::getBlue(uint32_t x, uint32_t y) const
{
	if (x >= width || y >= height)
	{
		return 0;
	}

	uint32_t offset = width * 3 / 2;
	if ((width * 3) % 2)
	{
		offset++;
	}
	offset *= y;
	offset += (x * 3 / 2);

	uint8_t color = reinterpret_cast<uint8_t *>(buffer)[offset];

	if ((x * 3) % 2)
	{
		color &= 0x0F;
	}
	else
	{
		color &= 0xF0;
		color >>= BYTES;
	}

	color <<= BYTES;

	return color;
}
