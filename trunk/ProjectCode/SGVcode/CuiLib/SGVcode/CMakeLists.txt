C11_Config()
#####################################
SET(DIR_SRCS    ManhattanHistogram.cpp
				SeparateSpBlock.cpp
				SpectralClustering.cpp
				SpAnalyze.cpp
				HsHg.cpp
				SpSetProperty.cpp
				
				ImageMemData.cpp
				ImageData.cpp
				SLIC.cpp
				#cui_GeneralImgProcess.h
				cui_GeneralImgProcess.cpp
				Cui_DrawDepth.cpp
				
				ComputeSVG.cpp
				ComputeSVG2.cpp
				SpectralClusterInDoor.cpp
				SplitTreeandBuilding.cpp
				LabColorAnalyzeCluster.cpp	
				
				ColorAnalyzeOnLab.cpp
				#rtthread.cpp
				MallocCui.cpp
				#LabColorAnalyzeCluster_veins.cpp
				ColorBarCluster.cpp
				##################
				SceneDetermine.cpp
				####
				CuiHough/CuiHough3.cpp
				CuiHough/HoughAnalyze.cpp
				CuiHough/HoughAnalyze.cpp
				###
				Harr/CuiHarrTransformLet.cpp
				Harr/HarrWaveletTransImg.cpp)  
 #ImageType.h

############≈‰÷√OPENCV#########################################################
ConfigOpenCv()
###############################################################################
ConfigMFC_2()
###############################################################################
SET(LINK_LIB 
	         FilePathName
	         cudaLib
	         Kmean
	         SVMcode
	)
ADD_LIBRARY(SGVcode  STATIC     ${DIR_SRCS}  )
TARGET_LINK_LIBRARIES(SGVcode ${LINK_LIB})


#CuiLib/SGVcode/ImageType.h
#CuiLib/SGVcode/ManhattanHistogram.cpp
#CuiLib/SGVcode/SeparateSpBlock.cpp
#CuiLib/SGVcode/SpectralClustering.cpp
#CuiLib/SGVcode/SpAnalyze.cpp
#CuiLib/SGVcode/HsHg.cpp
#CuiLib/SGVcode/SpSetProperty.cpp
#CuiLib/SGVcode/ImageMemData.cpp
#CuiLib/SGVcode/ImageData.cpp
#CuiLib/SGVcode/SLIC.cpp
#CuiLib/SGVcode/cui_GeneralImgProcess.h
#CuiLib/SGVcode/cui_GeneralImgProcess.cpp
#CuiLib/SGVcode/Cui_DrawDepth.cpp
#CuiLib/SGVcode/ComputeSVG.cpp
#CuiLib/SGVcode/ComputeSVG2.cpp
#CuiLib/SGVcode/SpectralClusterInDoor.cpp
#CuiLib/SGVcode/SplitTreeandBuilding.cpp
#CuiLib/SGVcode/LabColorAnalyzeCluster.cpp
#CuiLib/SGVcode/CuiHough/CuiHough3.cpp
#CuiLib/SGVcode/CuiHough/HoughAnalyze.cpp
#CuiLib/SGVcode/CuiHough/HoughAnalyze.cpp
#CuiLib/SGVcode/Harr/CuiHarrTransformLet.cpp
#CuiLib/SGVcode/Harr/HarrWaveletTransImg.cpp				
#CuiLib/SGVcode/ColorAnalyzeOnLab.cpp
#CuiLib/SGVcode/rtthread.cpp
#CuiLib/SGVcode/MallocCui.cpp
#CuiLib/SGVcode/LabColorAnalyzeCluster_veins.cpp
#CuiLib/SGVcode/ColorBarCluster.cpp