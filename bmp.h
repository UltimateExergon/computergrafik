#include <iostream>
#include <fstream>
#include <vector>
#include "model.h"


#pragma pack(push, 1)  // Ensure that structure members are not padded


// Define BMP Header Structure

struct BMPHeader {

    char header[2];          // 'BM' for Bitmap

    uint32_t fileSize;       // Size of the entire file

    uint32_t reserved;       // Reserved (usually 0)

    uint32_t dataOffset;     // Offset to the pixel data

};


struct DIBHeader {

    uint32_t headerSize;     // DIB Header size (40 bytes)

    int32_t width;           // Image width

    int32_t height;          // Image height

    uint16_t colorPlanes;    // Color planes (usually 1)

    uint16_t bitsPerPixel;   // Bits per pixel (24 for RGB)

    uint32_t compression;    // Compression type (0 for none)

    uint32_t imageSize;      // Image size (may be 0 for uncompressed)

    int32_t xRes;            // Horizontal resolution (pixels per meter)

    int32_t yRes;            // Vertical resolution (pixels per meter)

    uint32_t colors;         // Number of colors

    uint32_t importantColors; // Important colors

};

struct BMP {
	BMPHeader Bheader;
	DIBHeader Dheader;
	vector<Color> pixels;
};


#pragma pack(pop)  // Restore previous packing alignment


void loadBMP(const std::string& filename) {

    std::ifstream file(filename, std::ios::binary);

    

    if (!file.is_open()) {

        std::cerr << "Error opening file: " << filename << std::endl;

        return;

    }

    

    BMPHeader bmpHeader;

    DIBHeader dibHeader;

    

    // Read BMP Header

    file.read(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));

    if (bmpHeader.header[0] != 'B' || bmpHeader.header[1] != 'M') {

        std::cerr << "Not a valid BMP file" << std::endl;

        return;

    }

    

    // Read DIB Header

    file.read(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader));

    

    // Check if the image is 24-bit

    if (dibHeader.bitsPerPixel != 24) {

        std::cerr << "Only 24-bit BMP files are supported." << std::endl;

        return;

    }

    

    // Move file pointer to pixel data location

    file.seekg(bmpHeader.dataOffset, std::ios::beg);

    

    // Prepare to read pixel data

    std::vector<Color> pixels(dibHeader.width * dibHeader.height);

    

    for (int y = dibHeader.height - 1; y >= 0; --y) {

        for (int x = 0; x < dibHeader.width; ++x) {

            file.read(reinterpret_cast<char*>(&pixels[y * dibHeader.width + x]), sizeof(RGBPixel));

        }

    }

    

    // Now we have the pixel data in the `pixels` array.

    // Let's print out some pixel values (RGB)

    //for (int y = 0; y < 10 && y < dibHeader.height; ++y) {  // Limit to first 10 rows

    //    for (int x = 0; x < 10 && x < dibHeader.width; ++x) {  // Limit to first 10 columns

    //        RGBPixel& pixel = pixels[y * dibHeader.width + x];

    //        std::cout << "Pixel at (" << x << "," << y << ") - R: " << (int)pixel.red

    //                  << " G: " << (int)pixel.green << " B: " << (int)pixel.blue << std::endl;

    //    }

    //}


    file.close();

}

Color getPixel(BMP bmpdata, int x, int y){
	return bmpdata.pixels.at(y * bmpdata.Dheader.width + x);
}
