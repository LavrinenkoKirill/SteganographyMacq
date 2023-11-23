#include <iostream>
#include "bmp.h"

using namespace std;


class Direct_conversion {
private:
	RGB** rgb;
	RGB** new_rgb;
	BITMAPFILEHEADER* bfh;
	BITMAPINFOHEADER* bih;
	int height;
	int width;
	YCbCr** ycbcr;
	double M_Y = 0;
	double M_Cb = 0;
	double M_Cr = 0;
	double sigma_Y = 0;
	double sigma_Cb = 0;
	double sigma_Cr = 0;
public:
	Direct_conversion(BITMAPFILEHEADER* bfh, BITMAPINFOHEADER* bih, RGB** color, int h, int w) {
		height = h;
		width = w;
		this->bfh = bfh;
		this->bih = bih;
		rgb = color;
		new_rgb = new RGB * [height];
		for (int i = 0; i < height; i++)
			new_rgb[i] = new RGB[width];
		cout << endl << "5-6" << endl;
		get_direct_conversion();
		get_Y();
		get_Cb();
		get_Cr();

		average();
		sigma();
		r_YCb();
		r_YCr();
		r_CbCr();

	}
	~Direct_conversion() {
		for (int i = 0; i < height; i++)
			delete(new_rgb[i]);
		delete(new_rgb);
	}
	void get_direct_conversion() {
		ycbcr = new YCbCr * [height];
		for (int i = 0; i < height; i++) {
			ycbcr[i] = new YCbCr[width];
			for (int j = 0; j < width; j++) {
				ycbcr[i][j].Y = 0.299 * (double)rgb[i][j].R + 0.587 * (double)rgb[i][j].G + 0.114 * (double)rgb[i][j].B;
				ycbcr[i][j].Cb = 0.5643 * ((double)rgb[i][j].B - ycbcr[i][j].Y) + 128;
				ycbcr[i][j].Cr = 0.7132 * ((double)rgb[i][j].R - ycbcr[i][j].Y) + 128;
			}
		}
	}

	void get_Y() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				new_rgb[i][j].B = ycbcr[i][j].Y;
				new_rgb[i][j].G = ycbcr[i][j].Y;
				new_rgb[i][j].R = ycbcr[i][j].Y;
			}
		}
		FILE* file;
		file = fopen("6Y.bmp", "wb");
		write_bmp(file, new_rgb, bfh, bih, height, width);
		fclose(file);
	}

	void get_Cb() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				new_rgb[i][j].B = ycbcr[i][j].Cb;
				new_rgb[i][j].G = ycbcr[i][j].Cb;
				new_rgb[i][j].R = ycbcr[i][j].Cb;
			}
		}
		FILE* file;
		file = fopen("6Cb.bmp", "wb");
		write_bmp(file, new_rgb, bfh, bih, height, width);
		fclose(file);
	}
	void get_Cr() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				new_rgb[i][j].B = ycbcr[i][j].Cr;
				new_rgb[i][j].G = ycbcr[i][j].Cr;
				new_rgb[i][j].R = ycbcr[i][j].Cr;
			}
		}
		FILE* file;
		file = fopen("6Cr.bmp", "wb");
		write_bmp(file, new_rgb, bfh, bih, height, width);
		fclose(file);
	}

	YCbCr** get_YCbCr() {
		return ycbcr;
	}

	void average() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				M_Y += ycbcr[i][j].Y;
				M_Cb += ycbcr[i][j].Cb;
				M_Cr += ycbcr[i][j].Cr;
			}
		}
		M_Y /= (height * width);
		M_Cb /= (height * width);
		M_Cr /= (height * width);
	}
	void sigma() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				sigma_Y += pow(ycbcr[i][j].Y - M_Y, 2);
				sigma_Cb += pow(ycbcr[i][j].Cb - M_Cb, 2);
				sigma_Cr += pow(ycbcr[i][j].Cr - M_Cr, 2);
			}
		}
		sigma_Y = sqrt(sigma_Y / (height * width - 1));
		sigma_Cb = sqrt(sigma_Cb / (height * width - 1));
		sigma_Cr = sqrt(sigma_Cr / (height * width - 1));
	}
	void r_YCb() {
		double r = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				r += (ycbcr[i][j].Y - M_Y) * (ycbcr[i][j].Cb - M_Cb);
			}
		}
		r /= (height * width * sigma_Cb * sigma_Y);
		cout << "r_Y,Cb = " << r << endl;
	}

	void r_YCr() {
		double r = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				r += (ycbcr[i][j].Y - M_Y) * (ycbcr[i][j].Cr - M_Cr);
			}
		}
		r /= (height * width * sigma_Cr * sigma_Y);
		cout << "r_Y,Cr = " << r << endl;
	}

	void r_CbCr() {
		double r = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				r += (ycbcr[i][j].Cr - M_Cr) * (ycbcr[i][j].Cb - M_Cb);
			}
		}
		r /= (height * width * sigma_Cb * sigma_Cr);
		cout << "r_Cr,Cb = " << r << endl;
	}



};
