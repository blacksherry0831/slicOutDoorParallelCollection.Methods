/*----------------------------------------------------------------------------------------------------------------*/
#include "cudaLib/CUDA_Code_LAB_INT.h"
/*----------------------------------------------------------------------------------------------------------------*/
#include "ColorSpace/ImageColorSpaceLAB.h"
#include "ColorSpace/ImageColorSpaceThetaML.h"
#include "ColorSpace/DistanceColorSpace.h"
/*----------------------------------------------------------------------------------------------------------------*/
#include "MY_SDK_LIB/TimeMeasure.hpp"
/*----------------------------------------------------------------------------------------------------------------*/
#include "SLIC_LAB_CUDA_INT.h"
/*----------------------------------------------------------------------------------------------------------------*/
#include "SGVcode/ImageSaveProc.h"
/*----------------------------------------------------------------------------------------------------------------*/
/**
*构造函数
*完成变量初始化
*/
/*----------------------------------------------------------------------------------------------------------------*/
SLIC_LAB_CUDA_INT::SLIC_LAB_CUDA_INT(
	IplImage* _img,
	std::string _SavePath)
{
	this->initMem();

	this->mImgPtr	=	_img;
	this->mSavePath =	_SavePath;
	this->mWidth	=	_img->width;
	this->mHeight	=	_img->height;
	this->mSZ = this->mWidth*this->mHeight;

	this->createMem(mSZ);

}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------------------------------------------------------*/
SLIC_LAB_CUDA_INT::~SLIC_LAB_CUDA_INT()
{
	this->freeMem();
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA_INT::DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels_LAB_CUDA()
{
	TRACE_FUNC();
	
	TimeMeasure tm("Superpixel.Total");
	{
		//---------------------------------------------------------
		//if(K < 20 || K > sz/4) K = sz/200;//i.e the default size of the superpixel is 200 pixels

		/*	if(compactness < 1.0 || compactness > 80.0) compactness = 20.0;*/
		//---------------------------------------------------------		
		DoSuperpixelSegmentation_ForGivenSuperpixelSize_LAB_CUDA();
	}

}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------------------------------------------------------*/
void  SLIC_LAB_CUDA_INT::DoSuperpixelSegmentation_ForGivenSuperpixelSize_LAB_CUDA()
{
	TRACE_FUNC();

	const float	INVWT = DistanceColorSpace::CalDistLAB_Basic_Square_INVWT(mWidth, mHeight, m_slic_num, m_slic_M);
	const int	STEP = DistanceColorSpace::CalDistLAB_Basic_Square_STEP(mWidth, mHeight, m_slic_num);

	const int sz =mWidth*mHeight;
	//--------------------------------------------------
	bool perturbseeds(false);//perturb seeds is not absolutely necessary, one can set this flag to false
	vector<double> edgemag(0);
	/*if(perturbseeds) DetectLabEdges(mLvPtr, mAvPtr, mBvPtr, m_width, m_height, edgemag);*/
#if TRUE
	GetLABXYSeeds_ForGivenStepSize_Rectangle2(
		kseedsl,
		kseedsa,
		kseedsb,
		kseedsx,
		kseedsy,
		m_slic_num,
		perturbseeds,
		edgemag,
		mLvPtr,
		mAvPtr,
		mBvPtr);
#endif

#if 1
	PerformSuperpixelSLIC_cuda_lab_Int_F(
		m_slic_num,
		mWidth,
		mHeight,
		mLvPtr,
		mAvPtr,
		mBvPtr,

		kseedsl.data(),
		kseedsa.data(),
		kseedsb.data(),
		kseedsx.data(),
		kseedsy.data(),
		m_labels_slic,
		m_slic_M*m_slic_M,
		STEP*STEP);
#endif // 1
	
	EnforceLabelConnectivityEx(
		m_labels_slic,
		m_labels_connectivity,
		mWidth,
		mHeight,
		m_slic_num,
		m_slic_num);

	ImageSaveProc::SaveImgWithContours(
		(UINT32*) (mImgPtr->imageData),
		m_labels_slic,
		mWidth,
		mHeight,
		"",
		mSavePath,
		"SuperPixel");

	ImageSaveProc::SaveImgWithContours(
		(UINT32*)(mImgPtr->imageData),
		m_labels_connectivity,
		mWidth,
		mHeight,
		"",
		mSavePath,
		"Connectivity");


#if _DEBUG
	testListSize();
#endif // _DEBUG
	
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA_INT::EnforceLabelConnectivityEx(
	int*	 klabels,
	int*	nklabels,
	const int width,
	const int height,
	int & numlabels,
	const int & K)
{

	TimeMeasure tm("SuperPixel.Connectivity");

	EnforceLabelConnectivity_List(
			klabels,
			width,
			height,
			nklabels,
			numlabels,
			K);

}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*	EnforceLabelConnectivity
*	1. finding an adjacent label for each new component at the start
*	2. if a certain component is too small, assigning the previously found
*	adjacent label to this component, and not incrementing the label.
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA_INT::EnforceLabelConnectivity(
	const int*					labels,//input labels that need to be corrected to remove stray labels
	const int					width,
	const int					height,
	int*&						nlabels,//new labels
	int&						numlabels,//the number of labels changes in the end if segments are removed
	const int&					K) //the number of superpixels desired by the user
{
	//	const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	//	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};

	const int dx4[4] = { -1,  0,  1,  0 };
	const int dy4[4] = { 0, -1,  0,  1 };

	const int sz = width*height;
	const int SUPSZ = sz / K;

	for (int i = 0; i < sz; i++)
		nlabels[i] = -1;

	int label(0);
	int* xvec = new int[sz];
	int* yvec = new int[sz];
	int oindex(0);
	int adjlabel(0);//adjacent label
	for (register int j = 0; j < height; j++)
	{
		for (register int k = 0; k < width; k++)
		{
			if (0 > nlabels[oindex])
			{
				nlabels[oindex] = label;
				//--------------------
				// Start a new segment
				//--------------------
				xvec[0] = k;
				yvec[0] = j;
				//-------------------------------------------------------
				// Quickly find an adjacent label for use later if needed
				//-------------------------------------------------------
				{for (register int n = 0; n < 4; n++)
				{
					int x = xvec[0] + dx4[n];
					int y = yvec[0] + dy4[n];
					if ((x >= 0 && x < width) && (y >= 0 && y < height))
					{
						int nindex = y*width + x;
						if (nlabels[nindex] >= 0) adjlabel = nlabels[nindex];
					}
				}}

				int count(1);
				for (register int c = 0; c < count; c++)
				{
					for (register int n = 0; n < 4; n++)
					{
						int x = xvec[c] + dx4[n];
						int y = yvec[c] + dy4[n];

						if ((x >= 0 && x < width) && (y >= 0 && y < height))
						{
							int nindex = y*width + x;

							if (0 > nlabels[nindex] && labels[oindex] == labels[nindex])
							{
								xvec[count] = x;
								yvec[count] = y;
								nlabels[nindex] = label;
								count++;
							}
						}

					}
				}
				//-------------------------------------------------------
				// If segment size is less then a limit, assign an
				// adjacent label found before, and decrement label count.
				//-------------------------------------------------------
				if (count <= SUPSZ >> 2)
				{
					for (register int c = 0; c < count; c++)
					{
						int ind = yvec[c] * width + xvec[c];
						nlabels[ind] = adjlabel;
					}
					label--;
				}
				label++;
			}
			oindex++;
		}
	}
	numlabels = label;

	if (xvec) delete[] xvec;
	if (yvec) delete[] yvec;
}
/*----------------------------------------------------------------------------------------------------------------*/
/**
*	EnforceLabelConnectivity
*	1. finding an adjacent label for each new component at the start
*	2. if a certain component is too small, assigning the previously found
*	adjacent label to this component, and not incrementing the label.
*/
/*----------------------------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA_INT::EnforceLabelConnectivity_List(
	const int*					labels,//input labels that need to be corrected to remove stray labels
	const int					width,
	const int					height,
	int*&						nlabels,//new labels
	int&						numlabels,//the number of labels changes in the end if segments are removed
	const int&					K) //the number of superpixels desired by the user
{
#if 0
		const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
		const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};
#else
		const int dx4[4] = { -1,  0,  1,  0 };
		const int dy4[4] = { 0, -1,  0,  1 };
#endif // 0
		
	const int sz = width*height;
	const int SUPSZ = sz / K;

	for (int i = 0; i < sz; i++)
		nlabels[i] = -1;

	int label(0);
	int* xvec = new int[sz];
	int* yvec = new int[sz];
	int oindex(0);
	int adjlabel(0);//adjacent label
	for (int j = 0; j < height; j++)
	{
		for (int k = 0; k < width; k++)
		{
			if (0 > nlabels[oindex])
			{
				std::list<CvPoint>	super_pixel_block;
				nlabels[oindex] = label;
				//--------------------
				// Start a new segment
				//--------------------
				xvec[0] = k;
				yvec[0] = j;
				super_pixel_block.push_back(cvPoint(k, j));
				//-------------------------------------------------------
				// Quickly find an adjacent label for use later if needed
				//-------------------------------------------------------
				for (register int n = 0; n < 4; n++){
					int x = xvec[0] + dx4[n];
					int y = yvec[0] + dy4[n];
					if ((x >= 0 && x < width) && (y >= 0 && y < height))
					{
						const int nindex = y*width + x;
						if (nlabels[nindex] >= 0) adjlabel = nlabels[nindex];
					}
				}

				int count(1);
				
				for (register int c = 0; c < count; c++)
				{
					for (register int n = 0; n < 4; n++)
					{
						const int x = xvec[c] + dx4[n];
						const int y = yvec[c] + dy4[n];

						if ((x >= 0 && x < width) && (y >= 0 && y < height))
						{
							const int nindex = y*width + x;

							if (0 > nlabels[nindex] && labels[oindex] == labels[nindex])
							{
								xvec[count] = x;
								yvec[count] = y;
								nlabels[nindex] = label;

								super_pixel_block.push_back(cvPoint(x, y));

								count++;
							}
						}

					}
				}
				//-------------------------------------------------------
				// If segment size is less then a limit, assign an
				// adjacent label found before, and decrement label count.
				//-------------------------------------------------------
				mSuperPixel.resize(label + 1);

				if (count <= SUPSZ >> 2)
				{
					for (register int c = 0; c < count; c++)
					{
						const int ind = yvec[c] * width + xvec[c];
						nlabels[ind] = adjlabel;
						mSuperPixel[adjlabel].push_back(cvPoint(xvec[c], yvec[c]));
					}

					

					label--;

					
				}
				else
				{
					mSuperPixel[label]=super_pixel_block;
				}
				label++;
			}
			oindex++;
		}
	}
	numlabels = label;

	if (xvec) delete[] xvec;
	if (yvec) delete[] yvec;
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA_INT::GetLABXYSeeds_ForGivenStepSize_Rectangle2(
		vector<int>&				kseedsl,
		vector<int>&				kseedsa,
		vector<int>&				kseedsb,
		vector<int>&				kseedsx,
		vector<int>&				kseedsy,
		int&					    NumLabels,
		const bool&					perturbseeds,
		const vector<double>&		edgemag,
		int*  mLvPtr,
		int*  mAvPtr,
		int*  mBvPtr)
	{
		TRACE_FUNC();

		TimeMeasure tm("Superpixel.Seeds.First");

		float xStep;
		float yStep;
		const bool hexgrid = false;
		int numseeds(0);
		int n(0);
		int K = NumLabels;

		unsigned int wh_gcd = GetGCD(mWidth, mHeight);
		int width_min = mWidth / wh_gcd;
		int height_min = mHeight / wh_gcd;
		int block_min = width_min*height_min;
		double  wh_scale = 1.0*mWidth / mHeight;
		{
			/*int K_new=((int)(K/block_min+0.5))*block_min;*/
			yStep = sqrtl(mHeight*mHeight / K);
			int K_new = (mHeight*mHeight) / (yStep*yStep);
			{
				/*yStep=sqrtl(1.0*m_height*m_width/(K_new*wh_scale));*/
				xStep = wh_scale*yStep;
			}
		}


		float xstrips_test = (double(mWidth) / double(xStep));
		float ystrips_test = (double(mHeight) / double(yStep));
		int xstrips = (double(mWidth) / double(xStep));
		int ystrips = (double(mHeight) / double(yStep));
		ASSERT(xstrips == ystrips);

		const int xerr = mWidth - xStep*xstrips;

		const int yerr = mHeight - yStep*ystrips;


		double xerrperstrip = double(xerr) / double(xstrips);
		double yerrperstrip = double(yerr) / double(ystrips);

		int xoff = xStep / 2;
		int yoff = yStep / 2;
		//-------------------------
		NumLabels = numseeds = xstrips*ystrips;
		//------------------------- 
		kseedsl.resize(numseeds);
		kseedsa.resize(numseeds);
		kseedsb.resize(numseeds);
		kseedsx.resize(numseeds);
		kseedsy.resize(numseeds);

		for (register int y = 0; y < ystrips; y++)
		{
			int ye = y*yerrperstrip;
			for (register int x = 0; x < xstrips; x++)
			{
				int xe = x*xerrperstrip;
				int seedx = (x*xStep + xoff + xe);
				if (hexgrid) { seedx = x*xStep + (xoff << (y & 0x1)) + xe; seedx = min(mWidth - 1, seedx); }//for hex grid sampling
				int seedy = (y*yStep + yoff + ye);
				int i = seedy*mWidth + seedx;

				kseedsl[n] = mLvPtr[i];
				kseedsa[n] = mAvPtr[i];
				kseedsb[n] = mBvPtr[i];
				//ASSERT(0);
				kseedsx[n] = seedx;
				kseedsy[n] = seedy;
				n++;
			}
		}


		if (perturbseeds)
		{
			PerturbSeeds(
				kseedsl,
				kseedsa,
				kseedsb,
				kseedsx,
				kseedsy,
				edgemag);
		}
	}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
