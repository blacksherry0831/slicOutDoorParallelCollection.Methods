############����OPENCV#########################################################
C11_Config()
ConfigOpenCv()
###############################################################################
ConfigMFC_2()
###############################################################################
SET(DIR_SRCS   
		tinyxml2.cpp		 
		 )

ADD_LIBRARY(tinyxml2   STATIC    ${DIR_SRCS}  )



	