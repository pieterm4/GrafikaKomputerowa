#ifndef GK_H_
#define GK_H_

#include "Format.h"

class GK : public Format
{
	private:
	
	//GK Header
	uint16_t HeadNameGK = 0x474B;


	//BitmapHeader
	uint16_t HeadNameBM = 0x424D;


	//BitmapInfoheader
	uint32_t HeadSizeBMP = 0x28;

	uint16_t BytesPerPixel = 0x4;

	//Input buffer
	uint8_t *buffer;
public:
	GK(FileReader& _file);
	virtual ~GK();
	bool isValid();
};

#endif /* GK_H_ */
