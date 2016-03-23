
SET(DIR_SRCS   
		tinyxml2.cpp		 
		 )  	



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
	SET(BUILD_SHARED_LIBS ON)
	
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")  



ADD_LIBRARY(tinyxml2   STATIC    ${DIR_SRCS}  )



	