#include "LicenseScanner.h"
#include "LicensePoint.h"
#include "LicensePosition.h"
#include <vector>
#include <iostream>

typedef unsigned char byte; //The byte type is born!


bool LicenseScanner::isLicensePlate(byte * pixel, int imageWidth, int x, int y) {
	temp = pixel;
	if (x < 0)
		temp -= (3 * -x); //go left
	else
		temp += (3 * x);
	if (y < 0)
		temp -= ((imageWidth * -y) * 3);
	else
		temp += ((imageWidth * y) * 3);
	if (temp == NULL) { //walked outside picture
		return false;
	}
	if (temp[0] == 255 && temp[1] == 255 && temp[2] == 255) {
		return true;
	}
	else {
		return false;
	}
}


LicensePoint LicenseScanner::findLeftUpper(byte * pixel, int imageWidth, int imageHeight, int x, int y) {
	if (y <= 0 && x <= 0 || y >= imageHeight && x >= imageWidth) {
		return LicensePoint(x, y, pixel);
	}
	//|6| |S
	//|3|1|S
	//|2|0|S
	//|4|S|S
	//|5| |S
	//0 == current position, S == stack overflow

	if (isLicensePlate(pixel, imageWidth, 0, -1))
	{ //check up
		return findLeftUpper(temp, imageWidth, imageHeight, x, y - 1);
	}
	else if (isLicensePlate(pixel, imageWidth, -1, 0))
	{ //check left
		return findLeftUpper(temp, imageWidth, imageHeight, x - 1, y);
	}
	else if (isLicensePlate(pixel, imageWidth, -1, -1)) 
	{ //check left up
		return findLeftUpper(temp, imageWidth, imageHeight, x - 1, y - 1);
	}
	else if (isLicensePlate(pixel, imageWidth, -1, 1))
	{ //check left under
		return findLeftUpper(temp, imageWidth, imageHeight, x - 1, y + 1);
	}
	else if (isLicensePlate(pixel, imageWidth, -1, 2))
	{ //check left under2
		return findLeftUpper(temp, imageWidth, imageHeight, x - 1, y + 2);
	}
	else if (isLicensePlate(pixel, imageWidth, -1, -2))
	{ //check left up2
		return findLeftUpper(temp, imageWidth, imageHeight, x - 1, y - 2);
	}


	return LicensePoint(x, y, pixel);
}

LicensePoint LicenseScanner::findRightUpper(byte * pixel, int imageWidth, int imageHeight, int x, int y) {
	if (y <= 0 && x <= 0 || y >= imageHeight && x >= imageWidth) {
		return LicensePoint(x, y, pixel);
	}
	//|S| |6
	//|S|1|3
	//|S|0|2
	//|S|S|4
	//|S| |5
	//0 == current position, S == stack overflow

	if (isLicensePlate(pixel, imageWidth, 0, -1))
	{ //check up
		return findRightUpper(temp, imageWidth, imageHeight, x, y - 1);
	}
	else if (isLicensePlate(pixel, imageWidth, 1, 0))
	{ //check right
		return findRightUpper(temp, imageWidth, imageHeight, x + 1, y);
	}
	else if (isLicensePlate(pixel, imageWidth, 1, -1))
	{ //check right up
		return findRightUpper(temp, imageWidth, imageHeight, x + 1, y - 1);
	}
	else if (isLicensePlate(pixel, imageWidth, 1, 1))
	{ //check right under
		return findRightUpper(temp, imageWidth, imageHeight, x + 1, y + 1);
	}
	else if (isLicensePlate(pixel, imageWidth, 1, 2))
	{ //check right under2
		return findRightUpper(temp, imageWidth, imageHeight, x + 1, y + 2);
	}
	else if (isLicensePlate(pixel, imageWidth, 1, -2))
	{ //check right up2
		return findRightUpper(temp, imageWidth, imageHeight, x + 1, y - 2);
	}


	return LicensePoint(x, y, pixel);
}

