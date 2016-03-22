# The script creates the following macros:

# CUDA_INCLUDE_DIRECTORIES( path0 path1 ... )

# -- Sets the directories that should be passed to nvcc

#    (e.g. nvcc -Ipath0 -Ipath1 ... ). These paths usually contain other .cu

#    files.

#

# CUDA_ADD_LIBRARY( cuda_target file0 file1 ... )

# -- Creates a shared library "cuda_target" which contains all of the source

#    (*.c, *.cc, etc.) specified and all of the nvcc'ed .cu files specified.

#    All of the specified source files and generated .c files are compiled

#    using the standard CMake compiler, so the normal INCLUDE_DIRECTORIES,

#    LINK_DIRECTORIES, and TARGET_LINK_LIBRARIES can be used to affect their

#    build and link.

#

# CUDA_ADD_EXECUTABLE( cuda_target file0 file1 ... )

# -- Same as CUDA_ADD_LIBRARY except that an exectuable is created.

# FindCuda.cmake

# 设置 device 和 emulation 属性

IF( NOT CUDA_BUILD_TYPE )

SET( CUDA_BUILD_TYPE "Device" CACHE STRING "Cuda build type: Emulation or Device" )

ENDIF( NOT CUDA_BUILD_TYPE )

IF( CUDA_BUILD_TYPE MATCHES "Device" )

SET( CUDA_NVCC_FLAGS "")

ELSE( CUDA_BUILD_TYPE MATCHES "Device" )

SET( CUDA_NVCC_FLAGS --device-emulation -D_DEVICEEMU -g )

ENDIF( CUDA_BUILD_TYPE MATCHES "Device")

# 设置DEBUG 和 RELEASE 属性

IF( NOT CUDA_DEBUG_TYPE )

SET( CUDA_DEBUG_TYPE "Debug" CACHE STRING "Debug or Release" )

ENDIF( NOT CUDA_DEBUG_TYPE )

IF( CUDA_DEBUG_TYPE MATCHES "Debug" )

SET( CUDA_DEBUG -D_DEBUG )

SET( CUDA_DEBUG_FLAGS /0d,/MTd )

ELSE( CUDA_DEBUG_TYPE MATCHES "Debug" )

SET( CUDA_DEBUG "" )

SET( CUDA_DEBUG_FLAGS /2d,/MT )

ENDIF( CUDA_DEBUG_TYPE MATCHES "Debug" )

# 寻找CUDA安装路径

IF( NOT CUDA_INSTALL_PREFIX )

# 寻找 nvcc.exe

FIND_PROGRAM( CUDA_NVCC

nvcc

PATHS $ENV{CUDA_BIN_PATH}

)

IF( NOT CUDA_NVCC )

MESSAGE( STATUS "Could not find nvcc" )

ENDIF( NOT CUDA_NVCC )


# 找 CUDA 库所在的路径

FIND_PATH( CUDA_INSTALL_PREFIX

bin/nvcc.exe

PATHS $ENV{CUDA_BIN_PATH}/../

)

IF( NOT EXISTS ${CUDA_INSTALL_PREFIX} )

MESSAGE( STATUS "Specify CUDA_INSTALL_PREFIX" )

ENDIF( NOT EXISTS ${CUDA_INSTALL_PREFIX} )

ENDIF( NOT CUDA_INSTALL_PREFIX )

#  寻找 cuda 和 cutil 库的头文件和库文件

INCLUDE( FindCudaLibrary.cmake )

##################################################

# 自定义规则命令

##################################################

MACRO( CUDA_ADD_CUSTOM_COMMANDS )

  # 非 *.cu 文件

  SET( target_srcs "" )

  # *.cu 文件

  SET( cuda_cu_sources "" )

  FOREACH( file ${ARGN} )

IF( ${file} MATCHES ".*\\.cu$" )

GET_FILENAME_COMPONENT( file_we ${file} NAME_WE )

# 设置输出文件

SET( generated_object "${file_we}.obj" )

# 设置文件

SET( source_file ${file} )


# 设置文件属性

SET_SOURCE_FILES_PROPERTIES( ${source_file} PROPERTIES CPLUSPLUS ON )


# 自定义命令 － CUDA编译命令

ADD_CUSTOM_COMMAND(

OUTPUT ../bin/$(ConfigurationName)/${generated_object}

MAIN_DEPENDENCY ${source_file}

COMMAND ${CUDA_NVCC}

ARGS -c

${CUDA_DEBUG}

${CUDA_NVCC_FLAGS}

-Xcompiler

/EHsc,/W3,/nologo,/Wp64,/Zi,/RTC1,${CUDA_DEBUG_FLAGS}

${CUDA_NVCC_INCLUDE_ARGS}

${CUDA_INCLUDE_COMMAND_DIR}

-o ../bin/$(ConfigurationName)/${generated_object}

#${source_file}

$(InputPath)    # 注意：这些变量是针对于 VS2005的

COMMENT "Building NVCC ${file}: ${generated_object}\n"

)


SET( cuda_cu_sources ${cuda_cu_sources} ${source_file} )

ELSE( ${file} MATCHES ".*\\.cu$" )

SET( target_srcs ${target_srcs} ${file} )

ENDIF( ${file} MATCHES ".*\\.cu$" )

  ENDFOREACH( file ${ARGN} )

ENDMACRO( CUDA_ADD_CUSTOM_COMMANDS )

##################################################

# 加头文件路径宏

##################################################

MACRO( CUDA_INCLUDE_DIRECTORIES )

  FOREACH( dir ${ARGN} )

SET( CUDA_NVCC_INCLUDE_ARGS ${CUDA_NVCC_INCLUDE_ARGS} -I${dir} )  

  ENDFOREACH( dir ${ARGN} )

ENDMACRO( CUDA_INCLUDE_DIRECTORIES )

##################################################

# 建立生成静态库工程

##################################################

MACRO( CUDA_ADD_LIBRARY cuda_target )

  # 生成命令规则

  CUDA_ADD_CUSTOM_COMMANDS( ${ARGN} )

  # 加入头文件路径

  INCLUDE_DIRECTORIES( ${CUDA_INCLUDE_DIR} )

  # 库工程

  ADD_LIBRARY( ${cuda_target}

${target_srcs}

${cuda_cu_sources}

)

  # 加入工程所需的库名

  TARGET_LINK_LIBRARIES( ${cuda_target}

${CUDA_LIBRARIES}

)


ENDMACRO( CUDA_ADD_LIBRARY cuda_target )

##################################################

# 建立生成可执行文件的工程

##################################################

MACRO( CUDA_ADD_EXECUTABLE cuda_target )

  #  生成命令规则

  CUDA_ADD_CUSTOM_COMMANDS( ${ARGN} )

  # 加入头文件路径

  INCLUDE_DIRECTORIES( ${CUDA_INCLUDE_DIR} )

  # 加入库路径

  LINK_DIRECTORIES( ${CUDA_LIBRARY_DIR} )

  #可执行工程

  ADD_EXECUTABLE( ${cuda_target}

${target_srcs}

${cuda_cu_sources}

)


  # 加入的库名

  TARGET_LINK_LIBRARIES( ${cuda_target}

${CUDA_LIBRARIES}

)

ENDMACRO( CUDA_ADD_EXECUTABLE cuda_target )