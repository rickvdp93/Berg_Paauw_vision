#include <iostream>
#include <string>
#include <fstream>
#include "corona.h"
#include <stdlib.h>
#include <algorithm>    // std::max
#include "opdr2.h"
#include <direct.h>
#include "ImageMatrixWalker.h"
#include "MedianFilter.h"
#include "MinFilter.h"
#include "MaxFilter.h"
#include "AverageFilter.h"
#include "SaltPepperFilter.h"
#include "GrayFilter.h"
#include "kmeans.h"

using namespace std;

typedef unsigned char byte; //The byte type is born!

void opdr2_saveImage(corona::Image* image, string path) {
	bool output = corona::SaveImage(path.c_str(), corona::FF_PNG, image);
}


void opdr2()
{
	//GrayFilter gray;
	string source;
	string histrogram = "histogram.csv";
	string path;

	cout << "Location of the picture (like C:\\images\\): \n";
	cin >> path;
	cout << "Name + extension of the picture (like pic.jpg): \n";
	cin >> source;
	cout << "Please wait while the conversions take place...";

	corona::Image* image = corona::OpenImage((path + source).c_str(), corona::PF_R8G8B8);
	if (!image){
		cout << "Missing file";
		return;
	}
	corona::Image* destination = corona::CloneImage(image, corona::PF_R8G8B8);

	string command = "explorer " + path;

	int width = image->getWidth();
	int height = image->getHeight();
	int numberOfPixels = width * height;

	byte* p = (byte*)image->getPixels();
	byte* d = (byte*)destination->getPixels();

	kmeans filter;
	filter.filter(p, d, numberOfPixels, 6);
	/*

	//gray.makeGrayScale(numberOfPixels, p);
	AverageFilter filter;
	//filter.FilterRGB(width, height, 3, p, d);
	filter.walk(width, height, 11, p, d);*/
	opdr2_saveImage(destination, path + "KMEANS_" + source);
	cout << "Done.";
}