#ifndef BIT4_H
#define BIT4_H

#include "Format.h"
#include "Algorithm.h"

class Bit4 : public Algorithm
{
public:
    Bit4();
    virtual ~Bit4();
    bool compressAndSave(Format& format, FileWriter& writer, bool isGrayScale);
    bool decompressAndSave(Format& format, FileWriter& writer);
};

#endif // BIT4_H
