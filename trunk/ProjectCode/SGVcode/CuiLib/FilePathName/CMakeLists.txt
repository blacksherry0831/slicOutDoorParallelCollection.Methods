#add_library(FilePathName FileNameSplit.cpp PointWithDistance.cpp）
SET(DIR_SRCS   
				FileNameSplit.cpp 
				PointWithDistance.cpp )  	

ADD_LIBRARY(FilePathName  STATIC     ${DIR_SRCS}  )
TARGET_LINK_LIBRARIES(FilePathName tinyxml2)

if(MSVC_VERSION)
  set_target_properties(tinyxml2 PROPERTIES
    LINK_FLAGS " /INCREMENTAL:NO /MANIFEST /NOLOGO"
 #COMPILE_FLAGS "-DHELLO_EXPORTS"  #ajouté in last a verif demin
 )
endif(MSVC_VERSION)





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



	