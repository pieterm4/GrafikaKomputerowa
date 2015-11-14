#ifndef FORMAT_H_
#define FORMAT_H_

#include "FileReader.h"

class Format
{
public:
	Format(FileReader& _file);
	virtual ~Format();
	virtual bool isValid() = 0;

protected:
	FileReader& file;
};

#endif /* FORMAT_H_ */
