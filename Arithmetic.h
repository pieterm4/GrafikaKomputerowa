#ifndef ARITHMETIC_H_
#define ARITHMETIC_H_

#include "Format.h"
#include "Algorithm.h"

class Arithmetic : public Algorithm
{
public:
	Arithmetic();
	virtual ~Arithmetic();
	bool compressAndSave(Format& bmp, bool isGrayScale);
	bool decompressAndSave(Format& gk);
};

#endif /* ARITHMETIC_H_ */
