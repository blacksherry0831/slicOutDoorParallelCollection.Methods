#pragma once
/*----------------------------------------------------------------*/
#include "opencv_stl.h"
#include "cpp_def.h"
#include "cpp_def_debug.h"
/*----------------------------------------------------------------*/
#include "DbgRangeCheck.h"
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
class DistanceColorSpace
{
public:
	DistanceColorSpace();
	~DistanceColorSpace();

	static double CalDistLThetaMXY(
		const float& alpha,
		const float& betta,
		const float& gama,
		const float& fai,
		const double& sita_n0,
		const double& m_n0,
		const double& L_n0,
		const double& X_n0,
		const double& Y_n0,
		const double& sita_n1,
		const double& m_n1,
		const double& L_n1,
		const double& X_n1,
		const double& Y_n1);

	static void DbgCheckLThetaMXY(
		const double& sita_n0,
		const double& m_n0,
		const double& L_n0,
		const double& X_n0,
		const double& Y_n0,
		const double& sita_n1,
		const double& m_n1,
		const double& L_n1,
		const double& X_n1,
		const double& Y_n1);

	static double CalDistLThetaMXY_Basic(
		const float& alpha,
		const float& betta,
		const float& gama,
		const float& fai,
		const double& sita_n0,
		const double& m_n0,
		const double& L_n0,
		const double& X_n0,
		const double& Y_n0,
		const double& sita_n1,
		const double& m_n1,
		const double& L_n1,
		const double& X_n1,
		const double& Y_n1);


};

