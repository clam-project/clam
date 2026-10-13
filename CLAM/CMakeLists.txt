include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckLibraryExists)
include(CMakePackageConfigHelpers)
include(FindPkgConfig)

clam_parse_version(CLAM_VERSION CLAM "${CMAKE_CURRENT_SOURCE_DIR}/CHANGES")
clam_abi_version(CLAM_SOVERSION "${CLAM_VERSION}")

set(CLAM_XML_BACKEND "xercesc" CACHE STRING "XML backend: xercesc or none")
set_property(CACHE CLAM_XML_BACKEND PROPERTY STRINGS xercesc none)

option(CLAM_WITH_CHECKS "Enable CLAM runtime checks" ON)
option(CLAM_WITH_RELEASE_ASSERTS "Use the release-mode CLAM asserts (throw, no debug checks) in Debug builds too" OFF)
option(CLAM_WITH_DOUBLE "Use double instead of float for CLAM::TData" OFF)
option(CLAM_WITH_OPTIMIZE "Use tabulated trigonometric functions and the like, losing precision (CLAM_OPTIMIZE)" OFF)
option(CLAM_WITH_LADSPA "Enable LADSPA support" ON)
option(CLAM_WITH_LV2 "Enable LV2 backend sources" ON)
option(CLAM_WITH_FFTW3 "Enable FFTW3 processing backend" ON)
option(CLAM_WITH_NR_FFT "Enable Numerical Recipes FFT fallback" ON)
option(CLAM_WITH_OGGVORBIS "Enable Ogg/Vorbis audio codecs" ON)
option(CLAM_WITH_MAD "Enable MPEG layer 3 audio codecs through libmad (needs CLAM_WITH_ID3)" ON)
option(CLAM_WITH_ID3 "Enable id3lib metadata support for MPEG files" ON)
option(CLAM_WITH_JACK "Enable JACK network player" ON)
option(CLAM_WITH_PIPEWIRE "Enable native PipeWire network player" ON)
option(CLAM_WITH_ALSA "Enable ALSA MIDI/audio helpers on Linux" ON)
option(CLAM_WITH_PORTAUDIO "Enable PortAudio network player" ON)
option(CLAM_WITH_PORTMIDI "Enable PortMidi MIDI backend" OFF)

# AudioFile and the MPEG codec are not written to build without these.
if(DEFINED CLAM_WITH_SNDFILE AND NOT CLAM_WITH_SNDFILE)
	message(FATAL_ERROR "libsndfile is required: AudioFile and OfflineNetworkPlayer use it unconditionally")
endif()
if(CLAM_WITH_MAD AND NOT CLAM_WITH_ID3)
	message(FATAL_ERROR "CLAM_WITH_MAD needs CLAM_WITH_ID3: the MPEG codec reads its metadata with id3lib")
endif()

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
	elseif(WIN32)
		# vcpkg's cppunit port ships only headers and the import library
		# on Windows -- no CMake config package, no .pc file. Build a
		# minimal IMPORTED target from find_path/find_library, with the
		# debug variant resolved separately when present.
		find_path(CPPUNIT_INCLUDE_DIR cppunit/TestCase.h REQUIRED)
		find_library(CPPUNIT_LIBRARY_RELEASE NAMES cppunit REQUIRED)
		find_library(CPPUNIT_LIBRARY_DEBUG   NAMES cppunitd)
		add_library(CppUnit::cppunit UNKNOWN IMPORTED)
		set_target_properties(CppUnit::cppunit PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES "${CPPUNIT_INCLUDE_DIR}"
			IMPORTED_LOCATION             "${CPPUNIT_LIBRARY_RELEASE}"
		)
		if(CPPUNIT_LIBRARY_DEBUG)
			set_property(TARGET CppUnit::cppunit APPEND PROPERTY
				IMPORTED_CONFIGURATIONS DEBUG
			)
			set_target_properties(CppUnit::cppunit PROPERTIES
				IMPORTED_LOCATION_DEBUG "${CPPUNIT_LIBRARY_DEBUG}"
			)
		endif()
		set(CLAM_CPPUNIT_TARGET CppUnit::cppunit)
	else()
		pkg_check_modules(CPPUNIT REQUIRED IMPORTED_TARGET cppunit)
		set(CLAM_CPPUNIT_TARGET PkgConfig::CPPUNIT)
	endif()
endif()

add_library(clam_config INTERFACE)
add_library(CLAM::config ALIAS clam_config)
set_target_properties(clam_config PROPERTIES EXPORT_NAME config)

# Public preprocessor definitions. They change the public headers, so they go
# to every consumer, both through CLAM::config and through the clam_*.pc files.
set(CLAM_PUBLIC_DEFINITIONS)
macro(clam_add_public_definitions)
	list(APPEND CLAM_PUBLIC_DEFINITIONS ${ARGN})
	target_compile_definitions(clam_config INTERFACE ${ARGN})
