#include "lung.h"
#define OTSU_OVERLAY 1

#define vis 1


lung::lung()
{
	
}

lung::lung(med in)
{
	Mat temp;
	erode(in.lungmask, temp, Mat::ones(10, 10, 0));
	up_lung = cv::min(in.ct, temp == 1);
	down_lung = cv::min(in.ct, temp == 2);
	both = cv::max(up_lung, down_lung);
	cv::Rect up_rect, down_rect;
	up_rect = boundingRect(temp == 1);
	down_rect = boundingRect(temp == 2);
	up_lung = cv::Mat(up_lung, up_rect);
	down_lung = cv::Mat(down_lung, down_rect);
	hist_computed = 0;
}

lung::~lung()
{
	up_lung.release();
	down_lung.release();
	both.release();
}

Mat lung::im(int side)
{
	switch (side)
	{
	case 0: return this->both;
	case 1: return this->up_lung;
	case 2: return this->down_lung;
	default:
		return Mat::zeros(2, 2, 0);
	}
}

/*

auto student0 = get_student(0);
std::cout << "ID: 0, "
<< "GPA: " << std::get<0>(student0) << ", "
<< "grade: " << std::get<1>(student0) << ", "
<< "name: " << std::get<2>(student0) << '\n';

*/


tuple <Mat, Mat> lung::thresholded_globally(int side)
{
	Mat ans;
	int t = 0;
	if (side == 1)
		t = thresh_globally(this->up_lung, &ans);
	else if (side == 2)
		t = thresh_globally(this->down_lung, &ans);
	else
		t = thresh_globally(this->both, &ans);


	Mat visual = this->drawhist(side);
	line(visual, cv::Point(t, 0), cv::Point(t, 255), Scalar(128));
	imshow("tata", visual);

	return std::make_tuple(ans, visual);
}

tuple <Mat, Mat> lung::thresholded_otsu(int side)
{
	Mat ans;
	int t = 0;
	if (side == 1)
		t = thresh_otsu(this->up_lung, &ans);
	else if (side == 2)
		t = thresh_otsu(this->down_lung, &ans);
	else
		t = thresh_otsu(this->both, &ans);


	Mat visual = this->drawhist(side);
	line(visual, cv::Point(t, 0), cv::Point(t, 255), Scalar(128));
	imshow("tata", visual);
	
	return std::make_tuple(ans, visual);
}

tuple <Mat, Mat> lung::thresholded_multi_otsu(int side)
{
	Mat ans;
	vector<int> t;
	if (side == 1)
		t = thresh_multi_otsu(this->up_lung, &ans);
	else if (side == 2)
		t = thresh_multi_otsu(this->down_lung, &ans);
	else
		t = thresh_multi_otsu(this->both, &ans);

	Mat visual = this->drawhist(side);
	for (int i = 0; i < t.size(); i++)
	{
		line(visual, cv::Point(t[i], 0), cv::Point(t[i], 255), Scalar(128));
	}
	
	return std::make_tuple(ans, visual);
}

int lung::thresh_globally(Mat in, Mat* out)
{
	int T0 = cv::mean(in,in)[0];
	Mat temp1;
	int dT = 255;
	int T = T0;
	while (dT > 5)
	{
		threshold(in, temp1, T, 255, THRESH_BINARY);
		Mat G1_mask = (temp1 == Mat::zeros(temp1.size(), 0));
		Mat G2_mask = (temp1 == (Mat::ones(temp1.size(), 0) * 255));
		int m1 = cv::mean(in, (G1_mask&in))[0];
		int m2 = cv::mean(in, (G2_mask&in))[0];
		int T_ = T;
		T = (m1 + m2) / 2;
		dT = abs(T - T_);
	}
	threshold(in, *out, T, 255, THRESH_BINARY);
	return T;

}

int lung::thresh_otsu(Mat in, Mat* out) 
{
	vector<unsigned long long> n = lung::histogram(in);
	n[0] = 0; //disregard the 0 int.

	//Step 1: normalized histogram
	vector<float> p = normalize_(n);

	//Step 2: cumulative sums
	vector<float> P1(256);
	P1[0] = 0;
	for (int k = 1; k < 256; k++)
	{
		P1[k] = P1[k - 1] + p[k];
	}

	//Step 3: cumulative mean
	vector<float> m(256);
	m[0] = 0;
	for (int k = 1; k < 256; k++)
		m[k] = m[k - 1] + k*p[k];

	//Step 4: global intensity mean
	float mg = m[255];
	
	//Step 5: between class variance sigma
	vector<float> s_B(256);
	for (int k = 0; k < 256; k++)
		s_B[k] = (float)(mg*P1[k] - m[k])*(mg*P1[k] - m[k]) 
				 /(P1[k] * (1.0 - P1[k]));

	//Step 6: optimal k
	float max_k = 125;
	for (int k = 0; k < 256; k++)
		if (s_B[k] > s_B[max_k])
			max_k = k;

	int K = 0, num_max = 0; // k_star
	for (int k = 0; k < 256; k++)
	{
		if (s_B[k] == s_B[max_k])
		{
			K += k; num_max += 1;
		}
	}
	K  /= num_max;

	threshold(in, *out, K, 255, THRESH_BINARY);
	if (OTSU_OVERLAY)
	{
		Mat otsu_overlayed = histogram_img(n);
		for (int i = 0; i < 256; i++)
		{
			int sb = s_B[i];
			if (sb <= 0)
				sb = 0;
			otsu_overlayed.at<Vec3b>(255 - (sb * 255.0 / s_B[K]), i)[0] = 0;
			otsu_overlayed.at<Vec3b>(255 - (sb * 255.0 / s_B[K]), i)[1] = 0;
			otsu_overlayed.at<Vec3b>(255 - (sb * 255.0 / s_B[K]), i)[2] = 255;
		}
		imshow("otsu_overlay", otsu_overlayed);
	}
	return K;
}

