#ifndef GK_H_
#define GK_H_

#include "Format.h"

class GK : public Format
{
public:
	GK(FileReader &_reader);
	bool verify();
	uint8_t getRed(uint32_t x, uint32_t y) const;
	uint8_t getGreen(uint32_t x, uint32_t y) const;
	uint8_t getBlue(uint32_t x, uint32_t y) const;
};


#endif /* GK_H_ */
