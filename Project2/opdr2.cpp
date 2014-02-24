#include <iostream>
#include <string>
#include <fstream>
#include "corona.h"
#include <stdlib.h>
#include "opdr2.h"
#include <direct.h>
#include "GrayFilter.h"
#include "RedFilter.h"
#include "GreenFilter.h"
#include "BlueFilter.h"
#include "EqualizeFilter.h"
using namespace std;

typedef unsigned char byte; //The byte type is born!

void opdr2_saveImage(corona::Image* image, string path) {
	bool output = corona::SaveImage(path.c_str(), corona::FF_PNG, image);
}

/*
*SOURCE: http://www.i-programmer.info/babbages-bag/505-quick-median.html?start=1
*/
void split(byte* a, int n, int x, int &i, int &j) // x=median value, i=start(0) j=end of window (120)
{
	do
	{
		while (a[i] < x) i++; // scan from left 
		while (x < a[j]) j--; // scan from right
		if (i <= j) // swap wrong values
		{
			byte t = a[i];
			a[i] = a[j];
			a[j] = t;
			i++; j--;
		}
	} while (i <= j); // continue till i and j cross each other

}

/*
*SOURCE: http://www.i-programmer.info/babbages-bag/505-quick-median.html?start=1
*/
void median(byte* a, int n)
{
	int L = 0;
	int R = n - 1; // odd window made even (120) (with 11*11 example)
	int k = n / 2; // median pixel of window (60)
	int i; int j;
	while (L < R)
	{
		int x = a[k]; //a[60] (get median value)
		i = L; j = R; // i = 0, j = 120
		split(a, n, x, i, j);
		L = (j < k ? i : L);
		R = (k < i ? j : R);
	}
}

void Filter(int imageWidth, int imageHeight, int windowSize, byte* sourceScan, byte* destinationScan, int sourceStride, int destinationStride) {

	int borderSize = (windowSize - 1) / 2; //get border size. This is size from after the middle point to the outside radius
	byte* array = new byte[windowSize * windowSize]; //create array to hold pixels 
	int index = (windowSize * windowSize) / 2; //index contains the middle pixel
	byte* numPtr2 = (byte*)destinationScan; //pointer to destination scan
	for (int i = borderSize; i < (imageHeight - borderSize); i++) //height loop (exclude the border)
	{
		for (int j = borderSize; j < (imageWidth - borderSize); j++) //width loop (exclude the border)
		{
			int arrCount = 0; //counter for array
			for (int k = -borderSize; k <= borderSize; k++) //border height loop
			{
				for (int m = -borderSize; m <= borderSize; m++) //border width loop
				{
					byte* numPtr = (byte*)sourceScan; //get pointer to source scan
					//((heightIndex * borderHeightIndex) * sourceStride  == pixel index height) + ((widthIndex * borderWidthIndex) * 3(RGB!)) == pointer to right RGB pixel
					numPtr += ((i + k) * sourceStride) + ((j + m) * 3);
					array[arrCount++] = numPtr[0];
				}
			}
			median(array, windowSize * windowSize); //sort! NOTE: any sorter based on sizeof won't work because of possible 0 byte!!
			//std::sort(array, array + (windowSize * windowSize) - 1); //slower but works...
			numPtr2 = (byte*)destinationScan; //get our destination image
			numPtr2 += (i * destinationStride) + (j * 3); //(heightIndex * destinationStride == pixel index height) * (widthIndex * 3(RGB));
			numPtr2[0] = array[index]; //B
			numPtr2[1] = array[index]; //G
			numPtr2[2] = array[index]; //R
		}
	}
	//delete array; //!important!
	//don't delete numPtr2 because our destinationScan is managed by C#!

}

void FilterRGB(int imageWidth, int imageHeight, int windowSize, byte* sourceScan, byte* destinationScan, int sourceStride, int destinationStride) {

	int borderSize = (windowSize - 1) / 2;//get border size. This is size from after the middle point to the outside radius
	byte* red = new byte[windowSize * windowSize]; //create array to hold red pixels 
	byte* green = new byte[windowSize * windowSize]; //create array to hold green pixels 
	byte* blue = new byte[windowSize * windowSize]; //create array to hold blue pixels 

	int index = (windowSize * windowSize) / 2; //index contains the middle pixel
	byte* numPtr2 = (byte*)destinationScan; //pointer to destination scan
	for (int i = borderSize; i < (imageHeight - borderSize); i++) //height loop (exclude the border)
	{
		for (int j = borderSize; j < (imageWidth - borderSize); j++) //width loop (exclude the border)
		{
			int arrCounter = 0; //counter for array
			for (int k = -borderSize; k <= borderSize; k++) //border height loop
			{
				for (int m = -borderSize; m <= borderSize; m++) //border width loop
				{
					byte* numPtr = (byte*)sourceScan; //get pointer to source scan
					//std::sort(array, array + (windowSize * windowSize) - 1); //slower but works...
					numPtr += ((i + k) * sourceStride) + ((j + m) * 3);
					blue[arrCounter] = numPtr[0];
					green[arrCounter] = numPtr[1];
					red[arrCounter++] = numPtr[2];
				}
			}
			median(red, windowSize * windowSize); //sort! NOTE: any sorter based on sizeof won't work because of possible 0 byte!!
			median(green, windowSize * windowSize);
			median(blue, windowSize * windowSize);

			//std::sort(array, array + (windowSize * windowSize) - 1); //slower but working
			numPtr2 = (byte*)destinationScan; //get our destination image
			numPtr2 += (i * destinationStride) + (j * 3); //(heightIndex * destinationStride == pixel index height) * (widthIndex * 3(RGB));
			numPtr2[0] = blue[index];  //B
			numPtr2[1] = green[index]; //G
			numPtr2[2] = red[index];   //R
		}
	}
	//delete blue; //!important!
	//delete green;
	//delete red;
	//don't delete numPtr2 because our destinationScan is managed by C#!

}

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

	corona::Image* image = corona::OpenImage((path + source).c_str(), corona::PF_R8G8B8);
	if (!image){
		cout << "Missing file";
		return;
	}

	string command = "explorer " + path;

	int width = image->getWidth();
	int height = image->getHeight();
	int numberOfPixels = width * height;

	void* pixels = image->getPixels();
	byte* p = (byte*)pixels;
	byte* d = (byte*)pixels;
	//Filter(width, height, 3, p, d, width * 3, width * 3);
	FilterRGB(width, height, 313, p, d, width * 3, width * 3);
	opdr2_saveImage(image, path + "MEDIAN_" + source);
	

	system(command.c_str());
}