#pragma once
typedef unsigned char byte; //The byte type is born!
class ImageMatrixWalker {
public:
	void walk(int imageWidth, int imageHeight, int windowSize, byte* sourceScan, byte* destinationScan);
private:
	virtual int filter(byte* a, int n) = 0;
};