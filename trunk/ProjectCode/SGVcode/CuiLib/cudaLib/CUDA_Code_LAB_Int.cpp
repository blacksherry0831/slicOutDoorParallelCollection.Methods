#include "CUDA_Code_LAB_Int.h"
/*------------------------------------------------------------------------------------------*/
#include <MY_SDK_LIB/TimeMeasure.hpp>
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/
extern "C"
void PerformSuperpixelSLIC_gpu_lab_host_int(
	const int			_kseedsSize,
	const int			_width,
	const int			_height,
	const int*			_host_matrix_l,
	const int*			_host_matrix_a,
	const int*			_host_matrix_b,
	
	int*				_host_kseedsl,
	int*				_host_kseedsa,
	int*				_host_kseedsb,
	int*				_host_kseedsx,
	int*				_host_kseedsy,
	int*				_host_klabels,

	const unsigned long		_f_m,
	const unsigned long		_f_d);

/*------------------------------------------------------------------------------------------*/
/**
*cuda����ʵ�г������㷨��
*
*
*@param    alpha  �ǶȲ���alpha
*@param    betta  ģֵbetta
*@param    gama   ����gama
*@param    fai    XY����
*@param    kseedsSize  ���Ӹ���
*@param    m_width   ͼ����
*@param    m_height  ͼ��߶�
*@param	   m_lvec  ͼ��L����
*@param	   m_avec  ͼ��A����
*@param	   m_bvec  ͼ��B����
*@param	   kseedsl  ����L����
*@param	   kseedsa  ����A����
*@param	   kseedsb  ����B����
*@param	   kseedsx  ����X����
*@param	   kseedsy  ����Y����
*@param	   klabels  ͼ��ͼ��ı궨ͼ��
*@param	   STEP     δʹ��
*@param	   edgemag  δʹ��
*@param	   M        δʹ���ܶ�����
*
*/
/*------------------------------------------------------------------------------------------*/
void PerformSuperpixelSLIC_cuda_lab_Int_F(
	const int kseedsSize,
	const int m_width,
	const int m_height,

	int*				m_lvec,
	int*				m_avec,
	int*				m_bvec,
	
	int*				kseedsl,
	int*				kseedsa,
	int*				kseedsb,
	int*				kseedsx,
	int*				kseedsy,

	int*				klabels,
	const unsigned long		_f_m,
	const unsigned long		_f_d)
{
			TimeMeasure tm("Superpixel.GPU.Int");

			PerformSuperpixelSLIC_gpu_lab_host_int(
				kseedsSize,
				m_width,
				m_height,

				m_lvec,
				m_avec,
				m_bvec,
				
				kseedsl,
				kseedsa,
				kseedsb,
				kseedsx,
				kseedsy,

				klabels,
				_f_m,
				_f_d);

}
/*------------------------------------------------------------------------------------------*/
/**
*
*
*
*/
/*------------------------------------------------------------------------------------------*/