#pragma once
#include "imglist.h"
#include"headers.h"
#include"lung.h"

class prediction
{
public:
	prediction();
	prediction(imglist, imglist, imglist, int);
	~prediction();


	void do_stuff(); // delete later


private:
	int method;
	imglist ct;
	imglist mask;
	imglist lungmask;

	int n;

	Mat* histogram_imgs_0;
	Mat* histogram_imgs_1;
	Mat* histogram_imgs_2;
	vector<vector<unsigned long long>> histograms_0;
	vector<vector<unsigned long long>> histograms_1;
	vector<vector<unsigned long long>> histograms_2;

	lung* lungs;
	void calculate_histograms();
	void construct_lungs();
	void costruct_histogram_imgs();
	void save_histogram_imgs();



	String result_path = "G:\\";
	
};

