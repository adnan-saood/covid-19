#pragma once
#include "C:\ocv42\build\include\opencv2\core\mat.hpp"
#include "headers.h"
class med :
	public cv::Mat
{
public:
	med();
	med(Mat ct);
	med(Mat ct, Mat mask, Mat lungmask);
	~med();

	vector<unsigned long long> histogram();
	Mat otsu();

	cv::Mat drawhist(String name);

	Mat ct;
	Mat mask;
	Mat lungmask;

	vector<unsigned long long> hist;

private:
	bool hist_computed;
};

