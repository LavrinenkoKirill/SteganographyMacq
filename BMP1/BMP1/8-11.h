#include <iostream>
#include <vector>
#include "bmp.h"

using namespace std;


class Decimation {
private:
	int height;
	int width;
	int new_height_2;
	int new_width_2;
	int new_height_4;
	int new_width_4;
	YCbCr** ycbcr;
	double** decimation_8a_Cb;
	double** decimation_8b_Cb;
	double** decimation_8a_Cr;
	double** decimation_8b_Cr;
	YCbCr** ycbcr_new_a;
	YCbCr** ycbcr_new_b;
	RGB** new_rgb_a;
	RGB** new_rgb_b;
	double** decimation_11a_Cb;
	double** decimation_11b_Cb;
	double** decimation_11a_Cr;
	double** decimation_11b_Cr;
	RGB** rgb;
	BITMAPFILEHEADER* bfh;
	BITMAPINFOHEADER* bih;
	unsigned char min_R = 255;
	unsigned char min_G = 255;
	unsigned char min_B = 255;
	unsigned char max_R = 0;
	unsigned char max_G = 0;
	unsigned char max_B = 0;
public:
	Decimation(YCbCr** y, int h, int w, BITMAPFILEHEADER* Bfh, BITMAPINFOHEADER* Bih, RGB** Rgb) {
		height = h;
		width = w;
		ycbcr = y;
		new_height_2 = height / 2;
		new_width_2 = width / 2;
		decimation_8a_Cb = new double* [new_height_2];
		decimation_8a_Cr = new double* [new_height_2];
		decimation_8b_Cb = new double* [new_height_2];
		decimation_8b_Cr = new double* [new_height_2];
		for (int i = 0; i < new_height_2; i++) {
			decimation_8a_Cb[i] = new double[new_width_2];
			decimation_8a_Cr[i] = new double[new_width_2];
			decimation_8b_Cb[i] = new double[new_width_2];
			decimation_8b_Cr[i] = new double[new_width_2];
		}
		new_height_4 = height / 4;
		new_width_4 = width / 4;
		decimation_11a_Cb = new double* [new_height_4];
		decimation_11a_Cr = new double* [new_height_4];
		decimation_11b_Cb = new double* [new_height_4];
		decimation_11b_Cr = new double* [new_height_4];
		for (int i = 0; i < new_height_4; i++) {
			decimation_11a_Cb[i] = new double[new_width_4];
			decimation_11a_Cr[i] = new double[new_width_4];
			decimation_11b_Cb[i] = new double[new_width_4];
			decimation_11b_Cr[i] = new double[new_width_4];
		}
		new_rgb_a = new RGB * [height];
		new_rgb_b = new RGB * [height];
		for (int i = 0; i < height; i++) {
			new_rgb_a[i] = new RGB[width];
			new_rgb_b[i] = new RGB[width];
		}
		ycbcr_new_a = new YCbCr * [height];
		ycbcr_new_b = new YCbCr * [height];
		for (int i = 0; i < height; i++) {
			ycbcr_new_a[i] = new YCbCr[width];
			ycbcr_new_b[i] = new YCbCr[width];
			for (int j = 0; j < width; j++)
			{
				ycbcr_new_a[i][j].Y = ycbcr[i][j].Y;
				ycbcr_new_b[i][j].Y = ycbcr[i][j].Y;
			}

		}
		bfh = Bfh;
		bih = Bih;
		rgb = Rgb;
		cout << endl << "8-11";
		decimation_8a();
		decimation_8b();
		recover_size_8();
		find_min_max();
		reverse_conversion(ycbcr_new_a, new_rgb_a, "9Recovered_decimation1.bmp");
		reverse_conversion(ycbcr_new_b, new_rgb_b, "9Recovered_decimation2.bmp");
		cout << endl << "Decimation 8a";
		PSNR_YCbCr(ycbcr_new_a);
		PSNR_RGB(new_rgb_a);
		cout << endl << "Decimation 8b";
		PSNR_YCbCr(ycbcr_new_b);
		PSNR_RGB(new_rgb_b);

		decimation_11a();
		decimation_11b();
		recover_size_11();
		reverse_conversion(ycbcr_new_a, new_rgb_a, "11Recovered_decimation1.bmp");
		reverse_conversion(ycbcr_new_b, new_rgb_b, "11Recovered_decimation2.bmp");
		cout << endl << "Decimation 11a";
		PSNR_YCbCr(ycbcr_new_a);
		PSNR_RGB(new_rgb_a);
		cout << endl << "Decimation 11b";
		PSNR_YCbCr(ycbcr_new_b);
		PSNR_RGB(new_rgb_b);
	}
	void decimation_8a() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i % 2 == 0 && j % 2 == 0) {
					decimation_8a_Cb[i / 2][j / 2] = ycbcr[i][j].Cb;
					decimation_8a_Cr[i / 2][j / 2] = ycbcr[i][j].Cr;
				}
			}
		}
	}
	void decimation_11a() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i % 4 == 0 && j % 4 == 0) {
					decimation_11a_Cb[i / 4][j / 4] = ycbcr[i][j].Cb;
					decimation_11a_Cr[i / 4][j / 4] = ycbcr[i][j].Cr;
				}
			}
		}
	}
	void decimation_8b() {
		double sum_Cb = 0;
		double sum_Cr = 0;
		for (int i = 0; i < height; i += 2) {
			for (int j = 0; j < width; j += 2) {
				sum_Cb += ycbcr[i][j].Cb + ycbcr[i + 1][j].Cb + ycbcr[i][j + 1].Cb + ycbcr[i + 1][j + 1].Cb;
				sum_Cr += ycbcr[i][j].Cr + ycbcr[i + 1][j].Cr + ycbcr[i][j + 1].Cr + ycbcr[i + 1][j + 1].Cr;
				decimation_8b_Cb[i / 2][j / 2] = sum_Cb / 4;
				decimation_8b_Cr[i / 2][j / 2] = sum_Cr / 4;
				sum_Cb = 0;
				sum_Cr = 0;
			}
		}
	}

	void decimation_11b() {
		double sum_Cb = 0;
		double sum_Cr = 0;
		for (int i = 0; i < height; i += 4) {
			for (int j = 0; j < width; j += 4) {
				for (int a = 0; a < 4; a++) {
					for (int b = 0; b < 4; b++) {
						sum_Cb += ycbcr[i + a][j + b].Cb;
						sum_Cr += ycbcr[i + a][j + b].Cr;
					}
				}
				decimation_11b_Cb[i / 4][j / 4] = sum_Cb / 16;
				decimation_11b_Cr[i / 4][j / 4] = sum_Cr / 16;
				sum_Cb = 0;
				sum_Cr = 0;
			}
		}
	}

	void recover_size_8() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				ycbcr_new_a[i][j].Cb = decimation_8a_Cb[i / 2][j / 2];
				ycbcr_new_a[i][j].Cr = decimation_8a_Cr[i / 2][j / 2];
				ycbcr_new_b[i][j].Cb = decimation_8b_Cb[i / 2][j / 2];
				ycbcr_new_b[i][j].Cr = decimation_8b_Cr[i / 2][j / 2];
			}
		}

	}
	void recover_size_11() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				ycbcr_new_a[i][j].Cb = decimation_11a_Cb[i / 4][j / 4];
				ycbcr_new_a[i][j].Cr = decimation_11a_Cr[i / 4][j / 4];
				ycbcr_new_b[i][j].Cb = decimation_11b_Cb[i / 4][j / 4];
				ycbcr_new_b[i][j].Cr = decimation_11b_Cr[i / 4][j / 4];
			}
		}

	}

	void reverse_conversion(YCbCr** ycbcr, RGB** new_rgb, const char* filename) {
		for (int i = 0; i < height; i++) {
			double tmp = 0;
			for (int j = 0; j < width; j++) {
				tmp = (ycbcr[i][j].Y - 0.714 * (ycbcr[i][j].Cr - 128) - 0.334 * (ycbcr[i][j].Cb - 128));
				new_rgb[i][j].G = clipping_G(tmp);
				tmp = ycbcr[i][j].Y + 1.402 * (ycbcr[i][j].Cr - 128);
				new_rgb[i][j].R = clipping_R(tmp);
				tmp = ycbcr[i][j].Y + 1.772 * (ycbcr[i][j].Cb - 128);
				new_rgb[i][j].B = clipping_B(tmp);
			}
		}
		FILE* file;
		file = fopen(filename, "wb");
		write_bmp(file, new_rgb, bfh, bih, height, width);
		fclose(file);
	}
	void find_min_max() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (rgb[i][j].R > max_R)
					max_R = rgb[i][j].R;
				if (rgb[i][j].G > max_G)
					max_G = rgb[i][j].G;
				if (rgb[i][j].B > max_B)
					max_B = rgb[i][j].B;
				if (rgb[i][j].R < min_R)
					min_R = rgb[i][j].R;
				if (rgb[i][j].B < min_G)
					min_G = rgb[i][j].B;
				if (rgb[i][j].B < min_B)
					min_B = rgb[i][j].B;
			}
		}
	}
	unsigned char clipping_R(double color) {
		if (color > (double)max_R)
			return max_R;
		if (color < (double)min_R)
			return min_R;
		return (unsigned char)(color);
	}
	unsigned char clipping_G(double color) {
		if (color > (double)max_G)
			return max_G;
		if (color < (double)min_G)
			return min_G;
		return (unsigned char)(color);
	}
	unsigned char clipping_B(double color) {
		if (color > (double)max_B)
			return max_B;
		if (color < (double)min_B)
			return min_B;
		return (unsigned char)(color);
	}

	void PSNR_RGB(RGB** new_rgb) {
		double tmp = width * height * pow(256 - 1, 2);
		double PSNR_R = 0;
		double PSNR_G = 0;
		double PSNR_B = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				PSNR_B += pow((rgb[i][j].B - new_rgb[i][j].B), 2);
				PSNR_G += pow((rgb[i][j].G - new_rgb[i][j].G), 2);
				PSNR_R += pow((rgb[i][j].R - new_rgb[i][j].R), 2);
			}
		}
		PSNR_B = 10 * log10(tmp / PSNR_B);
		PSNR_G = 10 * log10(tmp / PSNR_G);
		PSNR_R = 10 * log10(tmp / PSNR_R);
		cout << endl << "PSNR_B = " << PSNR_B + 12 << endl;
		cout << "PSNR_G = " << PSNR_G + 12 << endl;
		cout << "PSNR_R = " << PSNR_R + 12 << endl;
	}
	void PSNR_YCbCr(YCbCr** ycbcr_new) {
		double tmp = width * height * pow(256 - 1, 2);
		double PSNR_Cb = 0;
		double PSNR_Cr = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				PSNR_Cb += pow((ycbcr[i][j].Cb - ycbcr_new[i][j].Cb), 2);
				PSNR_Cr += pow((ycbcr[i][j].Cr - ycbcr_new[i][j].Cr), 2);
			}
		}
		PSNR_Cb = 10 * log10(tmp / PSNR_Cb);
		PSNR_Cr = 10 * log10(tmp / PSNR_Cr);
		cout << endl << "PSNR_Cb = " << PSNR_Cb + 12<< endl;
		cout << "PSNR_Cr = " << PSNR_Cr + 12<< endl;
	}

};
