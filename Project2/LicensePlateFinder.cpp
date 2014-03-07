#include <iostream>
#include <string>
#include <fstream>
#include "LicensePoint.h"
#include "LicensePosition.h"
#include "LicensePlateFinder.h"
#include "corona.h"
#include "YellowFilter.h"
#include "BinarizeFilter.h"
#include "LicenseScanner.h"
#include "MedianFilter.h"

using namespace std;



LicensePlateFinder::LicensePlateFinder(int argc, char * argv[])
{
	//GrayFilter gray;
	string source;
	//string histrogram = "histogram.csv";
	string path;

	if (argc == 2) {
		std::string file = argv[1];
		int pos = file.rfind("\\") + 1;
		path = file.substr(0, pos);
		source = file.substr(pos, (file.length() - pos));
	}
	else {
		cout << "Location of the picture (like C:\\images\\): \n";
		cin >> path;
		cout << "Name + extension of the picture (like pic.jpg): \n";
		cin >> source;

	}
	cout << "Please wait while the conversions take place..." << endl;

	corona::Image* sourceImg = corona::OpenImage((path + source).c_str(), corona::PF_R8G8B8);
	if (!sourceImg){
		cout << "Missing file";
		return;
	}
	byte* sourcePtr = (byte*)sourceImg->getPixels();

	int width = sourceImg->getWidth();
	int height = sourceImg->getHeight();
	int numberOfPixels = width * height;

	cout << "Creating yellow image" << endl;
	corona::Image* yellowImg = corona::CloneImage(sourceImg, corona::PF_R8G8B8);
	byte* yellow = (byte*)yellowImg->getPixels();
	YellowFilter yellowFilter;
	yellowFilter.Filter(numberOfPixels, yellow);
	corona::SaveImage((path + "DEBUG\\" + "YELLOW" + "_" + source).c_str(), corona::FF_PNG, yellowImg);
	cout << "Yellow image created" << endl;

	cout << "Getting license plate location" << endl;
	LicenseScanner scanner;
	LicensePoint pos = scanner.getYellowLicenseCoord(width, height, yellow);
	cout << "License plate location at X:" << pos.GetX() << " Y:" << pos.GetY() << endl;
	
	cout << "Creating binary image" << endl;
	corona::Image* binImg = corona::CloneImage(sourceImg, corona::PF_R8G8B8);
	byte* binary = (byte*)binImg->getPixels();
	BinarizeFilter binFilter;
	binFilter.Binarize(numberOfPixels, binary);
	corona::SaveImage((path + "DEBUG\\" + "BIN" + "_" + source).c_str(), corona::FF_PNG, binImg);
	cout << "Binary image created" << endl;

	cout << "Creating License plate image" << endl;
	LicensePosition position = scanner.walk(pos.GetX(), pos.GetY(), width, height, binary);
	int left = (position.GetLT().GetX() > position.GetLB().GetX()) ? position.GetLB().GetX() : position.GetLT().GetX();
	int right = (position.GetRT().GetX() > position.GetRB().GetX()) ? position.GetRT().GetX() : position.GetRB().GetX();
	int plateWidth = right - left;
	int up = (position.GetLT().GetY() > position.GetRT().GetY()) ? position.GetRT().GetY() : position.GetLT().GetY();
	int bottom = (position.GetLB().GetY() > position.GetRB().GetY()) ? position.GetLB().GetY() : position.GetRB().GetY();
	int plateHeight = bottom - up;
	int percentage = 0;
	if (plateHeight > 0 && plateWidth > 0) {
		percentage = (float)((float)plateHeight / (float)plateWidth) * 100;
	}

	if (position.GetRB().GetX() == 0 && position.GetRB().GetY() == 0 || percentage > 50) {
		cout << "Failed. Percentage:" << percentage << " Retrying with median" << endl;
		cout << "Creating median image" << endl;
		binImg = corona::CloneImage(sourceImg, corona::PF_R8G8B8);
		binary = (byte*)binImg->getPixels();
		MedianFilter medFilter;
		medFilter.FilterRGB(width, height, 3, sourcePtr, binary);
		corona::SaveImage((path + "DEBUG\\" + "MEDIAN" + "_" + source).c_str(), corona::FF_PNG, binImg);
		cout << "Median image created" << endl;
		cout << "Creating binary image" << endl;
		binFilter.Binarize(numberOfPixels, binary);
		corona::SaveImage((path + "DEBUG\\" + "BIN" + "_" + source).c_str(), corona::FF_PNG, binImg);
		cout << "Binary image created" << endl;
		cout << "Creating License plate image" << endl;
		position = scanner.walk(pos.GetX(), pos.GetY(), width, height, binary);
	}
	std::cout << "LT X:" << position.GetLT().GetX() << " Y:" << position.GetLT().GetY() << std::endl;
	std::cout << "RT X:" << position.GetRT().GetX() << " Y:" << position.GetRT().GetY() << std::endl;
	std::cout << "LB X:" << position.GetLB().GetX() << " Y:" << position.GetLB().GetY() << std::endl;
	std::cout << "RB X:" << position.GetRB().GetX() << " Y:" << position.GetRB().GetY() << std::endl;

	//create license plate image
	left = (position.GetLT().GetX() > position.GetLB().GetX()) ? position.GetLB().GetX() : position.GetLT().GetX();
	right = (position.GetRT().GetX() > position.GetRB().GetX()) ? position.GetRT().GetX() : position.GetRB().GetX();
	plateWidth = right - left;
	up = (position.GetLT().GetY() > position.GetRT().GetY()) ? position.GetRT().GetY() : position.GetLT().GetY();
	bottom = (position.GetLB().GetY() > position.GetRB().GetY()) ? position.GetLB().GetY() : position.GetRB().GetY();
	plateHeight = bottom - up;
	corona::Image* plateImg = corona::CreateImage(plateWidth, plateHeight, corona::PixelFormat::PF_R8G8B8);
	byte* plate = (byte*)plateImg->getPixels();

	for (int i = up; i < bottom ; i++) //height loop (main img)
	{
		for (int j = left; j < right ; j++) //width loop (main img)
		{
			byte* numPtr = (byte*)sourcePtr + ((i * (width * 3)) + (j * 3));
			byte* dest = (byte*)plate + (((i - up) * (plateWidth * 3)) + ((j - left) * 3));
			dest[0] = numPtr[0];
			dest[1] = numPtr[1];
			dest[2] = numPtr[2];
		}
	}
	corona::SaveImage((path + "DEBUG\\" + "PLATE" + "_" + source).c_str(), corona::FF_PNG, plateImg);
	cout << "License plate image created" << endl;
	cout << "Done :)";
	cin.get();
}


LicensePlateFinder::~LicensePlateFinder()
{
}
