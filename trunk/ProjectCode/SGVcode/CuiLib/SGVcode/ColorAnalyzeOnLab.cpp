#include "StdAfx.h"
#include "ColorAnalyzeOnLab.h"
/*--------------------------------------------------------*/
/**
*����ͼ��
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
*��������
*/
/*--------------------------------------------------------*/
ColorAnalyzeOnLab::~ColorAnalyzeOnLab(void)
{

}
/*--------------------------------------------------------*/
/**
*��ó����ؿ�����ߴ������ɫ
*@return  Lab��ɫֵ
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
