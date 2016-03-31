#ifndef OpenCV_STL
#define OpenCV_STL          

//#include <vld.h>
/*********************************************************/
//OPENCV
#include <cv.h>
#include <highgui.h>

#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)
#include <opencv2/ml/ml.hpp>
#endif
#if (CV_MAJOR_VERSION==3)
#include <opencv2/ml.hpp> 
#endif

#include <opencv2/opencv.hpp>
#if _MSC_VER
#include <opencv2/gpu/gpu.hpp>
#endif
using namespace cv;
/*********************************************************/
//C++
#include<stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
//#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <ios>
#include <fstream>
#include <stdexcept>
#include <cstdio> 
#include<stdio.h>
#include<string.h>
#if _MSC_VER
#include <mbctype.h>  
#endif
//#if _MSC_VER
//#include<atlstr.h>
//#endif
#if linux||__linux__||__linux||__GNUC__
#include<memory>
#include <tr1/memory>
#include <dirent.h>
#endif
using namespace std;
/*********************************************************/
#endif









