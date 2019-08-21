#pragma once
//#//if !defined(_SLIC_H_INCLUDED_)
//#//define _SLIC_H_INCLUDED_
#include <platform_cui.h>

#include "SLIC_BASE.h"

/**�ܹ����г����طָ�����ֵ*/
#define  Num_Hist 2048 
#define  UseTextureInColorWithCombine  FALSE 
/*----------------------------------------------------------------------------------------------------------------*/
/**
*  SLIC   class.
*  ������ɳ����غ��׾���Ĺ���
*
*/
/*----------------------------------------------------------------------------------------------------------------*/
class SLIC_LAB_CUDA : public SLIC_BASE
{
public:
	SLIC_LAB_CUDA();
	SLIC_LAB_CUDA(ImageMemData*  MemData_t);
	SLIC_LAB_CUDA(ImageData*  MemData_t);
	virtual ~SLIC_LAB_CUDA();
	//============================================================================
	// Superpixel segmentation for a given step size (superpixel size ~= step*step)
	//============================================================================
	void DoSuperpixelSegmentation_ForGivenSuperpixelSize(
		const unsigned int*                            ubuff,//Each 32 bit unsigned int contains ARGB pixel values.
		const int					width,
		const int					height,
		INT32*&						klabels,
		int&						numlabels,
		const int&					superpixelsize,
		const double&                                   compactness);

	void DoSuperpixelSegmentation_ForGivenSuperpixelSize_sitaMLxy(
		const unsigned int*         ubuff,//Each 32 bit unsigned int contains ARGB pixel values.
		const int					width,
		const int					height,
		INT32*&						klabels,
		int&						numlabels,
		const int&					superpixelsize);

	void DoSuperpixelSegmentation_ForGivenSuperpixelSize_LAB_CUDA(
		const unsigned int*         ubuff,
		const int					width,
		const int					height,
		INT32*&						klabels,
		int&						numlabels,
		const double				_M);
	
	void DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_LAB_CUDA(const int _M);

	//============================================================================
	// Superpixel segmentation for a given number of superpixels
	//============================================================================

	//============================================================================
	// Supervoxel segmentation for a given step size (supervoxel size ~= step*step*step)
	//============================================================================
	void DoSupervoxelSegmentation(
		unsigned int**&		ubuffvec,
		const int&					width,
		const int&					height,
		const int&					depth,
		int**&						klabels,
		int&						numlabels,
		const int&					supervoxelsize,
		const double&               compactness);
	//============================================================================
	// Save supervoxel labels in a text file in raster scan, depth order
	//============================================================================
	void SaveSupervoxelLabels(
		const int**&				labels,
		const int&					width,
		const int&					height,
		const int&					depth,
		const string&				filename,
		const string&				path);
	//============================================================================
	// Function to draw boundaries around superpixels of a given 'color'.
	// Can also be used to draw boundaries around supervoxels, i.e layer by layer.
	//============================================================================
public:
	double* Cui_Matrix_W_Vein;
public:
	string FileReadFullPath;
	string FileWritePath;
private:
	//============================================================================
	// The main SLIC algorithm for generating superpixels
	//============================================================================
	void PerformSuperpixelSLIC(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		int*&						klabels,
		const int&					STEP,
		const vector<double>&		edgemag,
		const double&				m = 10.0);
	//============================================================================
	// The main SLIC algorithm for generating supervoxels
	//============================================================================
	void PerformSupervoxelSLIC(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		vector<double>&				kseedsz,
		int**&						klabels,
		const int&					STEP,
		const double&				compactness);
	//============================================================================
	// Pick seeds for superpixels when step size of superpixels is given.
	//============================================================================
	void GetLABXYSeeds_ForGivenStepSize(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		const int&					STEP,
		const bool&					perturbseeds,
		const vector<double>&		edgemag);
	//============================================================================
	// Pick seeds for supervoxels
	//============================================================================
	void GetKValues_LABXYZ(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		vector<double>&				kseedsz,
		const int&					STEP);
	//============================================================================
	// Move the superpixel seeds to low gradient positions to avoid putting seeds
	// at region boundaries.
	//============================================================================
	void PerturbSeeds(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		const vector<double>&		edges);
	//============================================================================
	// Detect color edges, to help PerturbSeeds()
	//============================================================================
	void DetectLabEdges(
		const double*				lvec,
		const double*				avec,
		const double*				bvec,
		const int&					width,
		const int&					height,
		vector<double>&				edges);
	//============================================================================
	// Post-processing of SLIC segmentation, to avoid stray labels.
	//============================================================================
	void EnforceLabelConnectivity(
		const int*					labels,
		const int					width,
		const int					height,
		int*&						nlabels,//input labels that need to be corrected to remove stray labels
		int&						numlabels,//the number of labels changes in the end if segments are removed
		const int&					K); //the number of superpixels desired by the user


