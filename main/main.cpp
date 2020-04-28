#include "headers.h"
#include "imglist.h"
#include "med.h"
#include "lung.h"
cv::String data_dir = "C:\\Users\\adnan\\OneDrive\\Documents\\image_proc\\hw3\\covid19\\";
int main()
{
	imglist ct(100, data_dir + "tr_im\\", "tr_im_z", "png");
	ct.load_entire_set();

	imglist mask(100, data_dir + "tr_mask\\", "tr_mask_z", "png");
	mask.load_entire_set();

	imglist lungmask(100, data_dir + "tr_lungmasks\\", "tr_lungmasks_updated_z", "png");
	lungmask.load_entire_set();
	lungmask.load_entire_set_as_string();

	med a(ct[5], mask[5], lungmask[5]);

	lung l(a);
	Mat te;

	l.thresh_globally(ct[5], te);


	waitKey();
	system("pause");

	return 0;
}
