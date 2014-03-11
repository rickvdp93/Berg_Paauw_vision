#include "Threshold.h"
#include <iostream>
typedef unsigned char byte; //The byte type is born!



void Threshold::Filter(int imageWidth, int imageHeight, byte * pixels) {
	int grey1 = (pixels[0] + pixels[1] + pixels[2]) / 3; //upper left corner
	int startRight = (imageWidth - 1) * 3;
	int grey2 = (pixels[startRight] + pixels[startRight + 1] + pixels[startRight + 2]) / 3; //upper right corner
	int startBottom = ((imageWidth * 3) * (imageHeight - 1));
	int grey3 = (pixels[startBottom] + pixels[startBottom + 1] + pixels[startBottom + 2]) / 3; //under left corner
	int grey4 = (pixels[startBottom + startRight] + pixels[startBottom + startRight + 1] + pixels[startBottom + startRight + 2]) / 3; //under right corner
	int meanGreyCorner = (grey1 + grey2 + grey3 + grey4) / 4; //corner pixels mean
	int histogram[256] { }; //make histogram!
	unsigned long sumGreyLevel = 0; //holds all gray levels of all pixels EXEPT corners
	for (int i = 0; i < (imageWidth * imageHeight) * 3; i += 3)
	{
		int grey = (pixels[i] + pixels[i + 1] + pixels[i + 2]) / 3; 
		histogram[grey] = histogram[grey] + 1; //fill histogram
		if (i != 0 && i != startRight && i != startBottom && i != (startBottom + startRight)) //corner check
		{
			sumGreyLevel += grey;
		}
	}
	int meanGrey = sumGreyLevel / ((imageHeight * imageWidth) - 4); //other pixels mean
	int Told = 0, Tnew = (meanGreyCorner + meanGrey) / 2;

	while (Tnew != Told) {
		unsigned long cumLower = 0, cumUpper = 0;
		unsigned int lowerGreys = 0; //counter
		
		for (int i = 0; i < Tnew; i++){
			cumLower += histogram[i] * i;
			lowerGreys += histogram[i];
		}
		cumLower = cumLower / lowerGreys;

		for (int i = Tnew; i < 256; i++)
		{
			cumUpper += histogram[i] * i;
		}
		cumUpper = cumUpper / ((imageWidth * imageHeight) - lowerGreys);
		Told = Tnew;
		Tnew = (cumLower + cumUpper) / 2;
	}
	std::cout << "Threshold: " << Tnew;
	//binarize starts here
	for (int i = 0; i < (imageWidth * imageHeight) * 3; i += 3)
	{
		int grey = (pixels[i] + pixels[i + 1] + pixels[i + 2]) / 3;
		int color = 255;
		if (grey < Tnew) //threshold check
			color = 0;
		pixels[i] = pixels[i + 1] = pixels[i + 2] = color; //modify image
	}
}
