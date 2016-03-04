#include "StdAfx.h"
#include "ColorAnalyzeOnLab.h"
/*--------------------------------------------------------*/
/**
*输入图像
*
*/
/*--------------------------------------------------------*/
ColorAnalyzeOnLab::ColorAnalyzeOnLab(ImageMemData* MemData_t)
{
	this->pMD=MemData_t;
	pMD->SplitImgBGRALab();
}
/*--------------------------------------------------------*/
/**
*析构函数
*/
/*--------------------------------------------------------*/
ColorAnalyzeOnLab::~ColorAnalyzeOnLab(void)
{

}
/*--------------------------------------------------------*/
/**
*获得超像素块中最具代表的颜色
*@return  Lab颜色值
*/
/*--------------------------------------------------------*/
LabColor ColorAnalyzeOnLab::GetImageColorMostly()
{
	LabColor color;
	Kmean   km(pMD->src_Img_bgra,pMD,pMD->src_ImgMaskLables);
	km.ChangeColor2LabModule();
	color=km.GetMaxAreaClassifications();
	return color;
}
/*--------------------------------------------------------*/
/**
*
*/
/*--------------------------------------------------------*/
