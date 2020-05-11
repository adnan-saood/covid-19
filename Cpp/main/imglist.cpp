#include "imglist.h"
#include "headers.h"

imglist::imglist()
{

}

imglist::imglist(int num, String dirc, String base_name, String file_type)
{
	n = num;
	dir = dirc;
	list = new Mat[num];
	list_names = new String[num];
	base_string = base_name;
	file_ext = file_type;
}



imglist::~imglist()
{
}
Mat & imglist::operator[](int index)
{
	return this->list[index];
}
void imglist::set_base_string(String s)
{
	imglist::base_string = s;
}
int imglist::load_entire_set()
{
	for (int i = 1; i <= imglist::n; i++)
	{
		String temp;
		String order;
		if (i < 10)
		{
			order = "00" + to_string(i);
		}
		else if (i < 100 && i >= 10)
		{
			order = "0" + to_string(i);
		}
		else if (i >= 100)
		{
			order = to_string(i);
		}
		temp = imglist::dir + base_string + order + "." + file_ext;
		imglist::list[i-1] = imread(temp,0);
	}
	if (list[0].rows == 0)
	{
		return -1;
	}
	return 0;
}

int imglist::load_entire_set_as_string()
{
	for (int i = 1; i <= imglist::n; i++)
	{
		String temp;
		String order;
		if (i < 10)
		{
			order = "00" + to_string(i);
		}
		else if (i < 100 && i >= 10)
		{
			order = "0" + to_string(i);
		}
		else if (i >= 100)
		{
			order = to_string(i);
		}
		temp = imglist::dir + base_string + order + "." + file_ext;
		imglist::list_names[i-1] = temp;
	}
	return 0;
}

void imglist::print_set_names()
{
	for (int i = 0; i < imglist::n; i++)
	{
		cout <<i<<": "<< list_names[i] << endl;
	}
}
