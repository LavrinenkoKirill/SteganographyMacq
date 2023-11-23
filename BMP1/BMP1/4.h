#include <iostream>
#include <vector>
#include <fstream>
#include "bmp.h"

using namespace std;

class Corelation {
private:
	int height;
	int width;
	RGB** rgb;
	double M_R = 0;
	double M_G = 0;
	double M_B = 0;
	double sigma_R = 0;
	double sigma_G = 0;
	double sigma_B = 0;

public:
	Corelation(int h, int w, RGB** colors) {
		height = h;
		width = w;
		rgb = colors;

		cout << endl << "4" << endl;
		average();
		sigma();
		r_GB();
		r_RB();
		r_RG();

		/*
			vector<double> autocorrGreen1 = Autocorrelation_func('G', -10);
			write_txt("4G-10.txt", autocorrGreen1, width);
			cout << "G-10" << endl;

			vector<double> autocorrGreen2 = Autocorrelation_func('G', -5);
			write_txt("4G-5.txt", autocorrGreen2, width);
			cout << "G-5" << endl;

			vector<double> autocorrGreen3 = Autocorrelation_func('G', 0);
			write_txt("4G0.txt", autocorrGreen3, width);
			cout << "G0" << endl;

			vector<double> autocorrGreen4 = Autocorrelation_func('G', 5);
			write_txt("4G5.txt", autocorrGreen4, width);
			cout << "G5" << endl;

			vector<double> autocorrGreen5 = Autocorrelation_func('G', 10);
			write_txt("4G10.txt", autocorrGreen5, width);
			cout << "G10" << endl;

				vector<double> autocorrBlue1 = Autocorrelation_func('B', -10);
				write_txt("4B-10.txt", autocorrBlue1, width);
				cout << "B-10" << endl;

				vector<double> autocorrBlue2 = Autocorrelation_func('B', -5);
				write_txt("4B-5.txt", autocorrBlue2, width);
				cout << "B-5" << endl;

				vector<double> autocorrBlue3 = Autocorrelation_func('B', 0);
				write_txt("4B0.txt", autocorrBlue3, width);
				cout << "B0" << endl;

				vector<double> autocorrBlue4 = Autocorrelation_func('B', 5);
				write_txt("4B5.txt", autocorrBlue4, width);
				cout << "B5" << endl;

				vector<double> autocorrBlue5 = Autocorrelation_func('B', 10);
				write_txt("4B10.txt", autocorrBlue5, width);
				cout << "B10" << endl;


				vector<double> autocorrRed1 = Autocorrelation_func('R', -10);
				write_txt("4R-10.txt", autocorrRed1, width);
				cout << "R-10" << endl;
				vector<double> autocorrRed2 = Autocorrelation_func('R', -5);
				write_txt("4R-5.txt", autocorrRed2, width);
				cout << "R-5" << endl;
				vector<double> autocorrRed3 = Autocorrelation_func('R', 0);
				write_txt("4R0.txt", autocorrRed3, width);
				cout << "R0" << endl;
				vector<double> autocorrRed4 = Autocorrelation_func('R', 5);
				write_txt("4R5.txt", autocorrRed4, width);
				cout << "R5" << endl;
				vector<double> autocorrRed5 = Autocorrelation_func('R', 10);
				write_txt("4R10.txt", autocorrRed5, width);
				cout << "R10" << endl;
				*/

	}
	void average() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				M_B += rgb[i][j].B;
				M_R += rgb[i][j].R;
				M_G += rgb[i][j].G;
			}
		}
		M_B /= (height * width);
		M_R /= (height * width);
		M_G /= (height * width);
	}
	void sigma() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				sigma_B += pow(rgb[i][j].B - M_B, 2);
				sigma_R += pow(rgb[i][j].R - M_R, 2);
				sigma_G += pow(rgb[i][j].G - M_G, 2);
			}
		}
		sigma_B = sqrt(sigma_B / (height * width - 1));
		sigma_R = sqrt(sigma_R / (height * width - 1));
		sigma_G = sqrt(sigma_G / (height * width - 1));
	}
	void r_GB() {
		double r = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				r += (rgb[i][j].G - M_G) * (rgb[i][j].B - M_B);
			}
		}
		r /= (height * width * sigma_G * sigma_B);
		cout << "r_G,B = " << r << endl;
	}

	void r_RB() {
		double r = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				r += (rgb[i][j].R - M_R) * (rgb[i][j].B - M_B);
			}
		}
		r /= (height * width * sigma_R * sigma_B);
		cout << "r_R,B = " << r << endl;
	}

	void r_RG() {
		double r = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				r += (rgb[i][j].R - M_R) * (rgb[i][j].G - M_G);
			}
		}
		r /= (height * width * sigma_G * sigma_R);
		cout << "r_G,R = " << r << endl;
	}


	vector<double> Autocorrelation_func(char letter, int y) {
		vector<double> array(width / 4);
		vector<vector<double>> color(height);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (letter == 'R') {
					color[i].push_back(rgb[i][j].R);
					continue;
				}
				if (letter == 'G') {
					color[i].push_back(rgb[i][j].G);
					continue;
				}
				if (letter == 'B') {
					color[i].push_back(rgb[i][j].B);
					continue;
				}
			}
		}

		for (int x = -width / 4, counter = 0; x < width / 4; x += 2, counter++) {
			vector<vector<double>> firstSelection = calculateFirstSelection(color, x, y);
			vector<vector<double>> secondSelection = calculateSecondSelection(color, x, y);
			array[counter] = Correlation_func(firstSelection, secondSelection);
		}
		return array;
	}

	vector<vector<double>> calculateFirstSelection(vector<vector<double>>& array, int x, int y) {
		int width = array.size();
		int height = array[0].size();
		vector<vector<double>> result(width);

		for (int i = 0; i < width; ++i) {
			result[i].resize(height);
		}

		if (y >= 0) {
			if (x >= 0) {
				for (int j = 0; j < width - x; ++j) {
					for (int i = 0; i < height - y; ++i) {
						result[j][i] = array[j][i];
					}
				}
			}
			else {
				for (int j = -x; j < width; ++j) {
					for (int i = 0; i < height - y; ++i) {
						result[j + x][i] = array[j][i];
					}
				}
			}
		}
		else {
			if (x >= 0) {
				for (int j = 0; j < width - x; ++j) {
					for (int i = -y; i < height; ++i) {
						result[j][i + y] = array[j][i];
					}
				}
			}
			else {
				for (int j = -x; j < width; j++) {
					for (int i = -y; i < height; i++) {
						result[j + x][i + y] = array[j][i];
					}
				}
			}
		}
		return result;
	}

	vector<vector<double>> calculateSecondSelection(vector<vector<double>>& array, int x, int y) {
		int height = array[0].size();
		int width = array.size();
		vector<vector<double>> result(width);

		for (int i = 0; i < width; ++i) {
			result[i].resize(height);
		}
		if (y >= 0) {
			if (x >= 0) {
				for (int j = x; j < width; ++j) {
					for (int i = y; i < height; ++i) {
						result[j - x][i - y] = array[j][i];
					}
				}
			}
			else {
				for (int j = 0; j < width + x; ++j) {
					for (int i = y; i < height; ++i) {
						result[j][i - y] = array[j][i];
					}
				}
			}
		}
		else {
			if (x >= 0) {
				for (int j = x; j < width; ++j) {
					for (int i = 0; i < height + y; ++i) {
						result[j - x][i] = array[j][i];
					}
				}
			}
			else {
				for (int j = 0; j < width + x; ++j) {
					for (int i = 0; i < height + y; ++i) {
						result[j][i] = array[j][i];
					}
				}
			}
		}
		return result;
	}

	double Correlation_func(vector<vector<double>> A, vector<vector<double>> B) {
		double m1 = Get_expected_value(A);
		double m2 = Get_expected_value(B);

		double d1 = Get_dispersion(A);
		double d2 = Get_dispersion(B);

		for (int i = 0; i < A.size(); i++) {
			for (int j = 0; j < A[0].size(); j++) {

				if (A[i].size() != 0 && B[i].size() != 0) {
					A[i][j] = A[i][j] - m1;
					B[i][j] = B[i][j] - m2;
					A[i][j] = A[i][j] * B[i][j];
				}
			}
		}
		double res = Get_expected_value(A) / (d1 * d2);
		return res;
	}

	double Get_expected_value(vector<vector<double>> rgb) {
		double res = 0;
		double WH = (double)width * (double)height;

		for (int i = 0; i < rgb.size(); i++) {
			for (int j = 0; j < rgb[0].size(); j++) {
				if (rgb[i].size() != 0)
					res += rgb[i][j];
			}
		}
		res = res / WH;
		return res;
	}
	double Get_dispersion(vector<vector<double>> rgb) {
		double res = 0;
		double WH = (double)width * (double)height;
		double m = Get_expected_value(rgb);

		for (int i = 0; i < rgb.size(); i++) {
			for (int j = 0; j < rgb[0].size(); j++) {
				if (rgb[i].size() != 0)
					res += pow((rgb[i][j] - m), 2);
			}
		}
		res = res / (WH - 1);
		return sqrt(res);
	}

	void write_txt(string filename, vector<double> result, int width) {
		ofstream fout;
		fout.open(filename);
		int count = 0;
		if (fout.is_open()) {
			int j = 0;
			for (int i = -width / 4; i < width / 4; i += 2) {
				fout << i << " " << result[j] << "\n";
				j++;
			}
		}
		fout.close();
	}
};
