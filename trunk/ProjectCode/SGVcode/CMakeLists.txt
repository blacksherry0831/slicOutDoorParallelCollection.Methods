#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#项目名称
PROJECT(SGV_cmake)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
   
ELSE (CMAKE_SYSTEM_NAME MATCHES "Windows")  
   INCLUDE(FindCuda.cmake)  
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")  

MESSAGE(STATUS "###################################")  
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")   
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
    MESSAGE(STATUS "current platform: Linux ")  
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")  
    MESSAGE(STATUS "current platform: Windows")
    #ADD_DEFINITIONS(-D_AFXDLL)　
	#SET(CMAKE_MFC_FLAG 2)
	#ADD_EXECUTABLE(detect WIN32 ${DIR_SRCS})
	set(CMAKE_MFC_FLAG 2)
   # set_target_properties(MyApp PROPERTIES
   # COMPILE_DEFINITIONS _AFXDLL,_UNICODE,UNICODE,_BIND_TO_CURRENT_CRT_VERSION,_BIND_TO_CURRENT_MFC_VERSION
   # LINK_FLAGS "/ENTRY:\"wWinMainCRTStartup\"")

   #INCLUDE(FindCudaLibrary.cmake)
  

   INCLUDE_DIRECTORIES( #相关头文件的目录		
		#${SGV_cmake_SOURCE_DIR}/opencv/build/include
		#${SGV_cmake_SOURCE_DIR}/opencv/build/include/opencv2
		#${SGV_cmake_SOURCE_DIR}/opencv/build/include/opencv	
		C:/opencv/build/include
		C:/opencv/build/include/opencv2
		C:/opencv/build/include/opencv	
		${SGV_cmake_SOURCE_DIR}/CuiLib
		${SGV_cmake_SOURCE_DIR}
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
		opencv_ocl249d.lib
		cudart.lib
		kernel32.lib
		user32.lib
		gdi32.lib
		winspool.lib
		comdlg32.lib
		advapi32.lib
		shell32.lib
		ole32.lib
		oleaut32.lib
		uuid.lib
		odbc32.lib
		odbccp32.lib)

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")  
    MESSAGE(STATUS "current platform: FreeBSD")  
ELSE ()  
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")  
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
  
MESSAGE(STSTUS "###################################")  



SET(SRC_LIST                	  
               CuiLib/FilePathName/FileNameSplit.cpp
			   CuiLib/FilePathName/PointWithDistance.cpp
			   CuiLib/ML/Kmean/Kmean.cpp
			 
			    CuiLib/SGVcode/ImageType.h
			    CuiLib/SGVcode/ManhattanHistogram.cpp
				CuiLib/SGVcode/SeparateSpBlock.cpp
				CuiLib/SGVcode/SpectralClustering.cpp
				CuiLib/SGVcode/SpAnalyze.cpp
				CuiLib/SGVcode/HsHg.cpp
				CuiLib/SGVcode/SpSetProperty.cpp
				CuiLib/SGVcode/ImageMemData.cpp
				CuiLib/SGVcode/ImageData.cpp
				CuiLib/SGVcode/SLIC.cpp
				CuiLib/SGVcode/cui_GeneralImgProcess.h
				CuiLib/SGVcode/cui_GeneralImgProcess.cpp
				CuiLib/SGVcode/Cui_DrawDepth.cpp
				CuiLib/SGVcode/ComputeSVG.cpp
				CuiLib/SGVcode/ComputeSVG2.cpp
				CuiLib/SGVcode/SpectralClusterInDoor.cpp
				CuiLib/SGVcode/SplitTreeandBuilding.cpp
				CuiLib/SGVcode/LabColorAnalyzeCluster.cpp
				CuiLib/SGVcode/CuiHough/CuiHough3.cpp
				CuiLib/SGVcode/CuiHough/HoughAnalyze.cpp
				CuiLib/SGVcode/CuiHough/HoughAnalyze.cpp
				CuiLib/SGVcode/Harr/CuiHarrTransformLet.cpp
				CuiLib/SGVcode/Harr/HarrWaveletTransImg.cpp
				
				CuiLib/SGVcode/ColorAnalyzeOnLab.cpp
				CuiLib/SGVcode/rtthread.cpp
				CuiLib/SGVcode/MallocCui.cpp
				CuiLib/SGVcode/LabColorAnalyzeCluster_veins.cpp
				CuiLib/SGVcode/ColorBarCluster.cpp
				
			    CuiLib/tinyxml2/tinyxml2.cpp
			    
			    CuiLib/SVMcode/TrainTreeBuilding.cpp
				CuiLib/SVMcode/TrainImgUseSvm.cpp
			    CuiLib/SVMcode/svmlight/pr_loqo.cpp
				CuiLib/SVMcode/svmlight/svm_common.cpp
				CuiLib/SVMcode/svmlight/svm_learn.cpp
				CuiLib/SVMcode/svmlight/svm_loqo.cpp
				 
				 CUDA_Code.cpp
			     fuzzy.cu
			     SlicKernel.cu
			     HarrWaveletKernel.cu
				 main.cpp

 )

#AUX_SOURCE_DIRECTORY(. SRC_LIST) #把当前目录(.)下所有源代码文件和头文件加入变量SRC_LIST

ADD_EXECUTABLE(SpectralClusteringGndVSkyOutDoor main.cpp) #生成应用程序SpectralClusteringGndVSkyOutDoor (在windows下会自动生成hello.exe)