#ifndef LZ77_H_
#define LZ77_H_

#include "Format.h"
#include "Algorithm.h"

class LZ77 : public Algorithm
{
public:
	LZ77();
	virtual ~LZ77();
    bool compressAndSave(Format& format, FileWriter& writer, bool isGrayScale);
    bool decompressAndSave(Format& format, FileWriter& writer);
};

#endif /* LZ77_H_ */
