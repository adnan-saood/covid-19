#pragma once
#include "med.h"
class lung :
	public med
{
public:
	lung();
	lung(med in);
	~lung();

	Mat up_lung; //I = 1
	Mat down_lung; //I = 2

};

