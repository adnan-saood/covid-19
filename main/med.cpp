#include "med.h"



med::med()
{
	hist_computed = 0;
}

med::med(Mat ct)
{
	med::ct = ct;
	hist_computed = 0;
}

med::med(Mat ct, Mat mask, Mat lungmask)
{
	med::ct = ct;
	med::mask = mask;
	med::lungmask = lungmask;
	hist_computed = 0;
}

med::~med()
{
}

vector<unsigned long long> med::histogram()
{
	vector<unsigned long long> histo(256);
	if (hist_computed)
	{
		return this->hist;
	}
	for (int i = 0; i < 256; i++)
	{
		histo[i] = 0;
	}

	for (int y = 0; y < this->ct.cols; y++)
	{
		for (int x = 0; x < this->ct.rows; x++)
		{
			*(&histo[0] + this->ct.at<uchar>(x, y)) += 1;
		}
	}
	this->hist_computed = true;
	this->hist = histo;
	return histo;

}

cv::Mat med::drawhist(String name)
{
	hist = this->histogram();
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
