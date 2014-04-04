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

void RedFilter::writeCSV(int numberOfPixels, std::string path, bool ten_bins) {
	std::ofstream outFile;
	outFile.open(path);
	if (outFile.fail()) {
		std::cout << "csv write open failed";
		return;
	}
	if (ten_bins){
		int ten_bins[10];
		for (int i = 0; i < 256; i++){
			int temp = ((i * 10) / 256);
			ten_bins[temp] += redOccurrence[i];
		}
		for (int i = 0; i < 10; i++){
			outFile << i;
			outFile << ";";
			outFile << ten_bins[i];
			outFile << std::endl;
		}
	}
	else{
		for (int i = 0; i < 256; i++){
			outFile << i;
			outFile << ";";
			outFile << (float)redOccurrence[i] / numberOfPixels;
			outFile << std::endl;
		}
	}
	outFile.close();
}