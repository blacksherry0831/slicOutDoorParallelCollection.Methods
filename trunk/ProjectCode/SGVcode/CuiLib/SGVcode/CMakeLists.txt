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
				####
				CuiHough/CuiHough3.cpp
				CuiHough/HoughAnalyze.cpp
				CuiHough/HoughAnalyze.cpp
				###
				Harr/CuiHarrTransformLet.cpp
				Harr/HarrWaveletTransImg.cpp)  
 #ImageType.h

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
   
ELSE (CMAKE_SYSTEM_NAME MATCHES "Windows")  
    set(CMAKE_MFC_FLAG 2)
    INCLUDE_DIRECTORIES( #相关头文件的目录		
		#${SGV_cmake_SOURCE_DIR}/opencv/build/include
		#${SGV_cmake_SOURCE_DIR}/opencv/build/include/opencv2
		#${SGV_cmake_SOURCE_DIR}/opencv/build/include/opencv	
		C:/opencv/build/include
		C:/opencv/build/include/opencv2
		C:/opencv/build/include/opencv
	)

	LINK_DIRECTORIES(#
     	C:/opencv/build/x64/vc10/lib
	)
	link_libraries(
		opencv_calib3d249d.lib
		opencv_contrib249d.lib
		opencv_core249d.lib
		opencv_features2d249d.lib
		opencv_flann249d.lib
		opencv_gpu249d.lib
		opencv_highgui249d.lib
		opencv_imgproc249d.lib
		opencv_legacy249d.lib
		opencv_ml249d.lib
		opencv_objdetect249d.lib
		opencv_ts249d.lib
		opencv_video249d.lib
		opencv_ocl249d.lib)
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")  

ADD_LIBRARY(SGVcode  STATIC     ${DIR_SRCS}  )
TARGET_LINK_LIBRARIES(SGVcode FilePathName)


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