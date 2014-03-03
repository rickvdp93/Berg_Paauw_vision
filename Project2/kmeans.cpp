#include <stdlib.h> 
#include <math.h>       /* sqrt */
#include "kmeans.h"
#define MAX_ITERATIONS 25
typedef unsigned char byte; //The byte type is born!


void kmeans::filter(byte* p, byte* d, int numberOfPixels, int kCount) {
	byte * clusterPoints = new byte[kCount * 3]; //clusters * RGB. Holds all clusters
	//set random clusterPoints (colors). TODO: check duplicated
	for (int i = 0; i < kCount*3; i+=3) {
		clusterPoints[i] = rand() % 255 + 1;
		clusterPoints[i + 1] = rand() % 255 + 1;
		clusterPoints[i + 2] = rand() % 255 + 1;
	}
	int iterationCount = 0;
	while (true) {
		long * clusterSum = new long[kCount * 3] { }; //contains sum of color of pixels per cluster * RGB
		int * clusterSumCount = new int[kCount] {}; // contains number of additions per cluster
		byte * source = p;
		for (int i = 0; i < numberOfPixels; i++) {
			byte red = *source++;
			byte green = *source++;
			byte blue = *source++;
			int prev = 9999999;
			int cluster = 0;
			//get cluster this pixel belongs to
			for (int j = 0; j < kCount; j++) {
				//create distance value of the R,G and B values per cluster
				int result = sqrt(pow((float)(red - clusterPoints[(j * 3)]), 2) + pow((float)(green - clusterPoints[(j * 3) + 1]), 2) + pow((float)(blue - clusterPoints[(j * 3) + 2]), 2));
				if (result < prev) {
					prev = result;
					cluster = j;
				}
			}
			//add pixel value to the right cluster
			clusterSum[(cluster * 3)] += red;
			clusterSum[(cluster * 3) + 1] += green;
			clusterSum[(cluster * 3) + 2] += blue;
			clusterSumCount[cluster] += 1;
		}
		//divide clusterSum to valid cluster value. TODO: FIX DIVIDE BY ZERO
		for (int cluster = 0; cluster < kCount; cluster++) {
			for (int j = 0; j < 3; j++){
				if (clusterSumCount[cluster] != 0)
					clusterSum[(cluster * 3) + j] = clusterSum[(cluster * 3) + j] / clusterSumCount[cluster];
			}
		}
		//check if iteration differs
		bool difference = false;
		for (int i = 0; i < kCount * 3; i++) {
			if (clusterSum[i] != clusterPoints[i]) {
				difference = true;
			}
		}
		iterationCount += 1;
		//no difference! We're done
		if (difference == false || iterationCount >= MAX_ITERATIONS) {
			delete[] clusterSum;
			delete[] clusterSumCount;
			break;
		}
		else {
			//set clusterPoints to clusterSum and start next iteration
			for (int i = 0; i < kCount * 3; i++) 
				clusterPoints[i] = clusterSum[i];
			delete[] clusterSum;
			delete[] clusterSumCount;
		}
	}
	byte * source = p;
	for (int i = 0; i < numberOfPixels; i++) {
		byte red = *source++;
		byte green = *source++;
		byte blue = *source++;
		int prev = 9999999;
		int cluster = 0;
		//get cluster this pixel belongs to
		for (int j = 0; j < kCount; j++) {
			//create distance value of the R,G and B values per cluster
			int result = sqrt(pow((float)(red - clusterPoints[(j * 3)]), 2) + pow((float)(green - clusterPoints[(j * 3) + 1]), 2) + pow((float)(blue - clusterPoints[(j * 3) + 2]), 2));
			if (result < prev) {
				prev = result;
				cluster = j;
			}
		}
		//modify image
		for (int i = 0; i < 3; i++)
			*d++ = clusterPoints[(cluster * 3) + i];
	}
	delete[] clusterPoints;
	
}