//CUDA_Transfer.cpp
#include "stdafx.h"
#include "CUDA_Code.h"
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C" void PerformSuperpixelSLIC_gpu(
	float alpha,
	float betta,
	float gama,
	float fai,
	const int kseedsSize,
	const int m_width,
	const int m_height,
	double*				m_lvec,
	double*				m_avec,
	double*				m_bvec,
	double*				host_kseedsl,
	double*				host_kseedsa,
	double*				host_kseedsb,
	double*				host_kseedsx,
	double*				host_kseedsy,
	int*				host_klabels,
	const int			STEP,
	double*				host_edgemag,
	const double		M);
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C" void  GetSeedsLabxy_gpu(
	const int kseedsSize,
	const int m_width,
	const int m_height,
	double*				host_m_lvec,
	double*				host_m_avec,
	double*				host_m_bvec,
	double*				host_kseedsl,
	double*				host_kseedsa,
	double*				host_kseedsb,
	double*				host_kseedsx,
	double*				host_kseedsy,
	int*				host_klabels);
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C" void PerformSuperpixelSLIC_gpu_simplify(
	float alpha,
	float betta,
	float gama,
	float fai,
	const int kseedsSize,
	const int m_width,
	const int m_height,
	double*				m_lvec,
	double*				m_avec,
	double*				m_bvec,
	double*				host_kseedsl,
	double*				host_kseedsa,
	double*				host_kseedsb,
	double*				host_kseedsx,
	double*				host_kseedsy,
	int*				host_klabels,
	const int			STEP,
	double*				host_edgemag,
	const double		M);
extern "C" void PerformSuperpixelSLIC_gpu_simplify2(
	float alpha,
	float betta,
	float gama,
	float fai,
	const int kseedsSize,
	const int m_width,
	const int m_height,
	double*				m_lvec,
	double*				m_avec,
	double*				m_bvec,
	double*				host_kseedsl,
	double*				host_kseedsa,
	double*				host_kseedsb,
	double*				host_kseedsx,
	double*				host_kseedsy,
	int*				host_klabels,
	const int			STEP,
	double*				host_edgemag,
	const double		M);
