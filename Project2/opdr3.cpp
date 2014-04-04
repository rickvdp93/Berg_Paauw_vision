#include <iostream>
#include <string>
#include <fstream>
#include "corona.h"
#include <stdlib.h>
#include <algorithm>    // std::max
#include "opdr3.h"
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

void opdr3_saveImage(corona::Image* image, string path) {
	bool output = corona::SaveImage(path.c_str(), corona::FF_PNG, image);
}


void opdr3(int argc, char * argv[])
{
	//GrayFilter gray;
	string source;
	//string histrogram = "histogram.csv";
	string path;
	bool kmeanfilter = true;
	int kclusters = 3;

	if (argc == 3) {
		std::string file = argv[1];
		int pos = file.rfind("\\") + 1;
		path = file.substr(0, pos);
		source = file.substr(pos, (file.length() - pos));
		std::string command = argv[2];
		if (command == "t")
		{
			kmeanfilter = false;
			cout << "Running auto threshold" << endl;
		}
		else {
			kclusters = atoi(argv[2]);
			cout << "Running kmean on with " << kclusters << " clusters" << endl;
		}
	}
	else {
		cout << "No parameters given. Example: vision.exe \"C:\\test\\test.jpg\" 3. Last parameter can be t for threshold instead of kmeans cluster size" << endl;
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

	if (kmeanfilter) {
		kmeans kmeans_custom;
		cout << "Custom k-means started\n";
		kmeans_custom.filter(p, d, numberOfPixels, kclusters);
		opdr3_saveImage(destination, path + "KMEANS_CUSTOM" + "_" + source);
		kmeans kmeans_3;
		cout << "3 k-means started\n";
		kmeans_3.filter(p, d, numberOfPixels, 3);
		opdr3_saveImage(destination, path + "KMEANS_3" + "_" + source);
		kmeans kmeans_5;
		cout << "5 k-means started\n";
		kmeans_5.filter(p, d, numberOfPixels, 5);
		opdr3_saveImage(destination, path + "KMEANS_5" + "_" + source);
		kmeans kmeans_7;
		cout << "7 k-means started\n";
		kmeans_7.filter(p, d, numberOfPixels, 7);
		opdr3_saveImage(destination, path + "KMEANS_7" + "_" + source);
		kmeans kmeans_20;
		cout << "20 k-means started\n";
		kmeans_20.filter(p, d, numberOfPixels, 20);
		opdr3_saveImage(destination, path + "KMEANS_20" + "_" + source);
	}
	else {
		Threshold filter;
		filter.Filter(width, height, d);
		opdr3_saveImage(destination, path + "THRESHOLD" + "_" + source);
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