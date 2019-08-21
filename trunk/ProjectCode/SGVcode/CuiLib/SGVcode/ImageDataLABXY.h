#pragma once
/*----------------------------------------------------------------*/
#include "opencv_basic.h"
#include "cpp_basic.h"
/*----------------------------------------------------------------*/
/**
*本类包含了图像处理算法中一些通用的数据结构\n
*和运算用到的中间变量
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

