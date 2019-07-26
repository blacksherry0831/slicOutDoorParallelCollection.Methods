#include "StdAfx.h"
#include "TimeMeasure.hpp"
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
TimeMeasure::TimeMeasure(std::string _event)
{
	this->start(_event);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
TimeMeasure::TimeMeasure(void)
{
	this->IsTimeMeasure = FALSE;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
TimeMeasure::~TimeMeasure(void)
{
	if (this->IsTimeMeasure)
	{
		this->stop();
	}
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void TimeMeasure::start(std::string _event)
{
	mEVENT = _event;
#if _MSC_VER
	
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
							/////////////////////////////////////////////
#endif

#if TRUE
	start_time = clock();
#endif // TRUE

	
	this->IsTimeMeasure = TRUE;

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
std::string TimeMeasure::stop()
{
	std::string msg_t;
#if _MSC_VER
	///////////////////////////////////////////////
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2 - QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒

	msg_t.append("TimeMeasure --> ")
		.append(mEVENT).append(" : ")
		.append(Base::float2str(dfTim * 1000)).append("(ms)");
#endif

#if defined(linux) || defined(__linux) || defined(__linux__) ||defined( __GNUC__)
	end_time = clock(); 
	dur = (double)(end_time - start_time);	

	msg_t.append("TimeMeasure --> ")
		.append(mEVENT).append(" : ")
		.append(Base::float2str(dur / CLOCKS_PER_SEC)).append("(ms)");

#endif

	this->IsTimeMeasure = FALSE;

	return msg_t;
	
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/