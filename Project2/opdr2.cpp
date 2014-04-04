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
#include "LicenseScanner.h"
#include "BinarizeFilter.h"
#include "YellowFilter.h"
#include "Threshold.h"

using namespace std;

typedef unsigned char byte; //The byte type is born!

void opdr2_saveImage(corona::Image* image, string path) {
	bool output = corona::SaveImage(path.c_str(), corona::FF_PNG, image);
}


void opdr2(int argc, char * argv[])
{
	//GrayFilter gray;
	string source;
	//string histrogram = "histogram.csv";
	string path;
	bool filter = true;
	int windowSize = 3;

	if (argc == 3) {
		std::string file = argv[1];
		int pos = file.rfind("\\") + 1;
		path = file.substr(0, pos);
		source = file.substr(pos, (file.length() - pos));
		std::string command = argv[2];
		if (command == "x"){
			filter = false;
			cout << "Running salt & pepper noise";
		}
		else{
			windowSize = atoi(argv[2]);
			cout << "Running median, medianRGB, min en max filter with windows size " << windowSize << " " << endl;
		}
		
	}
	else {
		cout << "No parameters given. Example: vision.exe \"C:\\test\\test.jpg\" 3. Last parameter is the window size" << endl;
		cout << "Location of the picture (like C:\\images\\): " << endl;
		cin >> path;
		cout << "Name + extension of the picture (like pic.jpg): \n";
		cin >> source;

	}
	cout << "Please wait while the conversions take place..." << endl;

	corona::Image* image = corona::OpenImage((path + source).c_str(), corona::PF_R8G8B8);
	if (!image){
		cout << "Missing file";
		return;
	}

	string command = "explorer " + path;

	int width = image->getWidth();
	int height = image->getHeight();
	int numberOfPixels = width * height;

	byte* p = (byte*)image->getPixels();

	corona::Image* destination = corona::CloneImage(image, corona::PF_R8G8B8);
	byte* d = (byte*)destination->getPixels();
	if (filter){
		MinFilter min;
		min.walk(width, height, windowSize, p, d);
		opdr2_saveImage(destination, path + "MIN" + "_" + source);
		MaxFilter max;
		max.walk(width, height, windowSize, p, d);
		opdr2_saveImage(destination, path + "MAX" + "_" + source);
		MedianFilter median;
		median.Filter(width, height, windowSize, p, d);
		opdr2_saveImage(destination, path + "MEDIAN" + "_" + source);
		MedianFilter medianRGB;
		medianRGB.FilterRGB(width, height, windowSize, p, d);
		opdr2_saveImage(destination, path + "MEDIANRGB" + "_" + source);
	}
	else{
		SaltPepperFilter sp;
		sp.walk(width, height, windowSize, p, d);
		opdr2_saveImage(destination, path + "SALTANDPEPPERNOISE_" + source);
	}
	cout << endl << "Done";
	cin.get();
	/*
	for (int i = 2; i < 11; i++) {
	corona::Image* destination = corona::CloneImage(image, corona::PF_R8G8B8);
	byte* d = (byte*)destination->getPixels();
	kmeans filter;
	filter.filter(p, d, numberOfPixels, i);

	opdr2_saveImage(destination, path + "KMEANS" + std::to_string(i) + "_" + source);
	cout << "Done " + std::to_string(i);
	}*/


	/*

	//gray.makeGrayScale(numberOfPixels, p);
	AverageFilter filter;
	//filter.FilterRGB(width, height, 3, p, d);
	filter.walk(width, height, 11, p, d);*/
}