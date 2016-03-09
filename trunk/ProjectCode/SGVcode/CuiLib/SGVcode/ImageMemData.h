#pragma once
#include "ImageType.h"
#include <string>
#include <iostream>
#include "ManhattanHistogram.h"
/*----------------------------------------------------------------*/
/*----------------------------------------------------------------*/
using namespace std;
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/

#ifdef _MSC_VER
#pragma warning (error: 4482)
#endif 
/*----------------------------------------------------------------*/
/**
*本类包含了图像处理算法中一些通用的数据结构\n
*和运算用到的中间变量
*
*/
/*----------------------------------------------------------------*/
class ImageMemData
{
public:
	ImageMemData(
		string filename,
		string filesavepath,
		int spcount,
		double compactness,
		double horizontal_line_pos);
	ImageMemData(
		IplImage* img,
		string filesavepath="",
		int spcount=800,
		double compactness=8,
		double horizontal_line_pos=0.5);
	~ImageMemData(void);
	void ReleaseMemory(void);
private:
	 bool is_allcoate_mem;/**<是否已分批内存的标志位*/
public:
int   Seg_HorizontalLinePos;/**<设置视平线位置*/
double Seg_HorizontalLinePosScale;/**<*/
double PgOffset;/**<*/
double PsOffset;/**<*/
SP_PROPERTY *p_SpProperty;/**<代表超像素属性的数据结构*/

/*----------------------------------------*/
    IplImage *src_Img_bgra;/**<[a r g b]表示的图像矩阵*/
    UINT32  *Src_ImgData;/**<[a r g b]表示的图像矩阵*/
    int *src_ImgLabels;/**<超像素标定矩阵*/
    int ImgWidth;/**<矩阵宽度*/
	int ImgHeight;/**<矩阵高度*/
	////////////////
	INT32 *p_ImgLables_SVG;/**<天空、立面、地面的标定矩阵*/
	std::shared_ptr<int> ImgLables;/**<超像素标定矩阵*/
	////////////////
	std::shared_ptr<UINT32>Matrix_E;/**<E矩阵*/
	std::shared_ptr<double>Matrix_D;/**<D矩阵*/
	double *Matrix_W_Color;/**<颜色相似矩阵*/
	double *Matrix_W_Vein;/**<纹理相似矩阵*/
	std::shared_ptr<double> Matrix_L;/**<L拉普拉斯矩阵*/

	std::shared_ptr<UINT32> Matrix_E_InDoor;/**<E矩阵*/
	std::shared_ptr<double> Matrix_D_InDoor;/**<D矩阵*/
	std::shared_ptr<double> Matrix_W_InDoor;/**<D矩阵*/
	std::shared_ptr<double> Matrix_L_InDoor;/**<L拉普拉斯矩阵*/
	std::shared_ptr<double>  MatrixEigenVector_L_InDoor;
	std::shared_ptr<double> MatrixEigenValue_L_InDoor;
	std::shared_ptr<INT32>  Matrix_Category_Lable;
    std::shared_ptr<INT32>  Matrix_Category_Lable_InDoor;
	std::shared_ptr<double>  Matrix_Category_Simple_InDoor;
	std::shared_ptr<HistData180> Histogram180;

	std::shared_ptr<UINT32> Matrix_Visit;
	std::shared_ptr<UINT32> LinkConnection;
	std::shared_ptr<INT32> PositionCategoryForce;
public:
	IplImage *src_ImgMaskLables;
	IplImage *src_Img_lab;
	IplImage *l_plane;/**<Lab格式中，L代表分量的图像文件*/
	IplImage *a_plane;/**<Lab格式中，A代表分量的图像文件*/
	IplImage *b_plane;/**<Lab格式中，B代表分量的图像文件*/
	IplImage *src_Img_hsv;
	IplImage *h_plane;/**<Lab格式中，L代表分量的图像文件*/
	IplImage *s_plane;/**<Lab格式中，A代表分量的图像文件*/
	IplImage *v_plane;/**<Lab格式中，B代表分量的图像文件*/
public:
/*----------------------------------------*/
	string FileReadFullPath;/**<原图的位置*/
	string FileWritePath;/**<保存的文件位置*/
/*----------------------------------------*/
public:
/*----------------------------------------*/
    int slic_current_num;/**<当前的超像素个数*/
	int slic_spcount;/**<期望的超像素个数*/
	double slic_compactness;/**<做超像素时的密度因子*/
/*----------------------------------------*/
public:
	std::shared_ptr<UINT32> ImgData_Contours;
public:
	LINEDATA *LineContour;/**<在轮廓上进行Hough变换得到的直线*/
	int   LineContourLen;/**<轮廓上直线的数量*/

	LINEDATA *LineSource;/**<在轮廓上进行Hough变换得到的直线*/
	int   LineSourceLen;/**<轮廓上直线的数量*/

	HistData  hist_contours;/*全局轮廓直方图*/
private:
	void AllocaeMemoryByspNumber(int spnumber);
public:
	void SplitImgBGRALab(void);
	void SplitImgBGRAHsv(void);
	void SetImgMaskLables(IplImage *lables);
public:
	void SetImageData(string filename,string filesavepath);
	
    void InitMemoryData(
			IplImage* img,
			string filename,
			string filesavepath,
			int spcount,
			double compactness);
    void initParam(void);
    void AllocateMemRelated2spnumber(int spnumber);
	void AllocateMemRelated2spnumber(void);
	void SetSlicParameter(int spcount,double compactness);

public:
    void ImportMemData(int slic_num,INT32 *p_ImgLables);
	void DrawContours(void);
	double GetHorizontalThresholdLength(void);
	double GetSuperPixelDefaultEdgeLength(void);
	unsigned long GetSpSizeAvg(void);
	void InitSPLabProperty(void);
	void SaveLabProperty(string path,string filename);
	void SaveSpProperty(void);
public:
	void SaveSuperPixelNum();
	void SaveColorSpectralClusteringNum();
	void SaveColorIterationNum();
	void SaveShapeSpectralClusteringNum();
	void SaveFuzzyVerticalNum(int VerticalNum);
	void SaveStripSPVerticalNum(int VerticalNum);
public:
static	void StrReplace(string& src,string sub_old,string sub_new);

};

