#ifndef BMP_H_
#define BMP_H_

#include "Format.h"

class BMP : public Format
{
public:
	BMP(FileReader &_reader);
	bool verify();
	uint8_t getRed(uint32_t x, uint32_t y) const;
	uint8_t getGreen(uint32_t x, uint32_t y) const;
	uint8_t getBlue(uint32_t x, uint32_t y) const;
};


#endif /* BMP_H_ */
