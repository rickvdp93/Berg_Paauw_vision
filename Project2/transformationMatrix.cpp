#include "transformationMatrix.h"
#include <math.h>

typedef unsigned char byte; //The byte type is born!

void transformationMatrix::walk(int width, int height, float matrix[9], bool interpol, byte* sourceScan, byte* destinationScan ){
	//int determinant = (matrix[0] * matrix[4] * matrix[8]) + (matrix[1] * matrix[5] * matrix[6]) + (matrix[2] * matrix[3] * matrix[7]) - (matrix[2] * matrix[4] * matrix[6]) - (matrix[0] * matrix[5] * matrix[7]) - (matrix[1] * matrix[3] * matrix[8]);
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
					//todo
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