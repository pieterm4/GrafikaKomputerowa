#ifndef BMP_H_
#define BMP_H_

#include "Format.h"

class BMP : public Format
{
public:
	BMP(FileReader& _file);
	virtual ~BMP();
	bool isValid();
};

#endif /* BMP_H_ */
