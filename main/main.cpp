#include "headers.h"
#include "imglist.h"
#include "med.h"
#include "lung.h"
#include "prediction.h"

cv::String data_dir = "C:\\Users\\adnan\\OneDrive\\Documents\\image_proc\\hw3\\covid19\\";
int main()
{
	imglist ct(100, data_dir + "tr_im\\", "tr_im_z", "png");
	ct.load_entire_set();

	imglist mask(100, data_dir + "tr_mask\\", "tr_mask_z", "png");
	mask.load_entire_set();

	imglist lungmask(100, data_dir + "tr_lungmasks\\", "tr_lungmasks_updated_z", "png");
	lungmask.load_entire_set();

	prediction please(ct, mask, lungmask, 0);
	please.predict();

	system("pause");


	/*
	int index = 1;
	blur(ct[index], ct[index], Size(5, 5));
	med a(ct[index], mask[index], lungmask[index]);

	lung l(a);
	Mat te;
	imshow("aa",std::get<1>(l.thresholded_multi_otsu(1)));
	*/


	//waitKey();
	

	return 0;
}
