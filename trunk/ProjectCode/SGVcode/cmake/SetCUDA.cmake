macro(ConfigCUDA)

 
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
	  #Searching CUDA
	  FIND_PACKAGE(CUDA) 
	  #Include the FindCUDA script
	  INCLUDE(FindCUDA) 
ELSE (CMAKE_SYSTEM_NAME MATCHES "Windows")  
	  #Searching CUDA
	  FIND_PACKAGE(CUDA) 
	  #Include the FindCUDA script
	  INCLUDE(FindCUDA) 
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")  

endmacro()