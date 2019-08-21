#include "ImageDataBasic.h"
/*----------------------------------------------------------------*/
/**
*���캯��\n
*��������������֮ǰ�����û����Ĳ���
*
*@param  filename  ͼ����ļ���
*@param  filesavepath   �м������·��
*/
/*----------------------------------------------------------------*/
ImageDataBasic::ImageDataBasic(
	std::string filename,
	std::string filesavepath)
{
	this->initParam();
	assert(filename!="");
	SetImageDataBasic(filename,filesavepath);
	InitMemoryData(nullptr,filename,filesavepath);
	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
ImageDataBasic::ImageDataBasic(
	IplImage* img,
	std::string filesavepath)
{
	std::string filename="MemoryIMG";
	this->initParam();
	SetImageDataBasic(filename,filesavepath);
	InitMemoryData(img,filename,filesavepath);
}
/*----------------------------------------------------------------*/
/**
*��ʼ����Ա����
*
*
*/
/*----------------------------------------------------------------*/
void ImageDataBasic::initParam(void)
{	
	TRACE_FUNC();
	this->FileReadFullPath="";
	this->FileWritePath="";
	
	int sise_int=sizeof(int);
	int size_intp=sizeof(int*);
	
	this->srcCv_ImgBGRA=nullptr;
		
}
/*----------------------------------------------------------------*/
/**
*����ͼ������롢���·��
*
* @param filename      �ļ���
* @param filesavepath  �м��������·��
*
*/
/*----------------------------------------------------------------*/
void ImageDataBasic::SetImageDataBasic(std::string filename, std::string filesavepath){

	TRACE_FUNC();
	FileReadFullPath=filename;
	FileWritePath=filesavepath;
	
}
 /*----------------------------------------------------------------*/
 /**
 *���������ͷŷ�����ڴ�
 *
 *
 */
 /*----------------------------------------------------------------*/
ImageDataBasic::~ImageDataBasic()
{
	this->ReleaseMemory();
}
/*----------------------------------------------------------------*/
/**
*�����м���̷�����ڴ�
*
*
*/
/*----------------------------------------------------------------*/

void ImageDataBasic::ReleaseMemory(void)
{		
	cvReleaseImage(&srcCv_ImgBGRA); 	
}
/*----------------------------------------------------------------*/
 /**
 *��ʼ���ѷ�����ڴ�
 *
*@param filename  ͼ���ļ���
*@param filesavepath ͼ�񱣴�·��
 */
 /*----------------------------------------------------------------*/
 void ImageDataBasic::InitMemoryData(
	 IplImage* img,
	 std::string filename,
	 std::string filesavepath)
 {  
	 TRACE_FUNC();
	 ReleaseMemory();
	 /*************************************************************************************************/ 
	
	 IplImage *src_img_t;
	if (img==nullptr){
		 printf("cvLoadImage: %s \n",filename.c_str());
		 src_img_t=cvLoadImage(filename.c_str(),CV_LOAD_IMAGE_UNCHANGED); 
		 if (src_img_t==NULL){
			printf("cvLoadImage: Fail %s \n",filename.c_str());
		 }
	}else{
		printf("cvCreateImage: %s \n");
		src_img_t=cvCreateImage(cvGetSize(img),img->depth,4);
		if (img->nChannels==4){
			 cvCopyImage(img,src_img_t);
		}else if (img->nChannels==3){
			cvCvtColor(img,src_img_t,CV_BGR2BGRA);
		}
		else if (img->nChannels==1){
			cvCvtColor(img,src_img_t,CV_GRAY2BGRA);
		}else{
			ASSERT(0); 
		}
	   
	}	
	 ConvertImg2Eighth4Ch(&src_img_t);

	 srcCv_ImgBGRA=cvCloneImage(src_img_t);

	 ConvertImg3ChTo4Ch(&srcCv_ImgBGRA);

	 cvReleaseImage(&src_img_t);
	 /********************************************************************/	 
 }
/*-------------------------------------------------------------------------------------------*/
/**
*��ͼ��ת����8��������4ͨ��
*@param [in][out] src
*/
/*-------------------------------------------------------------------------------------------*/
void ImageDataBasic::ConvertImg2Eighth4Ch(IplImage **src)
{
	TRACE_FUNC();
	CvSize SizeOld = cvGetSize(*src);
	CvSize SizeNew = cvGetSize(*src);
	int depth = (*src)->depth;
	int nChannels = (*src)->nChannels;

	ASSERT(src != nullptr);
	ASSERT(*src != nullptr);
	if (SizeOld.width % 8 != 0) {
		SizeNew.width = SizeOld.width - SizeOld.width % 8 + 8;
	}
	if (SizeOld.height % 8 != 0) {
		SizeNew.height = SizeOld.height - SizeOld.height % 8 + 8;
	}
	if ((SizeOld.width == SizeNew.width)
		&& (SizeOld.height == SizeNew.height)) {
		return;
	}
	IplImage *temp = cvCloneImage(*src);
	cvReleaseImage(src);
	*src = cvCreateImage(SizeNew, depth, nChannels);
	cvResize(temp, *src);
	cvReleaseImage(&temp);
#if 1
	ConvertImg3ChTo4Ch(src);
	//	cvSaveImage("A.png",*src);
#endif

}
/*-------------------------------------------------------------------------------------------*/
/**
*��ͼ�����ͨ��ת������ͨ��
*@param [in][out] src ��ͨ����ͼ��
*/
/*-------------------------------------------------------------------------------------------*/
void ImageDataBasic::ConvertImg3ChTo4Ch(IplImage **src)
{
	TRACE_FUNC();
	CvSize SizeOld = cvGetSize(*src);
	int depth = (*src)->depth;
	int nChannels = (*src)->nChannels;

	ASSERT(src != nullptr);
	ASSERT(*src != nullptr);

	if (nChannels == 3) {
		IplImage *temp = cvCloneImage(*src);
		cvReleaseImage(src);
		*src = cvCreateImage(SizeOld, depth, 4);
		cvCvtColor(temp, *src, CV_BGR2BGRA);
		cvReleaseImage(&temp);

	}

}
/*-------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------*/
IplImage* ImageDataBasic::GetIplImagePtr()
{
	return this->srcCv_ImgBGRA;
}
/*-------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*-------------------------------------------------------------------------------------------*/