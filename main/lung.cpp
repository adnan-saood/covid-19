#include "lung.h"



lung::lung()
{
	
}

lung::lung(med in)
{
	up_lung = cv::min(in.ct, in.lungmask == 1);
	down_lung = cv::min(in.ct, in.lungmask == 2);
}


lung::~lung()
{
}
