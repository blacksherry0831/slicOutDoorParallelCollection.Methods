// SLIC.cpp: implementation of the SLIC class.
//
// Copyright (C) Radhakrishna Achanta 2012
// All rights reserved
// Email: firstname.lastname@epfl.ch
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "module_all_cui.h"
/*----------------------------------------------------------------------------------------------------------------*/
#include "cudaLib/CUDA_Code_LAB.h"
/*----------------------------------------------------------------------------------------------------------------*/
#include "ColorSpace/ImageColorSpaceLAB.h"
#include "ColorSpace/ImageColorSpaceThetaML.h"
#include "ColorSpace/DistanceColorSpace.h"
/*----------------------------------------------------------------------------------------------------------------*/
#include "MY_SDK_LIB/TimeMeasure.hpp"
/*----------------------------------------------------------------------------------------------------------------*/
#include "SLIC_LAB_CUDA.h"
/*----------------------------------------------------------------------------------------------------------------*/
/**
*构造函数
*完成变量初始化
*/
/*----------------------------------------------------------------------------------------------------------------*/
SLIC_LAB_CUDA::SLIC_LAB_CUDA()
{
	this->InitParameter();
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*构造函数...
*导入图像数据，完成变量初始化
*@param pMD_t 图像数据及暂存的中间结果结果
*/
/*----------------------------------------------------------------------------------------------------------------*/
SLIC_LAB_CUDA::SLIC_LAB_CUDA(ImageMemData* pMD_t)
{
	this->pMD = pMD_t;
	this->InitParameter();
	this->CuiGetImageData();
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*构造函数...
*导入图像数据，完成变量初始化
*@param pMD_t 图像数据及暂存的中间结果结果
*/
/*----------------------------------------------------------------------------------------------------------------*/
SLIC_LAB_CUDA::SLIC_LAB_CUDA(ImageData* pMD_t)
{
	TRACE_FUNC();
	this->pIMD = pMD_t;
	this->InitParameter();
	this->CuiGetImageDataPIMD();
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*初始化成员变量
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::InitParameter(void)
{

	TRACE_FUNC();
	m_lvecvec = NULL;
	m_avecvec = NULL;
	m_bvecvec = NULL;
	CuiImgData = NULL;
	Cui_Matrix_E = NULL;
	Cui_Matrix_D = NULL;
	Cui_Matrix_W = NULL;
	Cui_Matrix_W_Vein = NULL;
	Cui_Matrix_L = NULL;
	Cui_MatrixEigenVector_L = NULL;
	CUi_MatrixEigenValue_L = NULL;
	Cui_Matrix_Category_Lable = NULL;
	Cui_Matrix_Category_Simple = NULL;
	CuiImgLables = NULL;
	/////////////////
	CuiCvImage = NULL;
	Cui_ARGB_Image = NULL;
	Cui_lab_hsv_Image = NULL;
	h_l_plane = NULL;
	s_a_plane = NULL;
	v_b_plane = NULL;
	
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*析构函数，检查并释放内存
*/
/*----------------------------------------------------------------------------------------------------------------*/
SLIC_LAB_CUDA::~SLIC_LAB_CUDA()
{

	if (CuiImgData) delete[]CuiImgData;
	if (CuiImgLables) delete[]CuiImgLables;

	if (Cui_Matrix_E) delete[]Cui_Matrix_E;
	if (Cui_Matrix_D) delete[]Cui_Matrix_D;
	if (Cui_Matrix_W) delete[]Cui_Matrix_W;
	if (Cui_Matrix_L) delete[]Cui_Matrix_L;
	if (Cui_MatrixEigenVector_L) delete[]Cui_MatrixEigenVector_L;
	if (CUi_MatrixEigenValue_L)  delete[]CUi_MatrixEigenValue_L;
	if (Cui_Matrix_Category_Simple)  delete[]Cui_Matrix_Category_Simple;
	if (Cui_Matrix_Category_Lable)  delete[]Cui_Matrix_Category_Lable;

	/**********************************************************/
	if (CuiCvImage)  cvReleaseImage(&CuiCvImage);
	if (Cui_ARGB_Image) cvReleaseImage(&Cui_ARGB_Image);
	if (Cui_lab_hsv_Image)  cvReleaseImage(&Cui_lab_hsv_Image);
	if (h_l_plane)  cvReleaseImage(&h_l_plane);
	if (s_a_plane)  cvReleaseImage(&s_a_plane);
	if (v_b_plane)  cvReleaseImage(&v_b_plane);
	/********************************************/

	if (m_lvecvec)
	{
		for (int d = 0; d < m_depth; d++) delete[] m_lvecvec[d];
		delete[] m_lvecvec;
	}
	if (m_avecvec)
	{
		for (int d = 0; d < m_depth; d++) delete[] m_avecvec[d];
		delete[] m_avecvec;
	}
	if (m_bvecvec)
	{
		for (int d = 0; d < m_depth; d++) delete[] m_bvecvec[d];
		delete[] m_bvecvec;
	}


	delete[] Cui_Matrix_W_Vein;

}


//==============================================================================
///	RGB2XYZ
///
/// sRGB (D65 illuninant assumption) to XYZ conversion
//==============================================================================
//void SLIC_LAB_CUDA::RGB2XYZ(
//	const int&		sR,
//	const int&		sG,
//	const int&		sB,
//	double&			X,
//	double&			Y,
//	double&			Z)
//{
//	double R = sR/255.0;
//	double G = sG/255.0;
//	double B = sB/255.0;
//
//	double r, g, b;
//
//	if(R <= 0.04045)	r = R/12.92;
//	else				r = pow((R+0.055)/1.055,2.4);
//	if(G <= 0.04045)	g = G/12.92;
//	else				g = pow((G+0.055)/1.055,2.4);
//	if(B <= 0.04045)	b = B/12.92;
//	else				b = pow((B+0.055)/1.055,2.4);
//
//	X = r*0.4124564 + g*0.3575761 + b*0.1804375;
//	Y = r*0.2126729 + g*0.7151522 + b*0.0721750;
//	Z = r*0.0193339 + g*0.1191920 + b*0.9503041;
//}

//===========================================================================
///	RGB2LAB
//===========================================================================
//void SLIC_LAB_CUDA::RGB2LAB(const int& sR, const int& sG, const int& sB, double& lval, double& aval, double& bval)
//{
//	//------------------------
//	// sRGB to XYZ conversion
//	//------------------------
//	double X, Y, Z;
//	RGB2XYZ(sR, sG, sB, X, Y, Z);
//
//	//------------------------
//	// XYZ to LAB conversion
//	//------------------------
//	double epsilon = 0.008856;	//actual CIE standard
//	double kappa   = 903.3;		//actual CIE standard
//
//	double Xr = 0.950456;	//reference white
//	double Yr = 1.0;		//reference white
//	double Zr = 1.088754;	//reference white
//
//	double xr = X/Xr;
//	double yr = Y/Yr;
//	double zr = Z/Zr;
//
//	double fx, fy, fz;
//	if(xr > epsilon)	fx = pow(xr, 1.0/3.0);
//	else				fx = (kappa*xr + 16.0)/116.0;
//	if(yr > epsilon)	fy = pow(yr, 1.0/3.0);
//	else				fy = (kappa*yr + 16.0)/116.0;
//	if(zr > epsilon)	fz = pow(zr, 1.0/3.0);
//	else				fz = (kappa*zr + 16.0)/116.0;
//
//	lval = 116.0*fy-16.0;
//	aval = 500.0*(fx-fy);
//	bval = 200.0*(fy-fz);
//}
string SLIC_LAB_CUDA::cuiGetCurrentTime(void)
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
//===========================================================================
///	PerturbSeeds
//===========================================================================
void SLIC_LAB_CUDA::PerturbSeeds(
	vector<double>&				kseedsl,
	vector<double>&				kseedsa,
	vector<double>&				kseedsb,
	vector<double>&				kseedsx,
	vector<double>&				kseedsy,
	const vector<double>&       edges)
{
	TRACE_FUNC();
	double*  m_lvec = NULL;
	double*  m_avec = NULL;
	double*  m_bvec = NULL;
	const int dx8[8] = { -1, -1,  0,  1, 1, 1, 0, -1 };
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1 };

	int numseeds = kseedsl.size();

	for (int n = 0; n < numseeds; n++)
	{
		int ox = kseedsx[n];//original x
		int oy = kseedsy[n];//original y
		int oind = oy*m_width + ox;

		int storeind = oind;
		for (int i = 0; i < 8; i++)
		{
			int nx = ox + dx8[i];//new x
			int ny = oy + dy8[i];//new y

			if (nx >= 0 && nx < m_width && ny >= 0 && ny < m_height)
			{
				int nind = ny*m_width + nx;
				if (edges[nind] < edges[storeind])
				{
					storeind = nind;
				}
			}
		}
		if (storeind != oind)
		{
			kseedsx[n] = storeind%m_width;
			kseedsy[n] = storeind / m_width;
			kseedsl[n] = m_lvec[storeind];
			kseedsa[n] = m_avec[storeind];
			kseedsb[n] = m_bvec[storeind];
		}
	}
}


//===========================================================================
///	GetLABXYSeeds_ForGivenStepSize
///
/// The k seed values are taken as uniform spatial pixel samples.
//===========================================================================
void SLIC_LAB_CUDA::GetLABXYSeeds_ForGivenStepSize(
	vector<double>&				kseedsl,
	vector<double>&				kseedsa,
	vector<double>&				kseedsb,
	vector<double>&				kseedsx,
	vector<double>&				kseedsy,
	const int&					STEP,
	const bool&					perturbseeds,
	const vector<double>&       edgemag)
{
	double*  m_lvec = NULL;
	double*  m_avec = NULL;
	double*  m_bvec = NULL;
	const bool hexgrid = false;
	int numseeds(0);
	int n(0);

	//int xstrips = m_width/STEP;
	//int ystrips = m_height/STEP;
	int xstrips = (0.5 + double(m_width) / double(STEP));
	int ystrips = (0.5 + double(m_height) / double(STEP));

	int xerr = m_width - STEP*xstrips; if (xerr < 0) { xstrips--; xerr = m_width - STEP*xstrips; }
	int yerr = m_height - STEP*ystrips; if (yerr < 0) { ystrips--; yerr = m_height - STEP*ystrips; }

	double xerrperstrip = double(xerr) / double(xstrips);
	double yerrperstrip = double(yerr) / double(ystrips);

	int xoff = STEP / 2;
	int yoff = STEP / 2;
	//-------------------------
	numseeds = xstrips*ystrips;
	//-------------------------
	kseedsl.resize(numseeds);
	kseedsa.resize(numseeds);
	kseedsb.resize(numseeds);
	kseedsx.resize(numseeds);
	kseedsy.resize(numseeds);

	for (int y = 0; y < ystrips; y++)
	{
		int ye = y*yerrperstrip;
		for (int x = 0; x < xstrips; x++)
		{
			int xe = x*xerrperstrip;
			int seedx = (x*STEP + xoff + xe);
			if (hexgrid) { seedx = x*STEP + (xoff << (y & 0x1)) + xe; seedx = min(m_width - 1, seedx); }//for hex grid sampling
			int seedy = (y*STEP + yoff + ye);
			int i = seedy*m_width + seedx;

			kseedsl[n] = m_lvec[i];
			kseedsa[n] = m_avec[i];
			kseedsb[n] = m_bvec[i];
			kseedsx[n] = seedx;
			kseedsy[n] = seedy;
			n++;
		}
	}

#if 0
	if (perturbseeds)
	{
		PerturbSeeds(kseedsl, kseedsa, kseedsb, kseedsx, kseedsy, edgemag);
	}
#endif
}
void SLIC_LAB_CUDA::DetectLabEdges(
	const double*				lvec,
	const double*				avec,
	const double*				bvec,
	const int&					width,
	const int&					height,
	vector<double>&				edges)
{
	int sz = width*height;

	edges.resize(sz, 0);
	for (int j = 1; j < height - 1; j++)
	{
		for (int k = 1; k < width - 1; k++)
		{
			int i = j*width + k;

			double dx = (lvec[i - 1] - lvec[i + 1])*(lvec[i - 1] - lvec[i + 1]) +
				(avec[i - 1] - avec[i + 1])*(avec[i - 1] - avec[i + 1]) +
				(bvec[i - 1] - bvec[i + 1])*(bvec[i - 1] - bvec[i + 1]);

			double dy = (lvec[i - width] - lvec[i + width])*(lvec[i - width] - lvec[i + width]) +
				(avec[i - width] - avec[i + width])*(avec[i - width] - avec[i + width]) +
				(bvec[i - width] - bvec[i + width])*(bvec[i - width] - bvec[i + width]);

			//edges[i] = fabs(dx) + fabs(dy);
			edges[i] = dx*dx + dy*dy;
		}
	}
}
//===========================================================================
///	GetKValues_LABXYZ
///
/// The k seed values are taken as uniform spatial pixel samples.
//===========================================================================
void SLIC_LAB_CUDA::GetKValues_LABXYZ(
	vector<double>&				kseedsl,
	vector<double>&				kseedsa,
	vector<double>&				kseedsb,
	vector<double>&				kseedsx,
	vector<double>&				kseedsy,
	vector<double>&				kseedsz,
	const int&				STEP)
{
	const bool hexgrid = false;
	int numseeds(0);
	int n(0);

	int xstrips = (0.5 + double(m_width) / double(STEP));
	int ystrips = (0.5 + double(m_height) / double(STEP));
	int zstrips = (0.5 + double(m_depth) / double(STEP));

	int xerr = m_width - STEP*xstrips; if (xerr < 0) { xstrips--; xerr = m_width - STEP*xstrips; }
	int yerr = m_height - STEP*ystrips; if (yerr < 0) { ystrips--; yerr = m_height - STEP*ystrips; }
	int zerr = m_depth - STEP*zstrips; if (zerr < 0) { zstrips--; zerr = m_depth - STEP*zstrips; }

	double xerrperstrip = double(xerr) / double(xstrips);
	double yerrperstrip = double(yerr) / double(ystrips);
	double zerrperstrip = double(zerr) / double(zstrips);

	int xoff = STEP / 2;
	int yoff = STEP / 2;
	int zoff = STEP / 2;
	//-------------------------
	numseeds = xstrips*ystrips*zstrips;
	//-------------------------
	kseedsl.resize(numseeds);
	kseedsa.resize(numseeds);
	kseedsb.resize(numseeds);
	kseedsx.resize(numseeds);
	kseedsy.resize(numseeds);
	kseedsz.resize(numseeds);

	for (int z = 0; z < zstrips; z++)
	{
		int ze = z*zerrperstrip;
		int d = (z*STEP + zoff + ze);
		for (int y = 0; y < ystrips; y++)
		{
			int ye = y*yerrperstrip;
			for (int x = 0; x < xstrips; x++)
			{
				int xe = x*xerrperstrip;
				int i = (y*STEP + yoff + ye)*m_width + (x*STEP + xoff + xe);

				kseedsl[n] = m_lvecvec[d][i];
				kseedsa[n] = m_avecvec[d][i];
				kseedsb[n] = m_bvecvec[d][i];
				kseedsx[n] = (x*STEP + xoff + xe);
				kseedsy[n] = (y*STEP + yoff + ye);
				kseedsz[n] = d;
				n++;
			}
		}
	}
}

//===========================================================================
///	PerformSuperpixelSLIC
///
///	Performs k mean segmentation. It is fast because it looks locally, not
/// over the entire image.
//===========================================================================
void SLIC_LAB_CUDA::PerformSuperpixelSLIC(
	vector<double>&				kseedsl,
	vector<double>&				kseedsa,
	vector<double>&				kseedsb,
	vector<double>&				kseedsx,
	vector<double>&				kseedsy,
	int*&					klabels,
	const int&				STEP,
	const vector<double>&                   edgemag,
	const double&				M)
{
	double*  m_lvec = NULL;
	double*  m_avec = NULL;
	double*  m_bvec = NULL;
	int sz = m_width*m_height;
	const int numk = kseedsl.size();
	//----------------
	int offset = STEP;
	//if(STEP < 8) offset = STEP*1.5;//to prevent a crash due to a very small step size
	//----------------

	vector<double> clustersize(numk, 0);
	vector<double> inv(numk, 0);//to store 1/clustersize[k] values

	vector<double> sigmal(numk, 0);
	vector<double> sigmaa(numk, 0);
	vector<double> sigmab(numk, 0);
	vector<double> sigmax(numk, 0);
	vector<double> sigmay(numk, 0);
	vector<double> distvec(sz, DBL_MAX);

	double invwt = 1.0 / ((STEP / M)*(STEP / M));

	int x1, y1, x2, y2;
	double l, a, b;
	double dist;
	double distxy;
	for (int itr = 0; itr < 10; itr++)
	{
		distvec.assign(sz, DBL_MAX);
		for (int n = 0; n < numk; n++)
		{
			y1 = max(0.0, kseedsy[n] - offset);
			y2 = min((double)m_height, kseedsy[n] + offset);
			x1 = max(0.0, kseedsx[n] - offset);
			x2 = min((double)m_width, kseedsx[n] + offset);


			for (int y = y1; y < y2; y++)
			{
				for (int x = x1; x < x2; x++)
				{
					int i = y*m_width + x;

					l = m_lvec[i];
					a = m_avec[i];
					b = m_bvec[i];

					dist = (l - kseedsl[n])*(l - kseedsl[n]) +
						(a - kseedsa[n])*(a - kseedsa[n]) +
						(b - kseedsb[n])*(b - kseedsb[n]);

					distxy = (x - kseedsx[n])*(x - kseedsx[n]) +
						(y - kseedsy[n])*(y - kseedsy[n]);

					//------------------------------------------------------------------------
					dist += distxy*invwt;//dist = sqrt(dist) + sqrt(distxy*invwt);//this is more exact
										 //------------------------------------------------------------------------
					if (dist < distvec[i])
					{
						distvec[i] = dist;
						klabels[i] = n;
					}
				}
			}
		}
		//-----------------------------------------------------------------
		// Recalculate the centroid and store in the seed values
		//-----------------------------------------------------------------
		//instead of reassigning memory on each iteration, just reset.

		sigmal.assign(numk, 0);
		sigmaa.assign(numk, 0);
		sigmab.assign(numk, 0);
		sigmax.assign(numk, 0);
		sigmay.assign(numk, 0);
		clustersize.assign(numk, 0);
		//------------------------------------
		//edgesum.assign(numk, 0);
		//------------------------------------

		{int ind(0);
		for (int r = 0; r < m_height; r++)
		{
			for (int c = 0; c < m_width; c++)
			{
				sigmal[klabels[ind]] += m_lvec[ind];
				sigmaa[klabels[ind]] += m_avec[ind];
				sigmab[klabels[ind]] += m_bvec[ind];
				sigmax[klabels[ind]] += c;
				sigmay[klabels[ind]] += r;
				//------------------------------------
				//edgesum[klabels[ind]] += edgemag[ind];
				//------------------------------------
				clustersize[klabels[ind]] += 1.0;
				ind++;
			}
		}}

		{for (int k = 0; k < numk; k++)
		{
			if (clustersize[k] <= 0) clustersize[k] = 1;
			inv[k] = 1.0 / clustersize[k];//computing inverse now to multiply, than divide later
		}}

		{for (int k = 0; k < numk; k++)
		{
			kseedsl[k] = sigmal[k] * inv[k];
			kseedsa[k] = sigmaa[k] * inv[k];
			kseedsb[k] = sigmab[k] * inv[k];
			kseedsx[k] = sigmax[k] * inv[k];
			kseedsy[k] = sigmay[k] * inv[k];
			//------------------------------------
			//edgesum[k] *= inv[k];
			//------------------------------------
		}}
	}
}

//===========================================================================
///	PerformSupervoxelSLIC
///
///	Performs k mean segmentation. It is fast because it searches locally, not
/// over the entire image.
//===========================================================================
void SLIC_LAB_CUDA::PerformSupervoxelSLIC(
	vector<double>&				kseedsl,
	vector<double>&				kseedsa,
	vector<double>&				kseedsb,
	vector<double>&				kseedsx,
	vector<double>&				kseedsy,
	vector<double>&				kseedsz,
	int**&					klabels,
	const int&				STEP,
	const double&				compactness)
{
	int sz = m_width*m_height;
	const int numk = kseedsl.size();
	//int numitr(0);

	//----------------
	int offset = STEP;
	//if(STEP < 8) offset = STEP*1.5;//to prevent a crash due to a very small step size
	//----------------

	vector<double> clustersize(numk, 0);
	vector<double> inv(numk, 0);//to store 1/clustersize[k] values

	vector<double> sigmal(numk, 0);
	vector<double> sigmaa(numk, 0);
	vector<double> sigmab(numk, 0);
	vector<double> sigmax(numk, 0);
	vector<double> sigmay(numk, 0);
	vector<double> sigmaz(numk, 0);

	vector< double > initdouble(sz, DBL_MAX);
	vector< vector<double> > distvec(m_depth, initdouble);
	//vector<double> distvec(sz, DBL_MAX);

	double invwt = 1.0 / ((STEP / compactness)*(STEP / compactness));//compactness = 20.0 is usually good.

	int x1, y1, x2, y2, z1, z2;
	double l, a, b;
	double dist;
	double distxyz;
	for (int itr = 0; itr < 5; itr++)
	{
		distvec.assign(m_depth, initdouble);
		for (int n = 0; n < numk; n++)
		{
			y1 = max(0.0, kseedsy[n] - offset);
			y2 = min((double)m_height, kseedsy[n] + offset);
			x1 = max(0.0, kseedsx[n] - offset);
			x2 = min((double)m_width, kseedsx[n] + offset);
			z1 = max(0.0, kseedsz[n] - offset);
			z2 = min((double)m_depth, kseedsz[n] + offset);


			for (int z = z1; z < z2; z++)
			{
				for (int y = y1; y < y2; y++)
				{
					for (int x = x1; x < x2; x++)
					{
						int i = y*m_width + x;

						l = m_lvecvec[z][i];
						a = m_avecvec[z][i];
						b = m_bvecvec[z][i];

						dist = (l - kseedsl[n])*(l - kseedsl[n]) +
							(a - kseedsa[n])*(a - kseedsa[n]) +
							(b - kseedsb[n])*(b - kseedsb[n]);

						distxyz = (x - kseedsx[n])*(x - kseedsx[n]) +
							(y - kseedsy[n])*(y - kseedsy[n]) +
							(z - kseedsz[n])*(z - kseedsz[n]);
						//------------------------------------------------------------------------
						dist += distxyz*invwt;
						//------------------------------------------------------------------------
						if (dist < distvec[z][i])
						{
							distvec[z][i] = dist;
							klabels[z][i] = n;
						}
					}
				}
			}
		}
		//-----------------------------------------------------------------
		// Recalculate the centroid and store in the seed values
		//-----------------------------------------------------------------
		//instead of reassigning memory on each iteration, just reset.

		sigmal.assign(numk, 0);
		sigmaa.assign(numk, 0);
		sigmab.assign(numk, 0);
		sigmax.assign(numk, 0);
		sigmay.assign(numk, 0);
		sigmaz.assign(numk, 0);
		clustersize.assign(numk, 0);

		for (int d = 0; d < m_depth; d++)
		{
			int ind(0);
			for (int r = 0; r < m_height; r++)
			{
				for (int c = 0; c < m_width; c++)
				{
					sigmal[klabels[d][ind]] += m_lvecvec[d][ind];
					sigmaa[klabels[d][ind]] += m_avecvec[d][ind];
					sigmab[klabels[d][ind]] += m_bvecvec[d][ind];
					sigmax[klabels[d][ind]] += c;
					sigmay[klabels[d][ind]] += r;
					sigmaz[klabels[d][ind]] += d;

					clustersize[klabels[d][ind]] += 1.0;
					ind++;
				}
			}
		}

		{for (int k = 0; k < numk; k++)
		{
			if (clustersize[k] <= 0) clustersize[k] = 1;
			inv[k] = 1.0 / clustersize[k];//computing inverse now to multiply, than divide later
		}}

		{for (int k = 0; k < numk; k++)
		{
			kseedsl[k] = sigmal[k] * inv[k];
			kseedsa[k] = sigmaa[k] * inv[k];
			kseedsb[k] = sigmab[k] * inv[k];
			kseedsx[k] = sigmax[k] * inv[k];
			kseedsy[k] = sigmay[k] * inv[k];
			kseedsz[k] = sigmaz[k] * inv[k];
		}}
	}
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*
*将wstring类型的窄字符串，装换为string的宽字符
*@param  wideString 宽字符
*@param  窄字符串
*/
/*----------------------------------------------------------------------------------------------------------------*/
string SLIC_LAB_CUDA::Wide2Narrow(const wstring&		wideString)
{
#if _MSC_VER
	int m_codepage = ::_getmbcp();

	int result = ::WideCharToMultiByte(
		m_codepage,  // Code page
		0,		// Default
		wideString.c_str(), // WCS buffer
		-1,		// Assume null terminated str, calclate length auto
		0,      // Buffer to receive MBCS string
		0,		// Length of MB buffer ( 0 -> return length required )
		0,		// lpdefaultChar
		0		// lpUsedDefaultChar
	);
	assert(result);
	char *test = new char[result + 1];
	result = ::WideCharToMultiByte(
		m_codepage,  // Code page
		0,		// Default
		wideString.c_str(), // WCS buffer
		-1,		// Assume null terminated str, calclate length auto
		test,   // Buffer to receive MBCS string
		result,	// Length of MB buffer ( 0 -> return length required )
		0,		// lpdefaultChar
		0		// lpUsedDefaultChar
	);

	std::string temp(test);
	delete[]test;

	return temp;
#endif

#if linux||__linux||__linux__||__GNUC__
	ASSERT(0);
	std::string temp("linux_not_support");
	return  temp;
#endif


}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*
*将string类型的窄字符串，装换为wstring的宽字符
*@param  narrowString 窄字符
*@return  宽字符串
*/
/*----------------------------------------------------------------------------------------------------------------*/
wstring SLIC_LAB_CUDA::Narrow2Wide(const std::string& narrowString)
{
#if _MSC_VER
	int m_codepage = _getmbcp();

	int numChars =
		::MultiByteToWideChar(m_codepage,
			MB_PRECOMPOSED,
			narrowString.c_str(),
			-1,
			0,
			0
		);
	assert(numChars);
	//	TRACE("Number of characters in the string is %d", numChars);

	wchar_t* test = new wchar_t[numChars + 1];
	numChars =
		::MultiByteToWideChar(m_codepage,
			MB_PRECOMPOSED,
			narrowString.c_str(),
			-1,
			test,
			numChars
		);

	std::wstring temp(test);
	delete[]test;

	return temp;
#endif
#if linux||__linux||__linux__||__GNUC__
	ASSERT(0);
	std::wstring temp(L"linux_not_support");
	return  temp;
#endif

}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*建立连接矩阵E  \n
*从Lables中寻找并保存相邻矩阵E
*@param  SelfNighbour 自身不计算相邻（默认值为0）
*@param  filename  保存相邻矩阵的文件名
*@param  path   相邻矩阵E保存的路径
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::CuiFindSaveNighbour_E_matrix(
	const UINT32&                  SelfNighbour,
	const string&				filename,
	const string&				path)
{
#if 0
	const int dx8[8] = { -1, -1,  0,  1, 1, 1, 0, -1 };
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1 };
	int mainindex(0); int cind(0);

	if (Cui_Matrix_E) {
		delete[]Cui_Matrix_E;
	}
	Cui_Matrix_E = new UINT32[pMD->slic_current_num*pMD->slic_current_num];
	memset(Cui_Matrix_E, 0, pMD->slic_current_num*pMD->slic_current_num * sizeof(UINT32));

#ifdef WINDOWS
	char fname[256];
	char extn[256];
	_splitpath(filename.c_str(), NULL, NULL, fname, extn);
	string temp = fname;
	string finalpath = path + temp + string(".dat");
#else
	string nameandextension = filename;
	size_t pos = filename.find_last_of("/");
	if (pos != string::npos)//if a slash is found, then take the filename with extension
	{
		nameandextension = filename.substr(pos + 1);
	}
	string newname = nameandextension.replace(nameandextension.rfind(".") + 1, 3, "dat");//find the position of the dot and replace the 3 characters following it.
	string finalpath = path + newname;
#endif
	for (register int ii = 0; ii<pMD->slic_current_num; ii++) {
		Cui_Matrix_E[ii*pMD->slic_current_num + ii] = SelfNighbour;
	}

	for (register int j = 0; j <CuiHeight; j++) {
		for (register int k = 0; k <CuiWidth; k++) {
			int np(0);
			for (register int i = 0; i < 8; i++) {
				int x = k + dx8[i];
				int y = j + dy8[i];

				if ((x >= 0 && x < CuiWidth) && (y >= 0 && y <CuiHeight)) {
					int index = y*CuiWidth + x;

					//if( false == istaken[index] )//comment this to obtain internal contours
					{

						if (CuiImgLables[mainindex] != CuiImgLables[index]) {
							//index与mainindex超像素图块具有连接关系
							UINT32  matrix_x = CuiImgLables[index];
							UINT32  matrix_y = CuiImgLables[mainindex];
							//统计连接个数
							Cui_Matrix_E[matrix_x*pMD->slic_current_num + matrix_y] = ++Cui_Matrix_E[matrix_y*pMD->slic_current_num + matrix_x];
							if (Cui_Matrix_E[matrix_x*pMD->slic_current_num + matrix_y]>999 * 2) {
								Cui_Matrix_E[matrix_x*pMD->slic_current_num + matrix_y] = Cui_Matrix_E[matrix_y*pMD->slic_current_num + matrix_x] = 999 * 2;
							}
						}
					}
				}
			}
			///////////////////////////////////////////////////
			////////////////////////////////////////////////////
			mainindex++;
		}
	}
	/************************************************************/
	for (register int i = 0; i <pMD->slic_current_num; i++) {
		for (register int j = 0; j <pMD->slic_current_num; j++) {
			//修正连接个数		
			Cui_Matrix_E[i*pMD->slic_current_num + j] = (Cui_Matrix_E[i*pMD->slic_current_num + j] + 4) / 6;
		}

	}
	{
		char data_t[1024];
		ofstream outfile;
		outfile.open(finalpath.c_str(), ios::out);
		for (register int i = 0; i <pMD->slic_current_num; i++) {
			for (register int j = 0; j <pMD->slic_current_num; j++) {
				UINT32 cui_label_value = Cui_Matrix_E[i*pMD->slic_current_num + j];
				sprintf(data_t, "%4d", cui_label_value);
				outfile << data_t;
			}
			outfile << endl;
		}
		outfile.close();
	}

	/*************************************************************/
#if 0
	{
		double  threshold;		//计算超像素面积
		threshold = 4 * sqrtl(CuiWidth*CuiHeight / pMD->slic_current_num);	//周长
		threshold = threshold / 8; //8分之一周长

		for (int i = 0; i <pMD->slic_current_num; i++) {
			for (int j = 0; j <pMD->slic_current_num; j++) {

				if (Cui_Matrix_E[i*pMD->slic_current_num + j]<threshold) {
					Cui_Matrix_E[i*pMD->slic_current_num + j] = 0;
				}
				else {
					Cui_Matrix_E[i*pMD->slic_current_num + j] = 1;
				}
			}

		}
	}
#else
	{
		double  threshold;		//计算超像素面积
		threshold = 4 * sqrtl(CuiWidth*CuiHeight / pMD->slic_current_num);	//周长
		threshold = threshold / 8; //8分之一周长

		for (register int i = 0; i <pMD->slic_current_num; i++) {
			for (register int j = 0; j <pMD->slic_current_num; j++) {

				if (Cui_Matrix_E[i*pMD->slic_current_num + j]>0) {
					//相邻则置为1 
					Cui_Matrix_E[i*pMD->slic_current_num + j] = 1;
				}
				else {
					//不相邻则置为0
					Cui_Matrix_E[i*pMD->slic_current_num + j] = 0;
				}
			}

		}
	}
#endif
	{
		char data_t[1024];
		ofstream outfile;
		outfile.open("Matrix_E.data", ios::out);
		for (int i = 0; i <pMD->slic_current_num; i++) {
			for (int j = 0; j <pMD->slic_current_num; j++) {
				UINT32 cui_label_value = Cui_Matrix_E[i*pMD->slic_current_num + j];
				sprintf(data_t, "%4d", cui_label_value);
				outfile << data_t;
			}
			outfile << endl;
		}
		outfile.close();
	}

	/*************************************************************/
#endif
#if _MSC_VER
	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
#endif
	{
		if (Cui_Matrix_E) {
			delete[]Cui_Matrix_E;
		}
		Cui_Matrix_E = new UINT32[pMD->slic_current_num*pMD->slic_current_num];
		cui_GeneralImgProcess::CuiSetNighbour_E_matrix(CuiImgLables, pMD->slic_current_num, Cui_Matrix_E, pMD->ImgWidth, pMD->ImgHeight, filename, path);
	}
#if _MSC_VER
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2 - QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒

#endif

}

//===========================================================================
///	SaveSupervoxelLabels
///
///	Save labels in raster scan order.
//===========================================================================
void SLIC_LAB_CUDA::SaveSupervoxelLabels(
	const int**&				labels,
	const int&					width,
	const int&					height,
	const int&					depth,
	const string&				filename,
	const string&				path)
{
#ifdef WINDOWS
	char fname[256];
	char extn[256];
	_splitpath(filename.c_str(), NULL, NULL, fname, extn);
	string temp = fname;
	string finalpath = path + temp + string(".dat");
#else
	string nameandextension = filename;
	size_t pos = filename.find_last_of("/");
	if (pos != string::npos)//if a slash is found, then take the filename with extension
	{
		nameandextension = filename.substr(pos + 1);
	}
	string newname = nameandextension.replace(nameandextension.rfind(".") + 1, 3, "dat");//find the position of the dot and replace the 3 characters following it.
	string finalpath = path + newname;
#endif

	int sz = width*height;
	ofstream outfile;
	outfile.open(finalpath.c_str(), ios::binary);
	for (int d = 0; d < depth; d++)
	{
		for (int i = 0; i < sz; i++)
		{
			outfile.write((const char*)&labels[d][i], sizeof(int));
		}
	}
	outfile.close();
}

//===========================================================================
///	EnforceLabelConnectivity
///
///		1. finding an adjacent label for each new component at the start
///		2. if a certain component is too small, assigning the previously found
///		    adjacent label to this component, and not incrementing the label.
//===========================================================================
void SLIC_LAB_CUDA::EnforceLabelConnectivity(
	const int*					labels,//input labels that need to be corrected to remove stray labels
	const int					width,
	const int					height,
	int*&						nlabels,//new labels
	int&						numlabels,//the number of labels changes in the end if segments are removed
	const int&					K) //the number of superpixels desired by the user
{
	//	const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	//	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};

	const int dx4[4] = { -1,  0,  1,  0 };
	const int dy4[4] = { 0, -1,  0,  1 };

	const int sz = width*height;
	const int SUPSZ = sz / K;
	
	for (int i = 0; i < sz; i++) 
		nlabels[i] = -1;

	int label(0);
	int* xvec = new int[sz];
	int* yvec = new int[sz];
	int oindex(0);
	int adjlabel(0);//adjacent label
	for (register int j = 0; j < height; j++)
	{
		for (register int k = 0; k < width; k++)
		{
			if (0 > nlabels[oindex])
			{
				nlabels[oindex] = label;
				//--------------------
				// Start a new segment
				//--------------------
				xvec[0] = k;
				yvec[0] = j;
				//-------------------------------------------------------
				// Quickly find an adjacent label for use later if needed
				//-------------------------------------------------------
				{for (register int n = 0; n < 4; n++)
				{
					int x = xvec[0] + dx4[n];
					int y = yvec[0] + dy4[n];
					if ((x >= 0 && x < width) && (y >= 0 && y < height))
					{
						int nindex = y*width + x;
						if (nlabels[nindex] >= 0) adjlabel = nlabels[nindex];
					}
				}}

				int count(1);
				for (register int c = 0; c < count; c++)
				{
					for (register int n = 0; n < 4; n++)
					{
						int x = xvec[c] + dx4[n];
						int y = yvec[c] + dy4[n];

						if ((x >= 0 && x < width) && (y >= 0 && y < height))
						{
							int nindex = y*width + x;

							if (0 > nlabels[nindex] && labels[oindex] == labels[nindex])
							{
								xvec[count] = x;
								yvec[count] = y;
								nlabels[nindex] = label;
								count++;
							}
						}

					}
				}
				//-------------------------------------------------------
				// If segment size is less then a limit, assign an
				// adjacent label found before, and decrement label count.
				//-------------------------------------------------------
				if (count <= SUPSZ >> 2)
				{
					for (register int c = 0; c < count; c++)
					{
						int ind = yvec[c] * width + xvec[c];
						nlabels[ind] = adjlabel;
					}
					label--;
				}
				label++;
			}
			oindex++;
		}
	}
	numlabels = label;

	if (xvec) delete[] xvec;
	if (yvec) delete[] yvec;
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::EnforceLabelConnectivityEx(
	int*	klabels,
	const int width,
	const int height,
	int & numlabels,
	const int & K)
{
	TimeMeasure tm("SuperPixel.Connectivity");

	const int sz = width*height;
	const int sz_int = width*height*sizeof(int);
	
	int* nlabels = new int[sz];
	{
		EnforceLabelConnectivity(
		klabels,
		width,
		height,
		nlabels,
		numlabels,
		K);
		
		memcpy(klabels, nlabels,sz_int);	
	}		
	if (nlabels) delete[] nlabels;	
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
//===========================================================================
///	RelabelStraySupervoxels
//===========================================================================
void SLIC_LAB_CUDA::EnforceSupervoxelLabelConnectivity(
	int**&						labels,//input - previous labels, output - new labels
	const int&					width,
	const int&					height,
	const int&					depth,
	int&						numlabels,
	const int&					STEP)
{
	const int dx10[10] = { -1,  0,  1,  0, -1,  1,  1, -1,  0, 0 };
	const int dy10[10] = { 0, -1,  0,  1, -1, -1,  1,  1,  0, 0 };
	const int dz10[10] = { 0,  0,  0,  0,  0,  0,  0,  0, -1, 1 };

	int sz = width*height;
	const int SUPSZ = STEP*STEP*STEP;

	int adjlabel(0);//adjacent label
	int* xvec = new int[SUPSZ * 10];//a large enough size
	int* yvec = new int[SUPSZ * 10];//a large enough size
	int* zvec = new int[SUPSZ * 10];//a large enough size
									//------------------
									// memory allocation
									//------------------
	int** nlabels = new int*[depth];
	{for (int d = 0; d < depth; d++)
	{
		nlabels[d] = new int[sz];
		for (int i = 0; i < sz; i++) nlabels[d][i] = -1;
	}}
	//------------------
	// labeling
	//------------------
	int lab(0);
	{for (int d = 0; d < depth; d++)
	{
		int i(0);
		for (int h = 0; h < height; h++)
		{
			for (int w = 0; w < width; w++)
			{
				if (nlabels[d][i] < 0)
				{
					nlabels[d][i] = lab;
					//-------------------------------------------------------
					// Quickly find an adjacent label for use later if needed
					//-------------------------------------------------------
					{for (int n = 0; n < 10; n++)
					{
						int x = w + dx10[n];
						int y = h + dy10[n];
						int z = d + dz10[n];
						if ((x >= 0 && x < width) && (y >= 0 && y < height) && (z >= 0 && z < depth))
						{
							int nindex = y*width + x;
							if (nlabels[z][nindex] >= 0)
							{
								adjlabel = nlabels[z][nindex];
							}
						}
					}}

					xvec[0] = w; yvec[0] = h; zvec[0] = d;
					int count(1);
					for (int c = 0; c < count; c++)
					{
						for (int n = 0; n < 10; n++)
						{
							int x = xvec[c] + dx10[n];
							int y = yvec[c] + dy10[n];
							int z = zvec[c] + dz10[n];

							if ((x >= 0 && x < width) && (y >= 0 && y < height) && (z >= 0 && z < depth))
							{
								int nindex = y*width + x;

								if (0 > nlabels[z][nindex] && labels[d][i] == labels[z][nindex])
								{
									xvec[count] = x;
									yvec[count] = y;
									zvec[count] = z;
									nlabels[z][nindex] = lab;
									count++;
								}
							}

						}
					}
					//-------------------------------------------------------
					// If segment size is less then a limit, assign an
					// adjacent label found before, and decrement label count.
					//-------------------------------------------------------
					if (count <= (SUPSZ >> 2))//this threshold can be changed according to needs
					{
						for (int c = 0; c < count; c++)
						{
							int ind = yvec[c] * width + xvec[c];
							nlabels[zvec[c]][ind] = adjlabel;
						}
						lab--;
					}
					//--------------------------------------------------------
					lab++;
				}
				i++;
			}
		}
	}}
	//------------------
	// mem de-allocation
	//------------------
	{for (int d = 0; d < depth; d++)
	{
		for (int i = 0; i < sz; i++) labels[d][i] = nlabels[d][i];
	}}
	{for (int d = 0; d < depth; d++)
	{
		delete[] nlabels[d];
	}}
	delete[] nlabels;
	//------------------
	if (xvec) delete[] xvec;
	if (yvec) delete[] yvec;
	if (zvec) delete[] zvec;
	//------------------
	numlabels = lab;
	//------------------
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::DoSuperpixelSegmentation_ForGivenSuperpixelSize_sitaMLxy(
	const unsigned int*         ubuff,
	const int					width,
	const int					height,
	INT32*&						klabels,
	int&						numlabels,
	const int&					superpixelsize)
{
	//------------------------------------------------
	const int STEP = sqrt(double(superpixelsize)) + 0.5;
	//------------------------------------------------
	//--------------------------------------------------
	m_width = width;
	m_height = height;
	int sz = m_width*m_height;
	//klabels.resize( sz, -1 );
	//--------------------------------------------------

	for (int s = 0; s < sz; s++)
		klabels[s] = -1;
	//--------------------------------------------------

	if (1)//LAB, the default option
	{

	}
	//--------------------------------------------------
	bool perturbseeds(false);//perturb seeds is not absolutely necessary, one can set this flag to false
	vector<double> edgemag(0);
#if 0
	if (perturbseeds)
		DetectLabEdges(m_lvec, m_avec, m_bvec, m_width, m_height, edgemag);
#endif

#if TRUE
	pIMD->GetThetaMLXYSeeds_ForGivenStepSize_Rectangle(perturbseeds, edgemag);
#endif	
#if COLOR_ThetaML_PROC
	PerformSuperpixelSLIC_ThetaMLXY_cuda(
		1,2,3, 4,
		numlabels,
		pIMD->ImgWidth,
		pIMD->ImgHeight,
		pIMD->sita_n,
		pIMD->m_n,
		pIMD->L_n,
		pIMD->X_n,
		pIMD->Y_n,
		pIMD->kseedsTheta.data(),
		pIMD->kseedsM.data(),
		pIMD->kseedsL.data(),
		pIMD->kseedsX.data(),
		pIMD->kseedsY.data(),
		klabels,
		STEP);
	/*PerformSuperpixelSLIC_cuda(
	alpha,betta,gama,fai,
	kseedsl.size(),
	m_width,m_height,
	m_lvec,m_avec,m_bvec,
	kseedsl.data(),kseedsa.data(),kseedsb.data(),kseedsx.data(),kseedsy.data(),klabels,
	STEP,nullptr,compactness);*/
#endif

#if 1
	int* nlabels = new int[sz];
	EnforceLabelConnectivity(klabels, m_width, m_height, nlabels, numlabels, double(sz) / double(STEP*STEP));

	{
		for (int i = 0; i < sz; i++)
			klabels[i] = nlabels[i];
	}
	if (nlabels) delete[] nlabels;
#endif
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::DoSuperpixelSegmentation_ForGivenSuperpixelSize_LAB_CUDA(
	const unsigned int*         ubuff,
	const int					width,
	const int					height,
	INT32*&						klabels,
	int&						numlabels,
	const double						_M)
{
	TRACE_FUNC();
	
	const float	INVWT	=	DistanceColorSpace::CalDistLAB_Basic_Square_INVWT(width,height,numlabels,_M);
	const int	STEP	=	DistanceColorSpace::CalDistLAB_Basic_Square_STEP(width, height, numlabels);

	m_width = width;
	m_height = height;
	const int sz = m_width*m_height;
	//--------------------------------------------------
	bool perturbseeds(false);//perturb seeds is not absolutely necessary, one can set this flag to false
	vector<double> edgemag(0);
	/*if(perturbseeds) DetectLabEdges(m_lvec, m_avec, m_bvec, m_width, m_height, edgemag);*/
#if TRUE
	GetLABXYSeeds_ForGivenStepSize_Rectangle2(
		pIMD->kseedsl,
		pIMD->kseedsa,
		pIMD->kseedsb,
		pIMD->kseedsx,
		pIMD->kseedsy,
		numlabels,
		perturbseeds,
		edgemag,
		pIMD->m_lvec,
		pIMD->m_avec,
		pIMD->m_bvec);
#endif

#if COLOR_ThetaML_PROC
	PerformSuperpixelSLIC_cuda_lab(
		numlabels,
		m_width,
		m_height,
		pIMD->m_lvec,
		pIMD->m_avec,
		pIMD->m_bvec,
		pIMD->X_n,
		pIMD->Y_n,
		pIMD->kseedsl.data(),
		pIMD->kseedsa.data(),
		pIMD->kseedsb.data(),
		pIMD->kseedsx.data(),
		pIMD->kseedsy.data(),
		klabels,
		INVWT);
#endif

	EnforceLabelConnectivityEx(
		klabels,
		width,
		height,
		numlabels,
		numlabels);
	
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*根据图像的路径获取图像数据
*设置文件的保存位置
*@param filename 文件路径
*@param  filesavepath 文件保存位置
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::CuiGetImageData(string filename, string filesavepath)
{
	/**********************************************/
	if (CuiImgData) delete[]CuiImgData;
	//	if (CuiImgLables) delete[]CuiImgLables;

	if (Cui_Matrix_E) delete[]Cui_Matrix_E;
	if (Cui_Matrix_D) delete[]Cui_Matrix_D;
	if (Cui_Matrix_W) delete[]Cui_Matrix_W;
	if (Cui_Matrix_L) delete[]Cui_Matrix_L;
	if (Cui_MatrixEigenVector_L) delete[]Cui_MatrixEigenVector_L;
	if (CUi_MatrixEigenValue_L)  delete[]CUi_MatrixEigenValue_L;
	if (Cui_Matrix_Category_Simple)  delete[]Cui_Matrix_Category_Simple;
	if (Cui_Matrix_Category_Lable)  delete[]Cui_Matrix_Category_Lable;

	/**********************************************************/
	if (CuiCvImage)  cvReleaseImage(&CuiCvImage);
	if (Cui_ARGB_Image) cvReleaseImage(&Cui_ARGB_Image);
	if (Cui_lab_hsv_Image)  cvReleaseImage(&Cui_lab_hsv_Image);
	if (h_l_plane)  cvReleaseImage(&h_l_plane);
	if (s_a_plane)  cvReleaseImage(&s_a_plane);
	if (v_b_plane)  cvReleaseImage(&v_b_plane);
	//////////////////////////////////////////////////
	/* pMD->FileWritePath=filesavepath;
	pMD->FileReadFullPath=filename;*/
	this->FileReadFullPath = filename;
	this->FileWritePath = filesavepath;
#if TRUE
	//resize 图片尺寸到规范大小
	{
		CuiCvImage = cvLoadImage(filename.c_str());
		IplImage* CuiCvImage_new = cvCreateImage(cvSize(pMD->ImgWidth, pMD->ImgHeight), IPL_DEPTH_8U, CuiCvImage->nChannels);
		cvResize(CuiCvImage, CuiCvImage_new);
		cvReleaseImage(&CuiCvImage);
		CuiCvImage = cvCloneImage(CuiCvImage_new);
		cvReleaseImage(&CuiCvImage_new);
	}

#endif


	/*********************************************/
	Cui_ARGB_Image = cvCreateImage(cvGetSize(CuiCvImage), IPL_DEPTH_8U, 4);
	cvCvtColor(CuiCvImage, Cui_ARGB_Image, CV_BGR2BGRA);
	CuiWidth = Cui_ARGB_Image->width;
	CuiHeight = Cui_ARGB_Image->height;
	CuiImgData = new UINT32[CuiWidth*CuiHeight];
	memcpy(CuiImgData, Cui_ARGB_Image->imageData, sizeof(UINT)*CuiWidth*CuiHeight);
	/*********************************************/
	Cui_lab_hsv_Image = cvCreateImage(cvGetSize(CuiCvImage), IPL_DEPTH_8U, 3);
	h_l_plane = cvCreateImage(cvGetSize(CuiCvImage), IPL_DEPTH_8U, 1);
	s_a_plane = cvCreateImage(cvGetSize(CuiCvImage), IPL_DEPTH_8U, 1);
	v_b_plane = cvCreateImage(cvGetSize(CuiCvImage), IPL_DEPTH_8U, 1);
	cvCvtColor(CuiCvImage, Cui_lab_hsv_Image, CV_BGR2Lab);
	cvCvtPixToPlane(Cui_lab_hsv_Image, h_l_plane, s_a_plane, v_b_plane, NULL);
	/*On output 0≤L≤100, -127≤a≤127, -127≤b≤127
	The values are then converted to the destination data type:
	8-bit images:
	L <- L*255/100, a <- a + 128, b <- b + 128
	16-bit images are currently not supported
	32-bit images:
	L, a, b are left as is 	 */
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*根据图像的路径获取图像数据
*设置文件的保存位置
*@param filename 文件路径
*@param  filesavepath 文件保存位置
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::CuiGetImageData(void)
{
#if 1
	/**********************************************/
	if (CuiImgData) delete[]CuiImgData;
	if (CuiImgLables) delete[]CuiImgLables;
	if (Cui_Matrix_E) delete[]Cui_Matrix_E;
	if (Cui_Matrix_D) delete[]Cui_Matrix_D;
	if (Cui_Matrix_W) delete[]Cui_Matrix_W;
	if (Cui_Matrix_L) delete[]Cui_Matrix_L;
	if (Cui_MatrixEigenVector_L) delete[]Cui_MatrixEigenVector_L;
	if (CUi_MatrixEigenValue_L)  delete[]CUi_MatrixEigenValue_L;
	if (Cui_Matrix_Category_Simple)  delete[]Cui_Matrix_Category_Simple;
	if (Cui_Matrix_Category_Lable)  delete[]Cui_Matrix_Category_Lable;
	/**********************************************************/
	if (CuiCvImage)  cvReleaseImage(&CuiCvImage);
	if (Cui_ARGB_Image) cvReleaseImage(&Cui_ARGB_Image);
	if (Cui_lab_hsv_Image)  cvReleaseImage(&Cui_lab_hsv_Image);
	if (h_l_plane)  cvReleaseImage(&h_l_plane);
	if (s_a_plane)  cvReleaseImage(&s_a_plane);
	if (v_b_plane)  cvReleaseImage(&v_b_plane);
	this->FileReadFullPath = pMD->FileReadFullPath;
	this->FileWritePath = pMD->FileWritePath;
#endif
#if 0
	this->CuiGetImageData(pMD->FileReadFullPath, pMD->FileWritePath);
#endif

#if TRUE
	{
		CuiCvImage = cvLoadImage(pMD->FileReadFullPath.c_str());
		IplImage* CuiCvImage_new = cvCreateImage(cvSize(pMD->ImgWidth, pMD->ImgHeight), IPL_DEPTH_8U, CuiCvImage->nChannels);
		cvResize(CuiCvImage, CuiCvImage_new);
		cvReleaseImage(&CuiCvImage);
		CuiCvImage = cvCloneImage(CuiCvImage_new);
		cvReleaseImage(&CuiCvImage_new);
	}
#endif
	/*********************************************/
	Cui_ARGB_Image = cvCreateImage(cvGetSize(CuiCvImage), IPL_DEPTH_8U, 4);
	cvCvtColor(CuiCvImage, Cui_ARGB_Image, CV_BGR2BGRA);
	CuiWidth = Cui_ARGB_Image->width;
	CuiHeight = Cui_ARGB_Image->height;
	CuiImgData = new UINT32[CuiWidth*CuiHeight];
	memcpy(CuiImgData, Cui_ARGB_Image->imageData, sizeof(UINT)*CuiWidth*CuiHeight);
	/*********************************************/
	Cui_lab_hsv_Image = cvCreateImage(cvGetSize(CuiCvImage), IPL_DEPTH_8U, 3);
	h_l_plane = cvCreateImage(cvGetSize(CuiCvImage), IPL_DEPTH_8U, 1);
	s_a_plane = cvCreateImage(cvGetSize(CuiCvImage), IPL_DEPTH_8U, 1);
	v_b_plane = cvCreateImage(cvGetSize(CuiCvImage), IPL_DEPTH_8U, 1);
	cvCvtColor(CuiCvImage, Cui_lab_hsv_Image, CV_BGR2Lab);
	cvCvtPixToPlane(Cui_lab_hsv_Image, h_l_plane, s_a_plane, v_b_plane, NULL);
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*根据图像的路径获取图像数据
*设置文件的保存位置
*@param filename 文件路径
*@param  filesavepath 文件保存位置
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::CuiGetImageDataPIMD(void)
{
	TRACE_FUNC();
#if 1
	/**********************************************/
	if (CuiImgData) delete[]CuiImgData;
	if (CuiImgLables) delete[]CuiImgLables;
	if (Cui_ARGB_Image) cvReleaseImage(&Cui_ARGB_Image);
	/**********************************************************/
	this->FileReadFullPath = pIMD->FileReadFullPath;
	this->FileWritePath = pIMD->FileWritePath;
#endif

#if TRUE


	Cui_ARGB_Image = cvCloneImage(pIMD->srcCv_ImgBGRA);


#endif
	/*********************************************/

	CuiWidth = pIMD->ImgWidth;
	CuiHeight = pIMD->ImgHeight;

	CuiImgData = new UINT32[CuiWidth*CuiHeight];
	memcpy(CuiImgData, pIMD->src_ImgBGRA, sizeof(UINT)*CuiWidth*CuiHeight);
	/*********************************************/

}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*输入超像素分割参数完成超像素分割
*
*@param K 期望分割的超像素个数
*@param savelable 是否保存Lable图像矩阵（默认为false）
*
*@note 对图像进行基于像素色彩及空间位置的超像素图像分割，形成具有一定密度的超像素图像；\n
*
**/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_LAB_CUDA(const int _M)
{
	TRACE_FUNC();

	ASSERT(pIMD->ImgHeight == this->CuiHeight);
	ASSERT(pIMD->ImgWidth == this->CuiWidth);
	
	TimeMeasure tm("Superpixel.Total");

	{

		const int sz = CuiWidth*CuiHeight;
		//---------------------------------------------------------
		//if(K < 20 || K > sz/4) K = sz/200;//i.e the default size of the superpixel is 200 pixels

		/*	if(compactness < 1.0 || compactness > 80.0) compactness = 20.0;*/
		//---------------------------------------------------------		

		DoSuperpixelSegmentation_ForGivenSuperpixelSize_LAB_CUDA(
			CuiImgData,
			CuiWidth,
			CuiHeight,
			pIMD->src_ImgLabels,
			pIMD->slic_current_num,
			_M);

	}

	std::string str_add;
#if _DEBUG
	{
		std::stringstream ss;
		ss << "_Num_";
		ss << pIMD->slic_expect_num;
		ss << "_Factor_";
		ss << 1;
		ss << "_Fai_";
	
		ss >> str_add;
	}
#endif
#if	SaveContours2Disk
	pIMD->SaveImgWithContours(str_add);
	pIMD->DrawContours();
#endif
	/////////////////////////////////////////////////////
}
/*------------------------------------------------------------------------------------------------------------------*/
/**
*丢弃的聚类方法
*算法中未使用
*/
/*------------------------------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::Cui_Min_Cluster(void)
{
	const   UINT EigenvectorNum = 1;

	if (Cui_Matrix_Category_Simple) {
		delete[]Cui_Matrix_Category_Simple;
		Cui_Matrix_Category_Simple = NULL;

	}
	Cui_Matrix_Category_Simple = new float[pMD->slic_current_num];
	memset(Cui_Matrix_Category_Simple, 0, sizeof(float)*pMD->slic_current_num);

	for (int i = 0; i<pMD->slic_current_num; i++) {  //row

		Cui_Matrix_Category_Simple[i] = Cui_MatrixEigenVector_L[pMD->slic_current_num*i + pMD->slic_current_num - 1];

	}

#if _DEBUG
	{
		char data_t[1024];
		memset(data_t, 0, sizeof(data_t));
		ofstream outfile;
		outfile.open("Mim_Sample.dat", ios::out);
		for (int i = 0; i<pMD->slic_current_num; i++) {
			for (int j = 0; j<EigenvectorNum; j++) {
				//N*K						   //N*N

				double value_t_t = Cui_Matrix_Category_Simple[EigenvectorNum*i + j];
				sprintf(data_t, "%5.2f ", value_t_t);
				outfile << data_t;

			}
			outfile << endl;
		}

		outfile.close();
	}
#endif
#if 0
	for (int i = 0; i <pMD->slic_current_num; i++) {
		CvMat normalize_t;
		cvInitMatHeader(&normalize_t, 1, EigenvectorNum, CV_32FC1, &Cui_Matrix_Category_Simple[i*EigenvectorNum]);
		cvNormalize(&normalize_t, &normalize_t, 1, 0, CV_L2);	//a,b无用	
	}
#endif

#if 0

	{
		char data_t[1024];
		ofstream outfile;
		outfile.open("Min_sample_Normalize.data", ios::out);
		for (int i = 0; i <pMD->slic_current_num; i++) {
			for (int j = 0; j <EigenvectorNum; j++) {
				double value_t = Cui_Matrix_Category_Simple[i*EigenvectorNum + j];
				sprintf(data_t, " %5.2f ", value_t);
				outfile << data_t;
			}
			outfile << endl;
		}
		outfile.close();
	}
#endif	
	/************清空Lable****************************************/
	if (Cui_Matrix_Category_Lable) {
		delete[]Cui_Matrix_Category_Lable;
		Cui_Matrix_Category_Lable = NULL;

	}
	Cui_Matrix_Category_Lable = new INT32[pMD->slic_current_num];
	memset(Cui_Matrix_Category_Lable, 0xffffffff, sizeof(int)*pMD->slic_current_num - 1);
	/****************************************************************/
	double  B_like_t = 0;
	for (int i = 0; i<pMD->slic_current_num; i++) {

		for (int j = i + 1; j<pMD->slic_current_num; j++) {
			/****判断超像素i,j相似度*category=i行j行相似度**/
			if (Cui_Matrix_W[i*pMD->slic_current_num + j]>0) {
				double spi_value = fabsl(Cui_Matrix_Category_Simple[i]);
				double spj_value = fabsl(Cui_Matrix_Category_Simple[j]);
				if (max(spi_value, spj_value)) {
					B_like_t = fabsl(spi_value - spj_value) / max(spi_value, spj_value);
				}
				else {
					B_like_t = 0;
				}

				if (Cui_Matrix_Category_Lable[i] == 0xffffffff) {
					Cui_Matrix_Category_Lable[i] = i;
				}

				if (B_like_t<0.5) {

					Cui_Matrix_Category_Lable[j] = Cui_Matrix_Category_Lable[i];
				}

			}

			/*********************************************/
		}
	}

#if _DEBUG
	if (cui_GeneralImgProcess::SAVE_DEBUG_2DISK) {
		char data_t[1024];
		ofstream outfile;
		outfile.open("Min_mean_Lable.data", ios::out);
		for (int i = 0; i <pMD->slic_current_num; i++) {

			INT32 value_t = Cui_Matrix_Category_Lable[i];
			sprintf(data_t, "%3d ", value_t);
			outfile << data_t;
			outfile << endl;
		}

	}
#endif	
}
/*------------------------------------------------------------------------------------------------------------------*/
/**
*查找超像素块之间最大的相似度数值
*迭代过程中使用
*/
/*------------------------------------------------------------------------------------------------------------------*/
double SLIC_LAB_CUDA::Cui_Find_MaxSimilar(void)
{
	double Max_Similar = 0;
#if OUT_DOOR_SUPERPIXEL_COLOR_BAT
	for (register int spi = 0; spi<pMD->slic_current_num; spi++) {
		for (register int spj = spi + 1; spj<pMD->slic_current_num; spj++) {
			if (Cui_Matrix_W[spi*pMD->slic_current_num + spj] >= Max_Similar) {
				Max_Similar = Cui_Matrix_W[spi*pMD->slic_current_num + spj];

			}
		}
	}
#endif

#if OUT_DOOR_HUMAN
	for (register int spi = 0; spi<pIMD->slic_current_num; spi++) {
		for (register int spj = spi + 1; spj<pIMD->slic_current_num; spj++) {
			if (Cui_Matrix_W[spi*pIMD->slic_current_num + spj] >= Max_Similar) {
				Max_Similar = Cui_Matrix_W[spi*pIMD->slic_current_num + spj];

			}
		}

	}
#endif


	return Max_Similar;
}

void SLIC_LAB_CUDA::Cui_SurroundClassification(void) {
#if 0
	INT32*	category;
	category = new INT32[pMD->slic_current_num];
	cui_GeneralImgProcess::SurroundClassification(CuiImgLables, pMD->slic_current_num, CuiWidth, CuiHeight, category);
	cui_GeneralImgProcess::Cui_Combination_ImgLabs2(CuiImgData, CuiImgLables, category, NULL, pMD->slic_current_num, CuiWidth, CuiHeight, 0, FileReadFullPath, FileWritePath);
	delete[] category;
#endif
#if OUT_DOOR_SUPERPIXEL_COLOR_BAT
	cui_GeneralImgProcess::InSideClusteringByopencv(
		CuiImgData,
		CuiImgLables,
		pMD->slic_current_num,
		CuiWidth,
		CuiHeight,
		pMD->FileReadFullPath,
		pMD->FileWritePath);
#endif
#if OUT_DOOR_HUMAN
	pIMD->SurroundClassification();
#endif
}
/*------------------------------------------------------------------------------------------------------------------*/
/**
*导出超像素的标定图
*/
/*------------------------------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::ExportMemData(void)
{
	this->pMD->ImportMemData(pMD->slic_current_num, CuiImgLables);

}
/*---------------------------------------------------------------------------------*/
/**
*获取a、b最大公约数
*@param  a
*@param  b
*@return  最大公约数
*
*/
/*---------------------------------------------------------------------------------*/
unsigned int SLIC_LAB_CUDA::GetGCD(unsigned int a, unsigned int b)
{
	unsigned int t = 0;
	if (a>b) {
		t = b;
		b = a;
		a = t;
	}

	while (b%a)
	{
		t = b;
		b = a;
		a = t%a;
	}
	return a;
}

/*---------------------------------------------------------------------------------*/
/**
*显示距离图像
*聚类矩阵的长宽与图像等同
*@param clustersize 记录最小距离的矩阵
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::showDistance(vector<double> clustersize)
{
	float max = 0;
	for (unsigned int i = 0; i<clustersize.size(); i++) {
		if (clustersize[i]>max) {
			max = clustersize[i];
		}
	}
	UINT32* depth = new UINT32[m_width*m_height];
	for (unsigned int i = 0; i<clustersize.size(); i++) {
		/*depth[i]=((UINT32)(255-1.0*clustersize[i]/max*255));*/
		depth[i] = clustersize[i] * 4;
	}
	IplImage img_header;
	IplImage *img = cvInitImageHeader(&img_header, cvSize(m_width, m_height), IPL_DEPTH_8U, 4);
	img_header.imageData = (char*)depth;
	///////////////////////////////////////////////////////////////
	cvSaveImage("depth.jpg", img);
	///////////////////////////////////////////////////////////////
	delete[]depth;
}
/*---------------------------------------------------------------------------------*/
/**
*提取初始条件的种子，种子为自然数的平方数，以矩阵方式均匀分布种子
*
*@param 	[iout]	kseedsl 种子L分量
*@param 	[out]	kseedsa 种子A分量
*@param 	[out]	kseedsb 种子B分量
*@param 	[out]	kseedsx 种子X轴分量
*@param 	[out]	kseedsy 种子Y轴分量
*@param 	[in,out]	K       期望的种子数
*@param 	[in,out]	perturbseeds 未使用
*@param 	[in,out]    edgemag 未使用
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::GetLABXYSeeds_ForGivenStepSize_Rectangle(
	vector<double>&				kseedsl,
	vector<double>&				kseedsa,
	vector<double>&				kseedsb,
	vector<double>&				kseedsx,
	vector<double>&				kseedsy,
	const int&					K,
	const bool&					perturbseeds,
	const vector<double>&		edgemag)
{
	double*  m_lvec = NULL;
	double*  m_avec = NULL;
	double*  m_bvec = NULL;
	float xStep;
	float yStep;
	const bool hexgrid = false;
	int numseeds(0);
	int n(0);
#if FALSE
	int max_wh = (m_width>m_height) ? m_width : m_height;
	int min_wh = (m_width<m_height) ? m_width : m_height;
	float scale_wh = 1.0*min_wh / max_wh;
	unsigned int wh_gcd = GetGCD(m_width, m_height);
	float scale_3_2 = 1.0 * 2 / 3;
	if (scale_wh == scale_3_2) {
		int K_new = (K / 12 + 0.5) * 12;
		if (m_width>m_height) {
			xStep = sqrtl(m_height*m_width * 4 / (12 * 3));
			yStep = 3.0 / 4.0*xStep;
		}
		else {
			yStep = sqrtl(m_height*m_width * 4 / (12 * 3));
			xStep = 3.0 / 4.0*yStep;
		}
	}
	else {
		ASSERT(FALSE);
	}
#else
	unsigned int wh_gcd = GetGCD(m_width, m_height);
	int width_min = m_width / wh_gcd;
	int height_min = m_height / wh_gcd;
	int block_min = width_min*height_min;
	double  wh_scale = 1.0*m_width / m_height;
	{
		/*int K_new=((int)(K/block_min+0.5))*block_min;*/
		yStep = sqrtl(m_height*m_height / K);
		int K_new = (m_height*m_height) / (yStep*yStep);
		{
			/*yStep=sqrtl(1.0*m_height*m_width/(K_new*wh_scale));*/
			xStep = wh_scale*yStep;
		}
	}
#endif	

	float xstrips_test = (double(m_width) / double(xStep));
	float ystrips_test = (double(m_height) / double(yStep));
	int xstrips = (double(m_width) / double(xStep));
	int ystrips = (double(m_height) / double(yStep));
	ASSERT(xstrips == ystrips);
#if TRUE
	int xerr = m_width - xStep*xstrips;
	//if(xerr < 0){xstrips--;xerr = m_width - STEP*xstrips;}
	int yerr = m_height - yStep*ystrips;
	//if(yerr < 0){ystrips--;yerr = m_height- STEP*ystrips;}
#endif
	double xerrperstrip = double(xerr) / double(xstrips);
	double yerrperstrip = double(yerr) / double(ystrips);

	int xoff = xStep / 2;
	int yoff = yStep / 2;
	//-------------------------
	numseeds = xstrips*ystrips;
	//------------------------- 
	kseedsl.resize(numseeds);
	kseedsa.resize(numseeds);
	kseedsb.resize(numseeds);
	kseedsx.resize(numseeds);
	kseedsy.resize(numseeds);

	for (int y = 0; y < ystrips; y++)
	{
		int ye = y*yerrperstrip;
		for (int x = 0; x < xstrips; x++)
		{
			int xe = x*xerrperstrip;
			int seedx = (x*xStep + xoff + xe);
			if (hexgrid) { seedx = x*xStep + (xoff << (y & 0x1)) + xe; seedx = min(m_width - 1, seedx); }//for hex grid sampling
			int seedy = (y*yStep + yoff + ye);
			int i = seedy*m_width + seedx;

			kseedsl[n] = m_lvec[i];
			kseedsa[n] = m_avec[i];
			kseedsb[n] = m_bvec[i];
			ASSERT(0);
			kseedsx[n] = seedx;
			kseedsy[n] = seedy;
			n++;
		}
	}


	if (perturbseeds)
	{
		PerturbSeeds(kseedsl, kseedsa, kseedsb, kseedsx, kseedsy, edgemag);
	}
}
/*---------------------------------------------------------------------------------*/
/**
*提取初始条件的种子，种子为自然数的平方数，以矩阵方式均匀分布种子
*
*@param 	[iout]	kseedsl 种子L分量
*@param 	[out]	kseedsa 种子A分量
*@param 	[out]	kseedsb 种子B分量
*@param 	[out]	kseedsx 种子X轴分量
*@param 	[out]	kseedsy 种子Y轴分量
*@param 	[in,out]	K       期望的种子数
*@param 	[in,out]	perturbseeds 未使用
*@param 	[in,out]    edgemag 未使用
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::GetLABXYSeeds_ForGivenStepSize_Rectangle2(
	vector<double>&				kseedsl,
	vector<double>&				kseedsa,
	vector<double>&				kseedsb,
	vector<double>&				kseedsx,
	vector<double>&				kseedsy,
	int&					    NumLabels,
	const bool&					perturbseeds,
	const vector<double>&		edgemag,
	double*  m_lvec,
	double*  m_avec,
	double*  m_bvec)
{
	TRACE_FUNC();

	TimeMeasure tm("Superpixel.Seeds.First");
	
	float xStep;
	float yStep;
	const bool hexgrid = false;
	int numseeds(0);
	int n(0);
	int K = NumLabels;

	unsigned int wh_gcd = GetGCD(m_width, m_height);
	int width_min = m_width / wh_gcd;
	int height_min = m_height / wh_gcd;
	int block_min = width_min*height_min;
	double  wh_scale = 1.0*m_width / m_height;
	{
		/*int K_new=((int)(K/block_min+0.5))*block_min;*/
		yStep = sqrtl(m_height*m_height / K);
		int K_new = (m_height*m_height) / (yStep*yStep);
		{
			/*yStep=sqrtl(1.0*m_height*m_width/(K_new*wh_scale));*/
			xStep = wh_scale*yStep;
		}
	}


	float xstrips_test = (double(m_width) / double(xStep));
	float ystrips_test = (double(m_height) / double(yStep));
	int xstrips = (double(m_width) / double(xStep));
	int ystrips = (double(m_height) / double(yStep));
	ASSERT(xstrips == ystrips);

	int xerr = m_width - xStep*xstrips;
	
	int yerr = m_height - yStep*ystrips;
	

	double xerrperstrip = double(xerr) / double(xstrips);
	double yerrperstrip = double(yerr) / double(ystrips);

	int xoff = xStep / 2;
	int yoff = yStep / 2;
	//-------------------------
	NumLabels = numseeds = xstrips*ystrips;
	//------------------------- 
	kseedsl.resize(numseeds);
	kseedsa.resize(numseeds);
	kseedsb.resize(numseeds);
	kseedsx.resize(numseeds);
	kseedsy.resize(numseeds);

	for (register int y = 0; y < ystrips; y++)
	{
		int ye = y*yerrperstrip;
		for (register int x = 0; x < xstrips; x++)
		{
			int xe = x*xerrperstrip;
			int seedx = (x*xStep + xoff + xe);
			if (hexgrid) { seedx = x*xStep + (xoff << (y & 0x1)) + xe; seedx = min(m_width - 1, seedx); }//for hex grid sampling
			int seedy = (y*yStep + yoff + ye);
			int i = seedy*m_width + seedx;

			kseedsl[n] = m_lvec[i];
			kseedsa[n] = m_avec[i];
			kseedsb[n] = m_bvec[i];
			//ASSERT(0);
			kseedsx[n] = seedx;
			kseedsy[n] = seedy;
			n++;
		}
	}


	if (perturbseeds)
	{
		PerturbSeeds(kseedsl, kseedsa, kseedsb, kseedsx, kseedsy, edgemag);
	}
}

/*---------------------------------------------------------------------------------*/
/**
*利用提取的种子的LABXY综合特性填充图像
*
*@param [in] kseedsl  L分量
*@param [in] kseedsa  A分量
*@param [in] kseedsb  B分量
*@param [in] kseedsx  X坐标分量
*@param [in] kseedsy  Y坐标分量
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::FillImgWithSeeds(
	vector<double> kseedsl,
	vector<double> kseedsa,
	vector<double> kseedsb,
	vector<double> kseedsx,
	vector<double> kseedsy)
{
	int HistDimSPLAB = 180;
#if 1
	int vectorSize = kseedsa.size();
	{
		int negcount;
		//opencv L, a, b最后的取值变为了 0 - 255
		//origin L 0-100  a -127--128   b-127----128
		IplImage* img = cvCreateImage(cvSize(pMD->ImgWidth, pMD->ImgHeight), IPL_DEPTH_8U, 3);
		for (int xi = 0; xi<pMD->ImgWidth; xi++) {
			for (int yj = 0; yj<pMD->ImgHeight; yj++) {
				int seedIdx = CuiImgLables[yj*pMD->ImgWidth + xi];
				assert(seedIdx >= 0 && seedIdx<vectorSize);
				double L = kseedsl[seedIdx] * 2.55;
				double A = kseedsa[seedIdx] + 127;
				double B = kseedsb[seedIdx] + 127;
				CvScalar v_lab = cvScalar(L, A, B);
				cvSet2D(img, yj, xi, v_lab);
			}
		}
		cvCvtColor(img, img, CV_Lab2BGR);
		FileNameSplit fns;
		string filesaveimg_t = "SpSeeds.jpg";
#ifdef Use_CString&&_MSC_VER
#if Use_CString&&_MSC_VER
		fns.Parse(CString(FileReadFullPath.c_str()));
		filesaveimg_t = pMD->FileWritePath + FileNameSplit::ConvertCS2string(fns.filename) + "SpSeeds.jpg";
#endif
#endif
#ifdef linux
		filesaveimg_t = "linuxSpSeeds.jpg";
#endif
		cvSaveImage(filesaveimg_t.c_str(), img);
		cvReleaseImage(&img);
	}

#else  
	int negcount = 0;
	int vectorSize = kseedsa.size();
	for (int xi = 0; xi<pMD->ImgWidth; xi++) {
		for (int yj = 0; yj<pMD->ImgHeight; yj++) {
			int seedIdx = CuiImgLables[yj*pMD->ImgWidth + xi];
			assert(seedIdx >= -1 && seedIdx<vectorSize);
			if (seedIdx == -1) {
				negcount++;
			}
		}
	}
#endif

#if 1
	/////////////////////////////////////////////////////////////////
	double* hist_L = new double[HistDimSPLAB];//0-100
	double* hist_AB = new double[HistDimSPLAB];//-127--128
	double* hist_Count = new double[HistDimSPLAB];//count
	double* hist_AB_length = new double[HistDimSPLAB];//128*1.414
	memset(hist_L, 0, sizeof(double)*HistDimSPLAB);
	memset(hist_AB, 0, sizeof(double)*HistDimSPLAB);
	memset(hist_Count, 0, sizeof(double)*HistDimSPLAB);
	memset(hist_AB_length, 0, sizeof(double)*HistDimSPLAB);
	for (int i = 0; i<vectorSize; i++) {
		double L = kseedsl[i];//0-100
		double A = kseedsa[i];//-127--128
		double B = kseedsb[i];//-127--128
		double AB_length = sqrt(A*A + B*B);
		double angle = atan2(B, A);//-PI---PI
		double angule_360 = (angle + M_PI) * 180 / M_PI;
		assert(angule_360 >= -0.1&&angule_360 <= 360.1);
		int angle_index = cvFloor(angule_360 / 360 * HistDimSPLAB);
		//////////////////////////////////////////
		hist_Count[angle_index] += 1;
		hist_L[angle_index] += L;
		hist_AB[angle_index] += 1;
		hist_AB_length[angle_index] += AB_length;
		//////////////////////////////////////////
	}
	//////////////////////////////////////////
	for (int j = 0; j<HistDimSPLAB; j++) {
		hist_L[j] = hist_L[j] / hist_Count[j];
		hist_AB_length[j] = hist_AB_length[j] / hist_Count[j];
	}
	double AB_count_max = cui_GeneralImgProcess::GetMaxValue(&hist_AB[0], HistDimSPLAB);
	//////////////////////////////////////////////////////////////////
	int bin_w = 1800 / HistDimSPLAB;
	int h_bins = HistDimSPLAB;
	int height = 240;
	int scale = 2;
	int width = h_bins*bin_w;
	IplImage* hist_img = cvCreateImage(cvSize(width, height), 8, 3);
	cvRectangle(hist_img, cvPoint(0, 0), cvPoint(width, height), cvScalar(255, 255 / 2, 255 / 2), -1, 8, 0);
	/////////////////////////////////////////////////////////////////////////////////////
	for (int h = 0; h <h_bins; h++) {
		/** 获得直方图中的统计次数，计算显示在图像中的高度 */
		float bin_val = hist_AB[h];
		int intensity = cvRound(bin_val*height / AB_count_max);
		intensity = intensity<0 ? 0 : intensity;
		/** 获得当前直方图代表的颜色，转换成RGB用于绘制 */
		float sita = M_PI*(2 * (h + 0.5) / HistDimSPLAB - 1);
		CvScalar color = cvScalar(hist_L[h] * 2.55, hist_AB_length[h] * sin(sita) + 127, hist_AB_length[h] * cos(sita) + 127);
		cvRectangle(hist_img, cvPoint(h*bin_w, height), cvPoint((h + 1)*bin_w, height - intensity), cvScalar(0, 255 / 2, 255 / 2), 2);
		cvRectangle(hist_img, cvPoint(h*bin_w, height), cvPoint((h + 1)*bin_w, height - intensity + 1), color, -1, 8, 0);
	}
	for (int h = 0; h <h_bins; h++) {
		int cut_num = 8;
		int cut_step = h_bins / cut_num;
		if (h%cut_step == 0) {
			cvLine(hist_img, cvPoint((h + 0.5)*bin_w, height), cvPoint((h + 0.5)*bin_w, 0), cvScalar(180, 128, 128 + 70), 1);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////
	cvCvtColor(hist_img, hist_img, CV_Lab2BGR);
	FileNameSplit fns;
#if Use_CString&&_MSC_VER
	fns.Parse(CString(FileReadFullPath.c_str()));
	string filesaveimg = pMD->FileWritePath + FileNameSplit::ConvertCS2string(fns.filename) + "Histogram.jpg";
	cvSaveImage(filesaveimg.c_str(), hist_img);//在"H-S Histogtam"窗口中显示图像
	cvReleaseImage(&hist_img);
#endif
	delete[] hist_L;//0-100
	delete[] hist_AB;//-127--128
	delete[] hist_Count;//count
	delete[] hist_AB_length;//128*1.414
#endif
}
/*---------------------------------------------------------------------------------*/
/**
*根据直方图的统计结果每个单条直方图进行聚类
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::Clustering_ByHistogramOne(void)
{
#if 0
	int old_slic_num;
	int new_slic_num;
	double*  m_lvec = NULL;
	double*  m_avec = NULL;
	double*  m_bvec = NULL;
	INT32 *Matrix_Category_Lable = new INT32[pMD->slic_current_num];
	double* Matrix_W = new double[pMD->slic_current_num*pMD->slic_current_num];
	do {
		old_slic_num = pMD->slic_current_num;
		int HistDimSPLAB = 180;
		kseedsl.resize(pMD->slic_current_num);
		kseedsa.resize(pMD->slic_current_num);
		kseedsb.resize(pMD->slic_current_num);
		kseedsx.resize(pMD->slic_current_num);
		kseedsy.resize(pMD->slic_current_num);
		//重新获取种子
		GetSeedsLabxy_cuda(pMD->slic_current_num,
			m_width, m_height,
			m_lvec, m_avec, m_bvec,
			kseedsl.data(), kseedsa.data(), kseedsb.data(), kseedsx.data(), kseedsy.data(), CuiImgLables);
		//获取连接矩阵
		cui_GeneralImgProcess::CuiSetNighbour_E_matrix(
			CuiImgLables,
			pMD->slic_current_num,
			pMD->Matrix_E.get(),
			pMD->ImgWidth, pMD->ImgHeight,
			"Compute_matrix.matrix", "");
		//	绘制直方图
		//获得直方图数据
		KseedsHistData seeddata(HistDimSPLAB);

		cui_GeneralImgProcess::Get_Kseeds_Histogram(
			kseedsl, kseedsa, kseedsb, kseedsx, kseedsy,
			seeddata,
			CuiImgLables,
			pMD->ImgWidth, pMD->ImgHeight,
			pMD->FileReadFullPath,
			pMD->FileWritePath);

		HistogramOne2Matrix_Category_Lable(Matrix_Category_Lable, seeddata);

		cui_GeneralImgProcess::Draw_Kseeds_Histogram(
			kseedsl, kseedsa, kseedsb, kseedsx, kseedsy,
			seeddata,
			CuiImgLables, pMD->ImgWidth, pMD->ImgHeight,
			pMD->FileReadFullPath, pMD->FileWritePath);

		UINT32* Matrix_E = pMD->Matrix_E.get();
		for (int i = 0; i<pMD->slic_current_num; i++) {
			for (int j = 0; j<pMD->slic_current_num; j++) {
				Matrix_W[i + j*pMD->slic_current_num] = Matrix_E[i + j*pMD->slic_current_num];
			}
		}

		cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
			CuiImgData, CuiImgLables,
			Matrix_Category_Lable,
			Matrix_W,
			pMD->slic_current_num,
			pMD->ImgWidth, pMD->ImgHeight,
			0.9, pMD);
		new_slic_num = pMD->slic_current_num;
	} while (old_slic_num != new_slic_num);

	//////////////////////////////////////
	delete[]Matrix_Category_Lable;
	delete[]Matrix_W;
#endif

}
/*---------------------------------------------------------------------------------*/
/**
*将不同颜色角度统计到N维直方图中\n
*每一维直方图单独聚类，分别完成彩色聚类和灰度聚类
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::Clustering_ByHistogramOneColorGray(void)
{
#if 0
	double*  m_lvec = NULL;
	double*  m_avec = NULL;
	double*  m_bvec = NULL;
	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
							/*---------------------------------------------------*/
	int old_slic_num;
	int new_slic_num;
	int HistDimSPLABColor = 180;
	int HistDimSPLgray = 16;
	INT32 *Matrix_Category_Lable = new INT32[pMD->slic_current_num];
	double* Matrix_W = new double[pMD->slic_current_num*pMD->slic_current_num];
	do {
		old_slic_num = pMD->slic_current_num;
		kseedsl.clear();
		kseedsa.clear();
		kseedsb.clear();
		kseedsx.clear();
		kseedsy.clear();
		kseedsl.resize(pMD->slic_current_num);
		kseedsa.resize(pMD->slic_current_num);
		kseedsb.resize(pMD->slic_current_num);
		kseedsx.resize(pMD->slic_current_num);
		kseedsy.resize(pMD->slic_current_num);
		//重新获取种子
		GetSeedsLabxy_cuda(pMD->slic_current_num,
			m_width, m_height,
			m_lvec, m_avec, m_bvec,
			kseedsl.data(), kseedsa.data(), kseedsb.data(), kseedsx.data(), kseedsy.data(), CuiImgLables);
		//获取连接矩阵
		cui_GeneralImgProcess::CuiSetNighbour_E_matrix(
			CuiImgLables,
			pMD->slic_current_num,
			pMD->Matrix_E.get(),
			pMD->ImgWidth, pMD->ImgHeight,
			"Compute_matrix.matrix", "");
		//	绘制直方图
		//获得直方图数据
		KseedsHistColorGray seeddata;
		seeddata.ColorHist.resize(HistDimSPLABColor);
		seeddata.GrayHist.resize(HistDimSPLgray);
		cui_GeneralImgProcess::Get_Kseeds_Histogram(
			kseedsl, kseedsa, kseedsb, kseedsx, kseedsy,
			seeddata,
			CuiImgLables,
			pMD->ImgWidth, pMD->ImgHeight,
			pMD->FileReadFullPath,
			pMD->FileWritePath);
		/**/
		cui_GeneralImgProcess::Draw_Kseeds_Histogram(
			kseedsl, kseedsa, kseedsb, kseedsx, kseedsy,
			seeddata.ColorHist,
			CuiImgLables, pMD->ImgWidth, pMD->ImgHeight,
			pMD->FileReadFullPath, pMD->FileWritePath);
		cui_GeneralImgProcess::Draw_Kseeds_Histogram(
			kseedsl, kseedsa, kseedsb, kseedsx, kseedsy,
			seeddata.GrayHist,
			CuiImgLables, pMD->ImgWidth, pMD->ImgHeight,
			pMD->FileReadFullPath, pMD->FileWritePath);
#if 1
		HistogramOne2Matrix_Category_Lable(Matrix_Category_Lable, seeddata);
#endif
		UINT32* Matrix_E = pMD->Matrix_E.get();
		for (int i = 0; i<pMD->slic_current_num; i++) {
			for (int j = 0; j<pMD->slic_current_num; j++) {
				Matrix_W[i + j*pMD->slic_current_num] = Matrix_E[i + j*pMD->slic_current_num];
			}
		}

#if		UseTextureInColorWithCombine
		cui_GeneralImgProcess::CalculateAllSpPropertyRange(
			CuiImgLables,
			pMD->ImgWidth, pMD->ImgHeight,
			pMD->p_SpProperty,
			pMD->slic_current_num);
		cui_GeneralImgProcess::CalculateAllSpBlockEnergy(
			pMD->slic_current_num,
			pMD->p_SpProperty,
			pMD->Src_ImgData,
			CuiImgLables,
			pMD->ImgWidth, pMD->ImgHeight);
		cui_GeneralImgProcess::Cui_Combination_ImgLabsWithTextureInColor(
			CuiImgData, CuiImgLables,
			Matrix_Category_Lable,
			Matrix_W,
			pMD->slic_current_num,
			pMD->ImgWidth, pMD->ImgHeight,
			0.9, pMD);
#else
		cui_GeneralImgProcess::Cui_Combination_ImgLabs2(
			CuiImgData, CuiImgLables,
			Matrix_Category_Lable,
			Matrix_W,
			pMD->slic_current_num,
			pMD->ImgWidth, pMD->ImgHeight,
			0.9, pMD);
#endif

		new_slic_num = pMD->slic_current_num;
	} while (old_slic_num != new_slic_num);

	//////////////////////////////////////
	delete[]Matrix_Category_Lable;
	delete[]Matrix_W;
	/*---------------------------------------------------*/
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2 - QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
	TRACE("\n 单条聚类: %f（秒）", dfTim);
#endif

}
/*---------------------------------------------------------------------------------*/
/**
*
*@param [out]  Matrix_Category_Lable  超像素分类信息
*@param [in] seeddata    彩色直方图与灰度直方图数据
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::HistogramOne2Matrix_Category_Lable(INT32 *Matrix_Category_Lable, KseedsHistData& seeddata)
{
#if 1
	int HistDimSPLAB = seeddata.histDimSPLAB;
	INT32 Matrix_CL_TEST[1024];
	for (int i = 0; i<pMD->slic_current_num; i++) {
		Matrix_Category_Lable[i] = i;
		Matrix_CL_TEST[i] = i;
	}
	for (int j = 0; j<HistDimSPLAB; j++) {
		int SamSP = seeddata.hist_AB_detail[j].size();
		for (int i = 0; i<SamSP; i++) {
			if (SamSP >= 2 && i<SamSP - 1) {
				int orgIdx = seeddata.hist_AB_detail[j].at(0);
				int changeIdx = seeddata.hist_AB_detail[j].at(i + 1);
				Matrix_Category_Lable[changeIdx] = orgIdx;
				Matrix_CL_TEST[changeIdx] = orgIdx;
			}
		}
	}
#endif	
}
/*---------------------------------------------------------------------------------*/
/**
*
*
*@param [out]  Matrix_Category_Lable  超像素分类信息
*@param [in] seeddata    彩色直方图与灰度直方图数据
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::HistogramOne2Matrix_Category_Lable(INT32 *Matrix_Category_Lable, KseedsHistColorGray& seeddata)
{

	int HistDimSPLABColor = seeddata.ColorHist.histDimSPLAB;
	int HistDimSPLGray = seeddata.GrayHist.histDimSPLAB;
	INT32 Matrix_CL_TEST[1024];
	for (int i = 0; i<pMD->slic_current_num; i++) {
		Matrix_Category_Lable[i] = i;
		Matrix_CL_TEST[i] = i;
	}
#if 1	
	for (int j = 0; j<HistDimSPLABColor; j++) {
		int SamSP = seeddata.ColorHist.hist_AB_detail[j].size();
		for (int i = 0; i<SamSP; i++) {
			if (SamSP >= 2 && i<SamSP - 1) {
				int orgIdx = seeddata.ColorHist.hist_AB_detail[j].at(0);
				int changeIdx = seeddata.ColorHist.hist_AB_detail[j].at(i + 1);
				Matrix_Category_Lable[changeIdx] = orgIdx;
				Matrix_CL_TEST[changeIdx] = orgIdx;
			}
		}
	}
#endif	
#if 1
	for (int j = 0; j<HistDimSPLGray; j++) {
		int SamSP = seeddata.GrayHist.hist_AB_detail[j].size();
		for (int i = 0; i<SamSP; i++) {
			if (SamSP >= 2 && i<SamSP - 1) {
				int orgIdx = seeddata.GrayHist.hist_AB_detail[j].at(0);
				int changeIdx = seeddata.GrayHist.hist_AB_detail[j].at(i + 1);
				Matrix_Category_Lable[changeIdx] = orgIdx;
				Matrix_CL_TEST[changeIdx] = orgIdx;
			}
		}
	}
#endif	
}
/*---------------------------------------------------------------------------------*/
/**
*
*直方图范围聚类
*@deprecated  未使用
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::HistogramRange2Matrix_Category_Lable(
	INT32 *Matrix_Category_Lable,
	INT32 *Matrix_Category_SP_USED,
	KseedsHistData& seeddata,
	int ColorangleSpan)
{
	int HistDimSPLAB = seeddata.histDimSPLAB;
	int PieceNum = 9;
	int cut_step = HistDimSPLAB / PieceNum;
	int range = ColorangleSpan / 2;
	int stepLR = 1.0*range / 360 * HistDimSPLAB;
	vector<HistRangeData> histrangedata_t(PieceNum);
	int sort_t[2];
	/*-----获取9个分组的最大值---------------------------------------------------------------------*/
	for (int h = 0; h <HistDimSPLAB; h++) {
		if (h%cut_step == 0) {
			int rangeIdx = h / cut_step;
			histrangedata_t[rangeIdx].start = &(seeddata.hist_AB_Count[h]);
			histrangedata_t[rangeIdx].len = cut_step;
			histrangedata_t[rangeIdx].angle_up = h * 2;
			histrangedata_t[rangeIdx].angle_down = (h + cut_step) * 2 - 1;
			histrangedata_t[rangeIdx].StepLR = stepLR;
			cui_GeneralImgProcess::GetMaxValueIndexdouble(histrangedata_t[rangeIdx].start, cut_step, sort_t, 2);
			if (histrangedata_t[rangeIdx].start[sort_t[0]] == 0) {
				histrangedata_t[rangeIdx].maxHistIdx = -1;
				histrangedata_t[rangeIdx].maxHistLeft = 0;
				histrangedata_t[rangeIdx].maxHistRight = 0;
				histrangedata_t[rangeIdx].maxHistCenter = 0;
			}
			else {
				histrangedata_t[rangeIdx].maxHistIdx = sort_t[0] + h;
				histrangedata_t[rangeIdx].maxHistLeft = &(histrangedata_t[rangeIdx].start[sort_t[0] - stepLR]);
				histrangedata_t[rangeIdx].maxHistRight = &(histrangedata_t[rangeIdx].start[sort_t[0] + stepLR]);
				histrangedata_t[rangeIdx].maxHistCenter = &(histrangedata_t[rangeIdx].start[sort_t[0]]);
			}
		}
	}
	/*-----对获取9个分组的最大值进行校正-------------------------------------------*/
	for (int i = 0; i<PieceNum; i++) {
		unsigned int Dim = (unsigned int)histrangedata_t[i].maxHistRight - (unsigned int)histrangedata_t[i].maxHistLeft;
		Dim = Dim / sizeof(double);
		if (Dim != 0) {
			cui_GeneralImgProcess::GetMaxValueIndexdouble(histrangedata_t[i].maxHistLeft, Dim, sort_t, sizeof(sort_t) / sizeof(int));
			if (histrangedata_t[i].maxHistCenter != &histrangedata_t[i].maxHistLeft[sort_t[0]]) {
				histrangedata_t[i].maxHistIdx = -1;
				histrangedata_t[i].maxHistLeft = 0;
				histrangedata_t[i].maxHistRight = 0;
				histrangedata_t[i].maxHistCenter = 0;
			}
		}
	}
	/*---------------记录参与本次聚类的直方图-------------------------------------------------------------*/
	for (int i = 0; i<PieceNum; i++) {
		if (histrangedata_t[i].maxHistIdx != -1) {
			int SameClassTop = histrangedata_t[i].maxHistIdx + histrangedata_t[i].StepLR;
			int SameClassDown = histrangedata_t[i].maxHistIdx - histrangedata_t[i].StepLR;
			if (SameClassDown<0) {
				SameClassDown = 0;
			}
			if (SameClassTop>HistDimSPLAB - 1) {
				SameClassTop = HistDimSPLAB - 1;
			}
			for (int j = SameClassDown; j <= SameClassTop; j++) {
				seeddata.hist_AB_CountRemove[j] = -1;
			}
		}
	}
	/*----------------------------------------------------------------------------*/
#if 0
	/*-----对获取9个分组的范围检查-------------------------------------------*/
	for (int i = 0; i<PieceNum - 1; i++) {
		unsigned int Dim = (unsigned int)histrangedata_t[i].maxHistRight - (unsigned int)histrangedata_t[i].maxHistLeft;
		Dim = Dim / sizeof(double);
		if (Dim != 0) {
			unsigned int range_1Right = (unsigned int)histrangedata_t[i].maxHistRight;
			unsigned int range_2Left = (unsigned int)histrangedata_t[i + 1].maxHistLeft;
			if (range_1Right>range_2Left) {
				unsigned int index1 = histrangedata_t[i].maxHistIdx;
				unsigned int index2 = histrangedata_t[i + 1].maxHistIdx;
				double value_1 = seeddata.hist_AB_Count[index1];
				double value_2 = seeddata.hist_AB_Count[index2];
				if (value_1<value_2) {
					histrangedata_t[i].maxHistIdx = -1;
					histrangedata_t[i].maxHistLeft = 0;
					histrangedata_t[i].maxHistRight = 0;
					histrangedata_t[i].maxHistCenter = 0;
				}
				else {
					histrangedata_t[i + 1].maxHistIdx = -1;
					histrangedata_t[i + 1].maxHistLeft = 0;
					histrangedata_t[i + 1].maxHistRight = 0;
					histrangedata_t[i + 1].maxHistCenter = 0;
				}
			}
		}
	}
#endif
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(
		CuiImgLables,
		pMD->ImgWidth,
		pMD->ImgHeight,
		pMD->p_SpProperty,
		pMD->slic_current_num);
#if 1
	vector<vector<int>> SameClass(PieceNum);
	SameClass.resize(PieceNum);
	/*-----对获取9个分组进行填充------------------------------------------*/
	for (int i = 0; i<PieceNum; i++) {
		if (histrangedata_t[i].maxHistIdx>0) {
			int SameClassTop = histrangedata_t[i].maxHistIdx + histrangedata_t[i].StepLR;
			int SameClassDown = histrangedata_t[i].maxHistIdx - histrangedata_t[i].StepLR;
			if (SameClassDown<0) {
				SameClassDown = 0;
			}
			if (SameClassTop>HistDimSPLAB - 1) {
				SameClassTop = HistDimSPLAB - 1;
			}
			for (int j = SameClassDown; j <= SameClassTop; j++) {
				int SamSP = seeddata.hist_AB_detail[j].size();
				for (int k = 0; k<SamSP; k++) {
					double SameIdx_t = seeddata.hist_AB_detail[j].at(k);
					SameClass[i].push_back(SameIdx_t);
				}
			}
		}
	}
	/*-----填充LAB矩阵-------------------------------------------------------------*/
	/*------------------------------------------------------*/
	INT32 Matrix_CL_TEST[1024];
	for (int i = 0; i<pMD->slic_current_num; i++) {
		Matrix_Category_Lable[i] = i;
		Matrix_CL_TEST[i] = i;
	}
	/*------------------------------------------------------*/
	for (int j = 0; j<PieceNum; j++) {
		int SamSP = SameClass[j].size();
		for (int i = 0; i<SamSP; i++) {
			if (SamSP >= 2 && i<SamSP - 1) {
				int orgIdx = SameClass[j].at(0);
				int changeIdx = SameClass[j].at(i + 1);
				Matrix_Category_Lable[changeIdx] = orgIdx;
				Matrix_CL_TEST[changeIdx] = orgIdx;
			}
		}
	}

	for (int i = 0; i<pMD->slic_current_num; i++) {
		spGuid SPguid = pMD->p_SpProperty[i].SPGuid;
		if (Matrix_Category_SP_USED[SPguid.GUIDX + pMD->ImgWidth*SPguid.GUIDY] == -1) {
			Matrix_Category_Lable[i] = i;
			Matrix_CL_TEST[i] = i;
		}
	}
	/*------------------------------------------------------*/
	//for (int i=0;i<pMD->slic_current_num;i++){
	//	if (Matrix_Category_Lable_USED[i]==-1){
	//		Matrix_Category_Lable[i]=i;
	//	}
	//	
	//}
#endif
	/*-----------------------------------------------------------------------------*/
#if 1
	/*--------------标记参与聚类的超像素-------------------------------------------*/
	for (int j = 0; j<PieceNum; j++) {
		int SamSP = SameClass[j].size();
		for (int i = 0; i<SamSP; i++) {
			int SPIdx = SameClass[j].at(i);
			spGuid SPguid = pMD->p_SpProperty[SPIdx].SPGuid;
			Matrix_Category_SP_USED[SPguid.GUIDX + pMD->ImgWidth*SPguid.GUIDY] = -1;

		}
	}
#endif
}
/*---------------------------------------------------------------------------------*/
/**
*根据直方图统计数据填充SameClassLink数组
*
*@param [] Matrix_Category_Lable  矩阵
*@param [] seeddata       直方图统计数据
*@param [] SameClassLink 属于同一个类的超像素块投入到同一个vector
*@param  ColorangleSpan  颜色跨度
*
*
*/
/*---------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA::HistogramRange2Matrix_Category_Lable_SameLink(
	INT32 *Matrix_Category_Lable,
	KseedsHistData& seeddata,
	vector<vector<int>>& SameClassLink,
	int ColorangleSpan)
{
	int HistDimSPLAB = seeddata.histDimSPLAB;
	int PieceNum = 9;
	int cut_step = HistDimSPLAB / PieceNum;
	int range = ColorangleSpan / 2;
	int stepLR = 1.0*range / 360 * HistDimSPLAB;
	vector<HistRangeData> histrangedata_t(PieceNum);
	int sort_t[2];
	/*-----获取9个分组的最大值---------------------------------------------------------------------*/
	for (int h = 0; h <HistDimSPLAB; h++) {
		if (h%cut_step == 0) {
			int rangeIdx = h / cut_step;
			histrangedata_t[rangeIdx].start = &(seeddata.hist_AB_Count[h]);
			histrangedata_t[rangeIdx].len = cut_step;
			histrangedata_t[rangeIdx].angle_up = h * 2;
			histrangedata_t[rangeIdx].angle_down = (h + cut_step) * 2 - 1;
			histrangedata_t[rangeIdx].StepLR = stepLR;
			cui_GeneralImgProcess::GetMaxValueIndexdouble(histrangedata_t[rangeIdx].start, cut_step, sort_t, 2);
			if (histrangedata_t[rangeIdx].start[sort_t[0]] == 0) {
				histrangedata_t[rangeIdx].maxHistIdx = -1;
				histrangedata_t[rangeIdx].maxHistLeft = 0;
				histrangedata_t[rangeIdx].maxHistRight = 0;
				histrangedata_t[rangeIdx].maxHistCenter = 0;
			}
			else {
				histrangedata_t[rangeIdx].maxHistIdx = sort_t[0] + h;
				histrangedata_t[rangeIdx].maxHistLeft = &(histrangedata_t[rangeIdx].start[sort_t[0] - stepLR]);
				histrangedata_t[rangeIdx].maxHistRight = &(histrangedata_t[rangeIdx].start[sort_t[0] + stepLR]);
				histrangedata_t[rangeIdx].maxHistCenter = &(histrangedata_t[rangeIdx].start[sort_t[0]]);
				///////////////////////////////////////////////////////////////////////////////////
				histrangedata_t[rangeIdx].maxHistLeft = (histrangedata_t[rangeIdx].maxHistLeft<&seeddata.hist_AB_Count[0]) ? &seeddata.hist_AB_Count[0] : histrangedata_t[rangeIdx].maxHistLeft;
				histrangedata_t[rangeIdx].maxHistRight = (histrangedata_t[rangeIdx].maxHistRight>&seeddata.hist_AB_Count[seeddata.histDimSPLAB - 1]) ? &seeddata.hist_AB_Count[seeddata.histDimSPLAB - 1] : histrangedata_t[rangeIdx].maxHistRight;
				///////////////////////////////////////////////////////////////////////////////////
			}
		}
	}
	/*-----对获取9个分组的最大值进行校正-------------------------------------------*/
	for (int i = 0; i<PieceNum; i++) {
		unsigned int Dim = (unsigned int)histrangedata_t[i].maxHistRight - (unsigned int)histrangedata_t[i].maxHistLeft;
		Dim = Dim / sizeof(double);
		if (Dim != 0) {
			Dim += 1;
			cui_GeneralImgProcess::GetMaxValueIndexdouble(histrangedata_t[i].maxHistLeft, Dim, sort_t, sizeof(sort_t) / sizeof(int));
			if (histrangedata_t[i].maxHistCenter != &histrangedata_t[i].maxHistLeft[sort_t[0]]) {
				histrangedata_t[i].maxHistIdx = -1;
				histrangedata_t[i].maxHistLeft = 0;
				histrangedata_t[i].maxHistRight = 0;
				histrangedata_t[i].maxHistCenter = 0;
			}
		}
	}
	/*---------------记录参与本次聚类的直方图-------------------------------------------------------------*/
	for (int i = 0; i<PieceNum; i++) {
		if (histrangedata_t[i].maxHistIdx != -1) {
			int SameClassTop = histrangedata_t[i].maxHistIdx + histrangedata_t[i].StepLR;
			int SameClassDown = histrangedata_t[i].maxHistIdx - histrangedata_t[i].StepLR;
			if (SameClassDown<0) {
				SameClassDown = 0;
			}
			if (SameClassTop>HistDimSPLAB - 1) {
				SameClassTop = HistDimSPLAB - 1;
			}
			for (int j = SameClassDown; j <= SameClassTop; j++) {
				seeddata.hist_AB_CountRemove[j] = -1;
			}
		}
	}
	/*----------------------------------------------------------------------------*/
#if 0
	/*-----对获取9个分组的范围检查-------------------------------------------*/
	for (int i = 0; i<PieceNum - 1; i++) {
		unsigned int Dim = (unsigned int)histrangedata_t[i].maxHistRight - (unsigned int)histrangedata_t[i].maxHistLeft;
		Dim = Dim / sizeof(double);
		if (Dim != 0) {
			unsigned int range_1Right = (unsigned int)histrangedata_t[i].maxHistRight;
			unsigned int range_2Left = (unsigned int)histrangedata_t[i + 1].maxHistLeft;
			if (range_1Right>range_2Left) {
				unsigned int index1 = histrangedata_t[i].maxHistIdx;
				unsigned int index2 = histrangedata_t[i + 1].maxHistIdx;
				double value_1 = seeddata.hist_AB_Count[index1];
				double value_2 = seeddata.hist_AB_Count[index2];
				if (value_1<value_2) {
					histrangedata_t[i].maxHistIdx = -1;
					histrangedata_t[i].maxHistLeft = 0;
					histrangedata_t[i].maxHistRight = 0;
					histrangedata_t[i].maxHistCenter = 0;
				}
				else {
					histrangedata_t[i + 1].maxHistIdx = -1;
					histrangedata_t[i + 1].maxHistLeft = 0;
					histrangedata_t[i + 1].maxHistRight = 0;
					histrangedata_t[i + 1].maxHistCenter = 0;
				}
			}
		}
	}
#endif
#if 1	
	/*-----对获取9个分组进行填充------------------------------------------*/
	for (int i = 0; i<PieceNum; i++) {
		if (histrangedata_t[i].maxHistIdx>0) {
			int SameClassTop = histrangedata_t[i].maxHistIdx + histrangedata_t[i].StepLR;
			int SameClassDown = histrangedata_t[i].maxHistIdx - histrangedata_t[i].StepLR;
			if (SameClassDown<0) {
				SameClassDown = 0;
			}
			if (SameClassTop>HistDimSPLAB - 1) {
				SameClassTop = HistDimSPLAB - 1;
			}
			/////////////////////////////////////////////
			SameClassLink.push_back(vector<int>());
			int SameClassLinkIdx = SameClassLink.size() - 1;
			//////////////////////////////////////////////
			for (int j = SameClassDown; j <= SameClassTop; j++) {
				int SamSP = seeddata.hist_AB_detail[j].size();
				for (int k = 0; k<SamSP; k++) {
					int SameIdx_t = seeddata.hist_AB_detail[j].at(k);
					SameClassLink[SameClassLinkIdx].push_back(SameIdx_t);
				}
			}
		}
	}
#endif
#if 1
	/*-----填充Labels矩阵-------------------------------------------------------------*/
	INT32 Matrix_CL_TEST[2048];
	///初始化Category
	for (int i = 0; i<pMD->slic_current_num; i++) {
		Matrix_Category_Lable[i] = i;
		Matrix_CL_TEST[i] = i;
	}
	/*--修正Category--------------------------------------------*/
	for (int j = 0; j<SameClassLink.size(); j++) {
		int SamSP = SameClassLink[j].size();
		for (int i = 0; i<SamSP; i++) {
			if (SamSP >= 2 && i<SamSP - 1) {
				int orgIdx = SameClassLink[j].at(0);
				int changeIdx = SameClassLink[j].at(i + 1);
				Matrix_Category_Lable[changeIdx] = orgIdx;
				Matrix_CL_TEST[changeIdx] = orgIdx;
			}
		}
	}
#endif
}
/*---------------------------------------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------------------------------------*/