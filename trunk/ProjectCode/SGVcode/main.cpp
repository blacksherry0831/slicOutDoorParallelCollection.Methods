#include "stdafx.h"
#include "module_all_cui.h"
#include<stdio.h>
/*----------------------------------------------*/
/*
C++:��д��ƽ̨����Ĺؼ���C/C++�е����ú궨��
�������֣�

����ϵͳ�ж���

Windows:   WIN32

Linux:   linux

Solaris:   __sun

�������ж���

VC:  _MSC_VER

GCC/G++:   __GNUC__

SunCC:   __SUNPRO_C��__SUNPRO_CC
*/
/*----------------------------------------------*/
int main(void)
{
#if _DEBUG
	IplImage *img=cvCreateImage(cvSize(100,100),IPL_DEPTH_8U,4);
	cvReleaseImage(&img);

#endif

  vector<string> file;
  string out;
#ifdef _WIN64 ||_WIN32 ||_MSC_VER ||WIN32

 	file.push_back("D:\\ImageDataBase\\400img\\img-op39-p-015t000.jpg");
	out="E://OutPutImg//";
#endif 


	cui_GeneralImgProcess::THreadSuperPixel_CUDA_CollectionMethods(0,file,out,1000);

	printf("Done ! \n");
	return 0;

}