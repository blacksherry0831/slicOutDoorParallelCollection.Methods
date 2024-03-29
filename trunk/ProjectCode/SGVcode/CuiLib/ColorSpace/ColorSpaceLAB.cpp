#include "ColorSpaceLAB.h"
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
ColorSpaceLAB::ColorSpaceLAB()
{
	
}
/*----------------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------------*/
ColorSpaceLAB::~ColorSpaceLAB()
{

}
/*----------------------------------------------------------------*/
/**
*
*	RGB2LAB
*
*/
/*----------------------------------------------------------------*/
void ColorSpaceLAB::RGB2LAB(const int& sR, const int& sG, const int& sB, double& lval, double& aval, double& bval)
{
	//TRACE_FUNC();
	//------------------------
	// sRGB to XYZ conversion
	//------------------------
	double X, Y, Z;
	RGB2XYZ(sR, sG, sB, X, Y, Z);

	//------------------------
	// XYZ to LAB conversion
	//------------------------
	const double epsilon = 0.008856;	//actual CIE standard
	const double kappa = 903.3;		//actual CIE standard

	const double Xr = 0.950456;	//reference white
	const double Yr = 1.0;		//reference white
	const double Zr = 1.088754;	//reference white

	double xr = X / Xr;
	double yr = Y / Yr;
	double zr = Z / Zr;

	double fx, fy, fz;
	if (xr > epsilon)	fx = pow(xr, 1.0 / 3.0);
	else				fx = (kappa*xr + 16.0) / 116.0;
	if (yr > epsilon)	fy = pow(yr, 1.0 / 3.0);
	else				fy = (kappa*yr + 16.0) / 116.0;
	if (zr > epsilon)	fz = pow(zr, 1.0 / 3.0);
	else				fz = (kappa*zr + 16.0) / 116.0;

	lval = 116.0*fy - 16.0;
	aval = 500.0*(fx - fy);
	bval = 200.0*(fy - fz);
}
/*----------------------------------------------------------------*/
/**
*RGB2XYZ
*	
*sRGB (D65 illuninant assumption) to XYZ conversion
*/
/*----------------------------------------------------------------*/
void ColorSpaceLAB::RGB2XYZ(
	const int&		sR,
	const int&		sG,
	const int&		sB,
	double&			X,
	double&			Y,
	double&			Z)
{
	//TRACE_FUNC();
	double R = sR / 255.0;
	double G = sG / 255.0;
	double B = sB / 255.0;

	double r, g, b;

	if (R <= 0.04045)	r = R / 12.92;
	else				r = pow((R + 0.055) / 1.055, 2.4);
	if (G <= 0.04045)	g = G / 12.92;
	else				g = pow((G + 0.055) / 1.055, 2.4);
	if (B <= 0.04045)	b = B / 12.92;
	else				b = pow((B + 0.055) / 1.055, 2.4);

	X = r*0.4124564 + g*0.3575761 + b*0.1804375;
	Y = r*0.2126729 + g*0.7151522 + b*0.0721750;
	Z = r*0.0193339 + g*0.1191920 + b*0.9503041;
}
/*----------------------------------------------------------------*/
/**
*DoRGBtoLABConversion
*For whole image: overlaoded floating point version
*
*/
/*----------------------------------------------------------------*/
void ColorSpaceLAB::DoRGBtoLABConversion(
	const unsigned int*		ubuff,
	double*					lvec,
	double*					avec,
	double*					bvec,
	int                     width,
	int                     height)
{
	TRACE_FUNC();

	TimeMeasure tm("RGB2LAB");
	
	const int sz = width*height;
	/*lvec = new double[sz];
	avec = new double[sz];
	bvec = new double[sz];*/
	/*r:0--255*/
	/*g:0--255*/
	/*b:0--255*/
	int r, g, b;
	for (int j = 0; j < sz; j++)
	{
		r = (ubuff[j] >> 16) & 0xFF;
		g = (ubuff[j] >> 8) & 0xFF;
		b = (ubuff[j]) & 0xFF;
		//////////////////////////////////
		// a  r      g      b
		//////////////////////////////////
		ColorSpaceLAB::RGB2LAB(r, g, b, lvec[j], avec[j], bvec[j]);
	}
	
}