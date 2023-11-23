#include <iostream>
#include "bmp.h"

using namespace std;

class Get_color {
private:
	RGB** rgb;
	RGB** new_rgb;
	BITMAPFILEHEADER* bfh;
	BITMAPINFOHEADER* bih;
	int height;
	int width;
public:
	Get_color(BITMAPFILEHEADER* bfh, BITMAPINFOHEADER* bih, RGB** color, int h, int w) {
		height = h;
		width = w;
		this->bfh = bfh;
		this->bih = bih;
		rgb = color;
		new_rgb = new RGB * [height];
		for (int i = 0; i < height; i++)
			new_rgb[i] = new RGB[width];
		cout << endl << "3";
		get_R();
		get_G();
		get_B();
	}
	~Get_color() {
		for (int i = 0; i < height; i++)
			delete(new_rgb[i]);
		delete(new_rgb);
	}
	void get_R() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				new_rgb[i][j].B = 0;
				new_rgb[i][j].G = 0;
				new_rgb[i][j].R = rgb[i][j].R;
			}
		}
		FILE* file;
		file = fopen("3Red.bmp", "wb");
		write_bmp(file, new_rgb, bfh, bih, height, width);
		fclose(file);
	}

	void get_G() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				new_rgb[i][j].B = 0;
				new_rgb[i][j].R = 0;
				new_rgb[i][j].G = rgb[i][j].G;
			}
		}
		FILE* file;
		file = fopen("3Green.bmp", "wb");
		write_bmp(file, new_rgb, bfh, bih, height, width);
		fclose(file);
	}

	void get_B() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				new_rgb[i][j].R = 0;
				new_rgb[i][j].G = 0;
				new_rgb[i][j].B = rgb[i][j].B;
			}
		}
		FILE* file;
		file = fopen("3Blue.bmp", "wb");
		write_bmp(file, new_rgb, bfh, bih, height, width);
		fclose(file);
	}
};
