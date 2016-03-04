#pragma once
#include <cv.h>
using namespace cv;
using namespace std;
#include "SGVcode/ImageMemData.h"
class SeparateSpBlock
{
private:
	ImageMemData* pMD;/**<��Ҫ��ͼ�����ݺ�ͼ������õ��м����*/
	CvSVM SvmTest;/**<֧��������ģ��*/
public:
	UINT32* ImgData;/**<ͼ������*/
	int*    ImgLab;/**<ͼ��궨Lables*/
	INT32 * Matrix_Category_Lable;/**<�궨�ĳ����صı궨��*/
public:
	SeparateSpBlock(UINT32* imgdata_t,int*  ImgLab_t,ImageMemData* MemData_t);
	~SeparateSpBlock(void);
public:
	void SeparateSp(void);
	void SeparateSp_SvmPredict(void);
};

