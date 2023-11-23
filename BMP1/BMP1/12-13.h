#include <iostream>
#include <fstream>
#include "bmp.h"

using namespace std;

class Frequency {
private:
	int height;
	int width;
	RGB** rgb;
	YCbCr** ycbcr;
	double* freq;
	double H = 0;
public:
	Frequency(RGB** RGB, int h, int w, YCbCr** YCbCr) {
		rgb = RGB;
		ycbcr = YCbCr;
		height = h;
		width = w;
		freq = new double[256];
		cout << endl << "12-13" << endl;
		clear();
		get_RGB('R', "12R.txt");
		entropy(string("R"));
		clear();
		get_RGB('G', "12G.txt");
		entropy(string("G"));
		clear();
		get_RGB('B', "12B.txt");
		entropy(string("B"));
		clear();
		get_YCbCr('Y', "12Y.txt");
		entropy(string("Y"));
		clear();
		get_YCbCr('B', "12Cb.txt");
		entropy(string("Cb"));
		clear();
		get_YCbCr('R', "12Cr.txt");
		entropy(string("Cr"));
	}
	void clear() {
		for (int i = 0; i < 256; i++)
			freq[i] = 0;
		H = 0;
	}
	void get_RGB(char letter, const char* filename) {
		ofstream out;
		out.open(filename);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (letter == 'R')

					freq[rgb[i][j].R]++;
				if (letter == 'G')

					freq[rgb[i][j].G]++;
				if (letter == 'B')

					freq[rgb[i][j].B]++;
			}
		}
		for (int i = 0; i < 256; i++) {
			out << i << " " << freq[i] << endl;
		}
		out.close();
	}
	void get_YCbCr(char letter, const char* filename) {
		ofstream out;
		out.open(filename);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (letter == 'Y')

					freq[(int)ycbcr[i][j].Y]++;
				if (letter == 'B')

					freq[(int)ycbcr[i][j].Cb]++;
				if (letter == 'R')

					freq[(int)ycbcr[i][j].Cr]++;
			}
		}
		for (int i = 0; i < 256; i++) {
			out << i << " " << freq[i] << endl;
		}
		out.close();
	}
	void entropy(string letter) {
		for (int i = 0; i < 256; i++) {
			double p = (double)freq[i] / (height * width);
			if (p != 0) {
				H += p * log2(p);
			}
		}
		H *= (-1);
		cout << "H(" << letter << ") = " << H << endl;

	}
};
