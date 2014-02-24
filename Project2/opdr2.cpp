#include <iostream>
#include <string>
#include "corona.h"
#include "opdr1.h"
#include "opdr2.h"
#include <stdlib.h>
using namespace std;

void opdr2()
{
	string source;
	string histrogram = "histogram.csv";
	string path;

	cout << "Location of the picture (like C:\\images\\): \n";
	cin >> path;
	cout << "Name + extension of the picture (like pic.jpg): \n";
	cin >> source;
	cout << "Please wait while the conversions take place... (working directory will be openend when done)";

	corona::Image* image = corona::OpenImage((path + source).c_str(), corona::PF_B8G8R8A8);
	if (!image){
		cout << "Missing file";
		return;
	}

	string command = "explorer " + path;

	int width = image->getWidth();
	int height = image->getHeight();
	int numberOfPixels = width * height;

	void* pixels = image->getPixels();
	makeGrayScale(numberOfPixels, (byte*)pixels);
	setChannel(SALTPEPPER, numberOfPixels, (byte*)pixels);
	saveImage(image, path + "noise_" + source);
	//writeCSV(SALTPEPPER, numberOfPixels, path + "E_" + histrogram);
	image = NULL;
}