endmacro()

if(CLAM_WITH_DOUBLE)
	# The LADSPA wrappers hand TData buffers to plugins as LADSPA_Data (float)
	if(CLAM_WITH_LADSPA)
		message(FATAL_ERROR "CLAM_WITH_DOUBLE needs CLAM_WITH_LADSPA=OFF: the LADSPA host and exporter only support float samples")
	endif()
	clam_add_public_definitions(CLAM_DOUBLE)
else()
	clam_add_public_definitions(CLAM_FLOAT)
endif()

if(NOT CLAM_WITH_CHECKS)
	clam_add_public_definitions(CLAM_DISABLE_CHECKS)
endif()

# As in the SCons build, debug builds define _DEBUG, which turns on
# CLAM_DEBUG_ASSERT and friends (see Assert.hxx).
target_compile_definitions(clam_config INTERFACE $<$<CONFIG:Debug>:_DEBUG>)
if(CLAM_WITH_RELEASE_ASSERTS)
	clam_add_public_definitions(CLAM_USE_RELEASE_ASSERTS)
endif()

if(CLAM_WITH_OPTIMIZE)
	clam_add_public_definitions(CLAM_OPTIMIZE)
endif()

target_link_libraries(clam_config INTERFACE Threads::Threads)

set(CLAM_CORE_PRIVATE_LIBS)
set(CLAM_PROCESSING_PRIVATE_LIBS)
set(CLAM_AUDIOIO_PRIVATE_LIBS)
# Deps whose include dirs need to propagate publicly because they leak
# through clam_audioio's public headers. On Linux / macOS pkg-config
# returns a system or brew prefix that ends up on the default include
# search path anyway, so the deps stay PRIVATE there. Only Windows + vcpkg
# uses a sandboxed prefix that needs explicit propagation.
set(CLAM_AUDIOIO_PUBLIC_LIBS)

# CLAM/SConstruct writes its own CLAMVersion.{hxx,cxx} into src/Defines; the
# CMake build generates them in the build tree instead.
set(CLAM_CORE_BLACKLIST "^CLAMVersion\\.(hxx|cxx)$")
if(CLAM_XML_BACKEND STREQUAL "xercesc")
	pkg_check_modules(XERCESC REQUIRED IMPORTED_TARGET xerces-c)
	clam_add_public_definitions(USE_XERCES=1 CLAM_USE_XML)
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
	clam_add_public_definitions(USE_LADSPA=1)
else()
	list(APPEND CLAM_CORE_BLACKLIST "Ladspa.+" "RunTimeFaustLibraryLoader.+" "RunTimeLadspaLibraryLoader.*")
endif()

set(CLAM_PROCESSING_BLACKLIST "^Audio\\.hxx" "^Audio\\.cxx")

if(CLAM_WITH_FFTW3)
	pkg_check_modules(FFTW3 REQUIRED IMPORTED_TARGET fftw3)
	clam_add_public_definitions(USE_FFTW3=1)
	list(APPEND CLAM_PROCESSING_PRIVATE_LIBS PkgConfig::FFTW3)
else()
	list(APPEND CLAM_PROCESSING_BLACKLIST ".+fftw3.+")
endif()

if(NOT CLAM_WITH_NR_FFT)
	list(APPEND CLAM_PROCESSING_BLACKLIST ".*numrecipes.+" ".+numrec.+")
endif()
if(NOT CLAM_WITH_MAD)
	list(APPEND CLAM_PROCESSING_BLACKLIST "MpegAudioStream")
endif()

set(CLAM_AUDIOIO_BLACKLIST "main\\.cxx")

pkg_check_modules(SNDFILE REQUIRED IMPORTED_TARGET sndfile)
clam_add_public_definitions(USE_SNDFILE=1)
if(WIN32)
	# AudioFileFormats.hxx (public clam_audioio header) #includes
	# <sndfile.h>, so the include dir has to reach every consumer
	# of clam_audioio. With vcpkg the header lives in a sandboxed
	# prefix nothing else looks at -- propagate it publicly.
	list(APPEND CLAM_AUDIOIO_PUBLIC_LIBS PkgConfig::SNDFILE)
else()
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS PkgConfig::SNDFILE)
endif()

if(CLAM_WITH_OGGVORBIS)
	# OggVorbisAudioStream calls vorbis_* and ogg_* directly, but the vorbis
	# .pc files declare those libraries as Requires.private, which
	# non-static pkg-config does not follow. List them explicitly.
	pkg_check_modules(VORBIS REQUIRED IMPORTED_TARGET vorbisfile vorbisenc vorbis ogg)
	clam_add_public_definitions(USE_OGGVORBIS=1 WITH_VORBIS=1)
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS PkgConfig::VORBIS)
else()
	list(APPEND CLAM_AUDIOIO_BLACKLIST "Vorbis")
