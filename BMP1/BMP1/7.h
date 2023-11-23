#include <iostream>
#include <cmath>
#include "bmp.h"


class Reverse_conversion {
private:
	RGB** new_rgb;
	RGB** rgb;
	BITMAPFILEHEADER* bfh;
	BITMAPINFOHEADER* bih;
	int height;
	int width;
	YCbCr** ycbcr;
	unsigned char max_R = 0;
	unsigned char max_G = 0;
	unsigned char max_B = 0;
	unsigned char min_R = 255;
	unsigned char min_G = 255;
	unsigned char min_B = 255;

public:
	Reverse_conversion(BITMAPFILEHEADER* bfh, BITMAPINFOHEADER* bih, YCbCr** y, int h, int w, RGB** colors) {
		height = h;
		width = w;
		this->bfh = bfh;
		this->bih = bih;
		ycbcr = y;
		rgb = colors;
		new_rgb = new RGB * [height];
		for (int i = 0; i < height; i++) {
			new_rgb[i] = new RGB[width];
			for (int j = 0; j < width; j++) {
				new_rgb[i][j].B = 0;
				new_rgb[i][j].R = 0;
				new_rgb[i][j].G = 0;
			}
		}
		cout << endl << "7" << endl;
		find_min_max();
		reverse_conversion();
		PSNR();
	}
	~Reverse_conversion() {
		for (int i = 0; i < height; i++)
			delete(new_rgb[i]);
		delete(new_rgb);
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
		if (color > max_B)
			return max_B;
		if (color < (double)min_G)
			return min_B;
		return (unsigned char)(color);
	}
	void reverse_conversion() {
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
		file = fopen("7Recovered.bmp", "wb");
		write_bmp(file, new_rgb, bfh, bih, height, width);
		fclose(file);
	}
	void PSNR() {
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
		cout << "PSNR_B = " << PSNR_B << endl;
		cout << "PSNR_G = " << PSNR_G << endl;
		cout << "PSNR_R = " << PSNR_R << endl;
	}


	void PSNR_channel(int width, int height, RGB** rgb, RGB** new_rgb, char channel) {
		double tmp = width * height * pow(256 - 1, 2);
		double PSNR = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (channel == 'R') {
					PSNR += pow((rgb[i][j].R - new_rgb[i][j].R), 2);
				}

				else if (channel == 'B') {
					PSNR += pow((rgb[i][j].B - new_rgb[i][j].B), 2);
				}

				else if (channel == 'G') {
					PSNR += pow((rgb[i][j].G - new_rgb[i][j].G), 2);
				}

				
				
				
			}
		}
		PSNR = 10 * log10(tmp / PSNR);
		cout << "PSNR = " << PSNR << endl;
	}
	
};
