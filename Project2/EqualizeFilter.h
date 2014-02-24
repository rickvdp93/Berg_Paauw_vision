typedef unsigned char byte; //The byte type is born!
class EqualizeFilter {
public:
	unsigned int LUT[256]; // LookUpTable > used in histogram equalization
	void setChannel(GrayFilter filter, int numberOfPixels, byte * pixels);
	void writeCSV(GrayFilter filter, int numberOfPixels, std::string path);
};