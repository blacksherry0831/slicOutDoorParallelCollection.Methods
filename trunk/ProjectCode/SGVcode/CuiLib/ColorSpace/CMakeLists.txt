#####################################
C11_Config()
############≈‰÷√OPENCV#########################################################
ConfigOpenCv()
###############################################################################
ConfigMFC_2()
###############################################################################
SET(BASE_SRCS   
		DbgRangeCheck.cpp
		ImageColorSpaceLAB.cpp
		ImageColorSpaceThetaML.cpp
		DistanceColorSpace.cpp
		
		ColorSpaceLabTable.cpp
		ColorSpaceLAB.cpp
		
		)  
SET(BASE_LINK_LIB 
	         
	)
ADD_LIBRARY(ColorSpace  STATIC     ${BASE_SRCS}  )
TARGET_LINK_LIBRARIES(ColorSpace ${BASE_LINK_LIB})
###############################################################################