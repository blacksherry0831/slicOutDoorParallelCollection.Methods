#include "SGV_Method.h"
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
void SGV_Method::SVG_OUT_DOOR_HUMAN(string picvec, string saveLocation, int m_spcount)
{
		TimeMeasure tm;

		printf("1. ImageData \n");
		ImageData MemData(picvec, saveLocation, m_spcount, 0.5);

		printf("2. GPU Super \n");
		SLIC slic(&MemData);
		slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_sitaMLxy();//得到lable				

#if OUT_DOOR_HUMAN
		printf("3. Spectral Clustering \n");
		slic.Cui_Spectral_Clustering_B_2016_09_26();

		ComputeSVG2 svg(&MemData);
		svg.separateSVG_Zlm();

		MemData.SaveImgWithContours("ColorCluster");
		MemData.Draw_Kseeds_AverageImg();
#endif

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

		printf("2. GPU Super \n");
		SLIC slic(&MemData);
		slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_sitaMLxy();//得到lable				

#if OUT_DOOR_400_IMAGE_STABLE
		printf("3. ColorBarCluster \n");
		ColorBarCluster colorBarCluster(&MemData);
		colorBarCluster.Clustering();

		printf("4. ComputeSVG2 \n");
		ComputeSVG2 svg(&MemData);
		svg.separateSVG_Zlm();
#endif

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
void SGV_Method::SVG_NAVIGATION(string picvec, string saveLocation, int m_spcount)
{
	TimeMeasure tm;

	{
		printf("1. ImageData \n");
		ImageData MemData(picvec, saveLocation, m_spcount, 0.5);

		printf("2. GPU Super \n");
		SLIC slic(&MemData);
		slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_sitaMLxy();//得到lable				


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