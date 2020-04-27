#include "lung.h"



lung::lung()
{
	
}

lung::lung(med in)
{
	up_lung = cv::min(in.ct, in.lungmask == 1);
	down_lung = cv::min(in.ct, in.lungmask == 2);
	both = cv::max(up_lung, down_lung);
	hist_computed = 0;

}


lung::~lung()
{
}


Mat lung::otsu(int side)
{
	if (side == 1)
	{
		Mat ans;
		cv::threshold(this->up_lung, ans, 0, 255, THRESH_OTSU);
		return ans;
	}
	else if (side == 2)
	{
		Mat ans;
		cv::threshold(this->down_lung, ans, 0, 255, THRESH_OTSU);
		return ans;
	}
	else
	{
		Mat ans;
		cv::threshold(this->both, ans, 0, 255, THRESH_OTSU);
		return ans;
	}
}

Mat lung::thresholded_globally(int side)
{
	if (side == 1)
	{
		Mat ans;
		cv::adaptiveThreshold(this->up_lung, ans, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);
		return ans;
	}
	else if (side == 2)
	{
		Mat ans;
		cv::adaptiveThreshold(this->down_lung, ans, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);
		return ans;
	}
	else 
	{
		Mat ans;
		cv::adaptiveThreshold(this->both, ans, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);
		return ans;
	}
}



vector<unsigned long long> lung::histogram(int side)
{
	if (hist_computed)
	{
		if (side == 0)
			return hist_both;
		else if (side == 1)
			return hist_up;
		else if (side == 2)
			return hist_down;
	}

	vector<unsigned long long> histo_0(256);
	vector<unsigned long long> histo_1(256);
	vector<unsigned long long> histo_2(256);
	for (int i = 0; i < 256; i++)
	{
		histo_0[i] = 0;
		histo_1[i] = 0;
		histo_2[i] = 0;
	}

	for (int y = 0; y < this->both.cols; y++)
	{
		for (int x = 0; x < this->both.rows; x++)
		{
			*(&histo_0[0] + this->both.at<uchar>(x, y)) += 1;
			*(&histo_1[0] + this->up_lung.at<uchar>(x, y)) += 1;
			*(&histo_2[0] + this->down_lung.at<uchar>(x, y)) += 1;
		}
	}
	this->hist_computed = true;
	this->hist_both = histo_0;
	this->hist_up = histo_1;
	this->hist_down = histo_2;
	if (side == 0)
		return hist_both;
	else if (side == 1)
		return hist_up;
	else
		return hist_down;

}

cv::Mat lung::drawhist(String name, int side)
{
	vector<unsigned long long> hist;
	hist = this->histogram(side);

	Mat ans = Mat(256, 256, CV_8UC1);
	ans = Scalar(255);
	long long max1 = 0;
	for (int i = 0; i < 256; i++)
	{
		max1 = (hist[i] > max1 ? hist[i] : max1);
	}
	for (int i = 0; i < 256; i++)
	{
		double temp = hist[i] * 255.0 / max1;
		hist[i] = temp;
	}

	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j <= hist[i]; j++)
		{
			ans.at<uchar>(255 - j, i) = 0;
		}
	}
	imshow(name, ans);
	return ans;
}
