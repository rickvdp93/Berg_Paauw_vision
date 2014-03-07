#include "YellowFilter.h"

#include <iostream>

typedef unsigned char byte; //The byte type is born!

void YellowFilter::Filter(int numberOfPixels, byte * pixels) {
	for (int i = 0; i < numberOfPixels; i++) {
		byte * red = (byte*)pixels++;
		byte * green = (byte*)pixels++;
		byte * blue = (byte*)pixels++;
		//if (b > 200 && g > 200 && r > 200)
		//{
		//    lalala[0] = 255;
		//    lalala[1] = 0;
		//    lalala[2] = 255;
		//}
		//else 
		if (red[0] > blue[0] + 110 && green[0] > blue[0] + 50)
		{
			red[0] = 255;
			green[0] = 255;
			blue[0] = 0;
		}
		/*else if (blue[0] > red[0] + 80 && blue[0] > green[0] + 60)
		{
			red[0] = 255;
			green[0] = 0;
			blue[0] = 0;
		}*/
		else
		{
			red[0] = 0;
			green[0] = 0;
			blue[0] = 0;
		}
	}
}