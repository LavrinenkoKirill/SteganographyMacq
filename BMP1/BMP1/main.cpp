#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable : 4996)
#include <iostream>
#include "bmp.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <bitset>
#include "main.h"
#include "7.h"
#include <cmath>

using namespace std;
std::vector<int> E_global;
std::vector<std::bitset<8>> msg;


void read_bytes(std::vector<std::bitset<8>> bytes) {
	for (int i = 0; i < msg.size(); i++) {
		std::cout << (char)msg[i].to_ullong();
	}
	std::cout << std::endl;
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


RGB** GetBack(std::vector<RGB**> blocks, int height, int width, int block_size) {
	int a = height;
	int b = width;
	int i = 0;
	int counter = 0;
	
	
	
	RGB** res = new RGB * [a];
	for (int m = 0; m < a; m++)
	{
		res[m] = new RGB[b];
	}


	int h0 = 0;
	while (h0 < height) {

		int w0 = 0;
		while (w0 < width) {
				for (int j = 0; j < block_size; j++)
					for (int k = 0; k < block_size; k++) {
						
						res[h0 + j][w0 + k].R = (unsigned char)blocks[i][j][k].R;
						res[h0 + j][w0 + k].G = (unsigned char)blocks[i][j][k].G;
						res[h0 + j][w0 + k].B = (unsigned char)blocks[i][j][k].B;
					}
			i++;
			

		 w0 = w0 + block_size;
		
		}

		h0 += block_size;
		
		
		
	}
	return res;
} 









const char mask[8][8] = {   {'A', 'A', 'B', 'B', 'A', 'A', 'B', 'B'},
							{'B', 'B', 'A', 'A', 'B', 'B', 'A', 'A'},
							{'A', 'A', 'B', 'B', 'A', 'A', 'B', 'B'},
							{'B', 'B', 'A', 'A', 'B', 'B', 'A', 'A'},
							{'A', 'A', 'B', 'B', 'A', 'A', 'B', 'B'},
							{'B', 'B', 'A', 'A', 'B', 'B', 'A', 'A'},
							{'A', 'A', 'B', 'B', 'A', 'A', 'B', 'B'},
							{'B', 'B', 'A', 'A', 'B', 'B', 'A', 'A'}, };




std::vector<RGB**> GetBlocks(RGB** image, int height, int width, int block_size) {
	int h0 = 0;
	std::vector<RGB**> res;



	while (h0 < height) {
		int w0 = 0;
		while (w0 < width)
		{
			RGB** new_rgb = new RGB * [block_size];
			for (int i = 0; i < block_size; i++)
				new_rgb[i] = new RGB[block_size];


			
			for (int i = 0; i < block_size; i++) {
				for (int j = 0; j < block_size; j++) {
					int tmp1 = image[h0 + i][w0 + j].R;
					int tmp2 = image[h0 + i][w0 + j].G;
					int tmp3 = image[h0 + i][w0 + j].B;

					new_rgb[i][j].R = tmp1;
					new_rgb[i][j].G = tmp2;
					new_rgb[i][j].B = tmp3;
				}
			}

			res.push_back(new_rgb);
			w0 += block_size;

		}
		h0 += block_size;

	}

	return res;
}

std::vector<int> GetAverageBrightness(RGB** block, int alpha, const char channel, int block_size) {

	

	std::vector<int> A1;
	std::vector<int> B1;
	std::vector<int> A2;
	std::vector<int> B2;

	std::vector<int> zone1;
	std::vector<int> zone2;

	for (int i = 0; i < block_size; i++) {
		for (int j = 0; j < block_size; j++) {
			if (channel == 'R') {
				if (block[i][j].R < alpha && mask[i][j] == 'A') A1.push_back(block[i][j].R);
				else if (block[i][j].R > alpha && mask[i][j] == 'A') A2.push_back(block[i][j].R);
				else if (block[i][j].R < alpha && mask[i][j] == 'B') B1.push_back(block[i][j].R);
				else if (block[i][j].R > alpha && mask[i][j] == 'B') B2.push_back(block[i][j].R);
			}
			else if (channel == 'G') {
				if (block[i][j].G < alpha && mask[i][j] == 'A') A1.push_back(block[i][j].G);
				else if (block[i][j].G > alpha && mask[i][j] == 'A') A2.push_back(block[i][j].G);
				else if (block[i][j].G < alpha && mask[i][j] == 'B') B1.push_back(block[i][j].G);
				else if (block[i][j].G > alpha && mask[i][j] == 'B') B2.push_back(block[i][j].G);
			}
			else if (channel == 'B') {
				if (block[i][j].B < alpha && mask[i][j] == 'A') A1.push_back(block[i][j].B);
				else if (block[i][j].B > alpha && mask[i][j] == 'A') A2.push_back(block[i][j].B);
				else if (block[i][j].B < alpha && mask[i][j] == 'B') B1.push_back(block[i][j].B);
				else if (block[i][j].B > alpha && mask[i][j] == 'B') B2.push_back(block[i][j].B);
			}
		}
	}

	for (int i = 0; i < block_size; i++) {
		for (int j = 0; j < block_size; j++) {
			if (channel == 'R') {
				if (block[i][j].R < alpha) zone1.push_back(block[i][j].R);
				else zone2.push_back(block[i][j].R);
			}
			else if (channel == 'G') {
				if (block[i][j].G < alpha) zone1.push_back(block[i][j].G);
				else zone2.push_back(block[i][j].G);
			}
			else if (channel == 'B') {
				if (block[i][j].B < alpha) zone1.push_back(block[i][j].B);
				else zone2.push_back(block[i][j].B);
			}
		}
	}

	int A1_average;
	if (A1.size() != 0) {
		A1_average = accumulate(A1.begin(), A1.end(), 0) / A1.size();
	}
	else A1_average = 0;

	int A2_average;
	if (A2.size() != 0) {
		A2_average = accumulate(A2.begin(), A2.end(), 0) / A2.size();
	}
	else A2_average = 0;

	int B1_average;
	if (B1.size() != 0) {
		B1_average = accumulate(B1.begin(), B1.end(), 0) / B1.size();
	}
	else B1_average = 0;

	int B2_average;
	if (B2.size() != 0) {
		B2_average = accumulate(B2.begin(), B2.end(), 0) / B2.size();
	}
	else B2_average = 0;

	int zone1_average;
	if (zone1.size() != 0) {
		zone1_average = accumulate(zone1.begin(), zone1.end(), 0) / zone1.size();
	}
	else zone1_average = 0;

	int zone2_average;
	if (zone2.size() != 0) {
		zone2_average = accumulate(zone2.begin(), zone2.end(), 0) / zone2.size();
	}
	else zone2_average = 0;

	int E = 0;
	if (zone1_average >= zone2_average) E = zone1_average - zone2_average;
	else E = zone2_average - zone1_average;

	std::vector<int> res;
	res.push_back(A1_average);
	res.push_back(A2_average);
	res.push_back(B1_average);
	res.push_back(B2_average);
	res.push_back(E);

	E_global.push_back(E);
	return res;

}

/*
int getBit(std::bitset<8> byte,int position) {
	int bit = (byte >> position) & 1;
}
*/

std::vector<RGB**> Encode(std::vector<RGB**> blocks, int blocks_size,  const char channel, std::string message, int height, int width, BITMAPFILEHEADER* bfh, BITMAPINFOHEADER* bih, FILE* f) {


	std::vector<vector<int>> blocks_comp;
	for (int i = 0; i < blocks.size(); i++) {
		std::vector<int> intens;
		for (int j = 0; j < blocks_size; j++) {
			for (int k = 0; k < blocks_size; k++) {
				if (channel == 'R') {
					intens.push_back(blocks[i][j][k].R);
				}
				if (channel == 'G') {
					intens.push_back(blocks[i][j][k].G);
				}
				if (channel == 'B') {
					intens.push_back(blocks[i][j][k].B);
				}
			}
		}

		blocks_comp.push_back(intens);
	}



	for (int i = 0; i < blocks_comp.size(); i++) {
		std::sort(blocks_comp[i].begin(), blocks_comp[i].end());
	}


	std::vector<int> alphas;

	for (int i = 0; i < blocks_comp.size(); i++) {
		alphas.push_back((accumulate(blocks_comp[i].begin(), blocks_comp[i].end(), 0)) / blocks_comp[i].size());
	}

	std::vector<vector<int>> block_parameters;

	for (int i = 0; i < blocks.size(); i++) {
		std::vector<int> br = GetAverageBrightness(blocks[i], alphas[i], channel, 8);
		block_parameters.push_back(br);
	}

	vector<bitset<8> > bytes;
	for (std::size_t i = 0; i < message.size(); ++i)
	{
		std::bitset<8> byte =  bitset<8>(message.c_str()[i]);
		bytes.push_back(byte);
	}

	msg = bytes;




	for (int i = 0; i < bytes.size(); i++) {
		for (int j = 0; j < bytes[i].size(); j++) {
			if (bytes[i][j] == 0) {
				//std::cout << 0;
				
					double coef1 = static_cast<double>  (block_parameters[i][0] + block_parameters[i][4]) / (block_parameters[i][2]);
					double coef2 = static_cast<double> (block_parameters[i][1] + block_parameters[i][4]) / (block_parameters[i][3]);

					//if (i == 0)std::cout << coef1 << " " << coef2 << std::endl;

					for (int j = 0; j < blocks_size; j++) {
						for (int k = 0; k < blocks_size; k++) {
							if (blocks[i][j][k].R < alphas[i] && mask[j][k] == 'B') {
								blocks[i][j][k].R = static_cast<double>(blocks[i][j][k].R * coef1);
							}
							else if (blocks[i][j][k].R > alphas[i] && mask[j][k] == 'B') blocks[i][j][k].R = static_cast<double>(blocks[i][j][k].R * coef2);

							if (blocks[i][j][k].R > 255) blocks[i][j][k].R = 255;
						}
					}

				
			}
			if (bytes[i][j] == 1) {
				//std::cout << 1;
				double coef1 = static_cast<double> (block_parameters[i][2] + block_parameters[i][4]) / (block_parameters[i][0]);
				double coef2 = static_cast<double> (block_parameters[i][3] + block_parameters[i][4]) / (block_parameters[i][1]);
				


				for (int j = 0; j < blocks_size; j++) {
					for (int k = 0; k < blocks_size; k++) {
						if (blocks[i][j][k].R < alphas[i] && mask[j][k] == 'A') blocks[i][j][k].R = static_cast<double>(blocks[i][j][k].R * coef1);
						else if (blocks[i][j][k].R > alphas[i] && mask[j][k] == 'A') blocks[i][j][k].R = static_cast<double>(blocks[i][j][k].R * coef2);
						if (blocks[i][j][k].R > 255) blocks[i][j][k].R = 255;
						
					}
				}
			}
		}
	}

	return blocks;
}

std::vector<std::bitset<8>> Decode(RGB** image, int height, int width, int blocks_size, int channel, int length) {
	std::vector<RGB**>blocks = GetBlocks(image, height, width, blocks_size);
	std::vector<vector<int>> block_parameters;

	std::vector<vector<int>> blocks_comp;


	for (int i = 0; i < blocks.size(); i++) {
		std::vector<int> intens;
		for (int j = 0; j < blocks_size; j++) {
			for (int k = 0; k < blocks_size; k++) {
				if (channel == 'R') {
					intens.push_back(blocks[i][j][k].R);
				}
				if (channel == 'G') {
					
					intens.push_back(blocks[i][j][k].G);
					
				}
				if (channel == 'B') {

					intens.push_back(blocks[i][j][k].B);
					
				}
			}
		}
		blocks_comp.push_back(intens);
	}

	for (int i = 0; i < blocks_comp.size(); i++) {
		std::sort(blocks_comp[i].begin(), blocks_comp[i].end());
	}

	

	std::vector<int> alphas;

	for (int i = 0; i < blocks_comp.size(); i++) {
		alphas.push_back((accumulate(blocks_comp[i].begin(), blocks_comp[i].end(), 0)) / blocks_comp[i].size());
	}



	for (int i = 0; i < blocks.size(); i++) {
		std::vector<int> br = GetAverageBrightness(blocks[i], alphas[i], channel, 8);
		block_parameters.push_back(br);
	}
	int counter = 0;
	
	std::vector<std::bitset<8>> bytes;
	std::bitset<8> byte;
	int bit_counter = 0;
	for (int i = 0; i < blocks.size(); i++) {
		if ((block_parameters[i][2] > block_parameters[i][0]) && (block_parameters[i][3] > block_parameters[i][1])) {
			byte[bit_counter] = 0;
			counter = counter + 1;
			bit_counter++;
		}

		else if ((block_parameters[i][0] > block_parameters[i][2]) && (block_parameters[i][1] > block_parameters[i][3])) {
			byte[bit_counter] = 1;
			counter = counter + 1;
			bit_counter++;
		}

		if (bit_counter == 8) {
			bit_counter = 0;
			bytes.push_back(byte);
		}

		if (counter == length * 8) break;
		

	}



	return bytes;

	
}




int main() {
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	FILE* f1;
	f1 = fopen("lena.bmp", "rb");
	if (f1 == NULL)
	{
		std::cout << "ERROR";
		return 0;
	}

	RGB** rgb = read_bmp(f1, &bfh, &bih);
	fclose(f1);

	int height = bih.biHeight;
	int width = bih.biWidth;
	std::vector<RGB**> blocks = GetBlocks(rgb, height, width, 8);

	std::string str = "Hello from Kirill Lavrinenko";
	std::cout << "Message: " << str << std::endl;
	std::vector<RGB**> coded = Encode(blocks, 8, 'R', str, height, width, &bfh, &bih, f1);
	
	RGB** res = GetBack(coded, height, width, 8);
	FILE* f2;
	f2 = fopen("Encoded.bmp", "wb");
	if (f2 == NULL)
	{
		std::cout << "ERROR";
		return 0;
	}  
	write_bmp(f2, res, &bfh, &bih, 512, 512);
	

	

	std::vector<std::bitset<8>> result = Decode(res, height, width, 8, 'R', str.size());
	std::cout << "Recieved message: ";
	read_bytes(result);

	PSNR_channel(width, height, rgb, res, 'R');

	return 0;
}