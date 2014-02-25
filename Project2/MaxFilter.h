#pragma once
#include "ImageMatrixWalker.h"
typedef unsigned char byte; //The byte type is born!
class MaxFilter : public ImageMatrixWalker {
private:
	int filter(byte* a, int n);
};