include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckLibraryExists)
include(CMakePackageConfigHelpers)
include(FindPkgConfig)

clam_parse_version(CLAM_VERSION CLAM "${CMAKE_CURRENT_SOURCE_DIR}/CHANGES")

set(CLAM_XML_BACKEND "xercesc" CACHE STRING "XML backend: xercesc or none")
set_property(CACHE CLAM_XML_BACKEND PROPERTY STRINGS xercesc none)

option(CLAM_WITH_CHECKS "Enable CLAM runtime checks" ON)
option(CLAM_WITH_RELEASE_ASSERTS "Enable CLAM asserts in release builds" OFF)
option(CLAM_WITH_DOUBLE "Use double instead of float for CLAM::TData" OFF)
option(CLAM_WITH_LADSPA "Enable LADSPA support" ON)
option(CLAM_WITH_LV2 "Enable LV2 backend sources" ON)
option(CLAM_WITH_FFTW3 "Enable FFTW3 processing backend" ON)
option(CLAM_WITH_NR_FFT "Enable Numerical Recipes FFT fallback" ON)
option(CLAM_WITH_SNDFILE "Enable libsndfile audio codecs" ON)
option(CLAM_WITH_OGGVORBIS "Enable Ogg/Vorbis audio codecs" ON)
option(CLAM_WITH_MAD "Enable MPEG layer 3 audio codecs through libmad" ON)
option(CLAM_WITH_ID3 "Enable id3lib metadata support for MPEG files" ON)
option(CLAM_WITH_JACK "Enable JACK network player" ON)
option(CLAM_WITH_ALSA "Enable ALSA MIDI/audio helpers on Linux" ON)
option(CLAM_WITH_PORTAUDIO "Enable PortAudio network player" ON)
option(CLAM_WITH_PORTMIDI "Enable PortMidi MIDI backend" OFF)

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

if(BUILD_TESTING)
	find_package(CppUnit CONFIG QUIET)
	if(TARGET CppUnit::cppunit)
		set(CLAM_CPPUNIT_TARGET CppUnit::cppunit)
	elseif(TARGET CppUnit::CppUnit)
		set(CLAM_CPPUNIT_TARGET CppUnit::CppUnit)
	elseif(TARGET cppunit)
		set(CLAM_CPPUNIT_TARGET cppunit)
	else()
		pkg_check_modules(CPPUNIT REQUIRED IMPORTED_TARGET cppunit)
		set(CLAM_CPPUNIT_TARGET PkgConfig::CPPUNIT)
	endif()
endif()

add_library(clam_config INTERFACE)
add_library(CLAM::config ALIAS clam_config)

if(CLAM_WITH_DOUBLE)
	target_compile_definitions(clam_config INTERFACE CLAM_DOUBLE)
else()
	target_compile_definitions(clam_config INTERFACE CLAM_FLOAT)
endif()

if(NOT CLAM_WITH_CHECKS)
	target_compile_definitions(clam_config INTERFACE CLAM_DISABLE_CHECKS)
endif()

if(CLAM_WITH_RELEASE_ASSERTS)
	target_compile_definitions(clam_config INTERFACE CLAM_USE_RELEASE_ASSERTS)
endif()

target_link_libraries(clam_config INTERFACE Threads::Threads)

set(CLAM_CORE_PRIVATE_LIBS)
set(CLAM_PROCESSING_PRIVATE_LIBS)
set(CLAM_AUDIOIO_PRIVATE_LIBS)

set(CLAM_CORE_BLACKLIST)
if(CLAM_XML_BACKEND STREQUAL "xercesc")
	pkg_check_modules(XERCESC REQUIRED IMPORTED_TARGET xerces-c)
	target_compile_definitions(clam_config INTERFACE USE_XERCES=1 CLAM_USE_XML)
	list(APPEND CLAM_CORE_BLACKLIST "LibXml.+" "NullDom.+")
	list(APPEND CLAM_CORE_PRIVATE_LIBS PkgConfig::XERCESC)
elseif(CLAM_XML_BACKEND STREQUAL "none")
	list(APPEND CLAM_CORE_BLACKLIST "Xerces.+" "LibXml.+")
