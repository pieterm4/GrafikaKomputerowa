#ifndef FORMAT_H_
#define FORMAT_H_

#include <windows.h>
#include <cstdint>
#include "FileReader.h"

class Format
{
public:
	Format(FileReader &_reader);
	virtual ~Format();
	virtual bool verify() = 0;
	virtual uint8_t getRed(uint32_t x, uint32_t y) const = 0;
	virtual uint8_t getGreen(uint32_t x, uint32_t y) const = 0;
	virtual uint8_t getBlue(uint32_t x, uint32_t y) const = 0;
	uint32_t getWidth() const;
	uint32_t getHeight() const;
	uint32_t getPadding() const;
	uint32_t getBytesPerPixel() const;
    uint32_t getSizeOfBuffer() const;
	BITMAPFILEHEADER& getBfh();
	BITMAPINFOHEADER& getBih();
	const uint8_t BYTES;
	const WORD BMP_MAGIC;
    uint8_t* getBuffer() const;

protected:
	FileReader &reader;
	uint32_t width, height, padding, bytesPerPixel, sizeOfBuffer;
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
    uint8_t *buffer;
};
#endif /* FORMAT_H_ */
