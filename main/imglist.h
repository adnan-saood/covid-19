#pragma once
#include "headers.h"


class imglist
{
public:
	imglist(int n, String dir, String base_name, String file_type);
	void set_base_string(String s);
	int load_entire_set(); // load in list Mat files
	int load_entire_set_as_string(); //load as strings to save memory
	void print_set_names();
	~imglist();

	Mat& operator[](int);

	int n; //number of images
	String dir; //directory
	String base_string;
	String file_ext;

	Mat * list;
	String * list_names;
};