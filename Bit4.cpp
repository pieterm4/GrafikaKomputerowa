#include "Bit4.h"
#include <iostream>

Bit4::Bit4() : Algorithm()
{
}

Bit4::~Bit4()
{
}

bool Bit4::compressAndSave(Format& format, FileWriter &writer, bool isGrayScale)
{
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

                if (!writer.write<uint8_t>(data))
                {
                    std::cerr << "Cannot write to a file\n";
                    return false;
                }
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

                if (!writer.write<uint8_t>(data))
                {
                    std::cerr << "Cannot write to a file\n";
                    return false;
                }

                data = 0;
                data |= blue[1];
                data |= (green[1] << format.BYTES);

                if (!writer.write<uint8_t>(data))
                {
                    std::cerr << "Cannot write to a file\n";
                    return false;
                }
            }
        }

        if (format.getWidth() % 2)
        {
            uint8_t data = 0;
            data |= (blue[0] << format.BYTES);

            if (!writer.write<uint8_t>(data))
            {
                std::cerr << "Cannot write to a file\n";
                return false;
            }
        }
    }
    return true;
}

bool Bit4::decompressAndSave(Format& format, FileWriter &writer)
{
    for (uint32_t y = 0; y < format.getHeight(); y++)
    {
        for (uint32_t x = 0; x < format.getWidth(); x++)
        {
            uint8_t red = format.getRed(x, y);
            uint8_t green = format.getGreen(x, y);
            uint8_t blue = format.getBlue(x, y);

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

