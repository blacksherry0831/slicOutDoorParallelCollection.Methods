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
*���������ͼ�����㷨��һЩͨ�õ����ݽṹ\n
*�������õ����м����
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
	 bool is_allcoate_mem;/**<�Ƿ��ѷ����ڴ�ı�־λ*/
public:
int   Seg_HorizontalLinePos;/**<������ƽ��λ��*/
double Seg_HorizontalLinePosScale;/**<*/
double PgOffset;/**<*/
double PsOffset;/**<*/
SP_PROPERTY *p_SpProperty;/**<�����������Ե����ݽṹ*/

/*----------------------------------------*/
    IplImage *src_Img_bgra;/**<[a r g b]��ʾ��ͼ�����*/
    UINT32  *Src_ImgData;/**<[a r g b]��ʾ��ͼ�����*/
    int *src_ImgLabels;/**<�����ر궨����*/
    int ImgWidth;/**<������*/
	int ImgHeight;/**<����߶�*/
	////////////////
	INT32 *p_ImgLables_SVG;/**<��ա����桢����ı궨����*/
	std::shared_ptr<int> ImgLables;/**<�����ر궨����*/
	////////////////
	std::shared_ptr<UINT32>Matrix_E;/**<E����*/
	std::shared_ptr<double>Matrix_D;/**<D����*/
	double *Matrix_W_Color;/**<��ɫ���ƾ���*/
	double *Matrix_W_Vein;/**<�������ƾ���*/
	std::shared_ptr<double> Matrix_L;/**<L������˹����*/

	std::shared_ptr<UINT32> Matrix_E_InDoor;/**<E����*/
	std::shared_ptr<double> Matrix_D_InDoor;/**<D����*/
	std::shared_ptr<double> Matrix_W_InDoor;/**<D����*/
	std::shared_ptr<double> Matrix_L_InDoor;/**<L������˹����*/
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
	IplImage *l_plane;/**<Lab��ʽ�У�L���������ͼ���ļ�*/
	IplImage *a_plane;/**<Lab��ʽ�У�A���������ͼ���ļ�*/
	IplImage *b_plane;/**<Lab��ʽ�У�B���������ͼ���ļ�*/
	IplImage *src_Img_hsv;
	IplImage *h_plane;/**<Lab��ʽ�У�L���������ͼ���ļ�*/
	IplImage *s_plane;/**<Lab��ʽ�У�A���������ͼ���ļ�*/
	IplImage *v_plane;/**<Lab��ʽ�У�B���������ͼ���ļ�*/
public:
/*----------------------------------------*/
	string FileReadFullPath;/**<ԭͼ��λ��*/
	string FileWritePath;/**<������ļ�λ��*/
/*----------------------------------------*/
public:
/*----------------------------------------*/
    int slic_current_num;/**<��ǰ�ĳ����ظ���*/
	int slic_spcount;/**<�����ĳ����ظ���*/
	double slic_compactness;/**<��������ʱ���ܶ�����*/
/*----------------------------------------*/
public:
	std::shared_ptr<UINT32> ImgData_Contours;
public:
	LINEDATA *LineContour;/**<�������Ͻ���Hough�任�õ���ֱ��*/
	int   LineContourLen;/**<������ֱ�ߵ�����*/

	LINEDATA *LineSource;/**<�������Ͻ���Hough�任�õ���ֱ��*/
	int   LineSourceLen;/**<������ֱ�ߵ�����*/

	HistData  hist_contours;/*ȫ������ֱ��ͼ*/
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

