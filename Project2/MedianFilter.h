typedef unsigned char byte; //The byte type is born!
class MedianFilter {
public:
	void MedianFilter::FilterRGB(int imageWidth, int imageHeight, int windowSize, byte* sourceScan, byte* destinationScan);
	void MedianFilter::Filter(int imageWidth, int imageHeight, int windowSize, byte* sourceScan, byte* destinationScan);
private:
	void MedianFilter::median(byte* a, int n);
	void MedianFilter::split(byte* a, int n, int x, int &i, int &j);


};