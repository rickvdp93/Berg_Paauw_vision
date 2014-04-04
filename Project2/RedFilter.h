typedef unsigned char byte; //The byte type is born!
class RedFilter {
public:
	unsigned int redOccurrence[256]; // holds red value occurrence
	void setChannel(int numberOfPixels, byte * pixels);
	void writeCSV(int numberOfPixels, std::string path, bool ten_bins);
};