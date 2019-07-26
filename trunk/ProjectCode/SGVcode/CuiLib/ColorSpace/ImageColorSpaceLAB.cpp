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
*	RGB2LAB
*
*/
/*----------------------------------------------------------------*/
inline void ImageColorSpaceLAB::RGB2LAB(const int& sR, const int& sG, const int& sB, double& lval, double& aval, double& bval)
{
	//TRACE_FUNC();
	//------------------------
	// sRGB to XYZ conversion
	//------------------------
	double X, Y, Z;
	RGB2XYZ(sR, sG, sB, X, Y, Z);

	//------------------------
	// XYZ to LAB conversion
	//------------------------
	const double epsilon = 0.008856;	//actual CIE standard
	const double kappa = 903.3;		//actual CIE standard

	const double Xr = 0.950456;	//reference white
	const double Yr = 1.0;		//reference white
	const double Zr = 1.088754;	//reference white

	double xr = X / Xr;
	double yr = Y / Yr;
	double zr = Z / Zr;

	double fx, fy, fz;
	if (xr > epsilon)	fx = pow(xr, 1.0 / 3.0);
	else				fx = (kappa*xr + 16.0) / 116.0;
	if (yr > epsilon)	fy = pow(yr, 1.0 / 3.0);
	else				fy = (kappa*yr + 16.0) / 116.0;
	if (zr > epsilon)	fz = pow(zr, 1.0 / 3.0);
	else				fz = (kappa*zr + 16.0) / 116.0;

	lval = 116.0*fy - 16.0;
	aval = 500.0*(fx - fy);
	bval = 200.0*(fy - fz);
}
/*----------------------------------------------------------------*/
/**
*RGB2XYZ
*	
*sRGB (D65 illuninant assumption) to XYZ conversion
*/
/*----------------------------------------------------------------*/
inline void ImageColorSpaceLAB::RGB2XYZ(
	const int&		sR,
	const int&		sG,
	const int&		sB,
	double&			X,
	double&			Y,
	double&			Z)
{
	//TRACE_FUNC();
	double R = sR / 255.0;
	double G = sG / 255.0;
	double B = sB / 255.0;

	double r, g, b;

	if (R <= 0.04045)	r = R / 12.92;
	else				r = pow((R + 0.055) / 1.055, 2.4);
	if (G <= 0.04045)	g = G / 12.92;
	else				g = pow((G + 0.055) / 1.055, 2.4);
	if (B <= 0.04045)	b = B / 12.92;
	else				b = pow((B + 0.055) / 1.055, 2.4);

	X = r*0.4124564 + g*0.3575761 + b*0.1804375;
	Y = r*0.2126729 + g*0.7151522 + b*0.0721750;
	Z = r*0.0193339 + g*0.1191920 + b*0.9503041;
}
/*----------------------------------------------------------------*/
/**
*DoRGBtoLABConversion
*For whole image: overlaoded floating point version
*
*/
/*----------------------------------------------------------------*/
void ImageColorSpaceLAB::DoRGBtoLABConversion(
	const unsigned int*		ubuff,
	double*					lvec,
	double*					avec,
	double*					bvec,
	int                     width,
	int                     height)
{
	TRACE_FUNC();
	TimeCountStart();
	const int sz = width*height;
	//int sz = ImgWidth*ImgHeight;
	/*lvec = new double[sz];
	avec = new double[sz];
	bvec = new double[sz];*/
	/*r:0--255*/
	/*g:0--255*/
	/*b:0--255*/
	int r, g, b;
	for (register int j = 0; j < sz; j++)
	{
		r = (ubuff[j] >> 16) & 0xFF;
		g = (ubuff[j] >> 8) & 0xFF;
		b = (ubuff[j]) & 0xFF;
		//////////////////////////////////
		// a  r      g      b
		//////////////////////////////////
		ImageColorSpaceLAB::RGB2LAB(r, g, b, lvec[j], avec[j], bvec[j]);
		/*assert();*/
	}
	TimeCountStop("######RGB2LAB Cost Time :");
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
	DoRGBtoLABConversion(src_ImgBGRA, m_lvec, m_avec, m_bvec, ImgWidth, ImgHeight);
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