# Builds a small program against an installed CLAM, through its CMake
# package (CLAMConfig.cmake) or, with -DUSE_PKG_CONFIG=ON, its pkg-config
# files. Used by CI to check the install tree:
#   cmake -S CLAM/test/InstallConsumer -B consumer -DCMAKE_PREFIX_PATH=<prefix>
cmake_minimum_required(VERSION 3.22)
project(ClamInstallConsumer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(USE_PKG_CONFIG "Use the clam_*.pc files instead of CLAMConfig.cmake" OFF)

add_executable(clam_install_consumer main.cxx)
if(USE_PKG_CONFIG)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(CLAM REQUIRED IMPORTED_TARGET clam_audioio)
	target_link_libraries(clam_install_consumer PRIVATE PkgConfig::CLAM)
else()
	find_package(CLAM CONFIG REQUIRED)
	target_link_libraries(clam_install_consumer PRIVATE CLAM::processing CLAM::audioio)
	# Installed along with NetworkEditor
	if(TARGET CLAM::qtmonitors)
		add_executable(clam_qtmonitors_consumer qtmonitors.cxx)
		set_target_properties(clam_qtmonitors_consumer PROPERTIES AUTOMOC ON)
		target_link_libraries(clam_qtmonitors_consumer PRIVATE CLAM::qtmonitors)
	endif()
endif()
