#include "FileReader.h"

FileReader::FileReader(const char* const _path) : path(_path), size(0)
{

}

FileReader::~FileReader()
{
	in.close();
}

bool FileReader::open()
{
	in.open(path, in.in | in.binary);

	if (!in.is_open())
	{
		return false;
	}

	in.seekg(0, in.end);

	if (in.fail())
	{
		return false;
	}

	size = in.tellg();

	if (in.fail())
	{
		return false;
	}

	in.seekg(0, in.beg);

	if (in.fail())
	{
		return false;
	}

	return true;
}

bool FileReader::goToOffset(uint32_t offset)
{
	in.seekg(offset, in.beg);

	if (in.fail())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void FileReader::close()
{
	in.close();
}

uint64_t FileReader::getSize() const
{
	return size;
}

