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
#include <SLIC_EX/SLIC_LAB_CUDA.h>
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

static void initGlobalCfg();

static	void SVG_OUT_DOOR_HUMAN(
						string picvec,
						string saveLocation,
						int m_spcount);

static	void SVG_OUT_DOOR_400_IMAGE_STABLE(
								string picvec,
								string saveLocation,
								int m_spcount);

static	void SVG_NAVIGATION_LAB_SVG(
						IplImage* _img,
						string _saveLocation,
						const int _K = 1089,
						const float _M = 10,
						const float _HL_VP = 0.5);

static	void SVG_NAVIGATION_CAR(
						IplImage* _img,
						string _saveLocation,
						const int _K = 1089,
						const float _M = 10,
						const float _HL_VP = 0.5);

public:

static	void METHOD_FILE(const std::string _f);
static	void METHOD_MEM(const std::string _f,IplImage* _img);

};

