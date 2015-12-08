#include "Format.h"

Format::Format(FileReader& _reader) : BYTES(4), BMP_MAGIC(0x4D42), reader(_reader), width(0), height(0), padding(0), bytesPerPixel(0), sizeOfBuffer(0), buffer(nullptr)
{
	ZeroMemory(&bfh, sizeof(BITMAPFILEHEADER));
	ZeroMemory(&bih, sizeof(BITMAPINFOHEADER));
}

Format::~Format()
{
	delete[] buffer;
}

uint32_t Format::getWidth() const
{
	return width;
}

uint32_t Format::getHeight() const
{
	return height;
}

uint32_t Format::getPadding() const
{
	return padding;
}

uint32_t Format::getBytesPerPixel() const
{
	return bytesPerPixel;
}

BITMAPFILEHEADER& Format::getBfh()
{
	return bfh;
}

BITMAPINFOHEADER& Format::getBih()
{
	return bih;
}
