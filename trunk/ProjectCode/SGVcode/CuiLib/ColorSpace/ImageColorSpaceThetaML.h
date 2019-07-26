#pragma once
/*----------------------------------------------------------------*/
#include "opencv_stl.h"
#include "cpp_def.h"
/*----------------------------------------------------------------*/
#include "DbgRangeCheck.h"
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
class ImageColorSpaceThetaML
{
public:
	ImageColorSpaceThetaML();
	~ImageColorSpaceThetaML();
public:

	double*     sita_n;
	double*     m_n;
	double*     L_n;
	double*     X_n;
	double*     Y_n;
private:
	void ReleaseMemory(void);
	void CreateMemory(const int _sz);
public:
	void CvtLabXY2ThetaMLXY(
									const double* L,
									const double* A,
									const double* B,
									const int WIDTH,
									const int HEIGHT);

	void  fillTheta(IplImage* _img,	const float _s=255);
	void  fillM(IplImage* _img,		const float _s=255);
	void  fillL(IplImage* _img,		const float _s=255);

public:
	static void ColorSpaceLab2ThetaML(
		const double L,
		const double A,
		const double B,	
		double & sita_n,
		double & m_n,
		double & L_n);


	static void ConvertLab2ThetaML(
									const double L,
									const double A,
									const double B,
									const double X,
									const double Y,
									const int WIDTH,
									const int HEIGHT,
									double & sita_n,
									double & m_n,
									double & L_n,
									double & X_n,
									double & Y_n);

	static void CheckLab(
							const double L,
							const double A,
							const double B);

	void InitMemory(void);
		

	static void CheckXY(
		const double X,
		const double Y,
		const int WIDTH,
		const int HEIGHT);

	
public:

	
};

