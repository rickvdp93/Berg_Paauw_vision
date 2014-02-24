#include <iostream>
#include <fstream>
#include "GrayFilter.h"
#include "EqualizeFilter.h"

typedef unsigned char byte; //The byte type is born!

void EqualizeFilter::setChannel(GrayFilter filter, int numberOfPixels, byte * pixels) {
	byte* p = (byte*)pixels;
	byte* d = (byte*)pixels;
	EqualizeFilter::LUT[256] = { 0 };
	//http://www.songho.ca/dsp/histogram/histogram.html
	unsigned int sum = 0;
	for (int i = 0; i < 256; i++){
		sum += filter.greyOccurrence[i]; // create a cummulative histogram
		LUT[i] = (unsigned int)(sum*((float)255 / numberOfPixels));
	}
	for (int i = 0; i < numberOfPixels; i++) {
		*d++ = LUT[*p++];
		*d++ = LUT[*p++];
		*d++ = LUT[*p++];
		*d++ = *p++;
	}
}

void EqualizeFilter::writeCSV(GrayFilter filter, int numberOfPixels, std::string path) {
	std::ofstream outFile;
	outFile.open(path);
	if (outFile.fail()) {
		std::cout << "csv write open failed";
		return;
	}
	for (int i = 0; i < 256; i++){
		outFile << i;
		outFile << ";";
		outFile << (float)filter.greyOccurrence[i] / numberOfPixels;
		outFile << std::endl;
	}
	outFile.close();
}