#pragma once
#include "med.h"

#define LUNG_UP 1
#define LUNG_DOWN 2
#define LUNG_BOTH 0

class lung 
{
public:
	lung();
	lung(med in);
	~lung();

	Mat up_lung; //I = 1
	Mat down_lung; //I = 2
	Mat both;

	Mat otsu(int side);
	Mat thresholded_globally(int side);
	Mat thresholded_otsu(int side);

	vector<unsigned long long> histogram(int side);
	cv::Mat drawhist(String name, int side);

	vector<unsigned long long> hist_up;
	vector<unsigned long long> hist_down;
	vector<unsigned long long> hist_both;
	int thresh_globally(Mat in, Mat* out);
	int lung::thresh_otsu(Mat in, Mat* out);
private:
	bool hist_computed;
	vector<float> normalize_(vector<unsigned long long> in);
	vector<unsigned long long> histogram(Mat in);
	Mat histogram_img(vector<unsigned long long>);
};