unsigned int SLIC_LAB_CUDA_INT::GetGCD(unsigned int a, unsigned int b)
{
	unsigned int t = 0;
	if (a>b) {
		t = b;
		b = a;
		a = t;
	}

	while (b%a)
	{
		t = b;
		b = a;
		a = t%a;
	}
	return a;
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
void  SLIC_LAB_CUDA_INT::PerturbSeeds(
	vector<int>&				kseedsl,
	vector<int>&				kseedsa,
	vector<int>&				kseedsb,
	vector<int>&				kseedsx,
	vector<int>&				kseedsy,
	const vector<double>&                   edges)
{
	const int dx8[8] = { -1, -1,  0,  1, 1, 1, 0, -1 };
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1 };

	int numseeds = kseedsl.size();

	for (int n = 0; n < numseeds; n++)
	{
		int ox = kseedsx[n];//original x
		int oy = kseedsy[n];//original y
		int oind = oy*mWidth + ox;

		int storeind = oind;
		for (int i = 0; i < 8; i++)
		{
			int nx = ox + dx8[i];//new x
			int ny = oy + dy8[i];//new y

			if (nx >= 0 && nx < mWidth && ny >= 0 && ny < mHeight)
			{
				int nind = ny*mWidth + nx;
				if (edges[nind] < edges[storeind])
				{
					storeind = nind;
				}
			}
		}
		if (storeind != oind)
		{
			kseedsx[n] = storeind	%	mWidth;
			kseedsy[n] = storeind	/	mWidth;
			kseedsl[n] = mLvPtr[storeind];
			kseedsa[n] = mAvPtr[storeind];
			kseedsb[n] = mBvPtr[storeind];
		}
	}
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA_INT::testListSize()
{
	int count = 0;
	for(std::list<CvPoint> & sp_one :  mSuperPixel){
		for (CvPoint & pt:sp_one){
			count++;
		}
	}

	ASSERT(count==mSZ);
	
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA_INT::initLabPtr(
	int*	_l,
	int*	_a,
	int*	_b)
{
	mLvPtr=_l;
	mAvPtr=_a;
	mBvPtr=_b;
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA_INT::initSlic(
	const int _num,
	const float _M)
{
	m_slic_num=_num;
	m_slic_M=_M;
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA_INT::initMem()
{
	m_labels_slic=NULL;
	m_labels_connectivity=NULL;
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA_INT::createMem(const int _s)
{
	m_labels_slic = new	INT32[_s];
	m_labels_connectivity = new	INT32[_s];
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
void SLIC_LAB_CUDA_INT::freeMem()
{
	delete[] m_labels_slic;
	delete[] m_labels_connectivity;
}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/