#pragma once
/*----------------------------------------------------------------*/
#include "cpp_stl.h"
#include "cpp_def.h"
#include "cpp_def_debug.h"
/*----------------------------------------------------------------*/
#include <MY_SDK_LIB/TimeMeasure.hpp>
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
class ColorSpaceLAB
{

private:

	ColorSpaceLAB();
	~ColorSpaceLAB();

public:

static		void DoRGBtoLABConversion(
			const unsigned int*		ubuff,
			double*					lvec,
			double*					avec,
			double*					bvec,
			const int               width,
			const int               height);

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



};

