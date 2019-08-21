#pragma once
#if !defined(SLIC_LAB_CUDA_INT_H)
#define SLIC_LAB_CUDA_INT_H
/*----------------------------------------------------------------------------------------------------------------*/
#include "cpp_basic.h"
#include "cpp_def.h"
#include "cpp_def_debug.h"
/*----------------------------------------------------------------------------------------------------------------*/
/**
*  
*  本类完成超像素和谱聚类的功能
*
*/
/*----------------------------------------------------------------------------------------------------------------*/
class SLIC_LAB_CUDA_INT
{
public:
	SLIC_LAB_CUDA_INT(
		IplImage* _img,
		std::string _SavePath = "");

	virtual ~SLIC_LAB_CUDA_INT();
public:
	std::string mSavePath;
private:
	int			mSZ;
	int			mWidth;
	int			mHeight;
	IplImage*	mImgPtr;
private:
	vector<int> kseedsl;
	vector<int> kseedsa;
	vector<int> kseedsb;
	vector<int> kseedsx;
	vector<int> kseedsy;
private:
	int*	mLvPtr;
	int*	mAvPtr;
	int*	mBvPtr;
private:
	INT32*	m_labels_slic;
	INT32*	m_labels_connectivity;
	std::vector<std::list<CvPoint>> mSuperPixel;
private:
	int		m_slic_num;
	int		m_slic_M;
public:

	void initLabPtr(
			 int* const	_l,
			 int* const	_a,
			 int* const	_b);
	
	void initSlic(
			const int _num,
			const float _M);

private:
	void initMem();
	void createMem(const int _s);
	void freeMem();
public:
	void DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_LAB_CUDA();

	void DoSuperpixelSegmentation_ForGivenSuperpixelSize_LAB_CUDA();

	void EnforceLabelConnectivityEx(
		int*	 klabels,
		int*	nklabels,
		const int width,
		const int height,
		int & numlabels,
		const int & K);

	void EnforceLabelConnectivity(
		const int * labels,
		const int width,
		const int height,
		int *& nlabels,
		int & numlabels,
		const int & K);

	void EnforceLabelConnectivity_List(
		const int * labels,
		const int width,
		const int height,
		int *& nlabels,
		int & numlabels,
		const int & K);

	void GetLABXYSeeds_ForGivenStepSize_Rectangle2(
		vector<int>& kseedsl,
		vector<int>& kseedsa,
		vector<int>& kseedsb,
		vector<int>& kseedsx,
		vector<int>& kseedsy,
		int & NumLabels,
		const bool & perturbseeds,
		const vector<double>& edgemag,
		int * m_lvec,
		int * m_avec,
		int * m_bvec);

	unsigned int GetGCD(
		unsigned int a,
		unsigned int b);

	void PerturbSeeds(
		vector<int>& kseedsl,
		vector<int>& kseedsa,
		vector<int>& kseedsb,
		vector<int>& kseedsx,
		vector<int>& kseedsy,
		const vector<double>& edges);
	
public:
	void testListSize();


};

#endif
