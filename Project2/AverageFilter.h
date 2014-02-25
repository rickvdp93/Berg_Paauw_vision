#pragma once
#include "ImageMatrixWalker.h"
typedef unsigned char byte; //The byte type is born!
class AverageFilter : public ImageMatrixWalker {
private:
	int filter(byte* a, int n);
};