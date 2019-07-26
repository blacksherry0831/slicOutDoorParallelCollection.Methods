CMAKE_MINIMUM_REQUIRED(VERSION 3.8.1)
######################################
CONFIGQT()
ConfigOpenCv()
######################################
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
######################################
   
# Tell CMake to create the helloworld executable
					
add_executable(EXE_MAIN_UNIT_TEST
							main_unit_test.cpp
				)

TARGET_LINK_LIBRARIES(EXE_MAIN_UNIT_TEST
								)
				
######################################
