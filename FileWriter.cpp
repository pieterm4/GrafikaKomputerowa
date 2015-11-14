#include "FileWriter.h"

FileWriter::FileWriter(std::string _fileName) : isOpen(false), fileName(_fileName), size(0), buffer(nullptr)
{
	// TODO Auto-generated constructor stub
}

FileWriter::~FileWriter()
{
	// TODO Auto-generated destructor stub
}

bool FileWriter::open()
{
	return true;
}

bool FileWriter::close()
{
	return true;
}

bool FileWriter::writeAll()
{
	return true;
}

void FileWriter::setSize(uint32_t _size)
{
}

void FileWriter::setBuffer(uint8_t* _buffer)
{
}
