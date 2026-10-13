cmake_minimum_required(VERSION 3.22)

project(CLAMModernized
	VERSION 1.4.1
	LANGUAGES C CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)
include(CTest)
include(ClamSources)

option(CLAM_BUILD_LIBRARIES "Build the CLAM C++ libraries (OFF: build the apps against an installed CLAM)" ON)
option(CLAM_BUILD_GUI_APPS "Build NetworkEditor, its tools and the clam_qtmonitors library" OFF)
option(CLAM_BUILD_SMSTOOLS "Build the SMSTools Qt application" OFF)

# Let installed executables find the installed CLAM libraries.
if(NOT DEFINED CMAKE_INSTALL_RPATH)
	set(CMAKE_INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
endif()

if(CLAM_BUILD_LIBRARIES)
	add_subdirectory(CLAM)
endif()

if(CLAM_BUILD_GUI_APPS)
	add_subdirectory(NetworkEditor)
endif()

if(CLAM_BUILD_SMSTOOLS)
	add_subdirectory(SMSTools)
endif()
