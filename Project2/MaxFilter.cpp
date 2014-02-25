#include "ImageMatrixWalker.h"
#include "MaxFilter.h"

typedef unsigned char byte; //The byte type is born!

int MaxFilter::filter(byte* a, int n) {
	byte max = 0;
	int midPix = ((n*n) / 2);
	for (int i = 0; i < (n*n); i++){
		max = (a[i] > max ? a[i] : max);
		//if (a[i] > max)
		//	max = a[i];
	}
	return max;
}