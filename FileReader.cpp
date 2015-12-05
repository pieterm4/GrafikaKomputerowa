#include "FileReader.h"

FileReader::FileReader(std::string _fileName) : isOpen(false), fileName(_fileName), size(0), buffer(nullptr)
{
    // TODO Auto-generated constructor stub
}

FileReader::~FileReader()
{
    if (isOpen)
    {
        this->close();
        isOpen = false;
        delete [] buffer;
    }
}

bool FileReader::open()
{
    file.open(fileName, std::ios_base::binary | std::ios::in);

    if (file.is_open())
        return isOpen = true;
    else
        return isOpen = false;
}

void FileReader::close()
{
    if (isOpen)
    {
        file.close();
        isOpen = false;
        delete [] buffer;
    }
}

bool FileReader::readAll()
{
    if (isOpen)
    {
        file.seekg(0, std::ios::end);
        size = file.tellg();
        file.seekg(0, std::ios::beg);

        buffer = new  (std::nothrow)  uint8_t[size];

        if ( (file.rdstate() & std::ifstream::failbit ) == 0 && size > 0 && buffer != nullptr)
        {
            file.read((char*)buffer, size);

            if (file.gcount() == size && buffer)
                return true;
        }
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

