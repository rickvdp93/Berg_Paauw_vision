#pragma once
#include "ImageMatrixWalker.h"
typedef unsigned char byte; //The byte type is born!
class SaltPepperFilter : public ImageMatrixWalker {
public:
	SaltPepperFilter();
private:
	byte rand1;
	byte rand2;
	int filter(byte* a, int n);
};