else()
	message(FATAL_ERROR "Unsupported CLAM_XML_BACKEND: ${CLAM_XML_BACKEND}")
endif()

if(CLAM_WITH_LADSPA)
	check_include_file(ladspa.h CLAM_HAVE_LADSPA_H)
	if(NOT CLAM_HAVE_LADSPA_H)
		message(FATAL_ERROR "CLAM_WITH_LADSPA is ON but ladspa.h was not found")
	endif()
	target_compile_definitions(clam_config INTERFACE USE_LADSPA=1)
else()
	list(APPEND CLAM_CORE_BLACKLIST "Ladspa.+" "RunTimeFaustLibraryLoader.+" "RunTimeLadspaLibraryLoader.*")
endif()

if(CLAM_WITH_FFTW3)
	pkg_check_modules(FFTW3 REQUIRED IMPORTED_TARGET fftw3)
	target_compile_definitions(clam_config INTERFACE USE_FFTW3=1)
	list(APPEND CLAM_PROCESSING_PRIVATE_LIBS PkgConfig::FFTW3)
else()
	list(APPEND CLAM_PROCESSING_BLACKLIST ".+fftw3.+")
endif()

set(CLAM_PROCESSING_BLACKLIST "^Audio\\.hxx" "^Audio\\.cxx")
if(NOT CLAM_WITH_NR_FFT)
	list(APPEND CLAM_PROCESSING_BLACKLIST ".*numrecipes.+" ".+numrec.+")
endif()
if(NOT CLAM_WITH_MAD)
	list(APPEND CLAM_PROCESSING_BLACKLIST "MpegAudioStream")
endif()

set(CLAM_AUDIOIO_BLACKLIST "main\\.cxx")

if(CLAM_WITH_SNDFILE)
	pkg_check_modules(SNDFILE REQUIRED IMPORTED_TARGET sndfile)
	target_compile_definitions(clam_config INTERFACE USE_SNDFILE=1)
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS PkgConfig::SNDFILE)
else()
	list(APPEND CLAM_AUDIOIO_BLACKLIST "PCM" "AudioFileFormats")
endif()

if(CLAM_WITH_OGGVORBIS)
	pkg_check_modules(VORBIS REQUIRED IMPORTED_TARGET vorbisfile vorbisenc)
	target_compile_definitions(clam_config INTERFACE USE_OGGVORBIS=1 WITH_VORBIS=1)
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS PkgConfig::VORBIS)
else()
	list(APPEND CLAM_AUDIOIO_BLACKLIST "Vorbis")
endif()

if(CLAM_WITH_MAD)
	pkg_check_modules(MAD REQUIRED IMPORTED_TARGET mad)
	target_compile_definitions(clam_config INTERFACE USE_MAD=1 WITH_MAD=1)
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS PkgConfig::MAD)
else()
	list(APPEND CLAM_AUDIOIO_BLACKLIST "Mpeg")
endif()

if(CLAM_WITH_ID3)
	find_path(ID3_INCLUDE_DIR id3/tag.h)
	find_library(ID3_LIBRARY NAMES id3)
	if(NOT ID3_INCLUDE_DIR OR NOT ID3_LIBRARY)
		message(FATAL_ERROR "CLAM_WITH_ID3 is ON but id3lib headers/library were not found")
	endif()
	find_package(ZLIB REQUIRED)
	target_compile_definitions(clam_config INTERFACE USE_ID3=1)
	target_include_directories(clam_config INTERFACE "${ID3_INCLUDE_DIR}")
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS "${ID3_LIBRARY}" ZLIB::ZLIB)
else()
	list(APPEND CLAM_AUDIOIO_BLACKLIST "MpegCodec")
endif()

if(CLAM_WITH_JACK)
	pkg_check_modules(JACK REQUIRED IMPORTED_TARGET jack)
	target_compile_definitions(clam_config INTERFACE USE_JACK=1)
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS PkgConfig::JACK)
else()
	list(APPEND CLAM_AUDIOIO_BLACKLIST "JACKNetworkPlayer")
endif()

