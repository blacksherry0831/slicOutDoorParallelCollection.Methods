#pragma once
#include <string>
#include <iostream>
using namespace std;

#ifndef UINT32
typedef  unsigned int UINT32;
#endif

#ifndef INT32
typedef signed int INT32;
#endif

/*----------------------------------------------------------------*/
#define SaveContours2Disk TRUE
/*----------------------------------------------------------------*/
/**
*本类包含了图像处理算法中一些通用的数据结构\n
*和运算用到的中间变量
*
*/
/*----------------------------------------------------------------*/
class ImageData
{
public:
	ImageData(
		string filename,
		string filesavepath,
		int spcount,
		double compactness,
		double horizontal_line_pos);
	ImageData(
		string filename,
		string filesavepath,
		int spcount,
		double horizontal_line_pos);
	ImageData(
		IplImage* img,
		string filesavepath="",
		int spcount=800,
		double compactness=8,
		double horizontal_line_pos=0.5);
	~ImageData(void);
	void ReleaseMemory(void);
	void initParam(void);
	void SetImageData(string filename,string filesavepath);
	void SetSlicParameter(int spcount,double compactness);
	void InitMemoryData(
		IplImage* img,
		string filename,
		string filesavepath,
		int spcount,
		double compactness);
public:
	string FileReadFullPath;
	string FileWritePath;
public:
	int ImgWidth;
	int ImgHeight;
public:
	int   Seg_HorizontalLinePos;/**<设置视平线位置*/
	double Seg_HorizontalLinePosScale;/**<*/
	double PgOffset;/**<*/
	double PsOffset;/**<*/
public:
	int slic_expect_num;
	int slic_current_num;
	double slic_compactness;
public:
	int* src_ImgLabels;
	int* src_ImgLabelsSVG;
	UINT32* src_ImgBGRA;
	IplImage* srcCv_ImgBGRA;
	UINT32* Matrix_E;
public:
	  double*  pYweight_S;
	  double*  pYweight_V;
	  double*  pYweight_G;
	  double*  SkyWeightSp_host;
	  double*  VerticalWeightSp_host;
	  double*  GroundWeightSp_host;
public:
	double*		m_lvec;//(0,100)
	double*		m_avec;//(-128,127)
	double*		m_bvec;//(-128,127)
	double*     sita_n;
	double*     m_n;
	double*     L_n;
	double*     X_n;
	double*     Y_n;
	vector<double> kseedsl;
	vector<double> kseedsa;
	vector<double> kseedsb;
	vector<double> kseedsx;
	vector<double> kseedsy;
	vector<double> kseedsTheta;
	vector<double> kseedsM;
	vector<double> kseedsL;
	vector<double> kseedsX;
	vector<double> kseedsY;
	double alpha;
	double betta;
	double gama;
	double fai;
public:
	int InitTimes;
	int InitMemDataTimes;
	void initSpSet(void);
	void initLabels(int* labels);
	void initMemData(int* labels);
	SpSetProperty SpSet;
private:
	unsigned int _seq;
public:
	void ImageGetSeedsLabxy_cuda();
	void ImageGetSeedsThetaML_cuda();
    void DoRGBtoLABConversion(
		unsigned int*		ubuff,
		double*					lvec,
		double*					avec,
		double*					bvec);
	void RGB2LAB(
		const int& sR, const int& sG, const int& sB, 
		double& lval, double& aval, double& bval);
	void RGB2XYZ(
		const int&		sR,
		const int&		sG,
		const int&		sB,
		double&			X,
		double&			Y,
		double&			Z);
inline	void ConvertLab2oml(
		double L,
		double A,
		double B,
		double X,
		double Y,
		double& sita_n,
		double& m_n,
		double& L_n,
		double& X_n,
		double& Y_n);
void initThetaMLXY(void);
void GetMatrixE(void);
void SetImgLabels(void);
unsigned int GetGCD(unsigned int a, unsigned int b);
void GetThetaMLXYSeeds_ForGivenStepSize_Rectangle(
	const bool&					perturbseeds,
	const vector<double>&		edgemag);
void Combine2SPto1(void);
void Combination_ImgLabs(
	vector<vector<int>> simlarSp,
	bool UseMatrixE=false,
	bool UseMaxtrxW=false,
	bool UseEnergy=false);
///////////////////////////////////////////////
void SaveImgWithContours(string str_add="");
void SaveImgWithPointsFuzzy(string str_add="");
void SaveImgWithPointsCompute(string str_add="");
void SaveImgSVGCompute(string str_add="");
///////////////////////////////////////////////
void SurroundClassification(void);
void InSideClusteringByopencv(void);
void InitAllSpBlockEnergy(void);
void CalculateAllSpPropertyRange(void);
void CalculateAllSpBlockEnergy(void);
void CalculateAllSpBlockEnergy2(void);
void CalculateSpBlockEnergy2(int sp);
void InitAllSpBlockEnergy2(void);
void CalculateAllSpBlockEnergyRank(int RankNum);
void FillWeightArrayZlmParallel(void);
void CombinationImgSVG(void);
void FillHoleOnSVGLables(
	bool sky,
	bool vertical,
	bool ground,
	bool BorderSky,
	bool BorderGnd);
void DrawS_V_G_Lables_BorderLine(IplImage *img,UINT32 category);
void ImageData::SaveSuperpixelLabelsImagePNG(
	INT32*					labels,
	const int					width,
	const int					height,
	const string				filename,
	const string				path);
void ImageData::SaveSuperpixelLabelsImagePNG();
};

