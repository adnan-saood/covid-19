#pragma once
#include "imglist.h"
#include"headers.h"
#include"lung.h"
#include "prediction_methods.h"

class prediction
{
public:
	prediction();
	prediction(imglist, imglist, imglist, int);
	~prediction();


	void do_stuff(); // delete later
	void predict();

	void do_thresholding();


	void save_histogram_imgs();
	void save_threshholded_imgs();

	void save_results_as_imgs();


private:
	int method;
	imglist ct;
	imglist mask;
	imglist lungmask;

	int n;

	Mat* histogram_imgs_0;
	Mat* histogram_imgs_1;
	Mat* histogram_imgs_2;


	Mat* thresh_img_0;
	Mat* thresh_img_1;
	Mat* thresh_img_2;

	vector<vector<unsigned long long>> histograms_0;
	vector<vector<unsigned long long>> histograms_1;
	vector<vector<unsigned long long>> histograms_2;

	lung* lungs;
	void calculate_histograms();
	void construct_lungs();
	void costruct_histogram_imgs();

	void save_imgs(Mat * in, String file_name);


	void do_global_thresh();
	void do_otsu_thresh();


	String result_path = PRED_RES_PATH;
	
};

