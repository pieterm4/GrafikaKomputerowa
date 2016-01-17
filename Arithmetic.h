#ifndef ARITHMETIC_H_
#define ARITHMETIC_H_

#include "Format.h"
#include "Algorithm.h"

class Arithmetic : public Algorithm
{
public:
	Arithmetic();
	virtual ~Arithmetic();
    bool compressAndSave(Format& format, FileWriter& writer, bool isGrayScale);
    bool decompressAndSave(Format& format, FileWriter& writer);
};

#endif /* ARITHMETIC_H_ */
