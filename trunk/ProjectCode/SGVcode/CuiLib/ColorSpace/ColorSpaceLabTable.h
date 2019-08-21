#pragma once
/*----------------------------------------------------------------*/
#include "cpp_stl.h"
#include "cpp_def.h"
#include "cpp_def_debug.h"
/*----------------------------------------------------------------*/
#include <MY_SDK_LIB/TimeMeasure.hpp>
/*----------------------------------------------------------------*/
#define		WIDTH_TAB_MAX	(4096)
#define		HEIGHT_TAB_MAX	(2160)
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
class ColorSpaceLabTable
{
private:

	ColorSpaceLabTable();
	~ColorSpaceLabTable();
public:
	static void InitLabTable();
	static void InitXYTable_int(const int _w, const int _h);
private:
	static const  int RGB_MAX;
	static int LabTableLab_L[UCHAR_MAX][UCHAR_MAX][UCHAR_MAX];
	static int LabTableLab_A[UCHAR_MAX][UCHAR_MAX][UCHAR_MAX];
	static int LabTableLab_B[UCHAR_MAX][UCHAR_MAX][UCHAR_MAX];
private:
	static int LabTableLab_X[WIDTH_TAB_MAX * HEIGHT_TAB_MAX];
	static int LabTableLab_Y[WIDTH_TAB_MAX * HEIGHT_TAB_MAX];
public:
	static int* X();
	static int* Y();
public:
	static		void DoRGBtoLABConversion(
		const unsigned int*		ubuff,
		double*					lvec,
		double*					avec,
		double*					bvec,
		const int               width,
		const int               height);

	static		void DoRGBtoLABConversionInt(
		const unsigned int*		ubuff,
		int*					lvec,
		int*					avec,
		int*					bvec,
		const int               width,
		const int               height);

};

