#include "ImageMatrixWalker.h"
#include "MedianFilter.h"

typedef unsigned char byte; //The byte type is born!

/*
*SOURCE: http://www.i-programmer.info/babbages-bag/505-quick-median.html?start=1
*/
void MedianFilter::split(byte* a, int n, int x, int &i, int &j) // x=median value, i=start(0) j=end of window (120)
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
void MedianFilter::median(byte* a, int n)
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


void MedianFilter::FilterRGB(int imageWidth, int imageHeight, int windowSize, byte* sourceScan, byte* destinationScan) {

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
					numPtr += ((i + k) * (imageWidth * 3)) + ((j + m) * 3);
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
			numPtr2 += (i * (imageWidth * 3)) + (j * 3); //(heightIndex * destinationStride == pixel index height) * (widthIndex * 3(RGB));
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

void MedianFilter::Filter(int imageWidth, int imageHeight, int windowSize, byte* sourceScan, byte* destinationScan) {

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
					numPtr += ((i + k) * (imageWidth * 3)) + ((j + m) * 3);
					array[arrCount++] = numPtr[0];
				}
			}
			median(array, windowSize * windowSize); //sort! NOTE: any sorter based on sizeof won't work because of possible 0 byte!!
			//std::sort(array, array + (windowSize * windowSize) - 1); //slower but works...
			numPtr2 = (byte*)destinationScan; //get our destination image
			numPtr2 += (i * (imageWidth * 3)) + (j * 3); //(heightIndex * destinationStride == pixel index height) * (widthIndex * 3(RGB));
			numPtr2[0] = array[index]; //B
			numPtr2[1] = array[index]; //G
			numPtr2[2] = array[index]; //R
		}
	}
	//delete array; //!important!
	//don't delete numPtr2 because our destinationScan is managed by C#!

}