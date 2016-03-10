#include "StdAfx.h"
#include "ImageData.h"
/*-------------------------------------------------------------------*/
#if _MSC_VER
#pragma warning(disable: 4101)
#endif
/*----------------------------------------------------------------*/
/**
*���캯��\n
*��������������֮ǰ�����û����Ĳ���
*
*@param  filename  ͼ����ļ���
*@param  filesavepath   �м������·��
*@param  spcount �����ĳ����ظ���
*@param  compactness ��������ʱ���ܶ�����
*@param  horizontal_line_pos  ��ƽ�ߵ�λ��
*/
/*----------------------------------------------------------------*/
ImageData::ImageData(
	string filename,
	string filesavepath,
	int spcount,
	double compactness,
	double horizontal_line_pos)
{
	this->initParam();
	ASSERT(filename!="");
	SetImageData(filename,filesavepath);
	SetSlicParameter(spcount,compactness);	
	InitMemoryData(nullptr,filename,filesavepath,spcount,compactness);
	this->Seg_HorizontalLinePos=horizontal_line_pos*this->ImgHeight;
	this->Seg_HorizontalLinePosScale=horizontal_line_pos;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
ImageData::ImageData(
	IplImage* img,
	string filesavepath,
	int spcount,
	double compactness,
	double horizontal_line_pos)
{
	string filename="MemoryIMG";
	this->initParam();
	SetImageData(filename,filesavepath);
	SetSlicParameter(spcount,compactness);	
	InitMemoryData(img,filename,filesavepath,spcount,compactness);
	this->Seg_HorizontalLinePos=horizontal_line_pos*this->ImgHeight;
	this->Seg_HorizontalLinePosScale=horizontal_line_pos;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
ImageData::ImageData(
	string filename,
	string filesavepath,
	int spcount,
	double horizontal_line_pos)
{
	this->initParam();
	ASSERT(filename!="");
	SetImageData(filename,filesavepath);
	SetSlicParameter(spcount,0);	
	InitMemoryData(nullptr,filename,filesavepath,spcount,0);
	this->Seg_HorizontalLinePos=horizontal_line_pos*this->ImgHeight;
	this->Seg_HorizontalLinePosScale=horizontal_line_pos;
}
/*----------------------------------------------------------------*/
/**
*��ʼ����Ա����
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::initParam(void)
{	
	this->FileReadFullPath="";
	this->FileWritePath="";
	this->ImgHeight=0;
	this->ImgWidth=0;
	this->Seg_HorizontalLinePos=0;
	this->Seg_HorizontalLinePosScale=0;	
	this->slic_expect_num=0;
	this->slic_current_num=0;
	this->slic_compactness=0;
	this->src_ImgLabels=NULL;
	int sise_int=sizeof(int);
	int size_intp=sizeof(int*);
	this->src_ImgBGRA=NULL;
	this->pYweight_G=NULL;
	this->pYweight_S=NULL;
	this->pYweight_V=NULL;
	this->SkyWeightSp_host=NULL;
	this->VerticalWeightSp_host=NULL;
	this->GroundWeightSp_host=NULL;
	this->srcCv_ImgBGRA=NULL;
	m_lvec=NULL;
	m_avec=NULL;
	m_bvec=NULL;
	sita_n=NULL;
	m_n=NULL;
	L_n=NULL;
	X_n=NULL;
	Y_n=NULL;
	this->Matrix_E=NULL;
	this->InitTimes=0;
	src_ImgLabelsSVG=NULL;
	InitMemDataTimes=0;
	PgOffset=0;
	PsOffset=0;
}
/*----------------------------------------------------------------*/
/**
*����ͼ������롢���·��
*
* @param filename      �ļ���
* @param filesavepath  �м��������·��
*
*/
/*----------------------------------------------------------------*/
void ImageData::SetImageData(string filename,string filesavepath){

	
	FileReadFullPath=filename;
	FileWritePath=filesavepath;
	
}
/*----------------------------------------------------------------*/
/**
*���ó���������ʱ�����ĳ����ظ������ܶ�����
*
*@param spcount �����ĳ����ظ���
*@param compactness  �ܶ�����
*
*/
/*----------------------------------------------------------------*/
 void ImageData::SetSlicParameter(int spcount,double compactness){
	 slic_expect_num=spcount;
	 slic_current_num=spcount;
	 slic_compactness=compactness;
 }
 /*----------------------------------------------------------------*/
 /**
 *���������ͷŷ�����ڴ�
 *
 *
 */
 /*----------------------------------------------------------------*/
ImageData::~ImageData()
{
	this->ReleaseMemory();
}
/*----------------------------------------------------------------*/
/**
*�����м���̷�����ڴ�
*
*
*/
/*----------------------------------------------------------------*/

void ImageData::ReleaseMemory(void)
{
	delete[]  m_lvec;
	delete[]  m_avec;
	delete[]  m_bvec;
	//////////////////////
	delete[]    sita_n;
	delete[]    m_n;
	delete[]    L_n;
	delete[]    X_n;
	delete[]    Y_n;
	///////////////////////
	delete[]  src_ImgLabels;
	delete[]  src_ImgLabelsSVG;
	delete[]  src_ImgBGRA;
	delete[]  pYweight_S;
	delete[]  pYweight_V;
	delete[]  pYweight_G;
	///////////////////////////////
	delete[] SkyWeightSp_host;
	delete[] VerticalWeightSp_host;
	delete[] GroundWeightSp_host;
	///////////////////////////////
	delete[]  Matrix_E;
	cvReleaseImage(&srcCv_ImgBGRA); 
}
/*----------------------------------------------------------------*/
/**
*hough�任ʱ�����ˮƽ�ߵ���С����
*
*@return ˮƽ�ߵ���С����
*/
/*----------------------------------------------------------------*/


 /*----------------------------------------------------------------*/
 /**
 *��ʼ���ѷ�����ڴ�
 *
*@param filename  ͼ���ļ���
*@param filesavepath ͼ�񱣴�·��
*@param spcount   �����ظ���
*@param compactness �ܶ�����
 */
 /*----------------------------------------------------------------*/
 void ImageData::InitMemoryData(
	 IplImage* img,
	 string filename,
	 string filesavepath,
	 int spcount,
	 double compactness)
 {  
	
	 ReleaseMemory();
	 /*************************************************************************************************/ 
	
	 IplImage *src_img_t;
	if (img==nullptr){
		 src_img_t=cvLoadImage(filename.c_str(),CV_LOAD_IMAGE_UNCHANGED); 
	}else{
		src_img_t=cvCreateImage(cvGetSize(img),img->depth,4);
		if (img->nChannels==4){
			 cvCopyImage(img,src_img_t);
		}else if (img->nChannels==3){
			cvCvtColor(img,src_img_t,CV_BGR2BGRA);
		}
		else if (img->nChannels==1){
			cvCvtColor(img,src_img_t,CV_GRAY2BGRA);
		}else{
			ASSERT(0); 
		}
	   
	}	
	 cui_GeneralImgProcess::ConvertImg2Eighth4Ch(&src_img_t);

	 srcCv_ImgBGRA=cvCloneImage(src_img_t);
	 cui_GeneralImgProcess::ConvertImg3ChTo4Ch(&srcCv_ImgBGRA);

	
	
	 cvReleaseImage(&src_img_t);
	 ImgWidth=srcCv_ImgBGRA->width;
	 ImgHeight=srcCv_ImgBGRA->height;
	 int sz=ImgHeight*ImgWidth;
	 /*********************************************************************/
	   src_ImgBGRA=new UINT32[sz];
	   ASSERT(srcCv_ImgBGRA->widthStep==sizeof(UINT32)*ImgWidth);
	   memcpy(src_ImgBGRA,srcCv_ImgBGRA->imageData,sizeof(UINT32)*sz);
	 /*********************************************************************/
	   src_ImgLabels=new int[sz];
	   for( int s = 0; s < sz; s++ ) 
		   src_ImgLabels[s] = -1;
	 /*********************************************************************/
	   src_ImgLabelsSVG=new int[sz];
	  /*********************************************************************/
	   m_lvec=new double[sz];
	   m_avec=new double[sz];
	   m_bvec=new double[sz];
	   DoRGBtoLABConversion(src_ImgBGRA, m_lvec, m_avec, m_bvec);
	 /*********************************************************************/
	   sita_n=new double[sz];
	   m_n=new double[sz];
	   L_n=new double[sz];
	   X_n=new double[sz];
	   Y_n=new double[sz];
	   initThetaMLXY();
	   /*********************************************************************/
	   //Matrix_E=new UINT32[sz];
	   /*********************************************************************/
	   //this->FillWeightArrayZlmParallel();
	   this->pYweight_G=new double[ImgHeight];
	   this->pYweight_S=new double[ImgHeight];
	   this->pYweight_V=new double[ImgHeight];
	   /*********************************************************************/

 }
/*----------------------------------------------------------------*/
/**
*�ͷ��볬���ظ�����ص������ڴ�
*
*@param spnumber �����ظ���
*/
/*----------------------------------------------------------------*/
void ImageData::initSpSet(void)
{
#if 0
	static bool doOnce=true;
	ASSERT(doOnce==true);
	if (doOnce){
		this->SpSet.Parse(
			src_ImgLabels,
			slic_current_num,
			ImgWidth,
			ImgHeight,
			Seg_HorizontalLinePos);
		this->InitAllSpBlockEnergy();
		doOnce=false;
	}
#endif
	this->SpSet.Parse(
		src_ImgLabels,
		slic_current_num,
		ImgWidth,
		ImgHeight,
		Seg_HorizontalLinePos);
	this->CalculateAllSpBlockEnergy2();
	InitTimes++;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::initLabels(int* labels)
{
	if (labels==src_ImgLabels){
		return;
	}
  memcpy(src_ImgLabels,labels,sizeof(int)*ImgWidth*ImgHeight);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::initMemData(int*ImgLabels)
{	
	InitMemDataTimes++;
	if (InitMemDataTimes==1){
		Matrix_E=new UINT32[slic_current_num*slic_current_num];
		this->initLabels(ImgLabels);
		this->initSpSet();
		this->SkyWeightSp_host=new double[slic_current_num];
		this->VerticalWeightSp_host=new double[slic_current_num];
		this->GroundWeightSp_host=new double[slic_current_num];
	}	
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData:: ImageGetSeedsLabxy_cuda(void)
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
	/*---------------------------------------------------*/
	{
		kseedsl.clear();
		kseedsa.clear();
		kseedsb.clear();
		kseedsx.clear();
		kseedsy.clear();
		kseedsl.resize(slic_current_num);
		kseedsa.resize(slic_current_num);
		kseedsb.resize(slic_current_num);
		kseedsx.resize(slic_current_num);
		kseedsy.resize(slic_current_num);
		GetSeedsLabxy_cuda(
			slic_current_num,
			ImgWidth,
			ImgHeight,
			m_lvec,
			m_avec,
			m_bvec,
			kseedsl.data(),
			kseedsa.data(),
			kseedsb.data(),
			kseedsx.data(),
			kseedsy.data(),
			src_ImgLabels);
	}
	/*---------------------------------------------------*/
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
	TRACE("\n ͳ�Ƴ��������ĵ�ʱ��: %f���룩",dfTim);

}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData:: ImageGetSeedsThetaML_cuda(void)
{
	
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::GetMatrixE(void)
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
	/*---------------------------------------------------*/
	cui_GeneralImgProcess::CuiSetNighbour_E_matrix(
		src_ImgLabels,
		slic_current_num,
		Matrix_E,
		ImgWidth,
		ImgHeight,
		"Compute_matrix.matrix",
		"");
	/*---------------------------------------------------*/
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
	TRACE("\n ���Ӿ���E: %f���룩",dfTim);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::Combine2SPto1(void)
{
	;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
//===========================================================================
///	DoRGBtoLABConversion
///
///	For whole image: overlaoded doubleing point version
//===========================================================================
void ImageData::DoRGBtoLABConversion(
	unsigned int*		ubuff,
	double*					lvec,
	double*					avec,
	double*					bvec)
{
	int sz = ImgWidth*ImgHeight;
	/*lvec = new double[sz];
	avec = new double[sz];
	bvec = new double[sz];*/
	/*r:0--255*/
	/*g:0--255*/
	/*b:0--255*/
	for( int j = 0; j < sz; j++ )
	{
		int r = (ubuff[j] >> 16) & 0xFF;
		int g = (ubuff[j] >>  8) & 0xFF;
		int b = (ubuff[j]      ) & 0xFF;
		//////////////////////////////////
		// a  r      g      b
		//////////////////////////////////
		RGB2LAB( r, g, b, lvec[j], avec[j], bvec[j] );
		/*assert();*/
	}
}
//===========================================================================
///	RGB2LAB
//===========================================================================
void ImageData::RGB2LAB(const int& sR, const int& sG, const int& sB, double& lval, double& aval, double& bval)
{
	//------------------------
	// sRGB to XYZ conversion
	//------------------------
	double X, Y, Z;
	RGB2XYZ(sR, sG, sB, X, Y, Z);

	//------------------------
	// XYZ to LAB conversion
	//------------------------
	double epsilon = 0.008856;	//actual CIE standard
	double kappa   = 903.3;		//actual CIE standard

	double Xr = 0.950456;	//reference white
	double Yr = 1.0;		//reference white
	double Zr = 1.088754;	//reference white

	double xr = X/Xr;
	double yr = Y/Yr;
	double zr = Z/Zr;

	double fx, fy, fz;
	if(xr > epsilon)	fx = pow(xr, 1.0/3.0);
	else				fx = (kappa*xr + 16.0)/116.0;
	if(yr > epsilon)	fy = pow(yr, 1.0/3.0);
	else				fy = (kappa*yr + 16.0)/116.0;
	if(zr > epsilon)	fz = pow(zr, 1.0/3.0);
	else				fz = (kappa*zr + 16.0)/116.0;

	lval = 116.0*fy-16.0;
	aval = 500.0*(fx-fy);
	bval = 200.0*(fy-fz);
}
//==============================================================================
///	RGB2XYZ
///
/// sRGB (D65 illuninant assumption) to XYZ conversion
//==============================================================================
void ImageData::RGB2XYZ(
	const int&		sR,
	const int&		sG,
	const int&		sB,
	double&			X,
	double&			Y,
	double&			Z)
{
	double R = sR/255.0;
	double G = sG/255.0;
	double B = sB/255.0;

	double r, g, b;

	if(R <= 0.04045)	r = R/12.92;
	else				r = pow((R+0.055)/1.055,2.4);
	if(G <= 0.04045)	g = G/12.92;
	else				g = pow((G+0.055)/1.055,2.4);
	if(B <= 0.04045)	b = B/12.92;
	else				b = pow((B+0.055)/1.055,2.4);

	X = r*0.4124564 + g*0.3575761 + b*0.1804375;
	Y = r*0.2126729 + g*0.7151522 + b*0.0721750;
	Z = r*0.0193339 + g*0.1191920 + b*0.9503041;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
inline void ImageData::ConvertLab2oml(
	double L,
	double A,
	double B,
	double X,
	double Y,
	double& sita_n,
	double& m_n,
	double& L_n,
	double& X_n,
	double& Y_n)
{
	assert(L>=0&&L<=100+1);
	assert(A>=-128&&A<=128+1);
	assert(B>=-128&&B<=128+1);
	assert(X>=0&&X<=ImgWidth+1);
	assert(Y>=0&&Y<=ImgHeight+1);
#if 1
	///////////////////////////////////////
	sita_n=atan2(A,B);
	sita_n=sita_n/(2*3.1415927)+0.5;
	///////////////////////////////////////
	m_n=sqrt(A*A+B*B);
	m_n=m_n/(128*1.415);
	///////////////////////////////////////
	L_n=L/100;
	assert(L_n>=0&&L_n<=1+1E-1);
	///////////////////////////////////////
	X_n=X/ImgWidth;
	assert(X_n>=0&&X_n<=1+1E-1);
	Y_n=Y/ImgHeight;
	assert(Y_n>=0&&Y_n<=1+1E-1);
#endif
	assert(sita_n>=0-0.1&&sita_n<=1+0.1);
	assert(m_n>=0-0.1&&m_n<=1+0.1);
	assert(L_n>=0-0.1&&L_n<=1+0.1);
	assert(X_n>=0-0.1&&X_n<=1+0.1);
	assert(Y_n>=0-0.1&&Y_n<=1+0.1);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::initThetaMLXY(void)
{
	for (int x=0;x<ImgWidth;x++){
		for (int y=0;y<ImgHeight;y++){
			int Idx=y*ImgWidth+x;
			ConvertLab2oml(m_lvec[Idx],m_avec[Idx],m_bvec[Idx],x,y,
				sita_n[Idx],m_n[Idx],L_n[Idx],X_n[Idx],Y_n[Idx]);
		}
	}
}
/*---------------------------------------------------------------------------------*/
/**
*��ȡ��ʼ���������ӣ�����Ϊ��Ȼ����ƽ�������Ծ���ʽ���ȷֲ�����
*
*@param 	[iout]	kseedsl ����L����
*@param 	[out]	kseedsa ����A����
*@param 	[out]	kseedsb ����B����
*@param 	[out]	kseedsx ����X�����
*@param 	[out]	kseedsy ����Y�����
*@param 	[in,out]	K       ������������
*@param 	[in,out]	perturbseeds δʹ��
*@param 	[in,out]    edgemag δʹ��
*
*/
/*---------------------------------------------------------------------------------*/
void ImageData::GetThetaMLXYSeeds_ForGivenStepSize_Rectangle(
	const bool&					perturbseeds,
	const vector<double>&		edgemag)
{
	double xStep;
	double yStep;
	const bool hexgrid = false;
	int numseeds(0);
	int n(0);
	int K=slic_expect_num;
	bool SeedMethod=false;
#if FALSE
	int max_wh=(ImgWidth>ImgHeight)?ImgWidth:ImgHeight; 
	int min_wh=(ImgWidth<ImgHeight)?ImgWidth:ImgHeight;
	double scale_wh=1.0*min_wh/max_wh;
	unsigned int wh_gcd=GetGCD(ImgWidth,ImgHeight);
	double scale_3_2=1.0*2/3;
	if (scale_wh==scale_3_2){
		int K_new=(K/12+0.5)*12;
		if (ImgWidth>ImgHeight){
			xStep=sqrtl(ImgHeight*ImgWidth*4/(12*3));
			yStep=3.0/4.0*xStep;
		}else{
			yStep=sqrtl(ImgHeight*ImgWidth*4/(12*3));
			xStep=3.0/4.0*yStep;
		}
	}else{
		ASSERT(FALSE);
	}
#else
	unsigned int wh_gcd=GetGCD(ImgWidth,ImgHeight);
	int width_min=ImgWidth/wh_gcd;
	int height_min=ImgHeight/wh_gcd;
	int block_min=width_min*height_min;
	double  wh_scale=1.0*ImgWidth/ImgHeight;
	{
		/*int K_new=((int)(K/block_min+0.5))*block_min;*/
		yStep=sqrtl(ImgHeight*ImgHeight/K);
		int K_new=(ImgHeight*ImgHeight)/(yStep*yStep);
		{
			/*yStep=sqrtl(1.0*ImgHeight*ImgWidth/(K_new*wh_scale));*/
			xStep=wh_scale*yStep;
		}
	}
#endif	

	double xstrips_test = (double(ImgWidth)/double(xStep));
	double ystrips_test = (double(ImgHeight)/double(yStep));
	int xstrips = (double(ImgWidth)/double(xStep));
	int ystrips = (double(ImgHeight)/double(yStep));
	ASSERT(xstrips==ystrips);
#if TRUE
	int xerr = ImgWidth  - xStep*xstrips;
	//if(xerr < 0){xstrips--;xerr = ImgWidth - STEP*xstrips;}
	int yerr = ImgHeight - yStep*ystrips;
	//if(yerr < 0){ystrips--;yerr = ImgHeight- STEP*ystrips;}
#endif
	double xerrperstrip = double(xerr)/double(xstrips);
	double yerrperstrip = double(yerr)/double(ystrips);

	int xoff = xStep/2;
	int yoff = yStep/2;
	//-------------------------
	numseeds = xstrips*ystrips;
	slic_current_num=numseeds;
	//-------------------------
#if  DEBUG_CUI
	kseedsl.resize(numseeds);
	kseedsa.resize(numseeds);
	kseedsb.resize(numseeds);
	kseedsx.resize(numseeds);
	kseedsy.resize(numseeds);
#endif
	
	
		kseedsTheta.resize(numseeds);
		kseedsM.resize(numseeds);
		kseedsL.resize(numseeds);
		kseedsX.resize(numseeds);
		kseedsY.resize(numseeds);

	
	for( int y = 0; y < ystrips; y++ )
	{
		int ye = y*yerrperstrip;
		for( int x = 0; x < xstrips; x++ )
		{
			int xe = x*xerrperstrip;
			int seedx = (x*xStep+xoff+xe);
			if(hexgrid){ seedx = x*xStep+(xoff<<(y&0x1))+xe; seedx = min(ImgWidth-1,seedx); }//for hex grid sampling
			int seedy = (y*yStep+yoff+ye);
			int i = seedy*ImgWidth + seedx;
			{
#if DEBUG_CUI
				kseedsl[n] = m_lvec[i];
				kseedsa[n] = m_avec[i];
				kseedsb[n] = m_bvec[i];
				kseedsx[n] = seedx;
				kseedsy[n] = seedy;	
#endif				
				///////////////////////
				kseedsTheta[n]=sita_n[i];
				kseedsM[n]=m_n[i];
				kseedsL[n]=L_n[i];
				kseedsX[n]=X_n[i];
				kseedsY[n]=Y_n[i];
				n++;
			}
			
		}
	}

#if 0
if(perturbseeds)
	{
		PerturbSeeds(kseedsl, kseedsa, kseedsb, kseedsx, kseedsy, edgemag);
	}
#endif	
}
/*---------------------------------------------------------------------------------*/
/**
*��ȡa��b���Լ��
*@param  a  
*@param  b
*@return  ���Լ��
*
*/
/*---------------------------------------------------------------------------------*/
unsigned int ImageData::GetGCD(unsigned int a, unsigned int b)
{
	unsigned int t=0;
	if(a>b){
		t=b;
		b=a;
		a=t;
	}

	while(b%a)
	{
		t=b;
		b=a;
		a=t%a;
	}
	return a;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::SetImgLabels(void)
{
#if 0
int spNum=SpSet.SpPropertySet.size();
	for (int spi=0;spi<spNum;spi++){
		int pointNum=SpSet.SpPropertySet[spi].pointSet.size();
		for (int pi=0;pi<pointNum;pi++){
			int x=SpSet.SpPropertySet[spi].pointSet[pi].x;
			int y=SpSet.SpPropertySet[spi].pointSet[pi].y;
			this->src_ImgLabels[x+y*ImgWidth]=SpSet.SpPropertySet[spi].SpIndex;
		}
	}
#else
	int spNum=SpSet.SpPropertySet.size();
	for (register int spi=0;spi<spNum;spi++){
		int pointSetNum=SpSet.SpPropertySet[spi].pointSetIdx.size();
		int pointSetProp=SpSet.SpPropertySet[spi].SpIndex;
		for (register int seti=0;seti<pointSetNum;seti++){
		         int SetIdx=SpSet.SpPropertySet[spi].pointSetIdx[seti];
				 int pointNum=SpSet.PointSet[SetIdx].pointSet.size();
				 for (register int pi=0;pi<pointNum;pi++){
					 int x=SpSet.PointSet[SetIdx].pointSet[pi].x;
					 int y=SpSet.PointSet[SetIdx].pointSet[pi].y;
					 this->src_ImgLabels[x+y*ImgWidth]=pointSetProp;
				 }
		}
	}
#endif
	
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::Combination_ImgLabs(
	vector<vector<int>> simlarSp,
	bool UseMatrixE,
	bool UseMaxtrxW,
	bool UseEnergy)
{
	UINT32* Matrix_E=this->Matrix_E;	
	int sameNumSp=simlarSp.size();
	for (register int i=0;i<sameNumSp;i++){
		int sameNumlabels=simlarSp[i].size();
		for(register int spi=0;spi<sameNumlabels;spi++){
			for (register int spj=spi+1;spj<sameNumlabels;spj++){
					bool combine=true;
					int spx=simlarSp[i][spi];
					int	spy=simlarSp[i][spj];
					if (UseMatrixE){
						if (Matrix_E[spx*slic_current_num+spy]!=0){
							combine&=true;
						}else{
							combine&=false;
						}
					}
					if (UseMaxtrxW)	{
						combine=combine;
					}					
					if (UseEnergy){
						if (SpSet.SpPropertySet[spx].harr_energy.EnergyGrade==SpSet.SpPropertySet[spy].harr_energy.EnergyGrade){
							combine&=true;
						}else{
							combine&=false;
						}
					}

					if (combine){
						SpSet.Combine2in1(
							spx,
							spy,
							UseEnergy);
					}				
			}
		}
	}
	SpSet.AdjustSpSet();
	SetImgLabels();
	this->slic_current_num=SpSet.SpPropertySet.size();
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::SaveImgWithPointsCompute(string str_add)
{
	INT32 * Matrix_Category_Lable=new INT32[slic_current_num];
	/*---------------------------------------------------------*/
	for (register int spi=0;spi<slic_current_num;spi++){
		Matrix_Category_Lable[spi]=SpSet.SpPropertySet[spi].ComputeCategory;
	}
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		this->src_ImgBGRA,	
		this->src_ImgLabels,
		Matrix_Category_Lable,
		this->ImgWidth,
		this->ImgHeight,
		true,
		this->FileReadFullPath,
		this->FileWritePath,
		str_add);
	/*---------------------------------------------------------*/
	delete[] Matrix_Category_Lable;

}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::SaveImgWithPointsFuzzy(string str_add)
{
	INT32 * Matrix_Category_Lable=new INT32[slic_current_num];
	/*---------------------------------------------------------*/
	for (register int spi=0;spi<slic_current_num;spi++){
		Matrix_Category_Lable[spi]=SpSet.SpPropertySet[spi].fuzzyCategory;
	}
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		this->src_ImgBGRA,	
		this->src_ImgLabels,
		Matrix_Category_Lable,
		this->ImgWidth,
		this->ImgHeight,
		true,
		this->FileReadFullPath,
		this->FileWritePath,
		str_add);
	/*---------------------------------------------------------*/
	delete[] Matrix_Category_Lable;

}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::SaveImgWithContours(string str_add)
{
	cui_GeneralImgProcess::CuiSaveImgWithContours(
		this->src_ImgBGRA,	
		this->src_ImgLabels,
		this->ImgWidth,
		this->ImgHeight,
		this->FileReadFullPath,
		this->FileWritePath,
		str_add);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::SurroundClassification()
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
	//////////////////////////////////////////////////////////////
#if 1
	this->InSideClusteringByopencv();
	this->initSpSet();	
#else
	vector<vector<int>> MatrixE_Link(slic_current_num);
	this->GetMatrixE();
	for(register int spi=0;spi<this->slic_current_num;spi++){
		for(register int spj=0;spj<this->slic_current_num;spj++){
			if (Matrix_E[spi+spj*slic_current_num]!=0){
				MatrixE_Link[spi].push_back(spj);
			}
			
		}
	}
	vector<vector<int>> simlarSp;

	for(register int spi=0;spi<this->slic_current_num;spi++){
		vector<int> Sp;
		if (MatrixE_Link[spi].size()==1){
			if ((SpSet.SpPropertySet[spi].BorderCategory==0)||
				(SpSet.SpPropertySet[spi].BorderCategory==spBorderCategoryCrossHorz)){
					Sp.push_back(spi);
					Sp.push_back(MatrixE_Link[spi][0]);
					simlarSp.push_back(Sp);
			}
		}		
	}
	Combination_ImgLabs(simlarSp);
#endif
	
#if SaveContours2Disk
this->SaveImgWithContours();
#endif	
	//////////////////////////////////////////////////////////////
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
	TRACE("\n SurroundClassificationȫ��Χ: %f���룩",dfTim);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::InSideClusteringByopencv()
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
	/////////////////////////////////////////////
{
	UINT32* ubuff=this->src_ImgBGRA;
	int*	labels=this->src_ImgLabels;
	int& NumLabels=this->slic_current_num;
	const int		width=this->ImgWidth;
	const int		height=this->ImgHeight;
	string        filereadfullpath=this->FileReadFullPath;
	string        filewritepath=this->FileWritePath;
	///////////////////////////////////////////////////
	IplImage Img_labels;
	cvInitImageHeader(&Img_labels,cvSize(width,height),IPL_DEPTH_8U,4);
	Img_labels.imageData=(char *)labels;
	this->CalculateAllSpPropertyRange();
#if 1	
	UINT32 *Origin_img=NULL;	
		for (register int sp=0;sp<slic_current_num;sp++){
				int img_width=SpSet.SpPropertySet[sp].max_x-SpSet.SpPropertySet[sp].min_x;
				int img_height=SpSet.SpPropertySet[sp].max_y-SpSet.SpPropertySet[sp].min_y;
				Origin_img=new UINT32[img_width*img_height];
				IplImage Img_Contour_t;
				cvInitImageHeader(&Img_Contour_t,cvSize(img_width,img_height),IPL_DEPTH_8U,4);
				Img_Contour_t.imageData=(char *)Origin_img;
				/*********************************************/
				for(register int x=SpSet.SpPropertySet[sp].min_x;x<SpSet.SpPropertySet[sp].max_x;x++){
					for (register int y=SpSet.SpPropertySet[sp].min_y;y<SpSet.SpPropertySet[sp].max_y;y++){
						/////////////////////////////////
						int x_t=x-SpSet.SpPropertySet[sp].min_x;
						int y_t=y-SpSet.SpPropertySet[sp].min_y;
						/////////////////////////////////
						if (src_ImgLabels[y*ImgWidth+x]==sp){
							Origin_img[y_t*img_width+x_t]=0xffffffff;
						}else{
							Origin_img[y_t*img_width+x_t]=0;							
						}

					}
				}
				/**********************************************/
				cui_GeneralImgProcess::GetContour2Fill(&Img_Contour_t,
					SpSet.SpPropertySet[sp].min_x,
					SpSet.SpPropertySet[sp].min_y,
					&Img_labels,
					cvScalar(sp&0xff,(sp>>8)&0xff,(sp>>16)&0xff,(sp>>24)&0xff));
				/**********************************************/
				delete[]Origin_img;
		}
	/**********************************************/
	
#endif
this->slic_current_num=cui_GeneralImgProcess::AdjustLabFrom0toN(labels,width,height,NumLabels);
}


		///////////////////////////////////////////////
		QueryPerformanceCounter(&litmp);
		QPart2 = litmp.QuadPart;//�����ֵֹ
		dfMinus = (double)(QPart2-QPart1);
		dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
		TRACE("\n Labels��ȫ��Χ: %f���룩",dfTim);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::InitAllSpBlockEnergy()
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
	/////////////////////////////////////////////
	this->CalculateAllSpPropertyRange();
	this->CalculateAllSpBlockEnergy();
	///////////////////////////////////////////////
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
	TRACE("\n С��: %f���룩",dfTim);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::InitAllSpBlockEnergy2(void)
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
	/////////////////////////////////////////////
#if 1

	CuiHarrTransformLet Harr_Origin;
	Energy_Harr harr_origin_energy;
	/**********************************************/
	Harr_Origin.CuiGetImageData(src_ImgBGRA,ImgWidth,ImgHeight);
	Harr_Origin.CuiWaveletTrans(1);
	Harr_Origin.SetSuperPixelLabels(src_ImgLabels,ImgWidth,ImgHeight);
	//Harr_Origin.CuiCalculateEnergyALL();
	Harr_Origin.CalculateEnergyAllApBlock(SpSet);
	//Harr_Origin.CuiGetAllEnergy(&harr_origin_energy);
#if CUI_SAVE_HARR_IMG
	Harr_Origin.CuiSaveImg();
#endif
	
#endif
	///////////////////////////////////////////////
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
	TRACE("\n С��: %f���룩",dfTim);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::CalculateAllSpPropertyRange(void)
{
	int width=ImgWidth;
	int height=ImgHeight;
	int* labels=src_ImgLabels;
#if 1
	
for (int spi=0;spi<slic_current_num;spi++){
		SpSet.SpPropertySet[spi].max_x=0;
		SpSet.SpPropertySet[spi].min_x=0;
		SpSet.SpPropertySet[spi].max_y=0;
		SpSet.SpPropertySet[spi].min_y=0;
		SpSet.SpPropertySet[spi].SpSize=0;
#ifdef _DEBUG
		SpSet.SpPropertySet[spi].SpIndex=spi;
#endif
	}

	/******************************************/
	for (register int x=0;x<width;x++){
		//Y_SegPosition=GetSegmentPoint(x);
		for (register int y=height-1;y>=0;y--){
			///////////////////////////////////////
			ASSERT((y*width+x)<(width*height));
			INT32 spi=labels[y*width+x];
#if 1		
			//���㷶Χ
			ASSERT(spi>=0&&spi<slic_current_num);
			if (SpSet.SpPropertySet[spi].SpSize==0){
				SpSet.SpPropertySet[spi].SpSize=1;
				SpSet.SpPropertySet[spi].max_x=SpSet.SpPropertySet[spi].min_x=x;
				SpSet.SpPropertySet[spi].max_y=SpSet.SpPropertySet[spi].min_y=y;
			}else{
				if (SpSet.SpPropertySet[spi].min_y>=y){
					SpSet.SpPropertySet[spi].min_y=y;
				}
				if (SpSet.SpPropertySet[spi].max_y<=y){
					SpSet.SpPropertySet[spi].max_y=y;
				}
				if (SpSet.SpPropertySet[spi].min_x>=x){
					SpSet.SpPropertySet[spi].min_x=x;
				}
				if (SpSet.SpPropertySet[spi].max_x<=x){
					SpSet.SpPropertySet[spi].max_x=x;
				}
				SpSet.SpPropertySet[spi].SpSize++;
			}
			///////////////////////////////////////
#endif
		}
	}
	/*--------------------------------------------------------------*/
	/*-------------------------------------------------------------*/
	//����ʹ���ܱ�8 ����
	for (register int spi=0;spi<slic_current_num;spi++){
		int width_t= SpSet.SpPropertySet[spi].max_x-SpSet.SpPropertySet[spi].min_x;
		int height_t=SpSet.SpPropertySet[spi].max_y-SpSet.SpPropertySet[spi].min_y;
		if (width_t%8!=0){
			int add_width=8-width_t%8;
			if (SpSet.SpPropertySet[spi].min_x-add_width>=0){
				SpSet.SpPropertySet[spi].min_x-=add_width;
			}else if (SpSet.SpPropertySet[spi].max_x+add_width<width){
				SpSet.SpPropertySet[spi].max_x+=add_width;
			}

		}
		if (height_t%8!=0){
			int add_height=8-height_t%8;
			if (SpSet.SpPropertySet[spi].min_y-add_height>=0){
				SpSet.SpPropertySet[spi].min_y-=add_height;
			}else if (SpSet.SpPropertySet[spi].max_y+add_height<height){
				SpSet.SpPropertySet[spi].max_y+=add_height;
			}
		}

		SpSet.SpPropertySet[spi].width=SpSet.SpPropertySet[spi].max_x-SpSet.SpPropertySet[spi].min_x;
		SpSet.SpPropertySet[spi].height=SpSet.SpPropertySet[spi].max_y-SpSet.SpPropertySet[spi].min_y;
	}
#endif	
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::CalculateAllSpBlockEnergy(void)
{
	for (register int i=0;i<slic_current_num;i++){
		CalculateSpBlockEnergy2(i);
	}	
	this->CalculateAllSpBlockEnergyRank(8);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::CalculateAllSpBlockEnergy2(void)
{
	this->InitAllSpBlockEnergy2();	
	this->CalculateAllSpBlockEnergyRank(4);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::CalculateSpBlockEnergy2(int sp)
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;				
#if 1
	CuiHarrTransformLet Harr_Origin,Harr_Border;
	UINT32 *Origin_img=NULL,*Border_img=NULL;
	double  proportion_t=0;
	Energy_Harr harr_origin_energy;
	int img_width=SpSet.SpPropertySet[sp].max_x-SpSet.SpPropertySet[sp].min_x;
	int img_height=SpSet.SpPropertySet[sp].max_y-SpSet.SpPropertySet[sp].min_y;

	Origin_img=new UINT32[img_width*img_height];
	Border_img=new UINT32[img_width*img_height];
	ASSERT(img_width*img_height>0);
	/*********************************************/
	for(register int x=SpSet.SpPropertySet[sp].min_x;x<SpSet.SpPropertySet[sp].max_x;x++){
		for (register int y=SpSet.SpPropertySet[sp].min_y;y<SpSet.SpPropertySet[sp].max_y;y++){
			/////////////////////////////////
			ASSERT(x>=0&&x<ImgWidth);
			ASSERT(y>=0&&y<ImgHeight);
			/////////////////////////////////
			int x_t=x-SpSet.SpPropertySet[sp].min_x;
			int y_t=y-SpSet.SpPropertySet[sp].min_y;
			/////////////////////////////////
			if (src_ImgLabels[y*ImgWidth+x]==sp){
				Origin_img[y_t*img_width+x_t]=src_ImgBGRA[y*ImgWidth+x];
				Border_img[y_t*img_width+x_t]=0x00ffffff;
				proportion_t+=1;
			}else{
				Origin_img[y_t*img_width+x_t]=0;
				Border_img[y_t*img_width+x_t]=0;
			}

		}
	}
	/**********************************************/
	proportion_t=proportion_t/(img_width*img_height);
	/**********************************************/
	Harr_Origin.CuiGetImageData(Origin_img,img_width,img_height,proportion_t);
	Harr_Border.CuiGetImageData(Border_img,img_width,img_height,proportion_t);



	Harr_Origin.CuiWaveletTrans(1);//0.4ms





	Harr_Border.CalculateBoderImg(1);//�������α����������==0.9ms




	Harr_Origin.CuiAdjustBoderByContour(Harr_Border.img_dst_wavelet);//ȥ��Ӧͼ���Ե��������α���� 4.6ms




	Harr_Origin.CuiCalculateEnergyALL();//2.9ms



	Harr_Origin.CuiGetAllEnergy(&harr_origin_energy);//0.00087969494ms




#if CUI_SAVE_HARR_IMG
	Harr_Border.CuiSaveImg(); 
	Harr_Origin.CuiSaveImg();
#endif
	SpSet.SpPropertySet[sp].harr_energy.EnergyL_LL=harr_origin_energy.EnergyL_LL;
	SpSet.SpPropertySet[sp].harr_energy.EnergyL_LH=harr_origin_energy.EnergyL_LH;
	SpSet.SpPropertySet[sp].harr_energy.EnergyL_HL=harr_origin_energy.EnergyL_HL;
	SpSet.SpPropertySet[sp].harr_energy.EnergyL_HH=harr_origin_energy.EnergyL_HH;
	SpSet.SpPropertySet[sp].harr_energy.Energy_LL=harr_origin_energy.Energy_LL;
	SpSet.SpPropertySet[sp].harr_energy.Energy_LH=harr_origin_energy.Energy_LH;
	SpSet.SpPropertySet[sp].harr_energy.Energy_HL=harr_origin_energy.Energy_HL;
	SpSet.SpPropertySet[sp].harr_energy.Energy_HH=harr_origin_energy.Energy_HH;
	SpSet.SpPropertySet[sp].harr_energy.EnergyHighsigma=harr_origin_energy.EnergyHighsigma;
	SpSet.SpPropertySet[sp].harr_energy.harrDepth=harr_origin_energy.harrDepth;
	/**********************************************/

	delete[]Origin_img;
	delete[]Border_img;
#endif
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;
	double mstime=dfTim*1000;	
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::CalculateAllSpBlockEnergyRank(int RankNum)
{
#if 0
double* EnergyHighsigmaArray=new double[slic_current_num];
	for(int i=0;i<slic_current_num;i++){	
		EnergyHighsigmaArray[i]=SpSet.SpPropertySet[i].harr_energy.EnergyHighsigma;
	}
	double MaxEnergyHighsigma=cui_GeneralImgProcess::GetMaxValue(EnergyHighsigmaArray,slic_current_num);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for(int i=0;i<slic_current_num;i++){	
		SpSet.SpPropertySet[i].harr_energy.EnergyGrade=SpSet.SpPropertySet[i].harr_energy.EnergyHighsigma/MaxEnergyHighsigma*RankNum;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	delete[] EnergyHighsigmaArray;
#endif
	vector<float> EnergyHighsigmaArray(slic_current_num);
	for(register int i=0;i<slic_current_num;i++){	
		EnergyHighsigmaArray[i]=SpSet.SpPropertySet[i].harr_energy.EnergyHighsigma;
		
	}
	int MaxSort[2];
	cui_GeneralImgProcess::GetMaxValueIndex(EnergyHighsigmaArray.data(),slic_current_num, MaxSort,sizeof( MaxSort)/sizeof(int));

	double MaxEnergyHighsigma=EnergyHighsigmaArray[MaxSort[0]];
	if (MaxEnergyHighsigma<0.5){
		MaxEnergyHighsigma=1;
	}
	for(register int i=0;i<slic_current_num;i++){	
		SpSet.SpPropertySet[i].harr_energy.EnergyGrade=SpSet.SpPropertySet[i].harr_energy.EnergyHighsigma/MaxEnergyHighsigma*RankNum;
	}
	
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::FillWeightArrayZlmParallel(void)
{
	
#if 1
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ	
	/*---------------------------------------------------------------------------*/
	{	
		INT32* fuzzyCategory=new INT32[slic_current_num];
		double horizontal_line=Seg_HorizontalLinePos;
		classify_SkyVerticalGround_cuda(
			horizontal_line,
			fuzzyCategory,
			pYweight_S, 
			pYweight_V,		 
			pYweight_G,
			SkyWeightSp_host,
			VerticalWeightSp_host,
			GroundWeightSp_host,
			src_ImgLabels,
			slic_current_num,
			ImgWidth,
			ImgHeight);
		for (register int spi=0;spi<slic_current_num;spi++){
			SpSet.SpPropertySet[spi].fuzzyCategory=fuzzyCategory[spi];
			SpSet.SpPropertySet[spi].ComputeCategory=fuzzyCategory[spi];
		}

		delete[]fuzzyCategory;
	}	
	/*---------------------------------------------------------------------------*/
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λ Ϊ��
	TRACE("\n ģ����������ʱ��: %f���룩",dfTim);
	//ģ����������ʱ��: 2.077450 2.183845 1.978618���룩
#endif
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::CombinationImgSVG(void)
{
	vector<INT32> Matrix_Category_Lable(slic_current_num);
	for (int i=0;i<slic_current_num;i++){
		Matrix_Category_Lable[i]=SpSet.SpPropertySet[i].ComputeCategory;
	}
	cui_GeneralImgProcess::Cui_CombinationImgSVG(
		src_ImgBGRA,
		src_ImgLabels,
		src_ImgLabelsSVG,
		Matrix_Category_Lable.data(),
		ImgWidth,
		ImgHeight,
		"","","");
	FillHoleOnSVGLables(false,false,true,false,true);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::SaveImgSVGCompute(string str_add)
{
	
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		this->src_ImgBGRA,	
		this->src_ImgLabelsSVG,
		nullptr,
		this->ImgWidth,
		this->ImgHeight,
		false,
		this->FileReadFullPath,
		this->FileWritePath,
		str_add);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::FillHoleOnSVGLables(
	bool sky,
	bool vertical,
	bool ground,
	bool BorderSky,
	bool BorderGnd)
{
	INT32* Lables_SVG=this->src_ImgLabelsSVG;	
	IplImage *Lables_SVG_Img=cvCreateImage(cvSize(this->ImgWidth,this->ImgHeight),IPL_DEPTH_8U,4);
	IplImage *Lables_S_Img=cvCreateImage(cvSize(this->ImgWidth,this->ImgHeight),IPL_DEPTH_8U,4);
	IplImage *Lables_V_Img=cvCreateImage(cvSize(this->ImgWidth,this->ImgHeight),IPL_DEPTH_8U,4);
	IplImage *Lables_G_Img=cvCreateImage(cvSize(this->ImgWidth,this->ImgHeight),IPL_DEPTH_8U,4);
	ASSERT(Lables_SVG_Img->imageSize==this->ImgHeight*this->ImgWidth*sizeof(INT32));
	memcpy(Lables_SVG_Img->imageData,Lables_SVG,Lables_SVG_Img->imageSize);
	INT32* Lables_S=(INT32*)Lables_S_Img->imageData;
	INT32* Lables_V=(INT32*)Lables_V_Img->imageData;
	INT32* Lables_G=(INT32*)Lables_G_Img->imageData;
#if 1 
	if (sky==true){
		for (int x=0;x<this->ImgWidth;x++){
			for (int y=0;y<this->ImgHeight;y++){
				unsigned long index=y*this->ImgWidth+x;
				ASSERT(index<this->ImgWidth*this->ImgHeight);
				if (Lables_SVG[index]==Sky){
					Lables_S[index]=WhiteColorPNG;
				}else{
					Lables_S[index]=BlackColorPNG;
				}
			}
		}
		if (BorderSky==true){
			DrawS_V_G_Lables_BorderLine(Lables_S_Img,Sky);
		}
		cui_GeneralImgProcess::GetContour2Fill(Lables_S_Img,Lables_SVG_Img,SkyCv);
		ASSERT(Lables_SVG_Img->widthStep==sizeof(INT32)*ImgWidth);
		memcpy(Lables_SVG,Lables_SVG_Img->imageData,Lables_SVG_Img->imageSize);
	}
	if (vertical==true){
		for (int x=0;x<this->ImgWidth;x++){
			for (int y=0;y<this->ImgHeight;y++){
				unsigned long index=y*this->ImgWidth+x;
				ASSERT(index<this->ImgWidth*this->ImgHeight);
				if (Lables_SVG[index]==Vertical){
					Lables_V[index]=WhiteColorPNG;
				}else{
					Lables_V[index]=BlackColorPNG;
				}
			}
		}
		
		cui_GeneralImgProcess::GetContour2Fill(Lables_V_Img,Lables_SVG_Img,VerticalCv);
		ASSERT(Lables_SVG_Img->widthStep==sizeof(INT32)*ImgWidth);
		memcpy(Lables_SVG,Lables_SVG_Img->imageData,Lables_SVG_Img->imageSize);
	}
	if (ground==true){
		for (int x=0;x<this->ImgWidth;x++){
			for (int y=0;y<this->ImgHeight;y++){
				unsigned long index=y*this->ImgWidth+x;
				ASSERT(index<this->ImgWidth*this->ImgHeight);
				if (Lables_SVG[index]==Ground){
					Lables_G[index]=WhiteColorPNG;
				}else{
					Lables_G[index]=BlackColorPNG;
				}
			}
		}
		if (BorderGnd==true){
 			DrawS_V_G_Lables_BorderLine(Lables_G_Img,Ground);
		}
		cui_GeneralImgProcess::GetContour2Fill(Lables_G_Img,Lables_SVG_Img,GroundCv);
		ASSERT(Lables_SVG_Img->widthStep==sizeof(INT32)*ImgWidth);
		memcpy(Lables_SVG,Lables_SVG_Img->imageData,Lables_SVG_Img->imageSize);
	}
#endif
	cvReleaseImage(&Lables_SVG_Img);
	cvReleaseImage(&Lables_S_Img);
	cvReleaseImage(&Lables_V_Img);
	cvReleaseImage(&Lables_G_Img);
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::DrawS_V_G_Lables_BorderLine(IplImage *img,UINT32 category)
{
	//cvLine(Lables_S_Img,);
	vector<int> Y_0;
	vector<int> X_0;
	vector<int> X_W;
	vector<int> Y_W;
	INT32* Lables=(INT32*)img->imageData;
	for (int x=0;x<img->width;x++){
		for (int y=0;y<img->height;y++){
			unsigned long index=y*img->width+x;
			ASSERT(index<img->width*img->height);
			if (Lables[index]==WhiteColorPNG){
				if (y==0){
					Y_0.push_back(x);
				}
				if (y==img->height-1){
					Y_W.push_back(x);
				}
				if (x==0){
					X_0.push_back(y);
				}
				if (x==img->width-1){
					X_W.push_back(y);
				}

			}else if (Lables[index]==BlackColorPNG){
				;
			}else{
				ASSERT(FALSE);
			}
		}
	}
	if(category==Sky){
		cvLine(img,cvPoint(0,0),cvPoint(img->width,0), WhiteColorPNGCv,2);
		cvLine(img,cvPoint(0,0),cvPoint(0,img->height), WhiteColorPNGCv,2);
		cvLine(img,cvPoint(img->width,0),cvPoint(img->width,img->height), WhiteColorPNGCv,2);
	}
	if(category==Ground){
		cvLine(img,cvPoint(0,img->height),cvPoint(img->width,img->height), WhiteColorPNGCv,4);		
	}
#if TRUE
	cvSaveImage("Sky_lab.jpg",img);
#endif

}
/*----------------------------------------------------------------*/
/**
*BGRA 4��ͨ��
*
*
*/
/*----------------------------------------------------------------*/
void ImageData::SaveSuperpixelLabels(
	INT32*					labels,
	const int				width,
	const int				height,
	const string			filename,
	const string			path) 
{ 
	ASSERT(width==this->ImgWidth);
	ASSERT(height==this->ImgHeight);
	ASSERT(sizeof(INT32)==4);
	
	IplImage *img_t=cvCreateImage(cvSize(this->ImgWidth,this->ImgHeight),IPL_DEPTH_8U,4);
    ASSERT(width*height*sizeof(INT32)==img_t->imageSize);

	/*
	OPENCV  4�ֽڶ�������
	*/

	ASSERT(img_t->widthStep==width*sizeof(INT32));  


	memcpy(img_t->imageData,labels,img_t->imageSize);
  
	cvSaveImage(filename.c_str(),img_t);

	cvReleaseImage(&img_t);
		
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/