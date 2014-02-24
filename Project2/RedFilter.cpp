#include <iostream>
#include <fstream>
#include "RedFilter.h"

typedef unsigned char byte; //The byte type is born!

void RedFilter::setChannel(int numberOfPixels, byte * pixels) {
	byte* p = (byte*)pixels;
	byte* d = (byte*)pixels;
	RedFilter::redOccurrence[256] = { 0 };
	for (int i = 0; i < numberOfPixels; i++) {
		byte red = *p++;
		byte green = *p++;
		byte blue = *p++;
		byte alpha = *p++;
		redOccurrence[red] += 1;
		*d++ = 0;
		*d++ = 0;
		*d++ = red;
		*d++ = alpha;
	}
}

void RedFilter::writeCSV(int numberOfPixels, std::string path) {
	std::ofstream outFile;
	outFile.open(path);
	if (outFile.fail()) {
		std::cout << "csv write open failed";
		return;
	}
	for (int i = 0; i < 256; i++){
		outFile << i;
		outFile << ";";
		outFile << (float)redOccurrence[i] / numberOfPixels;
		outFile << std::endl;
	}
	outFile.close();
}