	void EnforceLabelConnectivityEx(
		int*	klabels,
		const int width,
		const int height,
		int & numlabels,
		const int & K); 
										//============================================================================
										// Post-processing of SLIC supervoxel segmentation, to avoid stray labels.
										//============================================================================
	void EnforceSupervoxelLabelConnectivity(
		int**&						labels,//input - previous labels, output - new labels
		const int&					width,
		const int&					height,
		const int&					depth,
		int&						numlabels,
		const int&					STEP);
public:
	/***********************************************************************************/
	static	wstring							Narrow2Wide(
		const string&		narrowString);

	static	string							Wide2Narrow(
		const wstring&		wideString);
	/************************************************************************************/
private:
	ImageMemData* pMD;/**<ͼ�����ݼ�������м����*/
	ImageData* pIMD;
public:
	UINT32*  CuiImgData;/**<ԭʼͼ��*/
	int CuiWidth;
	int CuiHeight;
	IplImage * CuiCvImage;
	IplImage * Cui_ARGB_Image;
public:
	void CuiFindSaveNighbour_E_matrix(
		const UINT32&               SelfNighbour = 0,
		const string&				filename = "NighbourMatrixPixels.data",
		const string&				path = "");

	void CuiGetImageData(string filename, string filesavepath);
private:
	void CuiGetImageData(void);
	void CuiGetImageDataPIMD(void);
public:
	
	string cuiGetCurrentTime(void);
	
	/**************�׾����м����*****************/
	//�������;
	int*  CuiImgLables;/**<lableͼ�飨�����أ� ͼ��width*height	32λint������ ���㿪ʼ����*/

					   /**<lable ��ͼ�飨�����أ��ĸ���*/
					   /********ֱ��ͼʹ��**���W����*********************/
	IplImage * Cui_lab_hsv_Image;/**<Lab��ʽ��ͼ���ļ�*/
	IplImage *h_l_plane;/**<Lab��ʽ�У�L���������ͼ���ļ�*/
	IplImage *s_a_plane;/**<Lab��ʽ�У�A���������ͼ���ļ�*/
	IplImage *v_b_plane;/**<Lab��ʽ�У�B���������ͼ���ļ�*/

						/**************�׾����м����*****************/
	UINT32 *Cui_Matrix_E;/**<���ھ���*/
	double *Cui_Matrix_D;/**<�Ⱦ���*/
	double *Cui_Matrix_W;/**<���ƾ���W*/
	double *Cui_Matrix_L;/**<������˹����*/
	float *Cui_MatrixEigenVector_L;/**<L�����N������ֵ��������ֵѭ������*/
	double *CUi_MatrixEigenValue_L;/**<L�����N������ֵ,��˳������*/
	float *Cui_Matrix_Category_Simple;/**<ǰK������ֵ�洢�ľ���*/
	INT32 *Cui_Matrix_Category_Lable;/**<Labeͼ�����ķ������ */
									 /*******************************/
	int					m_K;
public:
	double Cui_Find_MaxSimilar(void);
	void Cui_Min_Cluster(void);
	void Cui_SurroundClassification(void);

	void InitParameter(void);
	void ExportMemData(void);
	//////////////////////////////////////////////////////
	void showDistance(vector<double> clustersize);
	
	void GetLABXYSeeds_ForGivenStepSize_Rectangle(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		const int&					K,
		const bool&					perturbseeds,
		const vector<double>&		edgemag);
	void GetLABXYSeeds_ForGivenStepSize_Rectangle2(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		int&					    NumLabels,
		const bool&					perturbseeds,
		const vector<double>&		edgemag,
		double*  m_lvec,
		double*  m_avec,
		double*  m_bvec);
public:

	unsigned int GetGCD(unsigned int a, unsigned int b);
	void FillImgWithSeeds(
		vector<double> kseedsl,
		vector<double> kseedsa,
		vector<double> kseedsb,
		vector<double> kseedsx,
		vector<double> kseedsy);
	void Clustering_ByHistogramOne(void);
	void Clustering_ByHistogramOneColorGray(void);
	void HistogramOne2Matrix_Category_Lable(INT32 *Matrix_Category_Lable, KseedsHistData& seeddata);
	void HistogramOne2Matrix_Category_Lable(INT32 *Matrix_Category_Lable, KseedsHistColorGray& seeddata);
	
	void HistogramRange2Matrix_Category_Lable(
		INT32 *Matrix_Category_Lable,
		INT32 *Matrix_Category_SP_USED,
		KseedsHistData& seeddata,
		int ColorangleSpan);

	void HistogramRange2Matrix_Category_Lable_SameLink(
		INT32 *Matrix_Category_Lable,
		KseedsHistData& seeddata,
		vector<vector<int>>& SameClassLink,
		int ColorangleSpan);

	

};

//#endif // !defined(_SLIC_H_INCLUDED_)
