#include "ImageMatrixWalker.h"
#include "MinFilter.h"

typedef unsigned char byte; //The byte type is born!

int MinFilter::filter(byte* a, int n) {
	int min = 255;
	int midPix = ((n*n) / 2);
	for (int i = 0; i < (n*n); i++){
		min = (a[i] < min ? a[i] : min);
	}
	return min;
}