#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#项目名称
PROJECT(SGV_cmake)
###############################################################################
INCLUDE(cmake/SetMFC.cmake)
INCLUDE(cmake/FindOpencv.cmake)
###############################################################################
#INCLUDE(FindCuda.cmake)
INCLUDE_DIRECTORIES( #相关头文件的目录
	${SGV_cmake_SOURCE_DIR}/CuiLib
	${SGV_cmake_SOURCE_DIR}
)
ADD_SUBDIRECTORY(CuiLib)
###############################################################################
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")  
###############################################################################
############配置OPENCV#########################################################
ConfigOpenCv()
###############################################################################
############配置CUDA#########################################################
MESSAGE(STATUS "###################################")  
MESSAGE(STATUS "cuda")   
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
	
ELSE (CMAKE_SYSTEM_NAME MATCHES "Windows")  
    INCLUDE(FindCuda.cmake)
    IF(CUDA_HAVE_GPU)
      MESSAGE(STATUS "cudart.lib")  
	  link_libraries(cudart.lib)
	ENDIF()	
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
###############################################################################
#################编译器配置####################################################
MESSAGE(STATUS "###################################")  
MESSAGE(STATUS "编译器配置")   
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")   
	 ConfigMFC_2()
	link_libraries(${WindowsLib})
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  
MESSAGE(STSTUS "###################################")
###############################################################################
#AUX_SOURCE_DIRECTORY(. SRC_LIST) #把当前目录(.)下所有源代码文件和头文件加入变量SRC_LIST
#生成应用程序SpectralClusteringGndVSkyOutDoor (在windows下会自动生成hello.exe)
ADD_EXECUTABLE(SpectralClusteringGndVSkyOutDoor main.cpp)
TARGET_LINK_LIBRARIES(SpectralClusteringGndVSkyOutDoor 
					            SGVcode
					  )  