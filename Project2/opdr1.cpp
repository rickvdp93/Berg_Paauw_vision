#include <iostream>
#include <string>
#include <fstream>
#include "corona.h"
#include <stdlib.h>
#include "opdr1.h"
#include <direct.h>
#include "GrayFilter.h"
#include "RedFilter.h"
#include "GreenFilter.h"
#include "BlueFilter.h"
#include "EqualizeFilter.h"
using namespace std;

typedef unsigned char byte; //The byte type is born!

void saveImage(corona::Image* image, string path) {
	bool output = corona::SaveImage(path.c_str(), corona::FF_PNG, image);
}

void opdr1()
{
	GrayFilter grayFilter;
	RedFilter redFilter;
	GreenFilter greenFilter;
	BlueFilter blueFilter;
	EqualizeFilter equalizeFilter;
	string source;
	string histrogram = "histogram.csv";
	string path;

	cout << "Location of the picture (like C:\\images\\): \n";
	cin >> path;
	cout << "Name + extension of the picture (like pic.jpg): \n";
	cin >> source;
	cout << "Please wait while the conversions take place...";

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
	grayFilter.makeGrayScale(numberOfPixels, (byte*)pixels);
	//equalizeFilter.setChannel(grayFilter, numberOfPixels, (byte*)pixels);
	saveImage(image, path + "E_" + source);
	equalizeFilter.writeCSV(grayFilter, numberOfPixels, path + "E_" + histrogram);
	image = NULL;

	corona::Image* red = corona::OpenImage((path + source).c_str(), corona::PF_B8G8R8A8);
	pixels = red->getPixels();
	redFilter.setChannel(numberOfPixels, (byte*)pixels);
	saveImage(red, path + "R_" + source);
	redFilter.writeCSV(numberOfPixels, path + "R_" + histrogram);
	red = NULL;

	corona::Image* green = corona::OpenImage((path + source).c_str(), corona::PF_B8G8R8A8);
	pixels = green->getPixels();
	greenFilter.setChannel(numberOfPixels, (byte*)pixels);
	saveImage(green, path + "G_" + source);
	greenFilter.writeCSV(numberOfPixels, path + "G_" + histrogram);
	green = NULL;

	corona::Image* blue = corona::OpenImage((path + source).c_str(), corona::PF_B8G8R8A8);
	pixels = blue->getPixels();
	blueFilter.setChannel(numberOfPixels, (byte*)pixels);
	saveImage(blue, path + "B_" + source);
	blueFilter.writeCSV(numberOfPixels, path + "B_" + histrogram);
	blue = NULL;

	corona::Image* fun = corona::OpenImage((path + source).c_str(), corona::PF_B8G8R8A8);
	pixels = fun->getPixels();
	equalizeFilter.setChannel(grayFilter, numberOfPixels, (byte*)pixels);
	equalizeFilter.setChannel(grayFilter, numberOfPixels, (byte*)pixels);
	saveImage(fun, path + "FUN_" + source);
	//writeCSV(EQUALIZED, numberOfPixels, path + "FUN_" + histrogram);
	fun = NULL;
	cout << "Done.";
}