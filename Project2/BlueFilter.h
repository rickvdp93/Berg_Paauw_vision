typedef unsigned char byte; //The byte type is born!
class BlueFilter {
public:
	unsigned int blueOccurrence[256]; // holds red value occurrence
	void setChannel(int numberOfPixels, byte * pixels);
	void writeCSV(int numberOfPixels, std::string path);
};