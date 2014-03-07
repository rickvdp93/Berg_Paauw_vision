#pragma once
#include "LicensePoint.h"
#include "LicensePosition.h"

typedef unsigned char byte; //The byte type is born!
class LicenseScanner {
public:
	LicensePoint getYellowLicenseCoord(int imageWidth, int imageHeight, byte* sourceScan);
	LicensePosition walk(int xas, int yas, int imageWidth, int imageHeight, byte* pixel);
private:
	bool isLicensePlate(byte * pixel, int imageWidth, int x, int y);
	LicensePoint findLeftUpper(byte * pixel, int imageWidth, int imageHeight, int x, int y);
	LicensePoint findRightUpper(byte * pixel, int imageWidth, int imageHeight, int x, int y);
	LicensePoint findRightUnder(byte * pixel, int imageWidth, int imageHeight, int x, int y);
	LicensePoint findLeftUnder(byte * pixel, int imageWidth, int imageHeight, int x, int y);
	byte * temp;
};