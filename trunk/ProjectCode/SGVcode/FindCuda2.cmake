find_package(CUDA)
if(CUDA_FOUND)
 try_run(RUN_RESULT_VAR COMPILE_RESULT_VAR
  ${CMAKE_BINARY_DIR} 
  ${CMAKE_CURRENT_SOURCE_DIR}/has_cuda_gpu.c
  CMAKE_FLAGS 
   -DINCLUDE_DIRECTORIES:STRING=${CUDA_TOOLKIT_INCLUDE}
   -DLINK_LIBRARIES:STRING=${CUDA_CUDART_LIBRARY}
  COMPILE_OUTPUT_VARIABLE COMPILE_OUTPUT_VAR
  RUN_OUTPUT_VARIABLE RUN_OUTPUT_VAR)
 message("${RUN_OUTPUT_VAR}") # Display number of GPUs found
 # COMPILE_RESULT_VAR is TRUE when compile succeeds
 # RUN_RESULT_VAR is zero when a GPU is found
 if(COMPILE_RESULT_VAR AND NOT RUN_RESULT_VAR)
  set(CUDA_HAVE_GPU TRUE CACHE BOOL "Whether CUDA-capable GPU is present")
 else()
  set(CUDA_HAVE_GPU FALSE CACHE BOOL "Whether CUDA-capable GPU is present")
 endif()
endif(CUDA_FOUND)