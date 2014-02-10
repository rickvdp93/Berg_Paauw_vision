#include "main.h"
#include <iostream>
#include <string>
#include <fstream>
#include "corona.h"
using namespace std;

void main()
{	
	string filename = "lena.png";
	string path = "C:/" + filename;
	
	corona::Image* image = corona::OpenImage(path.c_str(), corona::PF_B8G8R8A8);
	if (!image){
		cout << "Missing file";
		return;
	}

	ofstream outFile;
	outFile.open("C:/csv.csv");
	if (outFile.fail()) {
		cout << "csv write open failed";
		return;
	}

	int width = image->getWidth();
	int height = image->getHeight();
	int numberOfPixels = width * height;
	void* pixels = image->getPixels();

	typedef unsigned char byte;
	unsigned int greyOccurrence[256] = { 0 }; // holds grey values
	unsigned int LUT[256]; // LookUpTable > holds cummulative histogram values

	byte* p = (byte*)pixels;
	byte* d = (byte*)pixels;

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

	//http://www.songho.ca/dsp/histogram/histogram.html
	unsigned int sum = 0;
	for (int i = 0; i < 256; i++){
		sum += greyOccurrence[i]; // create a cummulative histogram
		LUT[i] = (unsigned int)(sum*((float)255 / numberOfPixels));
	}
	p = (byte*)pixels;
	d = (byte*)pixels;
	string channel = "";
	string modification = "";
	if (channel == "R"){
		for (int i = 0; i < numberOfPixels; i++) {
			*d++ = *p++;
			*d++ = *p++;
			*d++ = LUT[*p++];
			*d++ = *p++;
		}
		modification = "R_";
	}
	else if (channel == "G"){
		for (int i = 0; i < numberOfPixels; i++) {
			*d++ = *p++;
			*d++ = LUT[*p++];
			*d++ = *p++;
			*d++ = *p++;
		}
		modification = "G_";
	}
	else if (channel == "B"){
		for (int i = 0; i < numberOfPixels; i++) {
			*d++ = LUT[*p++];
			*d++ = *p++;
			*d++ = *p++;
			*d++ = *p++;
		}
		modification = "B_";
	}
	else{
		for (int i = 0; i < numberOfPixels; i++) {
			*d++ = LUT[*p++];
			*d++ = LUT[*p++];
			*d++ = LUT[*p++];
			*d++ = *p++;
		}
		modification = "grey_";
	}


	for (int i = 0; i < 256; i++){
		outFile << i;
		outFile << ";";
		outFile << (float)greyOccurrence[i] / numberOfPixels;
		outFile << endl;
	}

	string greyPath = "C:/" + modification + filename;
	bool output = corona::SaveImage(greyPath.c_str(), corona::FF_PNG, image);
	outFile.close();
}