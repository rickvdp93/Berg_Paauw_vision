#include <iostream>
#include <fstream>
#include "GrayFilter.h";

typedef unsigned char byte; //The byte type is born!


void GrayFilter::makeGrayScale(int numberOfPixels, byte * pixels) {
	byte* p = (byte*)pixels;
	byte* d = (byte*)pixels;
	greyOccurrence[256] = { 0 };

	for (int i = 0; i < numberOfPixels; i++){
		byte red = *p++;
		byte green = *p++;
		byte blue = *p++;
		byte alpha = *p++;
		byte grey = ((red + green + blue) / 3);
		*d++ = grey;
		*d++ = grey;
		*d++ = grey;
		*d++ = alpha;
		greyOccurrence[grey] += 1;
	}
}

void GrayFilter::writeCSV(int numberOfPixels, std::string path) {
	std::ofstream outFile;
	outFile.open(path);
	if (outFile.fail()) {
		std::cout << "csv write open failed";
		return;
	}
	for (int i = 0; i < 256; i++){
		outFile << i;
		outFile << ";";
		outFile << (float)greyOccurrence[i] / numberOfPixels;
		outFile << std::endl;
	}
	outFile.close();
}