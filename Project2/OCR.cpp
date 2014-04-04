#include "OCR.h"
#include <iostream>
#include <string>
#include <fstream>
#include "corona.h"
#include <stdlib.h>
#include <algorithm>    // std::max
#include <direct.h>
#include "transformationMatrix.h"
#include "splitLicensePlate.h"

using namespace std;
typedef unsigned char byte; //The byte type is born!

void OCR_saveImage(corona::Image* image, string path) {
	bool output = corona::SaveImage(path.c_str(), corona::FF_PNG, image);
}

OCR::OCR(int argc, char * argv[])
{
	string source;
	string path;

	cout << "Location of the picture (like C:\\images\\): " << endl;
	cin >> path;
	cout << "Name + extension of the picture (like pic.jpg): \n";
	cin >> source;
	cout << "Please wait while the conversions take place..." << endl;

	corona::Image* image = corona::OpenImage((path + source).c_str(), corona::PF_R8G8B8);
	if (!image){
		cout << "Missing file";
		return;
	}

	corona::Image* destination = nullptr;
	int width = image->getWidth();
	int height = image->getHeight();
	int numberOfPixels = width * height;
	byte* p = (byte*)image->getPixels();


	splitLicensePlate split;
	split.split(width, height, p, destination);

	OCR_saveImage(destination, path + "OCR" + "_" + source);

	cout << endl << "Done";
	cin.get();
}
