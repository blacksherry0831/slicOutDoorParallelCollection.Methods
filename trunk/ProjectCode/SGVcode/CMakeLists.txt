#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#项目名称
PROJECT(SGV_cmake)
###############################################################################
INCLUDE(cmake/LinuxPrepare.cmake)
C11_Config()
INCLUDE(cmake/SetMFC.cmake)
INCLUDE(cmake/FindOpencv.cmake)
INCLUDE(cmake/setCUDA.cmake)
###############################################################################
INCLUDE_DIRECTORIES( #相关头文件的目录
	${SGV_cmake_SOURCE_DIR}/CuiLib
	${SGV_cmake_SOURCE_DIR}
)
ADD_SUBDIRECTORY(CuiLib)
###############################################################################
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")  
############配置OPENCV#########################################################

ConfigOpenCv()
############配置CUDA#########################################################
ConfigCUDA()
#################编译器配置####################################################
MESSAGE(STATUS "###################################")  

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")   
	 ConfigMFC_2()
	 MESSAGE(STATUS "MFC配置")   
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