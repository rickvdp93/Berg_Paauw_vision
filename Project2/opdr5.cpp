#include "opdr5.h"
#include <iostream>
#include <string>
#include <fstream>
#include "corona.h"
#include <stdlib.h>
#include <algorithm>    // std::max
#include <direct.h>
#include "transformationMatrix.h"
using namespace std;
typedef unsigned char byte; //The byte type is born!

void opdr5_saveImage(corona::Image* image, string path) {
	bool output = corona::SaveImage(path.c_str(), corona::FF_PNG, image);
}

opdr5::opdr5(int argc, char * argv[])
{
	string source;
	string path;

	if (argc == 3) {
		std::string file = argv[1];
		int pos = file.rfind("\\") + 1;
		path = file.substr(0, pos);
		source = file.substr(pos, (file.length() - pos));
		std::string command = argv[2];
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
	int hoek = 45;
	float matrix[9] = {cos(hoek), -sin(hoek), 0, sin(hoek), cos(hoek), 0, 0, 0, 1};
	float invmatrix[9] = {cos(hoek), sin(hoek), 0, -cos(hoek), cos(hoek), 0, 0, 0, 1};
	transformationMatrix trans;
	trans.walk(width, height, matrix, 0, p, d);
	opdr5_saveImage(destination, path + "TRANSMAT" + "_" + source);

	cout << endl << "Done";
	cin.get();
}
