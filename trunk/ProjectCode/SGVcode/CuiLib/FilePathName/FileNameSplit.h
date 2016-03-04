#pragma once
using namespace std;
#include <vector>
#include "opencv2/opencv.hpp"
#include "cv.h"
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;
using namespace std;
/*---------------------------------------------------*/
/**
*FileNameSplit �ֽ�ȫ·���ַ���
*@note ʹ��Parse�ֽ�\n
*��������������
*/
/*---------------------------------------------------*/
class FileNameSplit
{
public:
	CString  filepath;/**<�ļ�·��*/
	CString  filefullpath;/**<�ļ�·��+�ļ���+��չ��*/
	CString filename;/**<�ļ���+��չ��*/
	CString filefullname;/**<�ļ�ȫ��*/
	CString filetruename;/**<ȥ������ַ������ļ���*/
	CString  ImagePath;/**<ͼ��·��*/
	CString  XmlCfgPath;/**<Xml�����ļ�·��*/
	CString  XmlFileName;/**<Xml�ļ���*/
	CString  XmlSavePath;/**<Xml����·��*/
	vector<CvPoint> line_point;/**<ֱ���ϵĵ㼯��*/
public:
	 FileNameSplit(void);
	~ FileNameSplit(void);
	CString GetFileTitleFromFileName(CString FileName, BOOL Ext);   
	CString GetFileName(CString pathname);
	CString GetPath(CString pathname);
	void Parse(CString filepath_t);
	void ReadData2Mem(CString filepath_t);
	void clear(void);
public:
static string toLowerCase(const string& in);
static void getFilesInDirectory(const string& dirName, vector<string>& fileNames, const vector<string>& validExtensions);
static	string  ConvertCS2string(CString cstring);
static	CString Convertstring2CS(string cstring);
	int GetXmlCfgData(CString filename);
	void SaveImgFile();
	CvFileNode* IsNodExist(CvFileStorage *);
	void CalculatePointOnImg(void);
	void SaveImgFile2(void);
	void SaveImgandParam(void);
};
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
