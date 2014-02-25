#include "ImageMatrixWalker.h"
#include "SaltPepperFilter.h"

typedef unsigned char byte; //The byte type is born!

int SaltPepperFilter::filter(byte* a, int n) {
	int sum = 0;
	int midPix = ((n*n) / 2);
	for (int i = 0; i < (n*n); i++){
		sum += a[i];
	}
	int average = sum / (n*n);
	return average;
}