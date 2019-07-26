#include "ImageColorSpaceThetaML.h"
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
ImageColorSpaceThetaML::ImageColorSpaceThetaML()
{
	this->InitMemory();
}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
ImageColorSpaceThetaML::~ImageColorSpaceThetaML()
{
	this->ReleaseMemory();
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageColorSpaceThetaML::ColorSpaceLab2ThetaML(
	const double L,
	const double A,
	const double B,
	double & _theta,
	double & _m,
	double & _l)
{
	const float LAB_M_MAX = 128 * 1.4142135623731;
	const float X2_PI = 2 * CV_PI;
	///////////////////////////////////////
	if (A == B && B == 0) {
		_theta = 0;
	}
	else
	{
		_theta = atan2(B, A);//(-pi,pi]
		_theta = _theta / X2_PI + 0.5F;
	}
	///////////////////////////////////////
	_m = sqrt(A*A + B*B);
	_m = _m / (LAB_M_MAX);
	///////////////////////////////////////
	_l = L / 100;
	///////////////////////////////////////
}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
inline void ImageColorSpaceThetaML::ConvertLab2ThetaML(
	const double L,
	const double A,
	const double B,
	const double X,
	const double Y,
	const int WIDTH,
	const int HEIGHT,
	double& sita_n,
	double& m_n,
	double& L_n,
	double& X_n,
	double& Y_n)
{		
	CheckXY(X, Y, WIDTH, HEIGHT);		
	CheckLab(L, A, B);

	X_n = X / WIDTH;
	Y_n = Y / HEIGHT;
	ColorSpaceLab2ThetaML(L, A, B, sita_n, m_n, L_n);

	DbgRangeCheck::Check01(X_n);
	DbgRangeCheck::Check01(Y_n);
	DbgRangeCheck::Check01(sita_n);
	DbgRangeCheck::Check01(m_n);
	DbgRangeCheck::Check01(L_n);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
inline void ImageColorSpaceThetaML::CheckXY(const double X, const double Y, const int WIDTH, const int HEIGHT)
{
	const float EPS = 1E-3;
	ASSERT(X >= 0 - EPS && X <= WIDTH + EPS);
	ASSERT(Y >= 0 - EPS && Y <= HEIGHT + EPS);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
inline void ImageColorSpaceThetaML::CheckLab(const double L, const double A, const double B)
{
	const float EPS = 0.5;

	ASSERT(L >= 0 - EPS && L <= 100 + EPS);
	ASSERT(A >= -128 - EPS && A <= 127 + EPS);
	ASSERT(B >= -128 - EPS && B <= 127 + EPS);


}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageColorSpaceThetaML::InitMemory(void)
{
	sita_n=NULL;
	m_n = NULL;
	L_n = NULL;
	X_n = NULL;
	Y_n = NULL;

}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageColorSpaceThetaML::ReleaseMemory(void)
{
	delete[]    sita_n;
	delete[]     m_n;
	delete[]     L_n;
	delete[]    X_n;
	delete[]    Y_n;

	this->InitMemory();

}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageColorSpaceThetaML::CreateMemory(const int _sz)
{
		sita_n=new double[_sz];
		m_n = new double[_sz];
		L_n = new double[_sz];
		X_n = new double[_sz];
		Y_n = new double[_sz];

}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageColorSpaceThetaML::CvtLabXY2ThetaMLXY(const double* L,
	const double* A, 
	const double* B,
	const int WIDTH, 
	const int HEIGHT)
{
	const int SIZE = WIDTH*HEIGHT;
	this->ReleaseMemory();
	this->CreateMemory(SIZE);

	for (int x = 0; x<WIDTH; x++) {
		for (int  y = 0; y<HEIGHT; y++) {
		
			const int Idx = y*WIDTH + x;

			ConvertLab2ThetaML( L[Idx]		, A[Idx]	, B[Idx],		x,			y,		WIDTH,	HEIGHT,
								sita_n[Idx]	, m_n[Idx]	, L_n[Idx],		X_n[Idx],	Y_n[Idx]);
		}
	}

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageColorSpaceThetaML::fillTheta(IplImage * _img, const float _s)
{
	const int WIDTH	 = _img->width;
	const int HEIGHT = _img->height;

	for (int x = 0; x<WIDTH; x++) {
		for (int y = 0; y<HEIGHT; y++) {

				const int Idx = y*WIDTH + x;
				cvSetReal2D(_img,y,x,sita_n[Idx]*_s);
			
		}
	}

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageColorSpaceThetaML::fillM(IplImage * _img, const float _s)
{
	const int WIDTH = _img->width;
	const int HEIGHT = _img->height;

	for (int x = 0; x<WIDTH; x++) {
		for (int y = 0; y<HEIGHT; y++) {

			const int Idx = y*WIDTH + x;
			cvSetReal2D(_img, y, x, m_n[Idx] * _s);


		}
	}

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void ImageColorSpaceThetaML::fillL(IplImage * _img, const float _s)
{
	const int WIDTH = _img->width;
	const int HEIGHT = _img->height;

	for (int x = 0; x<WIDTH; x++) {
		for (int y = 0; y<HEIGHT; y++) {

			const int Idx = y*WIDTH + x;
			cvSetReal2D(_img, y, x, L_n[Idx] * _s);


		}
	}

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/