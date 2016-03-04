#pragma once
#include <cv.h>
using namespace cv;
using namespace std;
#include "SGVcode/ImageMemData.h"
class SeparateSpBlock
{
private:
	ImageMemData* pMD;/**<需要的图像数据和图像计算用的中间变量*/
	CvSVM SvmTest;/**<支持向量机模型*/
public:
	UINT32* ImgData;/**<图像数据*/
	int*    ImgLab;/**<图像标定Lables*/
	INT32 * Matrix_Category_Lable;/**<标定的超像素的标定集*/
public:
	SeparateSpBlock(UINT32* imgdata_t,int*  ImgLab_t,ImageMemData* MemData_t);
	~SeparateSpBlock(void);
public:
	void SeparateSp(void);
	void SeparateSp_SvmPredict(void);
};

