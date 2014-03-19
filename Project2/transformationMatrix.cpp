#include "transformationMatrix.h"
#include "corona.h"
#include <math.h>

typedef unsigned char byte; //The byte type is born!

void transformationMatrix::walk(int width, int height, float matrix[9], bool interpol, byte* sourceScan, corona::Image* &image){
	//int determinant = (matrix[0] * matrix[4] * matrix[8]) + (matrix[1] * matrix[5] * matrix[6]) + (matrix[2] * matrix[3] * matrix[7]) - (matrix[2] * matrix[4] * matrix[6]) - (matrix[0] * matrix[5] * matrix[7]) - (matrix[1] * matrix[3] * matrix[8]);
	// invert current matrix:
	//CALCULATE destination image coordinates.
	float xList[4];
	float yList[4];
	xList[0] = matrix[0] * (0) + matrix[1] * (height - 1); //BOTTOM LEFT
	yList[0] = matrix[3] * (0) + matrix[4] * (height - 1);
	xList[1] = 0; //TOP LEFT
	yList[1] = 0;
	xList[2] = matrix[0] * (width - 1) + matrix[1] * (0); //TOP RIGHT
	yList[2] = matrix[3] * (width - 1) + matrix[4] * (0);
	xList[3] = matrix[0] * (width - 1) + matrix[1] * (height - 1); //BOTTOM RIGHT
	yList[3] = matrix[3] * (width - 1) + matrix[4] * (height - 1);
	//calc image width
	float lowestX = 99999999.0f;
	for (int i = 0; i < 4; i++) {
		if (xList[i] < lowestX) {
			lowestX = xList[i];
		}
	}
	lowestX = abs(lowestX);
	int floor_lowestX = static_cast<int>(floor(lowestX));
	float highestX = 0;
	for (int i = 0; i < 4; i++) {
		if (xList[i] > highestX) {
			highestX = xList[i];
		}
	}
	int newImageWidth = static_cast<int>(ceil(lowestX) + ceil(highestX));
	//calc image height
	float lowestY = 99999999.0f;
	for (int i = 0; i < 4; i++) {
		if (yList[i] < lowestY) {
			lowestY = yList[i];
		}
	}
	lowestY = abs(lowestY);
	int floor_lowestY = static_cast<int>(floor(lowestY));
	float highestY = 0;
	for (int i = 0; i < 4; i++) {
		if (yList[i] > highestY) {
			highestY = yList[i];
		}
	}
	int newImageHeight = static_cast<int>(ceil(lowestY) + ceil(highestY));
	
	image = corona::CreateImage(newImageWidth, newImageHeight, corona::PixelFormat::PF_R8G8B8);
	byte * destinationScan = (byte*)image->getPixels();

	float matrixInv[9];
	float determinant = (matrix[0] * ((matrix[4] * matrix[8]) - (matrix[5] * matrix[7]))) - (matrix[1] * ((matrix[3] * matrix[8]) - (matrix[5] * matrix[6]))) + (matrix[2] * ((matrix[3] * matrix[7]) - (matrix[4] * matrix[6])));

	if (determinant > 0) {
		matrixInv[0] = ((matrix[4] * matrix[8]) - (matrix[5] * matrix[7])) * (1 / determinant);
		matrixInv[1] = ((matrix[2] * matrix[7]) - (matrix[1] * matrix[8])) * (1 / determinant);
		matrixInv[2] = ((matrix[1] * matrix[5]) - (matrix[2] * matrix[4])) * (1 / determinant);
		matrixInv[3] = ((matrix[5] * matrix[6]) - (matrix[3] * matrix[8])) * (1 / determinant);
		matrixInv[4] = ((matrix[0] * matrix[8]) - (matrix[2] * matrix[6])) * (1 / determinant);
		matrixInv[5] = ((matrix[2] * matrix[3]) - (matrix[0] * matrix[5])) * (1 / determinant);
		matrixInv[6] = ((matrix[3] * matrix[7]) - (matrix[4] * matrix[6])) * (1 / determinant);
		matrixInv[7] = ((matrix[1] * matrix[6]) - (matrix[0] * matrix[7])) * (1 / determinant);
		matrixInv[8] = ((matrix[0] * matrix[4]) - (matrix[1] * matrix[3])) * (1 / determinant);
	}


	for (int i = (floor_lowestY * -1); i < (newImageHeight - floor_lowestY); i++){ //NOTE when BL is asked in our matrix calculation we receive not our left bottom corner of our original image.
		for (int j = (floor_lowestX * -1); j < (newImageWidth - floor_lowestX); j++) //So we need to start negative
		{
			float x = matrixInv[0] * j + matrixInv[1] * i + matrixInv[2]; //calculate original pixel location from destination pixel location
			float y = matrixInv[3] * j + matrixInv[4] * i + matrixInv[5];
			byte* sourcePtr = (byte*)sourceScan;
			byte* destinationPtr = (byte*)destinationScan + (((i + floor_lowestY) * (newImageWidth * 3)) + ((j + floor_lowestX) * 3)); //get pointer to destination scan
			if (x >= 0 && x < width && y >= 0 && y < height) //x and y in range of original image
			{
				if (interpol == 0)
				{
					sourcePtr += (static_cast<int>(floor(y))* (width * 3)) + (static_cast<int>(floor(x)) * 3); //get source pointer to calculated original pixel location
					destinationPtr[0] = sourcePtr[0];
					destinationPtr[1] = sourcePtr[1];
					destinationPtr[2] = sourcePtr[2];
				}
				else
				{
					if (y < (height-1) && x < (width-1)){
						int x0 = static_cast<int>(floor(x)); //floor = round down
						int x1 = static_cast<int>(ceil(x)); //ceil = round up
						int y0 = static_cast<int>(floor(y));
						int y1 = static_cast<int>(ceil(y));

						float dx = x - x0;
						float dy = y - y0;
						//byte* original = sourcePtr;
						byte* x0y0 = sourcePtr + ((y0* (width * 3)) + (x0 * 3)); //get source pointer to calculated pixel
						byte* x1y0 = sourcePtr + ((y0* (width * 3)) + (x1 * 3)); //get source pointer to calculated pixel
						byte* x0y1 = sourcePtr + ((y1* (width * 3)) + (x0 * 3)); //get source pointer to calculated pixel
						byte* x1y1 = sourcePtr + ((y1* (width * 3)) + (x1 * 3)); //get source pointer to calculated pixel
						for (int colorIndex = 0; colorIndex < 3; colorIndex++) {
							float p = x0y0[colorIndex] + ((x1y0[colorIndex] - x0y0[colorIndex]) * dx);
							float q = x0y1[colorIndex] + ((x1y1[colorIndex] - x0y1[colorIndex]) * dx);
							float interp = p + ((q - p) * dy);
							destinationPtr[colorIndex] = interp;
						}
					}
				}
				
			}
			else 
			{
				destinationPtr[0] = 0;
				destinationPtr[1] = 0;
				destinationPtr[2] = 0;
			}
		}
	}
}