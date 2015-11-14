#ifndef FILEWRITER_H_
#define FILEWRITER_H_

#include <fstream>

class FileWriter
{
public:
	FileWriter(std::string _fileName);
	virtual ~FileWriter();
	bool open();
	bool close();
	bool writeAll();
	void setSize(uint32_t _size);
	void setBuffer(uint8_t *_buffer);

private:
	std::ofstream file;
	bool isOpen;
	std::string fileName;
	uint32_t size;
	uint8_t *buffer;
};

#endif /* FILEWRITER_H_ */
