#pragma once
/*----------------------------------------------------------------*/
#include "opencv_basic.h"
#include "cpp_basic.h"
/*----------------------------------------------------------------*/
/**
*���������ͼ�����㷨��һЩͨ�õ����ݽṹ\n
*�������õ����м����
*
*/
/*----------------------------------------------------------------*/
class ImageDataLABXY
{
public:
	ImageDataLABXY(
		IplImage* _img,
		std::string _SavePath="");
	~ImageDataLABXY();
private:
	int mSZ;
	IplImage* mImgPtr;
public:
	int ImagePoints();
	void CvtRGB2LAB(IplImage * _img);
public:
	std::string mSavePath;
private:
	void initMem();
	void createMem(const int _s);
	void freeMem();
public:
	void CvtRGB2LAB();
public:
	int * L_v;
	int * A_v;
	int * B_v;

};

