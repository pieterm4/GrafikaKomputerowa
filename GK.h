#ifndef GK_H_
#define GK_H_

#include "Format.h"

class GK : public Format
{
	private:
	////GK Header
	//uint16_t HeadNameGK = 0x474B;


	////BitmapHeader
	//uint16_t HeadNameBM = 0x424D;


	////BitmapInfoheader
	//uint32_t HeadSizeBMP = 0x28;

	//uint16_t BytesPerPixel = 0x4;


	////Input buffer
	uint8_t *buffer;

	typedef struct GKFILEHEADER
	{
		short type;
		int fileSize;
		int width;
		int height;
	} GKFILEHEADER;

	/* File Bitmap Header */
	typedef struct BMPFILEHEADER {
		short type;
		int fileSize;
		short reserved0;
		short reserved1;
		int dataOffset;
	} BMPFILEHEADER;

	/* Info Header */
	typedef struct BMPINFOHEADER {
		int hdrSize;
		int width;
		int height;
		short planes;
		short depth;
		int compression;
		int bmpDataSize;
		int hResolution;
		int vResolution;
		int nColors;
		int nImportantColors;
	} BMPINFOHEADER;

	typedef struct RGB {
		unsigned char R;
		unsigned char G;
		unsigned char B;
	} Pixel24;
public:
	GK(FileReader& _file);
	virtual ~GK();
	bool isValid();
};

#endif /* GK_H_ */
