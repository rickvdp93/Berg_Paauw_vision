#include <iostream>
#include <fstream>
#include "GreenFilter.h"

typedef unsigned char byte; //The byte type is born!

void GreenFilter::setChannel(int numberOfPixels, byte * pixels) {
	byte* p = (byte*)pixels;
	byte* d = (byte*)pixels;
	GreenFilter::greenOccurrence[256] = { 0 };
	for (int i = 0; i < numberOfPixels; i++) {
		byte red = *p++;
		byte green = *p++;
		byte blue = *p++;
		byte alpha = *p++;
		greenOccurrence[green] += 1;
		*d++ = 0;
		*d++ = green;
		*d++ = 0;
		*d++ = alpha;
	}
}

void GreenFilter::writeCSV(int numberOfPixels, std::string path) {
	std::ofstream outFile;
	outFile.open(path);
	if (outFile.fail()) {
		std::cout << "csv write open failed";
		return;
	}
	for (int i = 0; i < 256; i++){
		outFile << i;
		outFile << ";";
		outFile << (float)greenOccurrence[i] / numberOfPixels;
		outFile << std::endl;
	}
	outFile.close();
}