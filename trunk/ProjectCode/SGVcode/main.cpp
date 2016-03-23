#include "stdafx.h"
#include<stdio.h>

int main(void)
{
#if _DEBUG
	IplImage *img=cvCreateImage(cvSize(100,100),IPL_DEPTH_8U,4);
	cvReleaseImage(&img);

#endif
#if TRUE

    vector<string> file;
	file.push_back("");

	cui_GeneralImgProcess::THreadSuperPixel_CUDA_CollectionMethods(0,file,"D:",1000);
#endif
	
	printf("Hello,World\n");
	return 0;

}