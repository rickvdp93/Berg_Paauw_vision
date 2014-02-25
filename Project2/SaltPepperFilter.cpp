#include <stdlib.h> 
#include "ImageMatrixWalker.h"
#include "SaltPepperFilter.h"

typedef unsigned char byte; //The byte type is born!

SaltPepperFilter::SaltPepperFilter() {
	rand2 = rand1 = rand() % 255 + 1;
	while (rand2 == rand1) {
		rand2 = rand() % 255 + 1;
	}
}

int SaltPepperFilter::filter(byte* a, int n) {
	int sum = 0;
	int midPix = ((n*n) / 2);
	if (a[midPix] == rand1) {
		return 0;
	}
	else if (a[midPix] == rand2) {
		return 255;
	}
	else return a[midPix];
}