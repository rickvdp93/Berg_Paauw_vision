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
	double const Pi = 4 * atan(1);
	double corner = (Pi / 9); // corner of 20 degrees (pi = 180)
	float matrixRot20[9] = { cos(corner), -sin(corner), 0, sin(corner), cos(corner), 0, 0, 0, 1 };
	float matrixScaling3[9] = { 3, 0, 0, 0, 3, 0, 0, 0, 1 };
	float matrixShear3[9] = { 1, 3, 0, 0, 1, 0, 0, 0, 1 };
	float matrixScaling3point5[9] = { 3.5f, 0, 0, 0, 3.5f, 0, 0, 0, 1 };
	float matrix[9] = { 1, 3, 0, 0, 1, 0, 0, 0, 1 };
	int interpol = 1;

	if (argc == 4) {
		std::string file = argv[1];
		int pos = file.rfind("\\") + 1;
		path = file.substr(0, pos);
		source = file.substr(pos, (file.length() - pos));
		std::string line;
		int count = 0;
		std::ifstream backstory(argv[2]);
		if (backstory.is_open())
		{
			while (backstory.good())
			{
				getline(backstory, line);
				matrix[count] = (float)atof(line.c_str());
				count += 1;
			}
		}
		interpol = atoi(argv[3]);
	}
	else {
		cout << "No parameters given. Example: vision.exe \"C:\\test\\test.jpg\" \"C:\\test\\matrix.txt\"." << endl;
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

	corona::Image* destination = nullptr;

	string command = "explorer " + path;

	int width = image->getWidth();
	int height = image->getHeight();
	int numberOfPixels = width * height;

	byte* p = (byte*)image->getPixels();

	/*double const Pi = 4 * atan(1);
	double corner = (Pi / 9) * 17; // corner of 20 degrees (pi = 180)
	float matrix[9] = { cos(corner), -sin(corner), 0, sin(corner), cos(corner), 0, 0, 0, 1 };
	//float invmatrix[9] = { cos(corner), sin(corner), 0, -cos(corner), cos(corner), 0, 0, 0, 1 };
	*/
	transformationMatrix trans;
	trans.walk(width, height, matrix, interpol, p, destination);
	opdr5_saveImage(destination, path + "TRANSMAT" + "_" + source);

	cout << endl << "Done";
	cin.get();
}
