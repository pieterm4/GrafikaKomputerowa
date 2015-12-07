#ifndef FILEWRITER_H_
#define FILEWRITER_H_

#include <fstream>
#include <string>

class FileWriter
{
public:
	FileWriter(std::string _name);
	~FileWriter();
	bool create();
	template <typename T>
	bool write(const T &t);
	void close();

private:
	std::ofstream out;
	std::string name;
};

template<typename T>
bool FileWriter::write(const T& t)
{
	out.write(reinterpret_cast<char *>(&const_cast<T&>(t)), sizeof(t));

	if (out.fail())
	{
		return false;
	}
	else
	{
		return true;
	}
}

#endif /* FILEWRITER_H_ */
