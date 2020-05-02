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
			temp.push_back(thresh_img_0[i]); // B
			temp.push_back(Mat::zeros(global_masks_0[i].size(), 0)); // G
			temp.push_back(global_masks_0[i]); //R
		}
		cv::merge(temp, result__2ch[i]);
		if (0) 
		{
			imshow("res", result__2ch[i]);
			imshow("global_mask", global_masks_0[i]);
			imshow("thresh", thresh_img_0[i]);
			waitKey();
		}
	}


	validate();
	save_confusion_matrix_as_csv();

}




void prediction::validate_global()
{
	
	confusion = new Mat[n];
	for (int  i = 0; i < n; i++)
	{
		int true_positive = 0;
		int false_positive = 0;
		int true_negative = 0;
		int false_negative = 0;
		Mat temp = result__2ch[i];
		for (int r = 0; r < temp.rows; r++)
		{
			for (int c = 0; c < temp.cols; c++)
			{
				int a = temp.at<Vec3b>(r, c)[MASK_CH];
				int b = temp.at<Vec3b>(r, c)[PRED_CH];
				if (a == b)
					if (a > 0)
						true_positive++;
					else
						true_negative++;
				else
					if (a > 0)
						false_negative++;
					else
						false_positive++;
			}
		}
		confusion[i].create(Size(2, 2), CV_32F);
		confusion[i].at<float>TP = true_positive;
		confusion[i].at<float>FN = false_negative;
		confusion[i].at<float>FP = false_positive;
		confusion[i].at<float>TN = true_negative - lungs[i].histogram(0)[0]; //disregard the shit around lungs

	}

}
void prediction::validate_multi()
{

	confusion = new Mat[n];
	for (int i = 0; i < n; i++)
	{
		vector<long>  true_positive(3, 0);
		vector<long>  false_positive(3, 0);
		vector<long>  true_negative(3, 0);
		vector<long>  false_negative(3, 0);

		Mat temp = result__2ch[i];
		for (int r = 0; r < temp.rows; r++)
		{
			for (int c = 0; c < temp.cols; c++)
			{
				int val = temp.at<Vec3b>(r, c)[MASK_CH];
				int pred = temp.at<Vec3b>(r, c)[PRED_CH];
				if (val == pred)
				{
					if (pred == 0)
					{
						true_negative[0]++;
						true_negative[1]++;
						true_negative[2]++;
					}
					else
						true_positive[(pred / C1) - 1]++;
				}
				else
				{
					if (val == 0)
					{
						false_positive[pred / C1 - 1]++;
						true_negative[(((pred / C1) + 1) - 1) % 3]++;
						true_negative[(((pred / C1) + 2) - 1) % 3]++;
					}
					if (pred == 0)
					{
						false_negative[val / C1 - 1]++;
						true_negative[(((pred / C1) + 1) - 1) % 3]++;
						true_negative[(((pred / C1) + 2) - 1) % 3]++;
					}
					if ((val != 0) && (pred != 0))
					{
						false_positive[pred / C1 - 1]++;
						false_negative[val / C1 - 1]++;
						true_negative[(((pred / C1) + 1) - 1) % 3]++;
						true_negative[(((pred / C1) + 2) - 1) % 3]++;
					}
				}
			}
		}
		confusion[i].create(Size(2, 2), CV_32FC3);
		for (int iter = 0; iter < 3; iter++)
		{
			confusion[i].at<Vec3f>TP[iter] = true_positive[iter]; // error here in iterator idk what is ?
			confusion[i].at<Vec3f>FN[iter] = false_negative[iter];
			confusion[i].at<Vec3f>FP[iter] = false_positive[iter];
			confusion[i].at<Vec3f>TN[iter] = true_negative[iter] - lungs[i].histogram(0)[0]; //disregard the shit around lungs
		}

	}
}

void prediction::validate_multi_2()
{

	confusion = new Mat[n];
	for (int i = 0; i < n; i++)
	{
		//			VAL
		//PRED
		//
		//
		//
		confusion[i].create(Size(4, 4), CV_32F);
		confusion[i] = Scalar(0);
		Mat temp = result__2ch[i];
		for (int r = 0; r < temp.rows; r++)
		{
			for (int c = 0; c < temp.cols; c++)
			{
				int val = temp.at<Vec3b>(r, c)[MASK_CH] / C1;
				int pred = temp.at<Vec3b>(r, c)[PRED_CH] / C1;
				confusion[i].at<float>(pred, val)++;
			}
		}
		
		
	}
}

void prediction::save_confusion_matrix_as_csv()
{
	ofstream file;
	string temp;
	temp = PRED_RES_PATH + string(CONFUSION_MAT_FILE_NAME);
	file.open("hu.csv");
		if (this->method < 2)
		{
			for (int i = 0; i < n; i++)
			{
				file << confusion[i].at<float>TP << ',';
				file << confusion[i].at<float>FN << ',';
				file << confusion[i].at<float>FP << ',';
				file << confusion[i].at<float>TN << '\n';
			}
		}
		else
		{
			/////////////////////////////////////////////////////////////
			////File is like this////////////////////////////////////////
			//       C1             C2	           C3         
			//   TP FN FP TN || TP FN FP TN || TP FN FP TN 
			//   #  #  #  #  || #  #  #  #  || #  #  #  #  
			//  ...
			//
			for (int i = 0; i < n; i++)
			{
				if (DEBUG)
					cout << "At i = " << i << endl << confusion[i] << endl;
				for (int r = 0; r < 4; r++)
				{
					for (int c = 0; c < 4; c++)
					{
						file << confusion[i].at<float>(r, c) << ',';
					}
				}
				file << '\n';
			}
		}
	file.close();
}


void prediction::do_thresholding()
{
	if (method == PRED_GLOBAL_THRESH)
		do_global_thresh();
	else if (method == PRED_GLOBAL_OTSU)
		do_otsu_thresh();
	else if (method == PRED_MULTI_OTSU)
		do_multi_otsu_thresh();
	else
		cout << "WRONG FUCKTARD!!";
}

void prediction::validate()
{
	if (this->method < 2)
		validate_global();
	else
		validate_multi_2();
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
		{
			cout << "OTSU Prediction at: " << i << endl;
		}

	}
}

void prediction::do_multi_otsu_thresh()
{
	histogram_imgs_0 = new Mat[n];
	thresh_img_0 = new Mat[n];
	for (int i = 0; i < n; i++)
	{
		cout << "Multi-OTSU Prediction at: " << i << endl;
		auto global_0 = lungs[i].thresholded_multi_otsu(0);
		thresh_img_0[i] = std::get<0>(global_0);
		histogram_imgs_0[i] = std::get<1>(global_0);
		if (0)
		{
			imshow("threshed at: i=" + to_string(i), thresh_img_0[i]);
			imshow("Hist at: i=" + to_string(i), histogram_imgs_0[i]);
			waitKey();
			destroyAllWindows();
		}
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

void prediction::construct_global_masks()
{
	global_masks_0 = new Mat[n];
	for (int i = 0; i < n; i++)
	{
		global_masks_0[i] = mask[i];
		if (this->method == PRED_MULTI_OTSU)
			continue;
		threshold(mask[i], global_masks_0[i], 4, 255, THRESH_BINARY);	
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

