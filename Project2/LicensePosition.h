#pragma once
#include "LicensePoint.h"
class LicensePosition
{
public:
	LicensePosition(LicensePoint LT, LicensePoint RT, LicensePoint LB, LicensePoint RB) : LT(LT), RT(RT), LB(LB), RB(RB) {}
	LicensePoint GetLT() { return LT; };
	LicensePoint GetRT() { return RT; };
	LicensePoint GetLB() { return LB; };
	LicensePoint GetRB() { return RB; };
private:
	LicensePoint LT;
	LicensePoint RT;
	LicensePoint LB;
	LicensePoint RB;
};
