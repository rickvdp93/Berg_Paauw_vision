//#include "main.h" LOLLLLLLL
#include <iostream>
#include <string>
#include <fstream>
#include "corona.h"
using namespace std;
typedef unsigned char byte; //The byte type is born!
unsigned int greyOccurrence[256] = { 0 }; // holds grey value occurence
unsigned int redOccurrence[256] = { 0 }; // holds red value occurrence
unsigned int greenOccurrence[256] = { 0 }; // holds green value occurence
unsigned int blueOccurrence[256] = { 0 }; // holds blue value occurence
unsigned int LUT[256]; // LookUpTable > used in histogram equalization

void makeGrayScale(int numberOfPixels, byte * pixels) {
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
		redOccurrence[red] += 1;
		greenOccurrence[green] += 1;
		blueOccurrence[blue] += 1;
		greyOccurrence[grey] += 1;
	}
}

enum ChannelMode { RED, GREEN, BLUE, EQUALIZED };

void setChannel(ChannelMode channel, int numberOfPixels, byte * pixels) {
	byte* p = (byte*)pixels;
	byte* d = (byte*)pixels;
	if (channel == RED) {
		for (int i = 0; i < numberOfPixels; i++) {
			*d++ = 0;
			*d++ = 0;
			*p++ = *p++; //increment += 2
			*d++ = *p++;
			*d++ = *p++;
		}
	}
	else if (channel == GREEN) {
		for (int i = 0; i < numberOfPixels; i++) {
			*d++ = 0;
			*p++; //increment
			*d++ = *p++;
			*p++; //increment
			*d++ = 0;
			*d++ = *p++;
		}
	}
	else if (channel == BLUE){
		for (int i = 0; i < numberOfPixels; i++) {
			*d++ = *p++;
			*d++ = 0;
			*d++ = 0;
			*p++ = *p++; //increment += 2
			*d++ = *p++;
		}
	}
	else {
		//http://www.songho.ca/dsp/histogram/histogram.html
		unsigned int sum = 0;
		for (int i = 0; i < 256; i++){
			sum += greyOccurrence[i]; // create a cummulative histogram
			LUT[i] = (unsigned int)(sum*((float)255 / numberOfPixels));
		}
		for (int i = 0; i < numberOfPixels; i++) {
			*d++ = LUT[*p++];
			*d++ = LUT[*p++];
			*d++ = LUT[*p++];
			*d++ = *p++;
		}
	}
}

void writeCSV(ChannelMode channel, int numberOfPixels, string path) {
	ofstream outFile;
	outFile.open(path);
	if (outFile.fail()) {
		cout << "csv write open failed";
		return;
	}
	for (int i = 0; i < 256; i++){
		outFile << i;
		outFile << ";";
		switch (channel)
		{
		case RED:
			outFile << (float)redOccurrence[i] / numberOfPixels;
			break;
		case GREEN:
			outFile << (float)greenOccurrence[i] / numberOfPixels;
			break;
		case BLUE:
			outFile << (float)blueOccurrence[i] / numberOfPixels;
			break;
		case EQUALIZED:
			outFile << (float)greyOccurrence[i] / numberOfPixels;
			break;
		default:
			break;
		}
		outFile << endl;
	}
	outFile.close();
}

void saveImage(corona::Image* image, string path) {
	bool output = corona::SaveImage(path.c_str(), corona::FF_PNG, image);
}

void main()
{	
	
	string source = "lena.png";
	string histrogram = "histogram.csv";
	string prefix;
	string path = "C:/";
	
	corona::Image* image = corona::OpenImage((path + source).c_str(), corona::PF_B8G8R8A8);
	if (!image){
		cout << "Missing file";
		return;
	}	

	int width = image->getWidth();
	int height = image->getHeight();
	int numberOfPixels = width * height;

	void* pixels = image->getPixels();
	makeGrayScale(numberOfPixels, (byte*)pixels);
	setChannel(EQUALIZED, numberOfPixels, (byte*)pixels);
	saveImage(image, path + "E_" + source);
	writeCSV(EQUALIZED, numberOfPixels, path + "E_" + histrogram);
	image = NULL;
	
	corona::Image* red = corona::OpenImage((path + source).c_str(), corona::PF_B8G8R8A8);
	pixels = red->getPixels();
	setChannel(RED, numberOfPixels, (byte*)pixels);
	saveImage(red, path + "R_" + source);
	writeCSV(RED, numberOfPixels, path + "R_" + histrogram);
	red = NULL;
	
	corona::Image* green = corona::OpenImage((path + source).c_str(), corona::PF_B8G8R8A8);
	pixels = green->getPixels();
	setChannel(GREEN, numberOfPixels, (byte*)pixels);
	saveImage(green, path + "G_" + source);
	writeCSV(GREEN, numberOfPixels, path + "G_" + histrogram);
	green = NULL;

	corona::Image* blue = corona::OpenImage((path + source).c_str(), corona::PF_B8G8R8A8);
	pixels = blue->getPixels();
	setChannel(BLUE, numberOfPixels, (byte*)pixels);
	saveImage(blue, path + "B_" + source);
	writeCSV(BLUE, numberOfPixels, path + "B_" + histrogram);
	blue = NULL;
	
	corona::Image* fun = corona::OpenImage((path + source).c_str(), corona::PF_B8G8R8A8);
	pixels = fun->getPixels();
	setChannel(EQUALIZED, numberOfPixels, (byte*)pixels);
	setChannel(EQUALIZED, numberOfPixels, (byte*)pixels);
	saveImage(fun, path + "FUN_" + source);
	//writeCSV(EQUALIZED, numberOfPixels, path + "FUN_" + histrogram);
	fun = NULL;

}