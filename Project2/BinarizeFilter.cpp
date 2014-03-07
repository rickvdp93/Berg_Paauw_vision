#include "BinarizeFilter.h"

#include <iostream>

typedef unsigned char byte; //The byte type is born!

void BinarizeFilter::Binarize(int numberOfPixels, byte * pixels) {
	unsigned long cumulatief = 0;
	int averageCumulatief = 0, x = 0;
	byte * originalPixels = pixels;
	for (int i = 0; i < numberOfPixels; i++) {
		byte red = *originalPixels++;
		byte green = *originalPixels++;
		byte blue = *originalPixels++;
		byte intensity = ((red + green) + blue) / 3;
		cumulatief += intensity;
	}
	averageCumulatief = cumulatief / numberOfPixels;
	std::cout << averageCumulatief;

	if (averageCumulatief > 140){
		x = 110;
	}
	else if (averageCumulatief <= 140 && averageCumulatief > 120){
		x = 100;
	}
	else if (averageCumulatief <= 120 && averageCumulatief >= 100){
		x = 90;
	}
	else if (averageCumulatief < 100 && averageCumulatief >= 80)
	{
		x = 80;
	}
	else if (averageCumulatief < 80 && averageCumulatief >= 60){
		x = 40;
	}
	else {
		x = 50;
	}
	for (int i = 0; i < numberOfPixels; i++) {
		byte * target = (byte*)pixels++;
		byte intensity = ((target[0] + target[1]) + target[2]) / 3;
		if (target[0] > 155 && target[1] > 155 && target[2] > 155) {
			//gray to white always black
			target[0] = 0;
			*pixels++ = 0;
			*pixels++ = 0;
		}
		else if (target[0] > 60 && target[0] < 110 && target[1] > 80 && target[1] < 130 && target[2] > 100 && target[2] < 150) {
			//light blue always black
			target[0] = 0;
			*pixels++ = 0;
			*pixels++ = 0;
		}
		else if (intensity > x){
			target[0] = 255;
			*pixels++ = 255;
			*pixels++ = 255;
		}
		else {
			target[0] = 0;
			*pixels++ = 0;
			*pixels++ = 0;
			
		}
	}
}