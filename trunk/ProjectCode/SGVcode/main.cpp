#include "stdafx.h"
#include "module_all_cui.h"
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
void PrintARG(int argc,char *argv[])
{
  for(int i=0;i<argc;i++){
	  printf("###########################################\n");
	  printf("Param: %d>>%s \n",i,argv[i]);
	  printf("###########################################\n");
  }
}
/*----------------------------------------------*/
/*
*
*/
/*----------------------------------------------*/
void TestOpenCV()
{
#if _DEBUG
	{
		IplImage *img=cvCreateImage(cvSize(100,100),IPL_DEPTH_8U,4);
		cvReleaseImage(&img);
	}
#endif
}
/*----------------------------------------------*/
/*
*
*/
/*----------------------------------------------*/
int main(int argc,char *argv[])
{
	TestOpenCV();
	PrintARG(argc,argv);

  vector<string> file;
  string out;
  if (argc==1)
  {
#if _WIN64 ||_WIN32 ||_MSC_VER ||WIN32

 	file.push_back("D:\\ImageDataBase\\400img\\img-op39-p-015t000.jpg");
	out="E:\\OutPutImg\\";
#endif 

#if linux||__linux||__linux__||__GNUC__
	file.push_back("/home/blacksherry/400/400img/img-op39-p-015t000.jpg");
	out="/home/blacksherry/400/400out/";
#endif

  }else if (argc>1){
	  for (int i=1;i<argc;i++){
#if linux||__linux||__linux__||__GNUC__
		  if ( !access(argv[i], F_OK)){
			  printf("this is a file /n");
			  file.push_back(argv[i]);
		  }else{
			  printf("not a file   /n");
		  }
		  out="/home/blacksherry/400/400out/";
#endif
			


	  }
	

  }else{

  }


    
	cui_GeneralImgProcess::THreadSuperPixel_CUDA_CollectionMethods(0,file,out,1000);
	printf("Done ! \n");
	return 0;

}
/*----------------------------------------------*/
/*
*
*/
/*----------------------------------------------*/