LicensePoint LicenseScanner::findRightUnder(byte * pixel, int imageWidth, int imageHeight, int x, int y) {
	if (y <= 0 && x <= 0 || y >= imageHeight && x >= imageWidth) {
		return LicensePoint(x, y, pixel);
	}
	//S| | | |S
	//S|S|S|S|S
	// |S|0|2|
	//7|5|1|3|9
	//8|6| |4|10
	//0 == current position, S == stack overflow

	if (isLicensePlate(pixel, imageWidth, 0, 1))
	{ //check under
		return findRightUnder(temp, imageWidth, imageHeight, x, y + 1);
	}
	else if (isLicensePlate(pixel, imageWidth, 1, 0))
	{ //check right
		return findRightUnder(temp, imageWidth, imageHeight, x + 1, y);
	}
	else if (isLicensePlate(pixel, imageWidth, 1, 1))
	{ //check right under
		return findRightUnder(temp, imageWidth, imageHeight, x + 1, y + 1);
	}
	else if (isLicensePlate(pixel, imageWidth, 1, 2))
	{ //check right under2
		return findRightUnder(temp, imageWidth, imageHeight, x + 1, y + 2);
	}
	else if (isLicensePlate(pixel, imageWidth, -1, 1))
	{ //check left under
		return findRightUnder(temp, imageWidth, imageHeight, x - 1, y + 1);
	}
	else if (isLicensePlate(pixel, imageWidth, -1, 2))
	{ //check left under2
		return findRightUnder(temp, imageWidth, imageHeight, x - 1, y + 2);
	}
	else if (isLicensePlate(pixel, imageWidth, -2, 1))
	{ //check left2 under
		return findRightUnder(temp, imageWidth, imageHeight, x - 2, y + 1);
	}
	else if (isLicensePlate(pixel, imageWidth, -2, 2))
	{ //check left2 under2
		return findRightUnder(temp, imageWidth, imageHeight, x - 2, y + 2);
	}
	else if (isLicensePlate(pixel, imageWidth, 2, 1))
	{ //check right2 under
		return findRightUnder(temp, imageWidth, imageHeight, x + 2, y + 1);
	}
	else if (isLicensePlate(pixel, imageWidth, 2, 2))
	{ //check right2 under2
		return findRightUnder(temp, imageWidth, imageHeight, x + 2, y + 2);
	}
	return LicensePoint(x, y, pixel);
}

LicensePoint LicenseScanner::findLeftUnder(byte * pixel, int imageWidth, int imageHeight, int x, int y) {
	if (y <= 0 && x <= 0 || y >= imageHeight && x >= imageWidth) {
		return LicensePoint(x, y, pixel);
	}
	//S | | | |S
	//S |S|S|S|S
	//  |2|0|S|
	//9 |3|1|5|7
	//10|4| |6|8
	//0 == current position, S == stack overflow

	if (isLicensePlate(pixel, imageWidth, 0, 1))
	{ //check under
		return findLeftUnder(temp, imageWidth, imageHeight, x, y + 1);
	}
	else if (isLicensePlate(pixel, imageWidth, -1, 0))
	{ //check left
		return findLeftUnder(temp, imageWidth, imageHeight, x - 1, y);
	}
	else if (isLicensePlate(pixel, imageWidth, -1, 1))
	{ //check left under
		return findLeftUnder(temp, imageWidth, imageHeight, x - 1, y + 1);
	}
	else if (isLicensePlate(pixel, imageWidth, -1, 2))
	{ //check left under2
		return findLeftUnder(temp, imageWidth, imageHeight, x - 1, y + 2);
	}
	else if (isLicensePlate(pixel, imageWidth, 1, 1)) 
	{ //check right under
		return (findLeftUnder(temp, imageWidth, imageHeight, x + 1, y + 1));
	}
	else if (isLicensePlate(pixel, imageWidth, 1, 2))
	{ //check right under2
		return (findLeftUnder(temp, imageWidth, imageHeight, x + 1, y + 2));
	}
	else if (isLicensePlate(pixel, imageWidth, 2, 1))
	{ //check right2 under
		return (findLeftUnder(temp, imageWidth, imageHeight, x + 2, y + 1));
	}
	else if (isLicensePlate(pixel, imageWidth, 2, 2))
	{ //check right2 under2
		return (findLeftUnder(temp, imageWidth, imageHeight, x + 2, y + 2));
	}
	else if (isLicensePlate(pixel, imageWidth, -2, 1))
	{ //check left2 under
		return findLeftUnder(temp, imageWidth, imageHeight, x - 2, y + 1);
	}
	else if (isLicensePlate(pixel, imageWidth, -2, 2))
	{ //check left2 under2
		return findLeftUnder(temp, imageWidth, imageHeight, x - 2, y + 2);
	}
	return LicensePoint(x, y, pixel);
}

