#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include "Format.h"
#include "FileWriter.h"

const static uint8_t BMP_PADDING = 0x00;

class Algorithm
{
public:
	Algorithm();
	virtual ~Algorithm();
    virtual bool compressAndSave(Format& format, FileWriter& writer, bool isGrayScale) = 0;
    virtual bool decompressAndSave(Format& format, FileWriter& writer) = 0;
};

#endif /* ALGORITHM_H_ */
