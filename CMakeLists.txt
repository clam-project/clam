cmake_minimum_required(VERSION 3.22)

project(CLAMModernized
	VERSION 1.4.1
	LANGUAGES C CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)
include(ClamSources)

option(BUILD_SHARED_LIBS "Build shared libraries by default" ON)
option(CLAM_BUILD_LIBRARIES "Build the CLAM C++ libraries" ON)
option(CLAM_BUILD_GUI_APPS "Enable Qt code that has been ported to CMake" OFF)
option(CLAM_BUILD_SMSTOOLS "Build the SMSTools Qt application" OFF)

if(CLAM_BUILD_LIBRARIES)
	add_subdirectory(CLAM)
endif()

if(CLAM_BUILD_GUI_APPS)
	add_subdirectory(NetworkEditor)
endif()

if(CLAM_BUILD_SMSTOOLS)
	add_subdirectory(SMSTools)
endif()
