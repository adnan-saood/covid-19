#include "prediction.h"



prediction::prediction()
{
}


prediction::prediction(imglist ct, imglist mask, imglist lungmask, int method)
{
	this->method = method;
	this->ct = ct;
	this->mask = mask;
	this->lungmask = lungmask;
	this->n = ct.n;

	construct_lungs();
	calculate_histograms();
	construct_global_masks();
}

prediction::~prediction()
{
}

void prediction::predict()
{
	do_thresholding();
	result__2ch = new Mat[n];
	
	for (int i = 0; i < n; i++)
	{
		vector<Mat> temp;
		{
			temp.push_back(thresh_img_0[i]);
			temp.push_back(Mat::zeros(global_masks_0[i].size(), 0));
			temp.push_back(global_masks_0[i]); 
		}
		cv::merge(temp, result__2ch[i]);
		imshow("res", result__2ch[i]);
		imshow("global_mask", global_masks_0[i]);
		imshow("thresh", thresh_img_0[i]);
		waitKey();
	}

}




void prediction::do_thresholding()
{
	if (method == PRED_GLOBAL_THRESH)
		do_global_thresh();
	else if (method == PRED_GLOBAL_OTSU)
		do_otsu_thresh();
	else if (method == PRED_MULTI_OTSU)
		cout << "nan";
	else
		cout << "WRONG FUCKTARD!!";
}

void prediction::do_global_thresh()
{
	histogram_imgs_0 = new Mat[n];
	thresh_img_0 = new Mat[n];

	for (int i = 0; i < n; i++)
	{
		auto global_0 = lungs[i].thresholded_globally(0);

		thresh_img_0[i] = std::get<0>(global_0);
		histogram_imgs_0[i] = std::get<1>(global_0);
		if (DEBUG)
			cout << "Global Prediction at: " << i << endl;
	}
}

void prediction::do_otsu_thresh()
{
	histogram_imgs_0 = new Mat[n];
	thresh_img_0 = new Mat[n];

	for (int i = 0; i < n; i++)
	{
		auto global_0 = lungs[i].thresholded_otsu(0);

		thresh_img_0[i] = std::get<0>(global_0);
		histogram_imgs_0[i] = std::get<1>(global_0);
		if (DEBUG)
			cout << "OTSU Prediction at: " << i << endl;
	}
}

void prediction::do_multi_otsu_thresh()
{

}

void prediction::calculate_histograms()
{
	histograms_0 = vector<vector<unsigned long long>>(n, vector<unsigned long long>(256));
		for (int i = 0; i < n; i++)
	{
		histograms_0[i] = lungs[i].histogram(0);
	}

	if(DEBUG)
		cout << "Histograms Calculated" << endl;
}

void prediction::construct_lungs()
{
	lungs = new lung[n];
	for (int i = 0; i < n; i++)
	{
		med temp_med(ct[i],mask[i],lungmask[i]);
		lungs[i] = lung(temp_med);
	}

	if (DEBUG)
		cout << "Lungs Constructed" << endl;
}

void prediction::costruct_histogram_imgs()
{
	histogram_imgs_0 = new Mat[n];
	for (int i = 0; i < n; i++)
	{
		histogram_imgs_0[i] = lungs[i].drawhist(0);
	}

	if (DEBUG)
		cout << "Histogram images constructed" << endl;
}

void prediction::construct_global_masks()
{
	global_masks_0 = new Mat[n];
	for (int i = 0; i < n; i++)
	{
		Mat temp;
		threshold(mask[i], temp, 4, 255, THRESH_BINARY);
		global_masks_0[i] = temp;
	}
}

void prediction::save_imgs(Mat * in, String path)
{

	for (int i = 0; i < n; i++)
	{
		String order;
		if (i < 9)
			order = "00" + to_string(i + 1);
		else if (i >= 9 && i < 99)
			order = "0" + to_string(i + 1);
		else
			order = to_string(i + 1);

		string temp_path = path + order + "." + ct.file_ext;
		imwrite(temp_path, in[i]);
	}

}

void prediction::save_histogram_imgs()
{


	string temp_path = result_path
		+ HIST_IMG_FOLDER
		+ to_string(method)
		+"\\" + "h0\\";

	temp_path += HIST_IMG_NAME;
	save_imgs(histogram_imgs_0,temp_path);
	if(DEBUG)
		cout <<"Histogram images saved in:\n"<< temp_path << endl;

}

void prediction::save_masked_lungs_imgs()
{


	string temp_path = result_path
		+ MASKED_LUNGS_IMGS_FOLDER
		+ to_string(method)
		+ "\\" + "h0\\";

	temp_path += MASKED_LUNGS_IMGS_NAME;
	save_imgs(masked_lungs_0, temp_path);
	if (DEBUG)
		cout << "Masked lungs images saved in:\n" << temp_path << endl;

}

void prediction::construct_masked()
{
	masked_lungs_0 = new Mat[n];
	for (int i = 0; i < n; i++)
	{
		masked_lungs_0[i] = lungs[i].both;
	}
}

void prediction::save_threshholded_imgs()
{
	string temp_path = result_path
		+ THRESH_IMG_FOLDER
		+ to_string(method)+
		"\\" +
		"h0\\";

	temp_path += THRESH_IMG_NAME;
	save_imgs(thresh_img_0, temp_path);
	if (DEBUG)
		cout << "Thresholded images saved in:\n" << temp_path << endl;
}

void prediction::save_results_as_imgs()
{

	save_histogram_imgs();
	save_threshholded_imgs();
}

void prediction::do_stuff()
{
	construct_lungs();
	calculate_histograms();
	costruct_histogram_imgs();
	construct_masked();
	save_masked_lungs_imgs();
}

