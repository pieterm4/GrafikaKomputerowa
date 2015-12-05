#include "FileWriter.h"

FileWriter::FileWriter(std::string _fileName) : isOpen(false), fileName(_fileName), size(0), buffer(nullptr)
{
    // TODO Auto-generated constructor stub
}

FileWriter::~FileWriter()
{
    if (isOpen)
    {
        this->close();
        isOpen = false;
    }
}

bool FileWriter::open()
{
    file.open(fileName, std::ios_base::binary);

    if (file.is_open())
    {
        isOpen = true;
        return isOpen;
    }
    else
    {
        isOpen = false;
        return isOpen;
    }
}

void FileWriter::close()
{
    if (isOpen)
    {
        file.close();
        isOpen = false;
    }
}

bool FileWriter::writeAll()
{
    if (isOpen && size > 0 && buffer != nullptr)
    {
        file.write((char*)buffer, size);

        return true;
    }

    return false;
}

void FileWriter::setSize(uint32_t _size)
{
    size = _size;
}

void FileWriter::setBuffer(uint8_t* _buffer)
{
    buffer = _buffer;
}
