#include <iostream>
#include <fstream>
#include "bmp.h"

using namespace std;

class DPCM {
private:
	RGB** rgb;
	int height;
	int width;
	YCbCr** ycbcr;
	RGB** d_rgb;
	YCbCr** d_ycbcr;
	double* freq;
	double* freq_for_gist;
	double H = 0;

public:
	DPCM(int h, int w, RGB** r, YCbCr** y) {
		height = h;
		width = w;
		rgb = r;
		ycbcr = y;
		d_rgb = new RGB * [height];
		d_ycbcr = new YCbCr * [height];
		for (int i = 0; i < height; i++) {
			d_rgb[i] = new RGB[width];
			d_ycbcr[i] = new YCbCr[width];
		}
		freq = new double[256];
		freq_for_gist = new double[512];
		cout << endl << "14-16";
		cout << endl << "DPCM_left" << endl;
		modulation_left_RGB();
		cout << endl;
		modulation_left_YCbCr();
		cout << endl << "DPCM_right" << endl;
		modulation_up_RGB();
		cout << endl;
		modulation_up_YCbCr();
		cout << endl << "DPCM_up_left" << endl;
		modulation_up_left_RGB();
		cout << endl;
		modulation_up_left_YCbCr();
		cout << endl << "DPCM_average" << endl;
		modulation_average_RGB();
		cout << endl;
		modulation_average_YCbCr();
	}
	~DPCM() {
		delete freq;
		for (int i = 0; i < height; i++) {
			delete(ycbcr[i]);
			delete(rgb[i]);
		}
		delete(rgb);
		delete(ycbcr);
	}

	void clear() {
		for (int i = 0; i < 256; i++)
			freq[i] = 0;
		H = 0;
	}

