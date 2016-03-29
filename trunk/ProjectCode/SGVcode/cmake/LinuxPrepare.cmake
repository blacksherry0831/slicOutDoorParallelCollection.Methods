############C++11#############################
macro(C11_Config)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

endmacro()
#############################################