#ifndef bmp
#define bmp
#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

struct BITMAPFILEHEADER {
	short bfType;
	int bfSize;
	short bfReserved1;
	short bfOffBits;;
	int bfReserved2;
};

struct BITMAPINFOHEADER {
	int biSize;
	int biWidth;
	int biHeight;
	short int biPlanes;
	short int biBitCount;
	int biCompression;
	int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	int biClrUsed;
	int biClrImportant;
};

struct RGB {
	unsigned char B = 0;
	unsigned char G = 0;
	unsigned char R = 0;
	//unsigned char rgbReserved;
};

struct YCbCr {
	double Cr;
	double Cb;
	double Y;
};




RGB** read_bmp(FILE* f, BITMAPFILEHEADER* bfh, BITMAPINFOHEADER* bih)
{
	int k = 0;
	k = fread(bfh, sizeof(*bfh) - 2, 1, f);
	if (k == 0)
	{
		std::cout << "+";
		cout << "Error";
		return 0;
	}

	k = fread(bih, sizeof(*bih), 1, f);
	if (k == NULL)
	{
		cout << "-";
		cout << "Error";
		return 0;
	}
	int a = abs(bih->biHeight);
	int b = abs(bih->biWidth);
	RGB** rgb = new RGB * [a];
	for (int i = 0; i < a; i++)
	{
		rgb[i] = new RGB[b];
	}

	int pad = 4 - (b * 3) % 4;
	for (int i = 0; i < a; i++)
	{
		fread(rgb[i], sizeof(RGB), b, f);
		
		if (pad != 4)
		{
			fseek(f, pad, SEEK_CUR);
		}
		
	}
	return rgb;
}

void write_bmp(FILE* f, RGB** rgb, BITMAPFILEHEADER* bfh, BITMAPINFOHEADER* bih, int height, int width)
{
	bih->biHeight = height;
	bih->biWidth = width;
	fwrite(bfh, sizeof(*bfh) - 2, 1, f);
	fwrite(bih, sizeof(*bih), 1, f);
	int pad = 4 - ((width) * 3) % 4;
	char buf = 0;
	for (int i = 0; i < height; i++)
	{
		fwrite((rgb[i]), sizeof(RGB), width, f);
		
		if (pad != 4)
		{
			fwrite(&buf, 1, pad, f);
		}
		
	}
}


#endif bmp
