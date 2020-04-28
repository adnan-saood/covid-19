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

	

	int index = 76;
	blur(ct[index], ct[index], Size(5, 5));
	med a(ct[index], mask[index], lungmask[index]);

	lung l(a);
	Mat te;


	te = l.thresholded_otsu(1);

	imshow("otsu", te);

	/*Mat s1;
	s1 = Mat::ones(3, 3, 0) * 33;
	s1.at<uchar>(1, 1) = 255;
	cout << "s1 = \n" << s1 << endl;
	Mat s2 = Mat::zeros(3, 3, 0);
	s2.at<uchar>(1, 1) = 1;
	s2.at<uchar>(2, 1) = 1;
	cout << "s2 & s1 = \n" << (s1&s2) << endl;
	cout << "s2 & s1 = \n" << (s1.mul(s2)) << endl;

	cout << "mean(s1) = " << cv::mean(s1) << endl;
	cout << "mean(s2) = " << cv::mean(s1, s2) << endl;

	*/
	//system("pause");

	waitKey();
	

	return 0;
}
