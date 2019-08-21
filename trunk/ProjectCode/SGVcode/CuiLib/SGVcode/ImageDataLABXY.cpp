#include "ImageDataLABXY.h"
/*-------------------------------------------------------------------*/
#include <ColorSpace/ColorSpaceLabTable.h>
#include <ColorSpace/ImageColorSpaceLAB.h>
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
ImageDataLABXY::ImageDataLABXY(
	IplImage* _img,
	std::string	 _SavePath)
{
	this->initMem();

	this->mSZ =_img->width*_img->height;
	this->mSavePath = _SavePath;
	this->mImgPtr = _img;

	this->createMem(mSZ);
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
ImageDataLABXY::~ImageDataLABXY()
{
	freeMem();
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageDataLABXY::initMem()
{
	L_v=NULL;
	A_v=NULL;
	B_v=NULL;
	this->mImgPtr = NULL;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageDataLABXY::createMem(const int _s)
{
	ASSERT(L_v == NULL);
	ASSERT(A_v == NULL);
	ASSERT(B_v == NULL);

	L_v = new  int[_s];
	A_v = new  int[_s];
	B_v = new  int[_s];
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageDataLABXY::freeMem()
{
	delete[] L_v;
	delete[] A_v;
	delete[] B_v;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/

int ImageDataLABXY::ImagePoints()
{
	return this->mSZ;
}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
void ImageDataLABXY::CvtRGB2LAB()
{
	const unsigned int * ubuff= (unsigned int * )(mImgPtr->imageData);
	
	const int W = mImgPtr->width;
	const int H = mImgPtr->height;

	ColorSpaceLabTable::DoRGBtoLABConversionInt(
		ubuff,
		L_v,
		A_v,
		B_v,
		W,
		H);

}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
