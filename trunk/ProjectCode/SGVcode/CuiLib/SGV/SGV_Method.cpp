#include "SGV_Method.h"
/*-------------------------------------------------------------------------*/
#include <SGV/SGV_Method.h>
#include <SGV/SGV_CFG_DBG.h>
#include <SGV/SGV_CFG_ARITHMETIC.h>
/*-------------------------------------------------------------------------*/
#include <SGVcode/ImageDataBasic.h>
#include <SGVcode/ImageDataLABXY.h>
/*-------------------------------------------------------------------------*/
#include <SGVcode/cui_GeneralImgProcess.h>
#include <SGVcode/ImageSaveProc.h>
/*-------------------------------------------------------------------------*/
#include <ColorSpace/ColorSpaceLabTable.cpp>
/*-------------------------------------------------------------------------*/
#include <SLIC_EX/SLIC_LAB_CUDA_INT.h>
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
SGV_Method::SGV_Method(void)
{

}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
SGV_Method::~SGV_Method(void)
{

}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
void SGV_Method::initGlobalCfg()
{
	SGV_CFG_DBG cfg_dbg;

	cfg_dbg.InitLoad();

	cui_GeneralImgProcess::SAVE_IMAGE_2DISK = cfg_dbg.getImgSave();
	cui_GeneralImgProcess::SAVE_DEBUG_2DISK = cfg_dbg.getDebugSave();

	ImageSaveProc::SAVE_IMAGE_2DISK = cfg_dbg.getImgSave();
	ImageSaveProc::SAVE_DEBUG_2DISK = cfg_dbg.getDebugSave();
	
}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
void SGV_Method::SVG_OUT_DOOR_HUMAN(string picvec, string saveLocation, int m_spcount)
{
		TimeMeasure tm;

		printf("1. ImageData \n");
		ImageData MemData(picvec, saveLocation, m_spcount, 0.5);
		MemData.initThetaMLXY();

		printf("2. GPU Super \n");
		SLIC slic(&MemData);
		slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_sitaMLxy();//得到lable				

//#if OUT_DOOR_HUMAN
		printf("3. Spectral Clustering \n");
		slic.Cui_Spectral_Clustering_B_2016_09_26();

		ComputeSVG2 svg(&MemData);
		svg.separateSVG_Zlm();

		MemData.SaveImgWithContours("ColorCluster");
		MemData.Draw_Kseeds_AverageImg();
//#endif

		MemData.SeparateSp();

//#if	!(SaveContours2Disk)
		//		MemData.SaveImgWithContours();			
//#endif
	
}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
void SGV_Method::SVG_OUT_DOOR_400_IMAGE_STABLE(string picvec, string saveLocation, int m_spcount)
{
	TimeMeasure tm;

	{
		printf("1. ImageData \n");
		ImageData MemData(picvec, saveLocation, m_spcount, 0.5);
		MemData.initThetaMLXY();

		printf("2. GPU Super \n");
		SLIC slic(&MemData);
		slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_sitaMLxy();//得到lable				

//#if OUT_DOOR_400_IMAGE_STABLE
		printf("3. ColorBarCluster \n");
		ColorBarCluster colorBarCluster(&MemData);
		colorBarCluster.Clustering();

		printf("4. ComputeSVG2 \n");
		ComputeSVG2 svg(&MemData);
		svg.separateSVG_Zlm();
//#endif

		MemData.SeparateSp();

		//#if	!(SaveContours2Disk)
		//		MemData.SaveImgWithContours();			
		//#endif

	}
}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
void SGV_Method::SVG_NAVIGATION_LAB_SVG(
	IplImage* _img,
	string _saveLocation,
	const int _K,
	const float _M,
	const float _HL_VP)
{
	TimeMeasure tm;
	{
		printf("1. ImageData \n");
		ImageData MemData(_img, _saveLocation, _K, _HL_VP);
		MemData.initLABXY();

		printf("2. GPU Super \n");
		SLIC_LAB_CUDA slic(&MemData);
		slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_LAB_CUDA(_M);//得到lable				
	
		printf("3. ColorBarCluster \n");
		ColorBarCluster colorBarCluster(&MemData);
		colorBarCluster.Clustering();

		printf("4. ComputeSVG2 \n");
		ComputeSVG2 svg(&MemData);
		svg.separateSVG_Zlm();

		MemData.SeparateSp();
	}	
}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
void SGV_Method::SVG_NAVIGATION_CAR(IplImage* _img,
									string _saveLocation,
									const int _K,
									const float _M,
									const float _HL_VP)
{
	
	TimeMeasure tm("##################### NAVIGATION CAR #####################");
	{
#if 0
		printf("1. ImageData \n");
		ImageData MemData(_img, _saveLocation, _K, _HL_VP);
		MemData.initLABXY();

		printf("2. GPU Super \n");
		SLIC_LAB_CUDA slic(&MemData);
		slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_LAB_CUDA(_M);//得到lable	
#else
		ImageDataLABXY MemData(_img, _saveLocation);
		MemData.CvtRGB2LAB();
		
		SLIC_LAB_CUDA_INT slic(_img, _saveLocation);
		slic.initSlic(_K, _M);
		slic.initLabPtr(MemData.L_v, MemData.A_v, MemData.B_v);
		slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_LAB_CUDA();//得到lable	
		
#endif // 0
		
	}
}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
void SGV_Method::METHOD_FILE(const std::string _f)
{
	ImageDataBasic imgorg(_f,"");
	METHOD_MEM(_f,imgorg.GetIplImagePtr());
}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
void SGV_Method::METHOD_MEM(const std::string _f,IplImage* _img)
{
	initGlobalCfg();

	SGV_CFG_DBG cfg_dbg;
	SGV_CFG_ARITHMETIC cfg_arithmetic;

	cfg_dbg.InitLoad();
	cfg_arithmetic.InitLoad();

	const std::string saveLocation = cfg_dbg.getOutputPath().toStdString();
	const QString   method_t= cfg_arithmetic.getMethod_Current();

	const float K = cfg_arithmetic.getSuperPixel_Number();
	const float M = cfg_arithmetic.getSuperPixel_Compactness();
	const float AH_VP = cfg_arithmetic.getSVG_G_ApparentHorizon_VanishingPoint();

	if (method_t.compare(SGV_CFG_ARITHMETIC::NODE_METHOD_LAB_SVG)==0){
		
		SVG_NAVIGATION_LAB_SVG(
			_img,
			saveLocation,
			K,
			M,
			AH_VP);

	}else if (method_t.compare(SGV_CFG_ARITHMETIC::NODE_METHOD_NAV_CAR)==0){

		ColorSpaceLabTable::InitLabTable();
		
		SVG_NAVIGATION_CAR(_img,
			saveLocation,
			K,
			M,
			AH_VP);

	}else if (method_t.compare(SGV_CFG_ARITHMETIC::NODE_METHOD_OUT_DOOR_400_IMAGE_STABLE)==0){
		
		SVG_OUT_DOOR_400_IMAGE_STABLE(_f,saveLocation,K);

	}else if (method_t.compare(SGV_CFG_ARITHMETIC::NODE_METHOD_OUT_DOOR_HUMAN)==0){
		
		SVG_OUT_DOOR_HUMAN(_f, saveLocation,K);

	}else{
		Q_ASSERT(0);
	}
	

}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/