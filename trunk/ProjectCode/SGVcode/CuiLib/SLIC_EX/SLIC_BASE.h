#pragma once
// SLIC.h: interface for the SLIC class.
//===========================================================================
// This code implements the superpixel method described in:
//
// Radhakrishna Achanta, Appu Shaji, Kevin Smith, Aurelien Lucchi, Pascal Fua, and Sabine Susstrunk,
// "SLIC Superpixels",
// EPFL Technical Report no. 149300, June 2010.
//===========================================================================
//	Copyright (c) 2012 Radhakrishna Achanta [EPFL]. All rights reserved.
//===========================================================================
//////////////////////////////////////////////////////////////////////

#if !defined(_SLIC_BASE__H_INCLUDED_)
#define _SLIC_BASE_H_INCLUDED_


#include <vector>
#include <string>
#include <algorithm>
using namespace std;


class SLIC_BASE  
{
public:
	SLIC_BASE();
	~SLIC_BASE();
	//============================================================================
	// Save superpixel labels in a text file in raster scan order
	//============================================================================
	void SaveSuperpixelLabels(
		const int*&					labels,
		const int&					width,
		const int&					height,
		const string&				filename,
		const string&				path);
	//============================================================================
	// Save supervoxel labels in a text file in raster scan, depth order
	//============================================================================
	void SaveSupervoxelLabels(
		const int**&				labels,
		const int&					width,
		const int&					height,
		const int&					depth,
		const string&				filename,
		const string&				path);
	//============================================================================
	// Function to draw boundaries around superpixels of a given 'color'.
	// Can also be used to draw boundaries around supervoxels, i.e layer by layer.
	//============================================================================
	void DrawContoursAroundSegments(
		unsigned int*&				segmentedImage,
		int*&						labels,
		const int&					width,
		const int&					height,
		const unsigned int&			color );

private:
	//============================================================================
	// The main SLIC algorithm for generating superpixels
	//============================================================================
	void PerformSuperpixelSLIC(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		int*&						klabels,
		const int&					STEP,
                const vector<double>&		edgemag,
		const double&				m = 10.0);
	//============================================================================
	// The main SLIC algorithm for generating supervoxels
	//============================================================================
	void PerformSupervoxelSLIC(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		vector<double>&				kseedsz,
		int**&						klabels,
		const int&					STEP,
		const double&				compactness);
	//============================================================================
	// Pick seeds for superpixels when step size of superpixels is given.
	//============================================================================
	void GetLABXYSeeds_ForGivenStepSize(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		const int&					STEP,
		const bool&					perturbseeds,
		const vector<double>&		edgemag);
	//============================================================================
	// Pick seeds for supervoxels
	//============================================================================
	void GetKValues_LABXYZ(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		vector<double>&				kseedsz,
		const int&					STEP);
	//============================================================================
	// Move the superpixel seeds to low gradient positions to avoid putting seeds
	// at region boundaries.
	//============================================================================
	void PerturbSeeds(
		vector<double>&				kseedsl,
		vector<double>&				kseedsa,
		vector<double>&				kseedsb,
		vector<double>&				kseedsx,
		vector<double>&				kseedsy,
		const vector<double>&		edges);
	//============================================================================
	// Detect color edges, to help PerturbSeeds()
	//============================================================================
	void DetectLabEdges(
		const double*				lvec,
		const double*				avec,
		const double*				bvec,
		const int&					width,
		const int&					height,
		vector<double>&				edges);
	//============================================================================
	// Post-processing of SLIC segmentation, to avoid stray labels.
	//============================================================================
	void EnforceLabelConnectivity(
		const int*					labels,
		const int					width,
		const int					height,
		int*&						nlabels,//input labels that need to be corrected to remove stray labels
		int&						numlabels,//the number of labels changes in the end if segments are removed
		const int&					K); //the number of superpixels desired by the user
	//============================================================================
	// Post-processing of SLIC supervoxel segmentation, to avoid stray labels.
	//============================================================================
	void EnforceSupervoxelLabelConnectivity(
		int**&						labels,//input - previous labels, output - new labels
		const int&					width,
		const int&					height,
		const int&					depth,
		int&						numlabels,
		const int&					STEP);

protected:
	int										m_width;
	int										m_height;
	int										m_depth;

	double*									m_lvec;
	double*									m_avec;
	double*									m_bvec;

	double**								m_lvecvec;
	double**								m_avecvec;
	double**								m_bvecvec;
};

#endif // !defined(_SLIC_H_INCLUDED_)
