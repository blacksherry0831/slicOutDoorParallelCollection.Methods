#pragma once
/*----------------------------------------------------------------*/
#include "opencv_stl.h"
#include "cpp_def.h"
#include "cpp_def_debug.h"
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
class DbgRangeCheck
{
public:
	DbgRangeCheck();
	~DbgRangeCheck();
public:
	static void Check01(const double _f);
	static void CheckRange(const double & _f, const double & _min, const double & _max, const double & _eps=1E-6);
};