endif()

if(CLAM_WITH_MAD)
	pkg_check_modules(MAD REQUIRED IMPORTED_TARGET mad)
	clam_add_public_definitions(USE_MAD=1 WITH_MAD=1)
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
	clam_add_public_definitions(USE_ID3=1)
	target_include_directories(clam_config INTERFACE "${ID3_INCLUDE_DIR}")
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS "${ID3_LIBRARY}" ZLIB::ZLIB)
else()
	list(APPEND CLAM_AUDIOIO_BLACKLIST "MpegCodec")
endif()

if(CLAM_WITH_JACK)
	pkg_check_modules(JACK REQUIRED IMPORTED_TARGET jack)
	clam_add_public_definitions(USE_JACK=1)
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS PkgConfig::JACK)
else()
	list(APPEND CLAM_AUDIOIO_BLACKLIST "JACKNetworkPlayer")
endif()

if(CLAM_WITH_PIPEWIRE AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
	pkg_check_modules(PIPEWIRE REQUIRED IMPORTED_TARGET libpipewire-0.3)
	clam_add_public_definitions(USE_PIPEWIRE=1)
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS PkgConfig::PIPEWIRE)
else()
	list(APPEND CLAM_AUDIOIO_BLACKLIST "PipeWireNetworkPlayer")
endif()

if(CLAM_WITH_PORTAUDIO)
	pkg_check_modules(PORTAUDIO REQUIRED IMPORTED_TARGET portaudio-2.0)
	clam_add_public_definitions(USE_PORTAUDIO=1)
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS PkgConfig::PORTAUDIO)
else()
	list(APPEND CLAM_AUDIOIO_BLACKLIST "PANetworkPlayer")
endif()

if(CLAM_WITH_ALSA AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
	pkg_check_modules(ALSA REQUIRED IMPORTED_TARGET alsa)
	clam_add_public_definitions(USE_ALSA=1)
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS PkgConfig::ALSA)
endif()

if(CLAM_WITH_LV2)
	pkg_check_modules(LV2 REQUIRED IMPORTED_TARGET lv2)
	clam_add_public_definitions(USE_LV2=1)
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS PkgConfig::LV2)
else()
	list(APPEND CLAM_AUDIOIO_BLACKLIST "LV2")
endif()

if(CLAM_WITH_PORTMIDI)
	find_path(PORTMIDI_INCLUDE_DIR portmidi.h)
	find_library(PORTMIDI_LIBRARY NAMES portmidi)
	if(NOT PORTMIDI_INCLUDE_DIR OR NOT PORTMIDI_LIBRARY)
		message(FATAL_ERROR "CLAM_WITH_PORTMIDI is ON but portmidi headers/library were not found")
	endif()
	# portmidi 1.x ships the PortTime timer as a separate library.
	find_library(PORTTIME_LIBRARY NAMES porttime)
	clam_add_public_definitions(USE_PORTMIDI=1)
	target_include_directories(clam_config INTERFACE "${PORTMIDI_INCLUDE_DIR}")
	list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS "${PORTMIDI_LIBRARY}")
	if(PORTTIME_LIBRARY)
		list(APPEND CLAM_AUDIOIO_PRIVATE_LIBS "${PORTTIME_LIBRARY}")
	endif()
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
if(WIN32)
	# src/Defines/Windows hosts CLAM_windows.h, a thin shim around
	# <windows.h>. The header is only included from WIN32-guarded code
	# (e.g. RunTimeLibraryLoader.cxx), so its directory is added to the
	# include path only on Windows.
	list(APPEND CLAM_CORE_FOLDERS src/Defines/Windows)
endif()

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

# The libraries are always shared: every processing registers itself with the
# factories from a file-static object, which a static archive would drop.

