// main.cpp : Defines the entry point for the console application.
//

#include "headers.h"
#include "imglist.h"

int main()
{
	imglist a(100, "C:\\Users\\adnan\\OneDrive\\Documents\\image_proc\\hw3\\covid19\\tr_im\\", "tr_im_z", "png");

	a.load_entire_set();
	for (int i = 1; i <= 100; i++)
	{
		imshow("hi", a.list[i-1]);
		waitKey(250);
	}

	system("pause");
	return 0;
}
