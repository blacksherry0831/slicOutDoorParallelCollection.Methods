#pragma once
/*----------------------------------------------------------------*/
#include <SGVcode/ImageType.h>
#include <SGVcode/ImageMemData.h>
#include <SGVcode/ImageData.h>
/*----------------------------------------------------------------*/
#include <SGVcode/SLIC.h>
#include <SGVcode/ComputeSVG2.h>
#include <SGVcode/ColorBarCluster.h>
/*----------------------------------------------------------------*/
#include <MY_SDK_LIB/TimeMeasure.hpp>
/*----------------------------------------------------------------*/
#include <cpp_stl.h>
#include <cpp_def.h>
#include <cpp_def_debug.h>
/*----------------------------------------------------------------*/
#include <opencv_stl.h>
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
class SGV_Method
{
public:
	SGV_Method(void);
	~SGV_Method(void);
public:


static	void SVG_OUT_DOOR_HUMAN(
								string picvec,
								string saveLocation,
								int m_spcount);

static	void SVG_OUT_DOOR_400_IMAGE_STABLE(
								string picvec,
								string saveLocation,
								int m_spcount);


static	void SVG_NAVIGATION(
			string picvec,
			string saveLocation,
			int m_spcount);

};

