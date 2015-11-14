#include <iostream>
#include <cstring>
#include <string>
#include "FileReader.h"
#include "Format.h"
#include "BMP.h"
#include "GK.h"
#include "Algorithm.h"
#include "LZ77.h"
#include "Arithmetic.h"

const static char * const PARAMETER_COMPRESS = "-c";
const static char * const PARAMETER_DECOMPRESS = "-d";
const static char * const PARAMETER_GRAY_SCALE = "-s";
const static char * const PARAMETER_LZ77 = "-lz77";
const static char * const PARAMETER_ARITHMETIC = "-arithmetic";

const static int METHOD_INVALID = 0;
const static int METHOD_COMPRESS = 1;
const static int METHOD_DECOMPRESS = 2;

const static int ALGORITHM_INVALID = 0;
const static int ALGORITHM_LZ77 = 1;
const static int ALGORITHM_ARITHMETIC = 2;

int main(int argc, char **argv)
{
	if (argc < 3 || argc > 5)
	{
		std::cerr << "Usage\n";
		std::cerr << "Compress file using LZ77 compression: GrafikaKomputerowa -c file.bmp -lz77 -s\tparameter -s can be optional\n";
		std::cerr << "Compress file using arithmetic compression: GrafikaKomputerowa -c file.bmp -arithmetic -s\tparameter -s can be optional\n";
		std::cerr << "Decompress file: GrafikaKomputerowa -d file.gk\n";
		return 1;
	}

	FileReader *file = nullptr;
	Format *format = nullptr;
	Algorithm *alg = nullptr;
	bool isGrayScale = false;
	std::string name(argv[2]);
	int method = METHOD_INVALID, algorithm = ALGORITHM_INVALID;

	if (strcmp(argv[1], PARAMETER_COMPRESS) == 0)
	{
		file = new FileReader(name);

		if (strcmp(argv[3], PARAMETER_ARITHMETIC) == 0)
		{
			algorithm = ALGORITHM_ARITHMETIC;
		}
		if (strcmp(argv[3], PARAMETER_LZ77) == 0)
		{
			algorithm = ALGORITHM_LZ77;
		}
		else
		{
			std::cerr << "Invalid compression\n";
			delete file;
			return 1;
		}

		if (strcmp(argv[4], PARAMETER_GRAY_SCALE) == 0)
		{
			isGrayScale = true;
		}

		method = METHOD_COMPRESS;
	}
	else if (strcmp(argv[1], PARAMETER_DECOMPRESS) == 0)
	{
		file = new FileReader(name);

		method = METHOD_DECOMPRESS;
	}
	else
	{
		std::cerr << "Unrecognized option\n";
		return 1;
	}


	if (!file->open())
	{
		std::cerr << "Cannot open a file\n";
		delete file;
		return 1;
	}

	if (!file->readAll())
	{
		std::cerr << "Error while reading\n";
		delete file;
		return 1;
	}

	if (method == METHOD_COMPRESS)
	{
		format = new BMP(*file);
	}
	else if (method == METHOD_DECOMPRESS)
	{
		format = new GK(*file);
	}
	else
	{
		std::cerr << "Something went wrong\n";
		delete file;
		return 1;
	}

	if (!format->isValid())
	{
		std::cerr << "A file is invalid\n";
		delete format;
		delete file;
		return 1;
	}

	if (method == METHOD_COMPRESS)
	{
		if (algorithm == ALGORITHM_ARITHMETIC)
		{
			alg = new Arithmetic;
		}
		else if (algorithm == ALGORITHM_LZ77)
		{
			alg = new LZ77;
		}
		else
		{
			std::cerr << "Something went wrong\n";
			delete format;
			delete file;
			return 1;
		}

		if (!alg->compressAndSave(*format, isGrayScale))
		{
			std::cerr << "Something went wrong\n";
			delete format;
			delete file;
			delete alg;
			return 1;
		}
		else
		{
			std::cout << "Everything went okay\n";
		}
	}
	else
	{
		if (!alg->decompressAndSave(*format))
		{
			std::cerr << "Something went wrong\n";
			delete format;
			delete file;
			return 1;
		}
		else
		{
			std::cout << "Everything went okay\n";
		}
	}

	delete file;
	delete format;
	delete alg;

	return 0;
}

