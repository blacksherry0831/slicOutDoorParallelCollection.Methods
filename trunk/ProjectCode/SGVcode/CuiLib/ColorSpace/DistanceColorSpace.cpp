#include "DistanceColorSpace.h"
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
DistanceColorSpace::DistanceColorSpace()
{

}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
DistanceColorSpace::~DistanceColorSpace()
{
	
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
 double  DistanceColorSpace::CalDistLThetaMXY(
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
	 const double& Y_n1)
{
	/*alpha=0.6;
	betta=0.3;
	gama=1-alpha-betta;*/
	/*float fai=100;*/
	 DbgCheckLThetaMXY(
			sita_n0,
			m_n0,
			L_n0,
			X_n0,
			Y_n0,
			sita_n1,
			m_n1,
			L_n1,
			X_n1,
			Y_n1);

	 return DistanceColorSpace::CalDistLThetaMXY_Basic(
			alpha,
			betta,
			gama,
			fai,
			sita_n0,
			m_n0,
			L_n0,
			X_n0,
			Y_n0,
			sita_n1,
			m_n1,
			L_n1,
			X_n1,
			Y_n1);


}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void DistanceColorSpace::DbgCheckLThetaMXY(
	 const double& sita_n0,
	 const double& m_n0,
	 const double& L_n0,
	 const double& X_n0,
	 const double& Y_n0,
	 const double& sita_n1,
	 const double& m_n1,
	 const double& L_n1,
	 const double& X_n1,
	 const double& Y_n1)
 {
	DbgRangeCheck::Check01(sita_n0);
	DbgRangeCheck::Check01(m_n0);
	DbgRangeCheck::Check01(L_n0 );
	DbgRangeCheck::Check01(X_n0 );
	DbgRangeCheck::Check01(Y_n0 );

	DbgRangeCheck::Check01(sita_n1);
	DbgRangeCheck::Check01(m_n1);
	DbgRangeCheck::Check01(L_n1);
	DbgRangeCheck::Check01(X_n1);
	DbgRangeCheck::Check01(Y_n1);
 }
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
double DistanceColorSpace::CalDistLThetaMXY_Basic(
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
	const double& Y_n1)
{
	const double dst = alpha*fabs(sita_n1 - sita_n0) + betta*fabs(m_n1 - m_n0) + gama*fabs(L_n1 - L_n0);
	const double dst_xy = fai*(fabs(X_n1 - X_n0) + fabs(Y_n1 - Y_n0));
	const double dist_l_theta_m_x_y= dst + dst_xy;	
	return dist_l_theta_m_x_y;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
double DistanceColorSpace::CalDistLAB_Basic_Square(
	const double & L0,
	const double & A0,
	const double & B0,
	const double & X0,
	const double & Y0,
	const double & L1,
	const double & A1,
	const double & B1,
	const double & X1,
	const double & Y1,
	const double & INVWT)
{
	const double dst = (L1 - L0)*(L1 - L0)+(B1 - B0)*(B1 - B0) + (A1 - A0)*(A1 - A0);
	const double dst_xy = (X1 - X0)*(X1 - X0) + (Y1 - Y0)*(Y1 - Y0);
	const double dist_labxy = dst + dst_xy;
	return dist_labxy;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
double DistanceColorSpace::CalDistLAB_Basic_Square_INVWT(
	const int WIDTH,
	const int HEIGHT,
	const double & K,
	const double &M)
{
	const double STEP = CalDistLAB_Basic_Square_STEP(WIDTH,HEIGHT,K);
	const double invwt = 1.0 / ((STEP / M)*(STEP / M));
	return	invwt;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
double DistanceColorSpace::CalDistLAB_Basic_Square_STEP(
	const int WIDTH,
	const int HEIGHT,
	const double  K)
{
	const double SizeSuperPixel = 0.5 + (WIDTH*HEIGHT) / K;
	const double STEP = sqrt(SizeSuperPixel) + 0.5;
	return	STEP;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/