vector<int> lung::thresh_multi_otsu(Mat in, Mat* out)
{
	vector<unsigned long long> n = lung::histogram(in);
	n[0] = 0; //disregard the 0 int.

    //Step 1: normalized histogram
	vector<float> p = normalize_(n);
	//N is number of classes
	float mg;
	{
		vector<float> m(256);
		m[0] = 0;
		for (int k = 1; k < 256; k++)
			m[k] = m[k - 1] + k*p[k];

		//Step 4: global intensity mean
		mg = m[255];
	}

	vector<int> K(5);
	vector<int> max_K(3);
	K[0] = 0;
	K[4] = 255;
	float max_s_B = -1;
	for (int a = 1; a < 125; a+=4)
	{
		cout << a << endl;
		for (int b = 126; b < 175; b+=4)
		{
			for (int c = 176; c < 256; c+=2)
			{
				K[1] = a;
				K[2] = b;
				K[3] = c;
				
				float* P = new float[4];
				for (int i = 0; i < 4; i++)
				{
					P[i] = 0;
					for (int j = K[i]; j <= K[i + 1]; j++)
					{
						P[i] += p[j];
					}
				}
				float* m = new float[4];
				for (int i = 0; i < 4; i++)
				{
					m[i] = 0;
					for (int j = K[i]; j <= K[i + 1]; j++)
					{
						m[i] += 1.0 / P[i] * j*p[j];
					}
				}

				float s_B = 0;
				for (int i = 0; i < 4; i++)
				{
					s_B += P[i] * (m[i] - mg)*(m[i] - mg);
				}
				if (s_B > max_s_B)
				{
					max_s_B = s_B;
					max_K[0] = K[1];
					max_K[1] = K[2];
					max_K[2] = K[3];
				}
			}
		}
	}
	Mat ans;
	ans.create(in.size(), in.type());
	ans = Scalar(0);
	for (int i = 0; i < 3; i++)
	{
		Mat temp;
		int thresh = max_K[i];
		threshold(in, temp, thresh, (i + 1) * 85,THRESH_BINARY);
		ans = cv::max(temp,ans);
	}
	
	*out = ans;
	imshow("ans", ans);
	return max_K;
}

vector<float> lung::normalize_(vector<unsigned long long> in)
{
	vector<float> ans(256);
	float MN = 0;
	
	for (int i = 0; i < 256; i++)
		MN += in[i];

	for (int i = 0; i < 256; i++)
		ans[i] = (float)(in[i]* 1.0 / MN);

	return ans;
}

vector<unsigned long long> lung::histogram(Mat in)
{
	vector<unsigned long long> hist(256);
	for (int i = 0; i < 256; i++)
		hist[i] = 0;

	for (int i = 0; i < in.rows; i++)
		for (int j = 0; j < in.cols; j++)
			hist[in.at<uchar>(i, j)] += 1;

	return hist;
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
		for (int x = 0; x < this->both.rows; x++)
			*(&histo_0[0] + this->both.at<uchar>(x, y)) += 1;

	for (int y = 0; y < this->up_lung.cols; y++)
		for (int x = 0; x < this->up_lung.rows; x++)
			*(&histo_1[0] + this->up_lung.at<uchar>(x, y)) += 1;

	for (int y = 0; y < this->down_lung.cols; y++)
		for (int x = 0; x < this->down_lung.rows; x++)
			*(&histo_2[0] + this->down_lung.at<uchar>(x, y)) += 1;

	histo_0[0] = 0;
	histo_1[0] = 0;
	histo_2[0] = 0;
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

cv::Mat lung::drawhist(int side)
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
	return ans;
}

Mat lung::histogram_img(vector<unsigned long long> hist)
{
	Mat ans = Mat(256, 256, CV_8UC3);
	ans = Scalar(255,255,255);
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
			ans.at<Vec3b>(255 - j, i)[0] = 0;
			ans.at<Vec3b>(255 - j, i)[1] = 0;
			ans.at<Vec3b>(255 - j, i)[2] = 0;
		}
	}
	return ans;

}