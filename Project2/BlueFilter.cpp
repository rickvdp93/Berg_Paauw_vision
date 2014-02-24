#include <iostream>
#include <fstream>
#include "BlueFilter.h"

typedef unsigned char byte; //The byte type is born!

void BlueFilter::setChannel(int numberOfPixels, byte * pixels) {
	byte* p = (byte*)pixels;
	byte* d = (byte*)pixels;
	BlueFilter::blueOccurrence[256] = { 0 };
	for (int i = 0; i < numberOfPixels; i++) {
		byte red = *p++;
		byte green = *p++;
		byte blue = *p++;
		byte alpha = *p++;
		blueOccurrence[blue] += 1;
		*d++ = blue;
		*d++ = 0;
		*d++ = 0;
		*d++ = alpha;
	}
}

void BlueFilter::writeCSV(int numberOfPixels, std::string path) {
	std::ofstream outFile;
	outFile.open(path);
	if (outFile.fail()) {
		std::cout << "csv write open failed";
		return;
	}
	for (int i = 0; i < 256; i++){
		outFile << i;
		outFile << ";";
		outFile << (float)blueOccurrence[i] / numberOfPixels;
		outFile << std::endl;
	}
	outFile.close();
}