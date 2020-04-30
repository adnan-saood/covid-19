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

void prediction::calculate_histograms()
{
	histograms_0 = vector<vector<unsigned long long>>(n, vector<unsigned long long>(256));
	histograms_1 = vector<vector<unsigned long long>>(n, vector<unsigned long long>(256));
	histograms_2 = vector<vector<unsigned long long>>(n, vector<unsigned long long>(256));

	for (int i = 0; i < n; i++)
	{
		histograms_0[i] = lungs[i].histogram(0);
		histograms_1[i] = lungs[i].histogram(1);
		histograms_2[i] = lungs[i].histogram(2);
	}
}

void prediction::construct_lungs()
{
	lungs = new lung[n];
	for (int i = 0; i < n; i++)
	{
		med temp_med(ct[i],mask[i],lungmask[i]);
		lungs[i] = lung(temp_med);
	}
}

void prediction::costruct_histogram_imgs()
{
	histogram_imgs_0 = new Mat[n];
	histogram_imgs_1 = new Mat[n];
	histogram_imgs_2 = new Mat[n];
	for (int i = 0; i < n; i++)
	{
		histogram_imgs_0[i] = lungs[i].drawhist(0);
		histogram_imgs_1[i] = lungs[i].drawhist(1);
		histogram_imgs_2[i] = lungs[i].drawhist(2);
	}
}

void prediction::save_histogram_imgs()
{
	
	for (int i = 0; i < n; i++)
	{
		String order;
		if (i < 9)
			order = "00" + to_string(i+1);
		else if (i >= 9 && i < 99)
			order = "0" + to_string(i+1);
		else
			order = to_string(i+1);

		string temp_path = result_path + "histo_folder\\" + "h0\\";
		temp_path += ("histograms_0z" + order + "." + ct.file_ext);
		imwrite(temp_path, histogram_imgs_0[i]);
		//imshow("a", histogram_imgs_0[i]);
		//waitKey(250);
		cout << temp_path << endl;

		temp_path = result_path + "histo_folder\\" + "h1\\";
		temp_path += ("histograms_1z" + order + "." + ct.file_ext);
		imwrite(temp_path, histogram_imgs_1[i]);

		temp_path = result_path + "histo_folder\\" + "h2\\";
		temp_path += ("histograms_2z" + order + "." + ct.file_ext);
		imwrite(temp_path, histogram_imgs_2[i]);
	}

}

void prediction::do_stuff()
{
	construct_lungs();
	calculate_histograms();
	costruct_histogram_imgs();
	save_histogram_imgs();
}

