#include "ColorSpaceLabTable.h"
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
#include "ColorSpaceLAB.h"
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
const  int ColorSpaceLabTable::RGB_MAX=UCHAR_MAX;
/*----------------------------------------------------------------*/
int ColorSpaceLabTable::LabTableLab_L[UCHAR_MAX][UCHAR_MAX][UCHAR_MAX];
int ColorSpaceLabTable::LabTableLab_A[UCHAR_MAX][UCHAR_MAX][UCHAR_MAX];
int ColorSpaceLabTable::LabTableLab_B[UCHAR_MAX][UCHAR_MAX][UCHAR_MAX];
/*----------------------------------------------------------------*/
int ColorSpaceLabTable::LabTableLab_X[WIDTH_TAB_MAX * HEIGHT_TAB_MAX];
int ColorSpaceLabTable::LabTableLab_Y[WIDTH_TAB_MAX * HEIGHT_TAB_MAX];
/*----------------------------------------------------------------*/
ColorSpaceLabTable::ColorSpaceLabTable()
{
	
}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
ColorSpaceLabTable::~ColorSpaceLabTable()
{

}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
void ColorSpaceLabTable::InitLabTable()
{
	TimeMeasure tm("init lab table .");

	for (int ri = 0; ri <RGB_MAX; ri++){
		for (int gi = 0; gi < RGB_MAX; gi++){
			for (int bi = 0; bi < RGB_MAX; bi++){
				
				double L=0,A=0,B=0;
				ColorSpaceLAB::RGB2LAB(ri, gi, bi, L, A, B);
				LabTableLab_L[ri][gi][bi] =	L;
				LabTableLab_A[ri][gi][bi] = A;
				LabTableLab_B[ri][gi][bi] = B;
			}
		}
	}

}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
void ColorSpaceLabTable::InitXYTable_int(const int _w,const int _h)
{
	ASSERT(_w < WIDTH_TAB_MAX);
	ASSERT(_h < HEIGHT_TAB_MAX);

	for (int wi = 0; wi < _w; wi++){
		for (int hi = 0; hi < _h; hi++){
			const int idx = wi + hi*_w;
			LabTableLab_X[idx]=wi;
			LabTableLab_Y[idx]=hi;
		}
	}
	
}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
void ColorSpaceLabTable::DoRGBtoLABConversion(
	const unsigned int * ubuff,
	double * lvec,
	double * avec,
	double * bvec,
	const int width,
	const int height)
{
	TimeMeasure tm("RGB2LAB.Table");

	const int sz = width*height;
	/*lvec = new double[sz];
	avec = new double[sz];
	bvec = new double[sz];*/
	/*r:0--255*/
	/*g:0--255*/
	/*b:0--255*/
	int r, g, b;
	for (int j = 0; j < sz; j++)
	{
		r = (ubuff[j] >> 16) & 0xFF;
		g = (ubuff[j] >> 8) & 0xFF;
		b = (ubuff[j]) & 0xFF;
		//////////////////////////////////
		// a  r      g      b
		//////////////////////////////////
		lvec[j]=LabTableLab_L[r][g][b];
		avec[j]=LabTableLab_A[r][g][b];
		bvec[j]=LabTableLab_B[r][g][b];
	}
}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
void ColorSpaceLabTable::DoRGBtoLABConversionInt(
	const unsigned int * ubuff,
	int * lvec,
	int * avec,
	int * bvec,
	const int width,
	const int height)
{
	TimeMeasure tm("RGB2LAB.Table.Int");

	const int sz = width*height;
	int r, g, b;
	for (int j = 0; j < sz; j++)
	{
		r = (ubuff[j] >> 16) & 0xFF;
		g = (ubuff[j] >> 8) & 0xFF;
		b = (ubuff[j]) & 0xFF;
		//////////////////////////////////
		// a  r      g      b
		//////////////////////////////////
		lvec[j] = LabTableLab_L[r][g][b];
		avec[j] = LabTableLab_A[r][g][b];
		bvec[j] = LabTableLab_B[r][g][b];
	}

}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
int* ColorSpaceLabTable::X()
{
	return &LabTableLab_X[0];
}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
int* ColorSpaceLabTable::Y()
{
	return &LabTableLab_Y[0];
}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/

