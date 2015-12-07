#include "FileWriter.h"

FileWriter::FileWriter(std::string _name) : name(_name)
{

}

FileWriter::~FileWriter()
{
	out.close();
}

bool FileWriter::create()
{
	out.open(name, out.out | out.binary | out.trunc);

	return out.is_open();
}

void FileWriter::close()
{
	out.close();
}
