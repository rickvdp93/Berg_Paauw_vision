#include "main.h"
#include <iostream>
#include <string>
#include <fstream>
#include "corona.h"
using namespace std;

void main()
{	
	string filename = "C:/gtr.png";
	string filename2 = "C:/gtr2.png";
	
	corona::Image* image = corona::OpenImage(filename.c_str(), corona::PF_B8G8R8A8);
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
	void* pixels = image->getPixels();

	typedef unsigned char byte;
	unsigned int greyOccurrence[256] = { 0 };
	byte* p = (byte*)pixels;
	byte* d = (byte*)pixels;

	for (int i = 0; i < width*height; i++){
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
	for (int i = 0; i < 256; i++){
		outFile << i;
		outFile << ";";
		outFile << (float)greyOccurrence[i]/(width*height);
		outFile << endl;
	}
	bool output = corona::SaveImage(filename2.c_str(), corona::FF_PNG, image);
	outFile.close();
}