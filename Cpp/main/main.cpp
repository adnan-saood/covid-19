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


	Mat thresh, his;
	int index = 99;
	med m(ct[index], mask[index], lungmask[index]);
	lung l(m);

	prediction pr(ct, mask, lungmask, 2);


	//pr.construct_masked();
	//pr.save_masked_lungs_imgs();
	pr.predict();

	auto global_ = l.thresholded_globally(0);
	thresh = std::get<0>(global_);
	his = std::get<1>(global_);
	imshow("thresh", thresh);
	imshow("histogram", his);
	imshow("mask", m.mask);

	waitKey();
	system("pause");
	return 0;
}
