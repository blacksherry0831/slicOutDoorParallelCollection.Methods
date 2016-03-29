SET(OpencvInclude_Win64
		C:/opencv/build/include
		C:/opencv/build/include/opencv2
		C:/opencv/build/include/opencv
	 )
SET(OpencvLink_Win64
	  C:/opencv/build/x64/vc10/lib
	 )
SET(OpencvLib_Win64
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
	 )
###############################################################################
MESSAGE(STATUS "###################################")  
MESSAGE(STATUS "opencv")  
############����OPENCV#########################################################
macro(ConfigOpenCv)
	IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
		find_package( OpenCV REQUIRED )
		target_link_libraries( DisplayImage ${OpenCV_LIBS} )
	ELSE (CMAKE_SYSTEM_NAME MATCHES "Windows")  
	   INCLUDE_DIRECTORIES(${OpencvInclude_Win64})
	   LINK_DIRECTORIES(${OpencvLink_Win64})
	   link_libraries(${OpencvLib_Win64})
	ENDIF ()  
endmacro()


###############################################################################