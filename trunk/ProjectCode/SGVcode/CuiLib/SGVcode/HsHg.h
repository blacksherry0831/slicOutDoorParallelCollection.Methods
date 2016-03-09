#pragma once
#define  UseVerticalTop FALSE
#define  UseVerticalSpTopAvg TRUE
/*-------------------------------------------------------------------*/
/**
*计算室内地面分割位置、天花板分割位置
*/
/*-------------------------------------------------------------------*/
class HgHsLinePos
{
public:
	HgHsLinePos(){
		this->PgOffset=0;
		this->PsOffset=0;
		this->SkyGndCategoryChange=false;
	}
	HgHsLinePos(double PsOffset,
		        double PgOffset,
	            bool SkyGndCategoryChange){
		this->PgOffset=PgOffset;
		this->PsOffset=PsOffset;
		this->SkyGndCategoryChange=SkyGndCategoryChange;
	}
public:
	double PsOffset;/**<天花板分割位置*/
	double PgOffset;/**<地面分割位置*/
	bool  SkyGndCategoryChange;/**<分割线位置改变时，超像素块是否有图块发生变化*/
};
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/
class HsHg
{
private:
	ImageMemData* pMD;/**<计算用的公共变量和中间内存*/
	double CameraHeight;/**<摄像头高度*/
	double CameraOffAngle;/**<摄像头上下偏角*/
	double RoomLength;/**<房间盒子长度*/
	double RoomHeight;/**<房间盒子高度*/
	double ImgFocus;/**<图像焦距，像素级*/
	double ImgHeight;/**<图像高度*/
	double RoomHeight2Top;/**<摄像机到房顶的距离*/
	double PgOffset_Last;/**<上次地面分界线位置*/
	double PsOffset_Last;/**<上次天花板分界线位置*/
	std::shared_ptr<INT32>  Matrix_Category_Lable_SkyBoder;/**<处于天空分界线处的图块标号*/
	std::shared_ptr<INT32>  Matrix_Category_Lable_GndBoder;/**<处于地面分界线处的图块标号*/
	vector<HgHsLinePos>  HgHsLine;/**<记录历次边界线变化的结果*/
public:
	HsHg(ImageMemData* pMD_t);
	~HsHg(void);
public:
	
	double GetHs_InDoor_Geometry(double DeflectionAngle);
	double GetHg_InDoor_Geometry(double DeflectionAngle);	
	double GetHs_ByIteration(void);
	double GetHg_ByIteration(void);
	void  Get_HsHg_ByIteration(void);
	void  Get_HsHg_ByIterationNoGeometric(void);
	void  Get_HsHg_By_NBig_FSmall_5m4d(void);
	bool  IsConvergence(void);
	bool  IsConvergence_5m4d(void);
	bool  IsBorderLineConvergence_5m4d(void);
	bool  IsBoderCategoryChange_5m4d(void);
	void RecordBoderCategory_5m4d(void);
	void CheackBoderCategory_5m4d(void);
	void  Get_HsHg_By_SV_VG(void);
private:
	double GetHs_InDoor(double DeflectionAngle);
	double GetHg_InDoor(double DeflectionAngle);

	void  ForceGround_InDoor_DFS(void);
	void  ForceSky_InDoor_DFS(void);

	void  DFS_4Ground2Up(int spi);

	void  InitDFSLinkConnection_Ground(void);
	void  InitDFSLinkConnection_Sky(void);
	void  DFS_4Sky2Down(int spi);
	void  StatisticsTrapezoidHgHs_5m4d(vector<double>& SkyY,vector<double>& GndY);

	double GetHs_Nogeometry(void);
	double GetHg_Nogeometry(void);
};
/*-------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------*/

