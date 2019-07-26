#pragma once
/*----------------------------------------------------------------*/
#include "opencv_stl.h"
#include "cpp_def.h"
#include "cpp_def_debug.h"
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
class ImageColorSpaceLAB
{
public:
	ImageColorSpaceLAB(std::string _filename);
	ImageColorSpaceLAB(IplImage* _img);
private:
	std::string FileReadFullPath;
private:
	int ImgWidth;
	int ImgHeight;
public:
	int Width();
	int Height();
public:
	IplImage* srcCv_ImgBGRA;
public:
	void InitMemoryData(IplImage* img);
	void ReleaseMemory(void);
public:
	double*		m_lvec;//(0,100)
	double*		m_avec;//(-128,127)
	double*		m_bvec;//(-128,127)
private:
			void initParam();
public:

static		void DoRGBtoLABConversion(
			const unsigned int*		ubuff,
			double*					lvec,
			double*					avec,
			double*					bvec,
			int                     width,
			int                     height);

static		void RGB2LAB(
			const int& sR, const int& sG, const int& sB, 
			double& lval, double& aval, double& bval);

static		void RGB2XYZ(
			const int&		sR,
			const int&		sG,
			const int&		sB,
			double&			X,
			double&			Y,
			double&			Z);

static		void ConvertImg2Eighth4Ch(IplImage **src);
static		void ConvertImg3ChTo4Ch(IplImage **src);


};

