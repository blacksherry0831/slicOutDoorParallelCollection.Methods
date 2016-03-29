
ConfigCUDA()

SET(DIR_SRCS   
	     fuzzy.cu
	     HarrWaveletKernel.cu
	     SlicKernel.cu
		 CUDA_Code.cpp		 
		 )  	



############≈‰÷√OPENCV#########################################################
ConfigOpenCv()
###############################################################################
ConfigMFC_2()
###############################################################################



CUDA_ADD_LIBRARY(cudaLib   STATIC    ${DIR_SRCS}  )



	