typedef unsigned char byte; //The byte type is born!
class GrayFilter {
public:
	unsigned int greyOccurrence[256]; // holds grey value occurence
	void makeGrayScale(int numberOfPixels, byte * pixels);
	void writeCSV(int numberOfPixels, std::string path);
};