#ifndef OpenCV_STL
#define OpenCV_STL          
/*-----------------------------------------*/
#include "opencv_basic.h"
using namespace cv;
/*-----------------------------------------*/
#include "cpp_basic.h"
using namespace std;
/*-----------------------------------------*/
#ifndef TimeCountClockTest

#define TimeCountStart()	double dur=0;clock_t start,end;start = clock();

#define TimeCountStop(MESSAGE_TIME_END)		end = clock();dur = (double)(end - start);printf("%s(S):%0.3f\n",MESSAGE_TIME_END,(dur/CLOCKS_PER_SEC));

#endif
/*-----------------------------------------*/
#endif