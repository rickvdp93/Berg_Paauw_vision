#pragma once
#include "corona.h"
typedef unsigned char byte; //The byte type is born!

class transformationMatrix
{
public:
	
	void walk(int width, int height, float matrix[9], bool interpol, byte* sourceScan, corona::Image* &image);
	
};

