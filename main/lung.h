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
	
	tuple <Mat, Mat> thresholded_globally(int side);
	tuple <Mat, Mat> thresholded_otsu(int side);
	tuple <Mat, Mat> thresholded_multi_otsu(int side);

	vector<unsigned long long> histogram(int side = 0);
	cv::Mat drawhist(int side);

	Mat im(int side);

	Mat up_lung; //I = 1
	Mat down_lung; //I = 2
	Mat both;

private:

	bool hist_computed;


	vector<unsigned long long> hist_up;
	vector<unsigned long long> hist_down;
	vector<unsigned long long> hist_both;

	vector<float> normalize_(vector<unsigned long long> in);
	vector<unsigned long long> histogram(Mat in);
	Mat histogram_img(vector<unsigned long long>);


	int thresh_globally(Mat in, Mat* out);
	int thresh_otsu(Mat in, Mat* out);
	vector<int> thresh_multi_otsu(Mat in, Mat* out);

};