/*------------------------------------------------------------------------------------------*/
/**
*cuda����ʵ�г������㷨��
*
*
*@param    alpha  �ǶȲ���alpha
*@param    betta  ģֵbetta
*@param    gama   ����gama
*@param    fai    XY����
*@param    kseedsSize  ���Ӹ���
*@param    m_width   ͼ�����
*@param    m_height  ͼ��߶�
*@param	   m_lvec  ͼ��L����
*@param	   m_avec  ͼ��A����
*@param	   m_bvec  ͼ��B����
*@param	   kseedsl  ����L����
*@param	   kseedsa  ����A����
*@param	   kseedsb  ����B����
*@param	   kseedsx  ����X����
*@param	   kseedsy  ����Y����
*@param	   klabels  ͼ��ͼ��ı궨ͼ��
*@param	   STEP     δʹ��
*@param	   edgemag  δʹ��
*@param	   M        δʹ���ܶ�����
*
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
void PerformSuperpixelSLIC_cuda(
	float alpha,
	float betta,
	float gama,
	float fai,
	const int kseedsSize,
	const int m_width,
	const int m_height,
	double*				m_lvec,
	double*				m_avec,
	double*				m_bvec,
	double*				kseedsl,
	double*				kseedsa,
	double*				kseedsb,
	double*				kseedsx,
	double*				kseedsy,
	int*				klabels,
	const int			STEP,
	double*				edgemag,
	const double		M)
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
#if 1
{

		clock_t start, finish;
		start = clock();
		{
			PerformSuperpixelSLIC_gpu_simplify2(
				alpha,
				betta,
				gama,
				fai,
				kseedsSize,
				m_width,
				m_height,
				m_lvec,
				m_avec,
				m_bvec,
				kseedsl,
				kseedsa,
				kseedsb,
				kseedsx,
				kseedsy,
				klabels,
				STEP,
				edgemag,
				M);
		}

		finish = clock();
		double duration_ms = (double)(finish - start)*1000 / CLOCKS_PER_SEC;
		double duration_s = duration_ms/1000;
		TRACE("\n ʮ�ε���: %f���룩",duration_s);
		double duration_min =duration_s/60;
		printf( "gpu time is %f ms/n", duration_ms );
}
#else
	for (int i=0;i<m_width*m_height*10;i++){
		double a=atan2((double)10,(double)10);
	}	
#endif
		QueryPerformanceCounter(&litmp);	
		QPart2 = litmp.QuadPart;//�����ֵֹ	
		dfMinus = (double)(QPart2-QPart1);	
		dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��	
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C" void PerformSuperpixelSLIC_ThetaMLXY_gpu_simplify(
	float alpha,
	float betta,
	float gama,
	float fai,
	const int kseedsSize,
	const int m_width,   
	const int m_height,
	double*     host_sita_n,
	double*     host_m_n,
	double*     host_L_n,
	double*     host_X_n,
	double*     host_Y_n,
	double*     host_kseedsTheta,
	double*     host_kseedsM,
	double*     host_kseedsL,
	double*     host_kseedsX,
	double*     host_kseedsY,
	int*		host_klabels,
	const int			STEP);
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
void  PerformSuperpixelSLIC_ThetaMLXY_cuda(
	float alpha,
	float betta,
	float gama,
	float fai,
	const int kseedsSize,
	const int m_width,
	const int m_height,
	double*     sita_n,
	double*     m_n,
	double*     L_n,
	double*     X_n,
	double*     Y_n,
	double*     kseedsTheta,
	double*     kseedsM,
	double*     kseedsL,
	double*     kseedsX,
	double*     kseedsY,
	int*	    klabels,
	const int	STEP)
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
#if 1
	{
		PerformSuperpixelSLIC_ThetaMLXY_gpu_simplify(
			 alpha,
			 betta,
			 gama,
			 fai,
			 kseedsSize,
			 m_width,   
			 m_height,
			 sita_n,
			 m_n,
			 L_n,
			 X_n,
			 Y_n,
			 kseedsTheta,
			 kseedsM,
			 kseedsL,
			 kseedsX,
			 kseedsY,
			 klabels,
			 STEP);
	}
#endif
	QueryPerformanceCounter(&litmp);	
	QPart2 = litmp.QuadPart;//�����ֵֹ	
	dfMinus = (double)(QPart2-QPart1);	
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��	
	TRACE("\n ʮ�ε���: %f���룩",dfTim);
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C" void DrawContoursAroundSegments_gpu(
	unsigned int*			ubuff,
	int*					labels,
	const int				width,
	const int				height,
	const unsigned int		color_in,
	const unsigned int      color_out);
/*------------------------------------------------------------------------------------------*/
/**
*����������ͼ�񲢱��浽ͼ��
*
*@param	 [in]	ubuff    ͼ������
*@param	 [in]	labels   ͼ��labels��Ϣ
*@Parma			width    ͼ�����
*@param			height   ͼ��߶�
*@Parma			color   ������ɫ
*
*
*/
/*------------------------------------------------------------------------------------------*/
void DrawContoursAroundSegments_cuda(
	unsigned int*			ubuff,
	int*					labels,
	const int				width,
	const int				height,
	const unsigned int		color )
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
#if 1
	DrawContoursAroundSegments_gpu(ubuff,labels,width,height,color,BlackColorPNG);
#endif
	QueryPerformanceCounter(&litmp);	
	QPart2 = litmp.QuadPart;//�����ֵֹ	
	dfMinus = (double)(QPart2-QPart1);	
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��	
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
void HarrTransformLet_MatrixSub_gpu(
	unsigned char *matrixA, 
	unsigned char *matrixB, 
	int height, 
	int width, 
	unsigned char *result);
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
void HarrTransformLet_MatrixSub_cuda(
	unsigned char *matrixA, 
	unsigned char *matrixB, 
	int height, 
	int width, 
	unsigned char *result)
{
	HarrTransformLet_MatrixSub_gpu(
	matrixA, 
	matrixB, 
	height, 
	width, 
	result);
}
/*------------------------------------------------------------------------------------------*/
/**
*
*��ȡͼ���KseedsSize������
*
*@param               kseedsSize   ���Ӹ���
*@param               m_width      ͼ�񿴶�
*@param               m_height     ͼ��߶�
*@param 	[in]	  m_lvec       L��������
*@param 	[in]	  m_avec       A��������
*@param 	[in]	  m_bvec       B��������
*@param 	[out]	  kseedsl      ���ӵ�L����
*@param 	[out]	  kseedsa      ���ӵ�A����
*@param 	[out]	  kseedsb      ���ӵ�B����
*@param 	[out]	  kseedsx      ���ӵ�X����
*@param 	[out]	  kseedsy      ���ӵ�Y����
*@param 	[in]	  klabels      ͼ��ָ��Labels
*
*
*/
/*------------------------------------------------------------------------------------------*/
void  GetSeedsLabxy_cuda(
	const int kseedsSize,
	const int m_width,
	const int m_height,
	double*				m_lvec,
	double*				m_avec,
	double*				m_bvec,
	double*				kseedsl,
	double*				kseedsa,
	double*				kseedsb,
	double*				kseedsx,
	double*				kseedsy,
	int*				klabels)
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
#if 1
	GetSeedsLabxy_gpu(
		kseedsSize,
		m_width,
		m_height,
		m_lvec,
		m_avec,
		m_bvec,
		kseedsl,
		kseedsa,
		kseedsb,
		kseedsx,
		kseedsy,
		klabels);
