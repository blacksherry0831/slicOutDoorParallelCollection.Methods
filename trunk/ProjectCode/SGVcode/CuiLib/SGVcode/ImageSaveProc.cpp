#include "ImageSaveProc.h"
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
int ImageSaveProc::SAVE_DEBUG_2DISK = FALSE;
/*----------------------------------------------------------------*/
int ImageSaveProc::SAVE_IMAGE_2DISK = FALSE;
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
ImageSaveProc::ImageSaveProc(void)
{

}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
ImageSaveProc::~ImageSaveProc(void)
{

}
/*-------------------------------------------------------------------------------------------*/
/**
*����ͼ��Ӳ��ʱ����ͼ�������
*ͼ��ubuff������仯
*
*@param  ubuff  ͼ��ԭʼ����
*@param  labels ͼ��ĳ����ر궨Labels
*@param	 width  ͼ����
*@param	 height ͼ��߶�
*@param	 filereadfullpath  ͼ��Ķ�ȡ·��
*@param  filewritepath    ͼ���д��·��
*@param  fileadd       ��ӵ�ͼ���ļ����еĶ����ַ���
*
*/
/*-------------------------------------------------------------------------------------------*/
void ImageSaveProc::SaveImgWithContours(
	UINT32* ubuff,
	int* labels,
	const int&			width,
	const int&			height,
	std::string			filereadfullpath,
	std::string			filewritepath,
	std::string			fileadd)
{

	TimeMeasure tm("SaveImgWithContours");
	
	if (SAVE_IMAGE_2DISK == FALSE) {
		printf("SaveImgWithContours: --not-- \n");
	}
	else {
		unsigned int * imgbuf_t = new unsigned int[width*height];
		memcpy(imgbuf_t, ubuff, sizeof(unsigned int)*width*height);

		DrawContoursAroundSegments(imgbuf_t, labels, width, height, BlackColorPNG);

		if (fileadd == "") {
			fileadd = GetCurrentTimeStr();
		}
		SaveImageData(imgbuf_t, width, height, filereadfullpath, filewritepath, 1, fileadd);
		delete[]imgbuf_t;
	}

}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageSaveProc::DrawContoursAroundSegments(
	unsigned int*			ubuff,
	int*					labels,
	const int&				width,
	const int&				height,
	const unsigned int&		color)
{
	DrawContoursAroundSegments_cuda(ubuff, labels, width, height, color);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
std::string ImageSaveProc::GetCurrentTimeStr(void)
{

	std::string cui_t;

#ifdef _WIN32 || _WIN64
#if _WIN32 || _WIN64
#if _MSC_VER

	time_t curtime = time(0);
	struct tm tim;
	localtime_s(&tim, &curtime);

	char title[128];
	static int s_num = 0;
	sprintf(title, "%d_%02d_%02d_%02d_%02d_%02d", tim.tm_year + 1900, tim.tm_mon + 1,
		tim.tm_mday, tim.tm_hour, tim.tm_min, tim.tm_sec);

	/*SYSTEMTIME systime;
	cui_t="win32__win64";*/
	cui_t.append(title);
#endif


#ifdef Use_CString&&_MSC_VER
#if Use_CString	&&_MSC_VER
	SYSTEMTIME systime;
	CString strName, Time;
	GetSystemTime(&systime);
#if 1
	Time.Format(_T("%u_%u_%u_%u_%u_%u_%u"),
		systime.wYear,
		systime.wMonth,
		systime.wDay,
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		systime.wMilliseconds);
#else
	Time.Format(_T("%u:%u:%u__%uMs"),
		systime.wHour,
		systime.wMinute,
		systime.wSecond,
		systime.wMilliseconds);
#endif
	cui_t = cui_GeneralImgProcess::ConvertCS2string(Time);
#endif
#endif


#endif


#endif
	/**********************************/
#ifdef linux
#if linux 
	cui_t = "linux_time";
#endif
#endif
	/**********************************/
	return cui_t;
}
/*----------------------------------------------------------------*/
/**
*��ͼ����󱣴浽Ӳ��
*
*@param 	imgBuffer  ͼ�����
*@param     width      ͼ����
*@param 	height     ͼ��߶�
*@param 	outFilename  ͼ��Ҫ������ļ���
*@param 	saveLocation  ͼ��Ҫ�����·��
*@param 	format     ͼƬ��ʽ
*@param 	str       ����ͼƬʱ�ĸ����ַ���
*
*/
/*----------------------------------------------------------------*/
void ImageSaveProc::SaveImageData(
	UINT32*				imgBuffer,
	int					width,
	int					height,
	string&				outFilename,
	string&				saveLocation,
	int					format,
	const string&		str)
{

	if (SAVE_IMAGE_2DISK == FALSE) {
		printf("SaveImageData: --not-- \n");
	}
	else {
		int sz = width*height;
		IplImage *Save_Image_t;
		Save_Image_t = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 4);
		memcpy(Save_Image_t->imageData, imgBuffer, 4 * width*height);
		//-----------------------------------------
		// Prepare path and save the result images
		//-----------------------------------------
		string path = saveLocation;
		char fname[_MAX_FNAME];
		_splitpath(outFilename.c_str(), NULL, NULL, fname, NULL);
		path += fname;

		if (0 != str.compare("")) path.append(str);
		if (1 == format) path.append(".png");
		if (0 == format) path.append(".bmp");

		printf("cvSaveImage: %s \n", path.c_str());
		cvSaveImage(path.c_str(), Save_Image_t);
		cvReleaseImage(&Save_Image_t);
	}
	
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/