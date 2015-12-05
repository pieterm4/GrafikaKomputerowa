#ifndef FILEREADER_H_
#define FILEREADER_H_

#include <fstream>

class FileReader
{
public:
	FileReader(std::string _fileName);
	virtual ~FileReader();
	bool open();
	bool close();
	bool readAll();
	uint8_t* getContent();
	uint32_t getSize();

private:
	std::fstream file;
	bool isOpen;
	std::string fileName;
	uint32_t size;
	uint8_t *buffer;
};

#endif /* FILEREADER_H_ */