#endif
	QueryPerformanceCounter(&litmp);	
	QPart2 = litmp.QuadPart;//�����ֵֹ	
	dfMinus = (double)(QPart2-QPart1);	
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
	double dfms=dfTim*1000;//ms
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C" void Get_Nighbour_E_matrix_gpu(
	int*			host_labels,
	const  int		NumLabels,
	UINT32 *		host_Matrix_E,
	const int		width,
	const int		height);
/*------------------------------------------------------------------------------------------*/
/**
*���м����ȡE����
*
*@param [in] labels        ͼ��궨
*@param  NumLabels         labels����
*@param [out] Matrix_E     ���Ӿ���
*@param  width             ͼ�����
*@param  height            ͼ��߶�
*@param  filename          �ļ���
*@param  path              �ļ�·��
*
*/
/*------------------------------------------------------------------------------------------*/
void Get_Nighbour_E_matrix_cuda(
	int*					labels,
	const  int		NumLabels,
	UINT32 *Matrix_E,
	const int			width,
	const int			height,	
	const string		filename,
	const string		path)
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
#if 1
	Get_Nighbour_E_matrix_gpu(labels,NumLabels,Matrix_E,width,height);
#endif
	QueryPerformanceCounter(&litmp);	
	QPart2 = litmp.QuadPart;//�����ֵֹ	
	dfMinus = (double)(QPart2-QPart1);	
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C"  void classify_SkyVerticalGround_gpu(
	double horizontal_line,
	INT32*	 Matrix_Category_Lable_host,
	double* SkyWeightArray_host,
	double* VerticalWeightArray_host,
	double* GroundWeightArray_host,
	double* SkyWeightSp_host,
	double* VerticalWeightSp_host,
	double* GroundWeightSp_host,
	int*    labels_host,
	int     Numlabels,
	int Width,
	int Height);
/*------------------------------------------------------------------------------------------*/
/**
*
*@param [out] Matrix_Category_Lable
*
*/
/*------------------------------------------------------------------------------------------*/
void classify_SkyVerticalGround_cuda(
	double horizontal_line,
	INT32*	Matrix_Category_Lable,
	double *SkyWeightArray_host,
	double *VerticalWeightArray_host,
	double *GroundWeightArray_host,
	double*  SkyWeightSp_host,
	double*  VerticalWeightSp_host,
	double*  GroundWeightSp_host,
	int*    labels,
	int     Numlabels,
	int Width,
	int Height)
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
#if 1
	classify_SkyVerticalGround_gpu(
		horizontal_line,
		Matrix_Category_Lable,
		SkyWeightArray_host,
		VerticalWeightArray_host,
		GroundWeightArray_host,
		SkyWeightSp_host,
		VerticalWeightSp_host,
		GroundWeightSp_host,
		labels,
		Numlabels,
		Width,
		Height);
#endif
	QueryPerformanceCounter(&litmp);	
	QPart2 = litmp.QuadPart;//�����ֵֹ	
	dfMinus = (double)(QPart2-QPart1);	
	dfTim = dfMinus / dfFreq*1000;// ��ö�Ӧ��ʱ��ֵ����λΪMS
	//dfTim = 131.77898596090682 ms
}