#pragma once
typedef unsigned char byte; //The byte type is born!

class Threshold
{
public:
	void Filter(int imageWidth, int imageHeight, byte * pixels);
};