add_library(clam_core SHARED ${CLAM_CORE_SOURCES} ${CLAM_CORE_HEADERS} ${CLAM_VERSION_SOURCE})
add_library(CLAM::core ALIAS clam_core)
clam_set_legacy_cxx(clam_core)
set_target_properties(clam_core PROPERTIES
	OUTPUT_NAME clam_core
	EXPORT_NAME core
	VERSION "${CLAM_VERSION}"
	SOVERSION "${CLAM_SOVERSION}"
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

add_library(clam_processing SHARED ${CLAM_PROCESSING_SOURCES} ${CLAM_PROCESSING_HEADERS})
add_library(CLAM::processing ALIAS clam_processing)
clam_set_legacy_cxx(clam_processing)
set_target_properties(clam_processing PROPERTIES
	OUTPUT_NAME clam_processing
	EXPORT_NAME processing
	VERSION "${CLAM_VERSION}"
	SOVERSION "${CLAM_SOVERSION}"
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

add_library(clam_audioio SHARED ${CLAM_AUDIOIO_SOURCES} ${CLAM_AUDIOIO_HEADERS})
add_library(CLAM::audioio ALIAS clam_audioio)
clam_set_legacy_cxx(clam_audioio)
set_target_properties(clam_audioio PROPERTIES
	OUTPUT_NAME clam_audioio
	EXPORT_NAME audioio
	VERSION "${CLAM_VERSION}"
	SOVERSION "${CLAM_SOVERSION}"
)
target_include_directories(clam_audioio
	PUBLIC
		"$<BUILD_INTERFACE:${CLAM_AUDIOIO_INCLUDE_DIRS}>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CLAM>"
)
target_link_libraries(clam_audioio
	PUBLIC clam_core clam_processing ${CLAM_AUDIOIO_PUBLIC_LIBS}
	PRIVATE ${CLAM_AUDIOIO_PRIVATE_LIBS}
)

if(BUILD_TESTING)
	add_executable(clam_test_threading test/NonPortedTests/TestThreading.cxx)
	clam_set_legacy_cxx(clam_test_threading)
	target_link_libraries(clam_test_threading PRIVATE clam_core)
	add_test(NAME clam_test_threading COMMAND clam_test_threading)
	# The cppunit unit tests (CLAM/test/UnitTests)
	add_subdirectory(test)
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

# find_package(CLAM) support for projects built against an installed CLAM.
configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/CLAMConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/CLAMConfig.cmake"
	INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CLAM"
	PATH_VARS CMAKE_INSTALL_DATADIR
)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/CLAMConfigVersion.cmake"
	VERSION "${CLAM_VERSION}"
	COMPATIBILITY SameMinorVersion
)
install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/CLAMConfig.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/CLAMConfigVersion.cmake"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CLAM"
)

# pkg-config files and SCons tools, as installed by the SCons build. The
# plugins, examples and applications that still build with SCons find CLAM
# through them (env.Tool('clam') + EnableClamModules). The directories are
# relative to ${prefix}, so that pkg-config --define-prefix can relocate them.
foreach(_dir LIBDIR INCLUDEDIR)
	if(IS_ABSOLUTE "${CMAKE_INSTALL_${_dir}}")
		set(CLAM_PC_${_dir} "${CMAKE_INSTALL_${_dir}}")
	else()
		set(CLAM_PC_${_dir} "\${prefix}/${CMAKE_INSTALL_${_dir}}")
	endif()
endforeach()
set(CLAM_PC_CFLAGS)
foreach(_definition IN LISTS CLAM_PUBLIC_DEFINITIONS)
	string(APPEND CLAM_PC_CFLAGS " -D${_definition}")
endforeach()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	string(APPEND CLAM_PC_CFLAGS " -D_DEBUG")
endif()
foreach(_module core processing audioio)
	if(_module STREQUAL "core")
		set(CLAM_PC_REQUIRES "")
	elseif(_module STREQUAL "processing")
		set(CLAM_PC_REQUIRES "clam_core")
	else()
		set(CLAM_PC_REQUIRES "clam_core, clam_processing")
	endif()
	set(CLAM_PC_MODULE "${_module}")
	configure_file(clam.pc.in "${CMAKE_CURRENT_BINARY_DIR}/clam_${_module}.pc" @ONLY)
	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/clam_${_module}.pc"
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
	)
endforeach()
file(GLOB CLAM_SCONS_TOOLS "${CMAKE_CURRENT_SOURCE_DIR}/scons/sconstools/*.py")
install(FILES ${CLAM_SCONS_TOOLS}
	DESTINATION "${CMAKE_INSTALL_DATADIR}/clam/sconstools"
)
# The Python scripts, installed as the SCons build did: the script itself
# under share/clam, a command without the suffix, and its man page.
# NetworkEditor embeds clamrefactor.py to upgrade old networks.
set(CLAM_REFACTOR_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/scripts/clamrefactor.py" PARENT_SCOPE)
foreach(_script IN ITEMS clamrefactor clamnetwork2code clam_lv2_generator)
	install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/scripts/${_script}.py"
		DESTINATION "${CMAKE_INSTALL_DATADIR}/clam"
	)
	install(PROGRAMS "${CMAKE_CURRENT_SOURCE_DIR}/scripts/${_script}.py"
		DESTINATION "${CMAKE_INSTALL_BINDIR}"
		RENAME "${_script}"
	)
	install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/scripts/${_script}.1"
		DESTINATION "${CMAKE_INSTALL_MANDIR}/man1"
	)
endforeach()
