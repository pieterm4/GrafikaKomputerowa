#include "LZ77.h"
#include <string>
#include <iostream>
#include <vector>


LZ77::LZ77() : Algorithm()
{

}

LZ77::~LZ77()
{
}

bool LZ77::compressAndSave(Format& format, FileWriter& writer, bool isGrayScale)
{
    std::vector<uint8_t> buffer;

    uint8_t red[2] = { 0 }, green[2] = { 0 }, blue[2] = { 0 };
    for (uint32_t y = 0; y < format.getHeight(); y++)
    {
        for (uint32_t x = 0; x < format.getWidth(); x++)
        {
            if (x % 2 == 0)
            {
                red[0] = format.getRed(x, y);
                green[0] = format.getGreen(x, y);
                blue[0] = format.getBlue(x, y);

                if (isGrayScale)
                {
                    uint8_t gray = (red[0] + green[0] + blue[0]) / 3;
                    red[0] = gray;
                    green[0] = gray;
                    blue[0] = gray;
                }

                red[0] >>= format.BYTES;
                green[0] >>= format.BYTES;
                blue[0] >>= format.BYTES;

                uint8_t data = 0;
                data |= green[0];
                data |= (red[0] << format.BYTES);

                buffer.push_back(data);
            }
            else
            {
                red[1] = format.getRed(x, y);
                green[1] = format.getGreen(x, y);
                blue[1] = format.getBlue(x, y);

                if (isGrayScale)
                {
                    uint8_t gray = (red[1] + green[1] + blue[1]) / 3;
                    red[1] = gray;
                    green[1] = gray;
                    blue[1] = gray;
                }

                red[1] >>= format.BYTES;
                green[1] >>= format.BYTES;
                blue[1] >>= format.BYTES;

                uint8_t data = 0;
                data |= red[1];
                data |= (blue[0] << format.BYTES);

                buffer.push_back(data);

                data = 0;
                data |= blue[1];
                data |= (green[1] << format.BYTES);

                buffer.push_back(data);
            }
        }

        if (format.getWidth() % 2)
        {
            uint8_t data = 0;
            data |= (blue[0] << format.BYTES);

            buffer.push_back(data);
        }
    }

    int bufferSize = buffer.size();

	for (int i = 0; i < bufferSize; i++)
	{
		indexPosition.clear(); // Clear table with index of found elements

		int dictionarySize; // Size of dictionary

        if (i <= 255)
		{
			dictionarySize = i;
		}
		else
            dictionarySize = 255;

        for (uint8_t j = 0; j < dictionarySize; j++) // Searching the same number
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

            for (std::vector<uint8_t>::iterator j = indexPosition.begin(); j != indexPosition.end(); ++j)
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

    for (std::vector<uint8_t>::iterator i = codedTab.begin(); i != codedTab.end(); ++i)
	{
		if (!writer.write<uint8_t>(*i))
		{
            std::cerr << "Cannot write to a file\n";
			return false;
		}
	}

	return true;
}

uint8_t Red(uint8_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint8_t BYTES)
{
    uint32_t offset = width * 3 / 2;
    if ((width * 3) % 2)
    {
        offset++;
    }
    offset *= y;
    offset += (x * 3 / 2);

    uint8_t color = reinterpret_cast<uint8_t *>(buffer)[offset + 1];

    if ((x * 3) % 2)
    {
        color &= 0x0F;
    }
    else
    {
        color &= 0xF0;
        color >>= BYTES;
    }

    color <<= BYTES;

    return color;
}

uint8_t Green(uint8_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint8_t BYTES)
{
    uint32_t offset = width * 3 / 2;
    if ((width * 3) % 2)
    {
        offset++;
    }
    offset *= y;
    offset += (x * 3 / 2);

    uint8_t color = 0;

    if ((x * 3) % 2)
    {
        color = reinterpret_cast<uint8_t *>(buffer)[offset + 1];
        color &= 0xF0;
        color >>= BYTES;
    }
    else
    {
        color = reinterpret_cast<uint8_t *>(buffer)[offset];
        color &= 0x0F;
    }

    color <<= BYTES;

    return color;
}

uint8_t Blue(uint8_t *buffer, uint32_t x, uint32_t y, uint32_t width, uint8_t BYTES)
{
    uint32_t offset = width * 3 / 2;
    if ((width * 3) % 2)
    {
        offset++;
    }
    offset *= y;
    offset += (x * 3 / 2);

    uint8_t color = reinterpret_cast<uint8_t *>(buffer)[offset];

    if ((x * 3) % 2)
    {
        color &= 0x0F;
    }
    else
    {
        color &= 0xF0;
        color >>= BYTES;
    }

    color <<= BYTES;

    return color;
}

bool LZ77::decompressAndSave(Format& format, FileWriter& writer)
{
    uint8_t *buffer = format.getBuffer();
    uint32_t bufferSize = format.getSizeOfBuffer();
    uint32_t height = format.getHeight(); // Height of the picture
    uint32_t width = format.getWidth(); // Width of the picture

    uint8_t *decodedTab = new uint8_t[format.getBih().biSizeImage]; // Table of decoded elements

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

    for (uint32_t y = 0; y < format.getHeight(); y++)
    {
        for (uint32_t x = 0; x < format.getWidth(); x++)
        {
            uint8_t red = Red(decodedTab, x, y, width, 4);
            uint8_t green = Green(decodedTab, x, y, width, 4);
            uint8_t blue = Blue(decodedTab, x, y, width, 4);

            if (!writer.write<uint8_t>(red))
            {
                std::cerr << "Cannot write to a file\n";
                return false;
            }
            if (!writer.write<uint8_t>(green))
            {
                std::cerr << "Cannot write to a file\n";
                return false;
            }
            if (!writer.write<uint8_t>(blue))
            {
                std::cerr << "Cannot write to a file\n";
                return false;
            }
        }

        for (uint32_t i = 0; i < format.getPadding(); i++)
        {
            if (!writer.write<uint8_t>(BMP_PADDING))
            {
                std::cerr << "Cannot write to a file\n";
                return false;
            }
        }
    }

	return true;
}
