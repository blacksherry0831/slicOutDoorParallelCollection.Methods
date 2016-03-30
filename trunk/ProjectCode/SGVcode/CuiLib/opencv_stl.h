#ifndef OpenCV_STL
#define OpenCV_STL          

//#include <vld.h>
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
//C++
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

#endif