if(CLAM_WITH_PORTAUDIO)
	pkg_check_modules(PORTAUDIO REQUIRED IMPORTED_TARGET portaudio-2.0)
	target_compile_definitions(clam_config INTERFACE USE_PORTAUDIO=1)
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS PkgConfig::PORTAUDIO)
else()
	list(APPEND CLAM_AUDIOIO_BLACKLIST "PANetworkPlayer")
endif()

if(CLAM_WITH_ALSA AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
	pkg_check_modules(ALSA REQUIRED IMPORTED_TARGET alsa)
	target_compile_definitions(clam_config INTERFACE USE_ALSA=1)
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS PkgConfig::ALSA)
endif()

if(CLAM_WITH_LV2)
	pkg_check_modules(LV2 REQUIRED IMPORTED_TARGET lv2)
	target_compile_definitions(clam_config INTERFACE USE_LV2=1)
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS PkgConfig::LV2)
else()
	list(APPEND CLAM_AUDIOIO_BLACKLIST "LV2")
endif()

find_library(CLAM_M_LIBRARY m)
if(CLAM_M_LIBRARY)
	list(APPEND CLAM_CORE_PRIVATE_LIBS "${CLAM_M_LIBRARY}")
endif()

if(CMAKE_DL_LIBS)
	list(APPEND CLAM_CORE_PRIVATE_LIBS "${CMAKE_DL_LIBS}")
endif()

set(CLAM_CORE_FOLDERS
	src/core
	src/core/base
	src/core/standard
	src/core/storage
	src/core/processing
	src/core/ports
	src/core/controls
	src/core/sinksources
	src/core/data
	src/core/schedulers
	src/Data/Descriptors/Pool
	src/Data/Editable
	src/Defines
	src/Errors
	src/Flow/Networks
	src/Processing/Plugins
	src/System
	externals/CbLib
)

set(CLAM_PROCESSING_FOLDERS
	src/Data/BasicProcessing
	src/Data/Descriptors
	src/Data/Descriptors/Melody
	src/Processing/Analysis
	src/Processing/Analysis/Tonal
	src/Processing/ArithOps
	src/Processing/Generators
	src/Processing/Synthesis
	src/Processing/Transformations
	src/Processing/Monitors
	src/Processing/Controls
	externals/numrec
	src/Processing/SDIFIO
	src/Tools/SDIF
	src/Processing/SMS*.*xx
	src/Processing/Transformations/SMS
	src/Processing/Transformations/Spectral
)

set(CLAM_AUDIOIO_FOLDERS
	src/audiofileio/processings
	src/audiofileio/codecs
	src/midiio/processings
	src/Processing/AudioIO
	src/midiio/devices
	src/midiio/devices/File
	src/midiio/devices/TextFile
	src/midiio/data
	src/Tools/AudioIO
	src/Flow/Networks/BackEnds
	src/System/Application
)

if(CLAM_WITH_PORTMIDI)
	list(APPEND CLAM_AUDIOIO_FOLDERS src/midiio/devices/PortMIDI)
endif()

