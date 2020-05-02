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

	void validate();


	void save_histogram_imgs();
	void save_threshholded_imgs();
	void save_masked_lungs_imgs();
	void save_results_as_imgs();

	void prediction::construct_masked();


private:
	int method;
	imglist ct;
	imglist mask;
	imglist lungmask;

	Mat* masked_lungs_0;

	int n;
	String result_path = PRED_RES_PATH;


	vector<vector<unsigned long long>> histograms_0;

	lung* lungs;




	Mat * histogram_imgs_0;
	Mat * thresh_img_0;

	Mat * global_masks_0;

	Mat * result__2ch;

	Mat * confusion;

	void calculate_histograms();
	void construct_lungs();
	void costruct_histogram_imgs();
	void construct_global_masks();

	void save_imgs(Mat * in, String file_name);


	void do_global_thresh();
	void do_otsu_thresh();


	void do_multi_otsu_thresh();

	void validate_global();
	void validate_multi();
	void save_confusion_matrix_as_csv();
	
};

