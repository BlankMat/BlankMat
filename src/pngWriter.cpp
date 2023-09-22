#include "pngWriter.h"
#include "fileHelper.h"

void Png::WritePngToFile(std::string location, std::string fileName)
{
    std::vector<unsigned char> image;
    image.resize(width * height * 4);
    for (unsigned y = 0; y < height; y++) {
        for (unsigned x = 0; x < width; x++) {
            //if(x == y)
            //    std::cout << "Writing value: " << 
            image[4 * width * y + 4 * x + 0] = (char)glm::clamp(img[x + width * y].r, 0.0f, 255.0f);
            image[4 * width * y + 4 * x + 1] = (char)glm::clamp(img[x + width * y].g, 0.0f, 255.0f);
            image[4 * width * y + 4 * x + 2] = (char)glm::clamp(img[x + width * y].b, 0.0f, 255.0f);
            image[4 * width * y + 4 * x + 3] = (char)glm::clamp(img[x + width * y].a, 0.0f, 255.0f);
        }
    }
    // Find file location
    int counter = 0;
    std::string endLoc = location + fileName + ".png";
    while (FileExists(endLoc)) {
        endLoc = location + fileName + "_" + std::to_string(counter) + ".png";
        counter++;
    }
    lodepng::encode(endLoc.c_str(), image, width, height);
}

Png::Png(unsigned int _width, unsigned int _height)
{
    width = _width;
    height = _height;
    img.resize(width * height);
}