if(CLAM_WITH_ALSA AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
	list(APPEND CLAM_AUDIOIO_FOLDERS
		src/midiio/devices/Linux
		src/Tools/AudioIO/Linux
	)
endif()

clam_generate_version_sources(CLAM_VERSION_SOURCE CLAM_VERSION_INCLUDE_DIR
	NAMESPACE CLAM
	BASENAME CLAMVersion
	PRODUCT CLAM
	CHANGES_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CHANGES"
)

clam_collect_module_files(CLAM_CORE_SOURCES CLAM_CORE_HEADERS CLAM_CORE_INCLUDE_DIRS
	BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
	FOLDERS ${CLAM_CORE_FOLDERS}
	BLACKLIST ${CLAM_CORE_BLACKLIST}
)

clam_collect_module_files(CLAM_PROCESSING_SOURCES CLAM_PROCESSING_HEADERS CLAM_PROCESSING_INCLUDE_DIRS
	BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
	FOLDERS ${CLAM_PROCESSING_FOLDERS}
	BLACKLIST ${CLAM_PROCESSING_BLACKLIST}
)

clam_collect_module_files(CLAM_AUDIOIO_SOURCES CLAM_AUDIOIO_HEADERS CLAM_AUDIOIO_INCLUDE_DIRS
	BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
	FOLDERS ${CLAM_AUDIOIO_FOLDERS}
	BLACKLIST ${CLAM_AUDIOIO_BLACKLIST}
)

clam_generate_flat_include_tree(CLAM_LEGACY_INCLUDE_DIR
	HEADERS
		${CLAM_CORE_HEADERS}
		${CLAM_PROCESSING_HEADERS}
		${CLAM_AUDIOIO_HEADERS}
)
target_include_directories(clam_config INTERFACE
	"$<BUILD_INTERFACE:${CLAM_LEGACY_INCLUDE_DIR}>"
)

add_library(clam_core ${CLAM_CORE_SOURCES} ${CLAM_CORE_HEADERS} ${CLAM_VERSION_SOURCE})
add_library(CLAM::core ALIAS clam_core)
clam_set_legacy_cxx(clam_core)
set_target_properties(clam_core PROPERTIES
	OUTPUT_NAME clam_core
	VERSION "${CLAM_VERSION}"
	SOVERSION "1"
)
target_include_directories(clam_core
	PUBLIC
		"$<BUILD_INTERFACE:${CLAM_VERSION_INCLUDE_DIR}>"
		"$<BUILD_INTERFACE:${CLAM_VERSION_INCLUDE_DIR}/CLAM>"
		"$<BUILD_INTERFACE:${CLAM_CORE_INCLUDE_DIRS}>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CLAM>"
)
target_link_libraries(clam_core
	PUBLIC clam_config
	PRIVATE ${CLAM_CORE_PRIVATE_LIBS}
)

add_library(clam_processing ${CLAM_PROCESSING_SOURCES} ${CLAM_PROCESSING_HEADERS})
add_library(CLAM::processing ALIAS clam_processing)
clam_set_legacy_cxx(clam_processing)
set_target_properties(clam_processing PROPERTIES
	OUTPUT_NAME clam_processing
	VERSION "${CLAM_VERSION}"
	SOVERSION "1"
)
target_include_directories(clam_processing
	PUBLIC
		"$<BUILD_INTERFACE:${CLAM_PROCESSING_INCLUDE_DIRS}>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CLAM>"
)
target_link_libraries(clam_processing
	PUBLIC clam_core
	PRIVATE ${CLAM_PROCESSING_PRIVATE_LIBS}
)

add_library(clam_audioio ${CLAM_AUDIOIO_SOURCES} ${CLAM_AUDIOIO_HEADERS})
add_library(CLAM::audioio ALIAS clam_audioio)
clam_set_legacy_cxx(clam_audioio)
set_target_properties(clam_audioio PROPERTIES
	OUTPUT_NAME clam_audioio
	VERSION "${CLAM_VERSION}"
	SOVERSION "1"
)
target_include_directories(clam_audioio
	PUBLIC
		"$<BUILD_INTERFACE:${CLAM_AUDIOIO_INCLUDE_DIRS}>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CLAM>"
)
target_link_libraries(clam_audioio
	PUBLIC clam_core clam_processing
	PRIVATE ${CLAM_AUDIOIO_PRIVATE_LIBS}
)

if(BUILD_TESTING)
	add_executable(clam_test_threading test/NonPortedTests/TestThreading.cxx)
	clam_set_legacy_cxx(clam_test_threading)
	target_link_libraries(clam_test_threading PRIVATE clam_core)
	add_test(NAME clam_test_threading COMMAND clam_test_threading)
endif()

install(TARGETS clam_config clam_core clam_processing clam_audioio
	EXPORT CLAMTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

install(FILES ${CLAM_CORE_HEADERS} ${CLAM_PROCESSING_HEADERS} ${CLAM_AUDIOIO_HEADERS}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/CLAM"
)
install(FILES "${CLAM_VERSION_INCLUDE_DIR}/CLAM/CLAMVersion.hxx"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/CLAM"
)

install(EXPORT CLAMTargets
	NAMESPACE CLAM::
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CLAM"
)
