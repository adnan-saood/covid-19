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

}

prediction::~prediction()
{
}


void prediction::predict()
{
	
	construct_lungs();
	calculate_histograms();
	costruct_histogram_imgs();
	do_global_threh();

}



void prediction::do_global_threh()
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
	save_histogram_imgs();
}

