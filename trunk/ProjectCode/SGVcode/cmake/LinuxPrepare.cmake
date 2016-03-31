############C++11#############################
# Determines whether or not the compiler supports C++11
macro(check_for_cxx11_compiler _VAR)
    #message(STATUS "Checking for C++11 compiler")
    set(${_VAR})
    if((MSVC AND (MSVC10 OR MSVC11 OR MSVC12)) OR
       (CMAKE_COMPILER_IS_GNUCXX AND NOT ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 4.6) OR
       (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 3.1))
        set(${_VAR} 1)
        #message(STATUS "Checking for C++11 compiler - available")
    else()
        #message(STATUS "Checking for C++11 compiler - unavailable")
    endif()
endmacro()

# Sets the appropriate flag to enable C++11 support
macro(enable_cxx11)
    if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
		message(STATUS "-std=c++11")
    endif()
endmacro()


macro(C11_Config)

check_for_cxx11_compiler(CXX11_COMPILER)

# If a C++11 compiler is available, then set the appropriate flags
if(CXX11_COMPILER)
    enable_cxx11()
	#message(STATUS "Checking for C++11 compiler - available")
endif()
#if(CMAKE_COMPILER_IS_GNUCXX)
#    add_definitions(-std=gnu++11)
#endif()
IF(CMAKE_COMPILER_IS_GNUCXX)
set (CMAKE_CXX_FLAGS "-fpermissive")
#set( CMAKE_C_FLAGS   "-fstack-protector -fstack-protector-all" )
#set( CMAKE_C_FLAGS_DEBUG   "-O2 -Wall -ggdb" )
#set( CMAKE_C_FLAGS_RELEASE   "-Os -Wall" )

#set( CMAKE_CXX_FLAGS "-fstack-protector -fstack-protector-all" )
#set( CMAKE_CXX_FLAGS_DEBUG "-O2 -Wall -ggdb" )
#set( CMAKE_CXX_FLAGS_RELEASE "-Os -Wall" )

ENDIF()

endmacro()
#############################################