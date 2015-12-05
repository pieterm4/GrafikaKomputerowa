#include "GK.h"

GK::GK(FileReader& _file)  : Format(_file)
{
	buffer = _file.GetContent();

}

GK::~GK()
{
	delete[] buffer;
}

bool GK::isValid()
{
	uint8_t temp1 = *buffer;
	uint8_t temp2 = *buffer + 1;
	uint8_t temp3;
	uint8_t temp4;

	uint16_t HeaderGK = (temp2 << 8) | temp1;

	temp1 = *buffer + 14;
	temp2 = *buffer + 15;

	uint16_t HeaderBMP = (temp2 << 8) | temp1;

	temp1 = *buffer + 55;
	temp2 = *buffer + 56;
	temp3 = *buffer + 57;
	temp4 = *buffer + 58;
	uint16_t HeaderSizeBMP1 = (temp2 << 8) | temp1;
	uint16_t HeaderSizeBMP2 = (temp4 << 8) | temp1;

	uint32_t HeaderSizeBMP = (HeaderSizeBMP2 << 16) | HeaderSizeBMP1;
	temp1 = *buffer + 69;
	temp2 = *buffer + 70;

	uint16_t Deep = (temp2 << 8) | temp1;

	if (HeaderGK == HeadNameGK && HeaderBMP == HeadNameBM && HeaderSizeBMP == HeadSizeBMP && Deep == BytesPerPixel)
	{
		return true;
	}
	else
		return false;
}
