#ifndef GK_H_
#define GK_H_

#include "Format.h"

class GK : public Format
{
public:
	GK(FileReader& _file);
	virtual ~GK();
	bool isValid();
};

#endif /* GK_H_ */
