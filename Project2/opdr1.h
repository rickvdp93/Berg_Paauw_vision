#include <string>
#include "corona.h"
using namespace std;
typedef unsigned char byte; 

void makeGrayScale(int numberOfPixels, byte * pixels);

enum ChannelMode { RED, GREEN, BLUE, EQUALIZED, SALTPEPPER };

void setChannel(ChannelMode channel, int numberOfPixels, byte * pixels);

void writeCSV(ChannelMode channel, int numberOfPixels, string path);

void saveImage(corona::Image* image, string path);

void opdr1();