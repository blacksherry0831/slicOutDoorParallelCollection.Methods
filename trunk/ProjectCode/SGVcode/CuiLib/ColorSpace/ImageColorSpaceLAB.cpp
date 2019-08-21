#include "ImageColorSpaceLAB.h"
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
ImageColorSpaceLAB::ImageColorSpaceLAB(std::string filename)
{
	this->initParam();
	ASSERT(filename != "");
	this->FileReadFullPath = filename;
}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
ImageColorSpaceLAB::ImageColorSpaceLAB(IplImage* _img)
{
	this->initParam();
	InitMemoryData(_img);
}
/*----------------------------------------------------------------*/
/**
*初始化成员变量
*
*
*/
/*----------------------------------------------------------------*/
void ImageColorSpaceLAB::initParam(void)
{
	TRACE_FUNC();
	this->FileReadFullPath = "";

	this->ImgHeight = 0;
	this->ImgWidth = 0;

	this->srcCv_ImgBGRA = NULL;
	m_lvec = NULL;
	m_avec = NULL;
	m_bvec = NULL;
	
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageColorSpaceLAB::InitMemoryData(
								IplImage* img)
{	
	ReleaseMemory();
	
	IplImage *src_img_t;
	{
		src_img_t = cvCreateImage(cvGetSize(img), img->depth, 4);
		if (img->nChannels == 4) {
			cvCopyImage(img, src_img_t);
		}
		else if (img->nChannels == 3) {
			cvCvtColor(img, src_img_t, CV_BGR2BGRA);
		}
		else if (img->nChannels == 1) {
			cvCvtColor(img, src_img_t, CV_GRAY2BGRA);
		}
		else {
			ASSERT(0);
		}

	}

	ConvertImg2Eighth4Ch(&src_img_t);

	srcCv_ImgBGRA = cvCloneImage(src_img_t);
		
	cvReleaseImage(&src_img_t);
	/********************************************************************/
	ImgWidth = srcCv_ImgBGRA->width;
	ImgHeight = srcCv_ImgBGRA->height;
	const int sz = ImgHeight*ImgWidth;
	/*********************************************************************/
	const unsigned int*	src_ImgBGRA= (unsigned int *)(srcCv_ImgBGRA->imageData);
	ASSERT(sizeof(unsigned int) == 4);
	/*********************************************************************/
	m_lvec = new double[sz];
	m_avec = new double[sz];
	m_bvec = new double[sz];
	ColorSpaceLAB::DoRGBtoLABConversion(src_ImgBGRA, m_lvec, m_avec, m_bvec, ImgWidth, ImgHeight);
	/*********************************************************************/
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageColorSpaceLAB::ReleaseMemory(void)
{
	delete[]  m_lvec;
	delete[]  m_avec;
	delete[]  m_bvec;
	//////////////////////
	cvReleaseImage(&srcCv_ImgBGRA);
		
}
/*-------------------------------------------------------------------------------------------*/
/**
*将图像转换到8的整数倍4通道
*@param [in][out] src
*/
/*-------------------------------------------------------------------------------------------*/
void ImageColorSpaceLAB::ConvertImg2Eighth4Ch(IplImage **src)
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
*将图像从三通道转换到四通道
*@param [in][out] src 三通道的图像
*/
/*-------------------------------------------------------------------------------------------*/
void ImageColorSpaceLAB::ConvertImg3ChTo4Ch(IplImage **src)
{
	TRACE_FUNC();

	ASSERT(src != nullptr);
	ASSERT(*src != nullptr);

	CvSize SizeOld = cvGetSize(*src);
	int depth = (*src)->depth;
	int nChannels = (*src)->nChannels;



	if (nChannels == 3) {
		IplImage *temp = cvCloneImage(*src);
		cvReleaseImage(src);
		*src = cvCreateImage(SizeOld, depth, 4);
		cvCvtColor(temp, *src, CV_BGR2BGRA);
		cvReleaseImage(&temp);

	}

}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
int ImageColorSpaceLAB::Width()
{
	return ImgWidth;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
int ImageColorSpaceLAB::Height()
{
	return ImgHeight;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/