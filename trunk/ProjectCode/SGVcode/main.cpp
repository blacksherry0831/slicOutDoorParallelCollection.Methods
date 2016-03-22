#include "stdafx.h"
#include<stdio.h>

int main(void)
{
	vector<string> file;
	file.push_back("");
	cui_GeneralImgProcess::THreadSuperPixel_CUDA_CollectionMethods(0,file,"D:",1000);
	printf("Hello,World\n");
	return 0;

}