LicensePoint LicenseScanner::getYellowLicenseCoord(int imageWidth, int imageHeight, byte* sourceScan) {
	byte* numPtr = (byte*)sourceScan; //pointer to destination scan

	//finding the max yellow pixels on the y axle.
	int yellowPixels = 0;
	int height = 0;
	for (int i = 0; i < imageHeight; i++) //height loop (exclude the border)
	{
		int pixelCounter = 0;
		for (int j = 0; j < imageWidth; j++) //width loop (exclude the border)
		{
			byte* numPtr = (byte*)sourceScan + ((i * (imageWidth * 3)) + (j * 3));
			if (numPtr[0] == 255 && numPtr[1] == 255 && numPtr[2] == 0) {
				pixelCounter += 1;
			}
		}
		if (pixelCounter > yellowPixels)
		{
			yellowPixels = pixelCounter;
			height = i;
		}
	}

	//finding the most yellow pixels together on the x axle.
	sourceScan = (byte*)sourceScan + (height * (imageWidth * 3)); //pointer to the right yas coordination
	yellowPixels = 0;
	int width = 0;
	int tempWidth = 0;
	int pixelCounter = 0;
	for (int j = 0; j < imageWidth; j++) //width loop
	{
		byte* numPtr = (byte*)sourceScan + (j * 3);
		if (numPtr[0] == 255 && numPtr[1] == 255 && numPtr[2] == 0) {
			if (pixelCounter == 0) {
				//first yellow pixel.
				tempWidth = j;
			}
			pixelCounter += 1;
		}
		else {
			if (pixelCounter > yellowPixels) {
				yellowPixels = pixelCounter;
				width = tempWidth; //save first yellow pixel of row
			}
			pixelCounter = 0;
		}
	}

	return LicensePoint(width, height, NULL); //for yellow filter safety go down 4 row and column
}

LicensePosition LicenseScanner::walk(int xas, int yas, int imageWidth, int imageHeight, byte* pixel){

	byte * numPtr = pixel + ((yas * (imageWidth * 3)) + (xas * 3));

	int counter = 1;
	while (numPtr[0] == 0 && numPtr[1] == 0 && numPtr[2] == 0) {
		std::cout << "Encountered black pixel instead of white. Correcting position " << counter << " time" << std::endl;
		numPtr += (counter * (imageWidth * 3)) + (counter * 3);
		counter += 1;
		if (counter > 20) {
			return LicensePosition(LicensePoint(0, 0, NULL), LicensePoint(0, 0, NULL), LicensePoint(0, 0, NULL), LicensePoint(0, 0, NULL));
		}
	}
	if (numPtr[0] == 255 && numPtr[1] == 255 && numPtr[2] == 255) {
		//found yellow pixel
		//search for license plate
		LicensePoint LT = findLeftUpper(numPtr, imageWidth, imageHeight, xas, yas);
		LicensePoint LB = findLeftUnder(LT.GetPointer(), imageWidth, imageHeight, LT.GetX(), LT.GetY());
		LicensePoint RT = findRightUpper(numPtr, imageWidth, imageHeight, xas, yas);
		LicensePoint RB = findRightUnder(RT.GetPointer(), imageWidth, imageHeight, RT.GetX(), RT.GetY());
		/*leftupper.GetPointer()[0] = 255;
		leftupper.GetPointer()[1] = 0;
		leftupper.GetPointer()[2] = 0;
		leftUnder.GetPointer()[0] = 255;
		leftUnder.GetPointer()[1] = 0;
		leftUnder.GetPointer()[2] = 0;
		rightupper.GetPointer()[0] = 255;
		rightupper.GetPointer()[1] = 0;
		rightupper.GetPointer()[2] = 0;
		rightunder.GetPointer()[0] = 255;
		rightunder.GetPointer()[1] = 0;
		rightunder.GetPointer()[2] = 0;*/
		return LicensePosition(LT, RT, LB, RB);
	}
}