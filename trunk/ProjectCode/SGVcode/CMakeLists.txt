#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#项目名称
PROJECT(SGV_cmake)




MESSAGE(STSTUS "###################################")  
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

   INCLUDE_DIRECTORIES( #相关头文件的目录		
		#${SGV_cmake_SOURCE_DIR}/opencv/build/include
		#${SGV_cmake_SOURCE_DIR}/opencv/build/include/opencv2
		#${SGV_cmake_SOURCE_DIR}/opencv/build/include/opencv	
		C:/opencv/build/include
		C:/opencv/build/include/opencv2
		C:/opencv/build/include/opencv	
		${SGV_cmake_SOURCE_DIR}/CuiLib
	)

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")  
    MESSAGE(STATUS "current platform: FreeBSD")  
ELSE ()  
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")  
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
  
MESSAGE(STSTUS "###################################")  



SET(SRC_LIST main.cpp)

#AUX_SOURCE_DIRECTORY(. SRC_LIST) #把当前目录(.)下所有源代码文件和头文件加入变量SRC_LIST

ADD_EXECUTABLE(SpectralClusteringGndVSkyOutDoor ${SRC_LIST}) #生成应用程序SpectralClusteringGndVSkyOutDoor (在windows下会自动生成hello.exe)