	void modulation_left_RGB() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i == 0 || j == 0) {
					d_rgb[i][j].R = rgb[i][j].R;
					d_rgb[i][j].G = rgb[i][j].G;
					d_rgb[i][j].B = rgb[i][j].B;
				}
				else {
					d_rgb[i][j].R = rgb[i][j].R - rgb[i][j - 1].R;
					d_rgb[i][j].G = rgb[i][j].G - rgb[i][j - 1].G;
					d_rgb[i][j].B = rgb[i][j].B - rgb[i][j - 1].B;
				}
			}
		}
		clear();
		get_RGB('R', "14R1.txt");
		entropy(string("R"));
		clear();
		get_RGB('G', "14G1.txt");
		entropy(string("G"));
		clear();
		get_RGB('B', "14B1.txt");
		entropy(string("B"));
	}
	void modulation_left_YCbCr() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i == 0 || j == 0) {
					d_ycbcr[i][j].Y = ycbcr[i][j].Y;
					d_ycbcr[i][j].Cb = ycbcr[i][j].Cb;
					d_ycbcr[i][j].Cr = ycbcr[i][j].Cr;
				}
				else {
					d_ycbcr[i][j].Y = ycbcr[i][j].Y - ycbcr[i][j - 1].Y;
					d_ycbcr[i][j].Cb = ycbcr[i][j].Cb - ycbcr[i][j - 1].Cb;
					d_ycbcr[i][j].Cr = ycbcr[i][j].Cr - ycbcr[i][j - 1].Cr;
				}
			}
		}
		clear();
		get_YCbCr('Y', "14Y1.txt");
		entropy(string("Y"));
		clear();
		get_YCbCr('B', "14Cb1.txt");
		entropy(string("Cb"));
		clear();
		get_YCbCr('R', "14Cr1.txt");
		entropy(string("Cr"));
	}

	void modulation_up_RGB() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i == 0 || j == 0) {
					d_rgb[i][j].R = rgb[i][j].R;
					d_rgb[i][j].G = rgb[i][j].G;
					d_rgb[i][j].B = rgb[i][j].B;
				}
				else {
					d_rgb[i][j].R = rgb[i][j].R - rgb[i - 1][j].R;
					d_rgb[i][j].G = rgb[i][j].G - rgb[i - 1][j].G;
					d_rgb[i][j].B = rgb[i][j].B - rgb[i - 1][j].B;
				}
			}
		}
		clear();
		get_RGB('R', "14R2.txt");
		entropy(string("R"));
		clear();
		get_RGB('G', "14G2.txt");
		entropy(string("G"));
		clear();
		get_RGB('B', "14B2.txt");
		entropy(string("B"));
	}
	void modulation_up_YCbCr() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i == 0 || j == 0) {
					d_ycbcr[i][j].Y = ycbcr[i][j].Y;
					d_ycbcr[i][j].Cb = ycbcr[i][j].Cb;
					d_ycbcr[i][j].Cr = ycbcr[i][j].Cr;
				}
				else {
					d_ycbcr[i][j].Y = ycbcr[i][j].Y - ycbcr[i - 1][j].Y;
					d_ycbcr[i][j].Cb = ycbcr[i][j].Cb - ycbcr[i - 1][j].Cb;
					d_ycbcr[i][j].Cr = ycbcr[i][j].Cr - ycbcr[i - 1][j].Cr;
				}
			}
		}
		clear();
		get_YCbCr('Y', "14Y2.txt");
		entropy(string("Y"));
		clear();
		get_YCbCr('B', "14Cb2.txt");
		entropy(string("Cb"));
		clear();
		get_YCbCr('R', "14Cr2.txt");
		entropy(string("Cr"));
	}

	void modulation_up_left_RGB() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i == 0 || j == 0) {
					d_rgb[i][j].R = rgb[i][j].R;
					d_rgb[i][j].G = rgb[i][j].G;
					d_rgb[i][j].B = rgb[i][j].B;
				}
				else {
					d_rgb[i][j].R = rgb[i][j].R - rgb[i - 1][j - 1].R;
					d_rgb[i][j].G = rgb[i][j].G - rgb[i - 1][j - 1].G;
					d_rgb[i][j].B = rgb[i][j].B - rgb[i - 1][j - 1].B;
				}
			}
		}
		clear();
		get_RGB('R', "14R3.txt");
		entropy(string("R"));
		clear();
		get_RGB('G', "14G3.txt");
		entropy(string("G"));
		clear();
		get_RGB('B', "14B3.txt");
		entropy(string("B"));
	}
	void modulation_up_left_YCbCr() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i == 0 || j == 0) {
					d_ycbcr[i][j].Y = ycbcr[i][j].Y;
					d_ycbcr[i][j].Cb = ycbcr[i][j].Cb;
					d_ycbcr[i][j].Cr = ycbcr[i][j].Cr;
				}
				else {
					d_ycbcr[i][j].Y = ycbcr[i][j].Y - ycbcr[i - 1][j - 1].Y;
					d_ycbcr[i][j].Cb = ycbcr[i][j].Cb - ycbcr[i - 1][j - 1].Cb;
					d_ycbcr[i][j].Cr = ycbcr[i][j].Cr - ycbcr[i - 1][j - 1].Cr;
				}
			}
		}
		clear();
		get_YCbCr('Y', "14Y3.txt");
		entropy(string("Y"));
		clear();
		get_YCbCr('B', "14Cb3.txt");
		entropy(string("Cb"));
		clear();
		get_YCbCr('R', "14Cr3.txt");
		entropy(string("Cr"));
	}

	void modulation_average_RGB() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i == 0 || j == 0) {
					d_rgb[i][j].R = rgb[i][j].R;
					d_rgb[i][j].G = rgb[i][j].G;
					d_rgb[i][j].B = rgb[i][j].B;
				}
				else {
					d_rgb[i][j].R = rgb[i][j].R - (double)(rgb[i - 1][j - 1].R + rgb[i - 1][j].R + rgb[i][j - 1].R) / 3;
					d_rgb[i][j].G = rgb[i][j].G - (double)(rgb[i - 1][j - 1].G + rgb[i - 1][j].G + rgb[i][j - 1].G) / 3;
					d_rgb[i][j].B = rgb[i][j].B - (double)(rgb[i - 1][j - 1].B + rgb[i - 1][j].B + rgb[i][j - 1].B) / 3;
				}
			}
		}
		clear();
		get_RGB('R', "14R4.txt");
		entropy(string("R"));
		clear();
		get_RGB('G', "14G4.txt");
		entropy(string("G"));
		clear();
		get_RGB('B', "14B4.txt");
		entropy(string("B"));
	}
	void modulation_average_YCbCr() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i == 0 || j == 0) {
					d_ycbcr[i][j].Y = ycbcr[i][j].Y;
					d_ycbcr[i][j].Cb = ycbcr[i][j].Cb;
					d_ycbcr[i][j].Cr = ycbcr[i][j].Cr;
				}
				else {
					d_ycbcr[i][j].Y = ycbcr[i][j].Y - (double)(ycbcr[i - 1][j - 1].Y + ycbcr[i][j - 1].Y + ycbcr[i - 1][j].Y) / 3;
					d_ycbcr[i][j].Cb = ycbcr[i][j].Cb - (double)(ycbcr[i - 1][j - 1].Cb + ycbcr[i][j - 1].Cb + ycbcr[i - 1][j].Cb) / 3;
					d_ycbcr[i][j].Cr = ycbcr[i][j].Cr - (double)(ycbcr[i - 1][j - 1].Cr + ycbcr[i][j - 1].Cr + ycbcr[i - 1][j].Cr) / 3;
				}
			}
		}
		clear();
		get_YCbCr('Y', "14Y4.txt");
		entropy(string("Y"));
		clear();
		get_YCbCr('B', "14Cb4.txt");
		entropy(string("Cb"));
		clear();
		get_YCbCr('R', "14Cr4.txt");
		entropy(string("Cr"));
	}


	void get_RGB(char letter, const char* filename) {
		ofstream out;
		int count;
		out.open(filename);
		for (int i = 0; i < 512; i++) {
			freq_for_gist[i] = 0;
			//count++;
		}
		//double* freq = new double[256];
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (letter == 'R') {
					if (d_rgb[i][j].R >= 0) freq_for_gist[d_rgb[i][j].R + 256]++;
					else if (d_rgb[i][j].R < 0) {
						freq_for_gist[255 + (int)d_rgb[i][j].R]++;
						cout << " in else " << endl;
					}
					//else { freq[d_rgb[i][j].R]; }
					freq[(d_rgb[i][j].G + 256) % 256]++;
					//out << (int)d_rgb[i][j].R << endl;
					continue;
				}
				if (letter == 'G') {
					if (d_rgb[i][j].G >= 0) freq_for_gist[d_rgb[i][j].G + 256]++;
					else if (d_rgb[i][j].G < 0) freq_for_gist[255 + d_rgb[i][j].G]++;
					freq[(d_rgb[i][j].G + 256) % 256]++;
					//out << (int)d_rgb[i][j].G << endl;
					continue;
				}
				if (letter == 'B') {
					if (d_rgb[i][j].B >= 0) freq_for_gist[d_rgb[i][j].B + 256]++;
					else if (d_rgb[i][j].B < 0) freq_for_gist[255 + d_rgb[i][j].B]++;
					freq[(d_rgb[i][j].B + 256) % 256]++;
					//out << (int) d_rgb[i][j].B << endl;
					continue;
				}

			}
		}
		count = -255;
		for (int i = 0; i < 512; i++) {
			out << count << " " << freq_for_gist[i] << endl;
			count++;
		}
		out.close();
	}
	void get_YCbCr(char letter, const char* filename) {
		int count = 0;
		ofstream out;
		for (int i = 0; i < 512; i++) {
			freq_for_gist[i] = 0;
			//count++;
		}
		out.open(filename);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (letter == 'Y') {
					if ((int)d_ycbcr[i][j].Y >= 0) freq_for_gist[(int)d_ycbcr[i][j].Y + 256]++;
					else if ((int)d_ycbcr[i][j].Y < 0) {
						freq_for_gist[255 + (int)d_ycbcr[i][j].Y]++;
						//cout << " in else " << endl;
					}
					freq[((int)d_ycbcr[i][j].Y + 256) % 256]++;
					//out << d_ycbcr[i][j].Y << endl;
					//count++;
					continue;
				}
				if (letter == 'B') {
					if ((int)d_ycbcr[i][j].Cb >= 0) freq_for_gist[(int)d_ycbcr[i][j].Cb + 256]++;
					else if ((int)d_ycbcr[i][j].Cb < 0) {
						freq_for_gist[255 + (int)d_ycbcr[i][j].Cb]++;
						//cout << " in else " << endl;
					}
					freq[((int)d_ycbcr[i][j].Cb + 256) % 256]++;
					//out << d_ycbcr[i][j].Cb << endl;
					//count++;
					continue;
				}
				if (letter == 'R') {
					if ((int)d_ycbcr[i][j].Cr >= 0) freq_for_gist[(int)d_ycbcr[i][j].Cr + 256]++;
					else if ((int)d_ycbcr[i][j].Cr < 0) {
						freq_for_gist[255 + (int)d_ycbcr[i][j].Cr]++;
						//cout << " in else " << endl;
					}
					freq[((int)d_ycbcr[i][j].Cr + 256) % 256]++;
					//out << d_ycbcr[i][j].Cr << endl;
					//count++;
					continue;
				}
			}
		}
		count = -255;
		for (int i = 0; i < 512; i++) {
			out << count << " " << freq_for_gist[i] << endl;
			count++;
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