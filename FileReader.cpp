#include "FileReader.h"

FileReader::FileReader(std::string _fileName) : isOpen(false), fileName(_fileName), size(0), buffer(nullptr)
{
    // TODO Auto-generated constructor stub
}

FileReader::~FileReader()
{
    // TODO Auto-generated destructor stub
}

bool FileReader::open()
{
    file.open(fileName, std::ios_base::binary | std::ios::in);

    if (file.is_open())
        return isOpen = true;
    else
        return isOpen = false;
}

bool FileReader::close()
{
    file.close();

    return true;
}

bool FileReader::readAll()
{
    if (isOpen)
    {
        file.seekg(0, std::ios::end);
        size = file.tellg();
        file.seekg(0, std::ios::beg);

        buffer = new uint8_t[size];
        file.read((char*)buffer, size);

        if (file.gcount() == size)
            return true;
    }

    return false;
}

uint8_t* FileReader::getContent()
{
    return buffer;
}

uint32_t FileReader::getSize()
{
    return size;
}
