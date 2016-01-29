#ifndef LZ77_H_
#define LZ77_H_

#include "Format.h"
#include "Algorithm.h"
#include <string>
#include <vector>

class LZ77 : public Algorithm
{
private:
    std::vector<uint8_t> codedTab;
    std::vector<uint8_t> indexPosition;


public:
	LZ77();
	virtual ~LZ77();
    bool compressAndSave(Format& format, FileWriter& writer, bool isGrayScale);
    bool decompressAndSave(Format& format, FileWriter& writer);
};

#endif /* LZ77_H_ */
