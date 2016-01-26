#include "LZ77.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;


LZ77::LZ77() : Algorithm()
{

}

LZ77::~LZ77()
{
}

bool LZ77::compressAndSave(Format& format, FileWriter& writer, bool isGrayScale)
{
	int8_t *buffer = format.getBuffer();

    int bufferSize = format.getSizeOfBuffer(); // zmienilem ci na poprawnie!!

	for (int i = 0; i < bufferSize; i++)
	{
		indexPosition.clear(); // Clear table with index of found elements

		int dictionarySize; // Size of dictionary

		if (i <= 4096)
		{
			dictionarySize = i;
		}
		else
			dictionarySize = 4096;

		for (int j = 0; j < dictionarySize; j++) // Searching the same number
		{
			if (buffer[i - j - 1] == buffer[i])
			{
				indexPosition.push_back(j);
			}
		}

		if (indexPosition.size() == 0)
		{
			codedTab.push_back(0);
			codedTab.push_back(0);
			codedTab.push_back(buffer[i]);
		}
		else
		{
			int maxLength = 0; // The longest length of substring
			int maxIndex; // Index of the longest substring's beginning

			for (vector<uint8_t>::iterator j = indexPosition.begin(); j != indexPosition.end(); ++j)
			{
				int currentLength = 1;
				int help = i + 1;

				for (int k = i - *j; k < i && help < bufferSize - 1 && currentLength < *j; k++, help++)
				{
					if (buffer[k] == buffer[help])
					{
						currentLength++;
					}
					else
						break;
				}

				if (currentLength > maxLength) //Check if current length is the longest found
				{
					maxLength = currentLength;
					maxIndex = *j;
				}
			}

			i += maxLength;
			codedTab.push_back(maxIndex);
			codedTab.push_back(maxLength);
			codedTab.push_back(buffer[i]);
		}


	}

	//Writing to the file

	for (vector<uint8_t>::iterator i = codedTab.begin(); i != codedTab.end(); ++i)
	{
		if (!writer.write<uint8_t>(*i))
		{
			cerr << "Cannot write to a file\n";
			return false;
		}
	}

	return true;
}

bool LZ77::decompressAndSave(Format& format, FileWriter& writer)
{
	int8_t *buffer = format.getBuffer();
    int bufferSize = format.getSizeOfBuffer(); // zmienilem ci na to co ma byc
	int height = format.getHeight(); // Height of the picture
	int width = format.getWidth(); // Width of the picture

	uint8_t *decodedTab = new uint8_t[height * width * 3]; // Table of decoded elements

	int currentPosition = 0;

	for (int i = 0; i < bufferSize; i += 3) // Jump in buffer by step = 3 and rewrite from buffer a specified amount of positions
	{
		int codeBuff = currentPosition;
		for (int j = 0; j < buffer[i + 1]; j++)
		{
			decodedTab[currentPosition] = decodedTab[codeBuff - buffer[i] + j - 1];
			currentPosition++;
		}

		decodedTab[currentPosition++] = buffer[i + 2]; // The last number from 3 quantity
	}

	for (int i = 0; i < (height * width * 3); i++) // sprawdz sobie czy to tak napewno, bo miales rozmair wskaznika zamiast tablicy pobierany !!!
	{
		if (!writer.write<uint8_t>(decodedTab[i]))
		{
			cerr << "Cannot write to a file\n";
			return false;
		}
	}
	return true;
}
