#include "transformationMatrix.h"
#include <math.h>

typedef unsigned char byte; //The byte type is born!

void transformationMatrix::walk(int width, int height, float matrix[9], bool interpol, byte* sourceScan, byte* destinationScan ){
	//int determinant = (matrix[0] * matrix[4] * matrix[8]) + (matrix[1] * matrix[5] * matrix[6]) + (matrix[2] * matrix[3] * matrix[7]) - (matrix[2] * matrix[4] * matrix[6]) - (matrix[0] * matrix[5] * matrix[7]) - (matrix[1] * matrix[3] * matrix[8]);
	// invert current matrix:
	int determinant = matrix[0] * ((matrix[4] * matrix[8]) - (matrix[5] * matrix[7])) - matrix[1] * ((matrix[8] * matrix[3]) - (matrix[5] * matrix[6])) + matrix[2] * ((matrix[3] * matrix[7]) - (matrix[4] * matrix[6]));
	if (determinant > 0) {
		matrix[0] = ((matrix[4] * matrix[8]) - (matrix[5] * matrix[7])) * (1/determinant);
		matrix[1] = ((matrix[2] * matrix[7]) - (matrix[1] * matrix[8])) * (1/determinant);
		matrix[2] = ((matrix[1] * matrix[5]) - (matrix[2] * matrix[4])) * (1/determinant);
		matrix[3] = ((matrix[5] * matrix[6]) - (matrix[3] * matrix[8])) * (1/determinant);
		matrix[4] = ((matrix[0] * matrix[8]) - (matrix[2] * matrix[6])) * (1/determinant);
		matrix[5] = ((matrix[2] * matrix[3]) - (matrix[0] * matrix[5])) * (1/determinant);
		matrix[6] = ((matrix[3] * matrix[7]) - (matrix[4] * matrix[6])) * (1/determinant);
		matrix[7] = ((matrix[1] * matrix[6]) - (matrix[0] * matrix[7])) * (1/determinant);
		matrix[8] = ((matrix[0] * matrix[4]) - (matrix[1] * matrix[3])) * (1/determinant);		
	}
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++)
		{
			float x = matrix[0] * j + matrix[1] * i;
			float y = matrix[3] * j + matrix[4] * i;
			byte* sourcePtr = (byte*)sourceScan;
			if (x >= 0 && x < width && y >= 0 && y < height)
			{

				if (interpol == 0)
				{
					sourcePtr += (static_cast<int>(round(y))* (width * 3)) + (static_cast<int>(round(x)) * 3); //get source pointer to calculated pixel
				}
				else
				{
					float x0 = floor(x); //floor = round down
					float x1 = ceil(x); //ceil = round up
					float y0 = floor(x);
					float y1 = ceil(x);
					float h1 = ((x1 - x) / (x1 - x0)) * colorx0y0 + ((x - x0) / (x1 - x0)) * colorx1y0;
					float h2 = ((x1 - x) / (x1 - x0)) * colorx0y1 + ((x - x0) / (x1 - x0)) * colorx1y1;
					float interp = ((y1 - y) / (y1 - y0)) * h1 + ((y - y0) / (y1 - y0)) * h2;
				}
				byte* destinationPtr = (byte*)destinationScan + ((i * (width * 3)) + (j * 3)); //get pointer to destination scan
				destinationPtr[0] = sourcePtr[0];
				destinationPtr[1] = sourcePtr[1];
				destinationPtr[2] = sourcePtr[2];

			}
			else 
			{
				byte* destinationPtr = (byte*)destinationScan + ((i * (width * 3)) + (j * 3)); //get pointer to source scan
				destinationPtr[0] = 0;
				destinationPtr[1] = 0;
				destinationPtr[2] = 0;
			}
		}
	}
}