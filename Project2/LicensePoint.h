#pragma once
typedef unsigned char byte;
class LicensePoint
{
public:
	LicensePoint(int x, int y, byte * pointer) : x(x), y(y), pointer(pointer) {}
	int GetX() { return x; };
	int GetY() { return y; };
	byte* GetPointer() { return pointer; };
private:
	int x;
	int y;
	byte * pointer;
};

