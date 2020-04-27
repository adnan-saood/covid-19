#pragma once

#include "headers.h"
class med
{
public:
	med();
	med(Mat ct);
	med(Mat ct, Mat mask, Mat lungmask);
	~med();

	vector<unsigned long long> histogram();
	Mat otsu();
	Mat thresholded_globally();

	cv::Mat drawhist(String name);

	Mat ct;
	Mat mask;
	Mat lungmask;

	vector<unsigned long long> hist;

private:
	bool hist_computed;
};

