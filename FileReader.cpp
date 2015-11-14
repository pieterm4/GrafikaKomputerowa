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
	return true;
}

bool FileReader::close()
{
	return true;
}

bool FileReader::readAll()
{
	return true;
}

uint8_t* FileReader::GetContent()
{
	return buffer;
}

uint32_t FileReader::getSize()
{
	return size;
}
