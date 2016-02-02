#include "Arithmetic.h"
#include <iostream>
#include <cmath>

Arithmetic::Arithmetic() : Algorithm()
{

}

Arithmetic::~Arithmetic()
{
}

bool Arithmetic::compressAndSave(Format& format, FileWriter& writer, bool isGrayScale)
{
    double prob = 1.0 / 16.0;

    for (uint32_t y = 0; y < format.getHeight(); y++)
    {
        for (uint32_t x = 0; x < format.getWidth(); x++)
        {
            uint8_t red = 0, green = 0, blue = 0;
            double min = 0.0, max = 0.0, compress = 0.0;

            red = format.getRed(x, y);
            green = format.getGreen(x, y);
            blue = format.getBlue(x, y);

            if (isGrayScale)
            {
                uint8_t gray = (red + green + blue) / 3;
                red = gray;
                green = gray;
                blue = gray;
            }

            red >>= format.BYTES;
            green >>= format.BYTES;
            blue >>= format.BYTES;

            min += blue * pow(prob, 1.0);
            min += green * pow(prob, 2.0);
            min += red * pow(prob, 3.0);

            max = min + pow(prob, 3.0);
            compress = (min + max) / 2.0;

            if (!writer.write<double>(compress))
            {
                std::cerr << "Cannot write to a file\n";
                return false;
            }
        }
    }

	return true;
}

bool Arithmetic::decompressAndSave(Format& format, FileWriter& writer)
{
    double *buffer = reinterpret_cast<double*>(format.getBuffer());

    double prob = 1.0 / 16.0;

    for (uint32_t y = 0; y < format.getHeight(); y++)
    {
        for (uint32_t x = 0; x < format.getWidth(); x++)
        {
            double min = 0.0, max = prob, data = buffer[y * format.getWidth() + x];

            uint8_t color[3] = { 0 };

            for (int j = 1; j <= 3; j++)
            {
                for (uint8_t z = 0; z <= 255; z++)
                {
                    if (min < data && min + max > data)
                    {
                        color[j - 1] = z << format.BYTES;
                        break;
                    }
                    min += pow(prob, static_cast<double>(j));
                }
                max *= prob;
            }

            for (uint32_t i = 0; i < 3; i++)
            {
                if (!writer.write<uint8_t>(color[i]))
                {
                    std::cerr << "Cannot write to a file\n";
                    return false;
                }
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
