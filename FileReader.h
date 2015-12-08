#ifndef FILEREADER_H_
#define FILEREADER_H_

#include <cstdint>
#include <fstream>

class FileReader
{
public:
	FileReader(const char * const _path);
	~FileReader();
	bool open();
	template <typename T>
	bool read(T &t);
	template <typename T>
	bool read(T &t, uint64_t size);
	bool goToOffset(uint32_t offset);
	void close();
	uint64_t getSize() const;

private:
	const char * const path;
	uint64_t size;
	std::ifstream in;
};

template<typename T>
bool FileReader::read(T& t)
{
	in.read(reinterpret_cast<char *>(&t), sizeof(t));

	if (in.fail() || in.eof())
	{
		return false;
	}
	else
	{
		return true;
	}
}

template <typename T>
bool FileReader::read(T &t, uint64_t size)
{
	in.read(reinterpret_cast<char *>(&t), sizeof(t) * size);

	if (in.fail() || in.eof())
	{
		return false;
	}
	else
	{
		return true;
	}
}

#endif /* FILEREADER_H_ */
