# - Try to find CUDA

# Once done this will define

# neek known the var ENV CUDA_BIN_PATH and ENV NVSDKCUDA_ROOT

#  

#  CUDA_FOUND        - system has CUDA

#  CUDA_INCLUDE_DIR  - the include directory

#  CUDA_LIBRARY_DIR  - the directory containing the libraries

#  CUDA_LIBRARIES    - Link these to use CUDA

#  

IF( WIN32 )

  # 寻找CUDA库路径

  FIND_PATH( CUDA_LIBRARY_DIR

			NAMES cublas.lib cudart.lib cufft.lib

			PATHS $ENV{CUDA_BIN_PATH}/../lib

  )

  # 寻找CUDA头文件路径

  FIND_PATH( CUDA_INCLUDE_DIR

			NAMES cuda.h cublas.h cufft.h

			PATHS $ENV{CUDA_BIN_PATH}/../include

  )

  # 寻找CUDA SDK库路径

  FIND_PATH( CUDA_SDK_LIBRARY_DIR

			NAMES cutil.lib glut32.lib

			PATHS $ENV{NVSDKCUDA_ROOT}/common/lib

  )

  # 寻找CUDA SDK头文件路径

  FIND_PATH( CUDA_SDK_INCLUDE_DIR

			NAMES cutil.h cutil_gl_error.h

			PATHS $ENV{NVSDKCUDA_ROOT}/common/inc

  )

ELSE( WIN32 )

ENDIF( WIN32 )

SET( CUDA_FOUND FALSE )

IF ( CUDA_INCLUDE_DIR AND CUDA_LIBRARY_DIR )

  SET ( CUDA_FOUND TRUE )

  SET ( CUDA_INCLUDE_COMMAND_DIR

    -I${CUDA_INCLUDE_DIR}

    -I${CUDA_SDK_INCLUDE_DIR}

    )

  SET ( CUDA_INCLUDE_DIR

    ${CUDA_INCLUDE_DIR}

    ${CUDA_SDK_INCLUDE_DIR}

    )

  SET( CUDA_LIBRARY_DIR

${CUDA_LIBRARY_DIR}

${CUDA_SDK_LIBRARY_DIR}

)

  SET ( CUDA_LIBRARIES cutil32D.lib cudart.lib cufft.lib cublas.lib cuda.lib )

ENDIF ( CUDA_INCLUDE_DIR AND CUDA_LIBRARY_DIR )