#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include "Format.h"

class Algorithm
{
public:
	Algorithm();
	virtual ~Algorithm();
	virtual bool compressAndSave(Format& bmp, bool isGrayScale) = 0;
	virtual bool decompressAndSave(Format& gk) = 0;
};

#endif /* ALGORITHM_H_ */
