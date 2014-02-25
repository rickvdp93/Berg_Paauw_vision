#include "ImageMatrixWalker.h"

typedef unsigned char byte; //The byte type is born!

void ImageMatrixWalker::walk(int imageWidth, int imageHeight, int windowSize, byte* sourceScan, byte* destinationScan){
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
					red[arrCounter] = numPtr[0];
					green[arrCounter] = numPtr[1];
					blue[arrCounter++] = numPtr[2];
				}
			}

			//std::sort(array, array + (windowSize * windowSize) - 1); //slower but working
			numPtr2 = (byte*)destinationScan; //get our destination image
			numPtr2 += (i * (imageWidth * 3)) + (j * 3); //(heightIndex * destinationStride == pixel index height) * (widthIndex * 3(RGB));
			numPtr2[0] = filter(red, windowSize);//blue[index];  //B
			numPtr2[1] = filter(green, windowSize);//green[index]; //G
			numPtr2[2] = filter(blue, windowSize);//red[index];   //R
		}
	}
	//delete blue; //!important!
	//delete green;
	//delete red;
	//don't delete numPtr2 because our destinationScan is managed by C#!

}