include(ClamQtCompat)

if(NOT TARGET CLAM::core OR NOT TARGET CLAM::processing OR NOT TARGET CLAM::audioio)
	message(FATAL_ERROR "NetworkEditor CMake currently expects CLAM_BUILD_LIBRARIES=ON so it can link the in-tree CLAM libraries.")
endif()

clam_parse_version(NETWORKEDITOR_VERSION NetworkEditor "${CMAKE_CURRENT_SOURCE_DIR}/CHANGES")
clam_generate_version_sources(NETWORKEDITOR_VERSION_SOURCE NETWORKEDITOR_VERSION_INCLUDE_DIR
	NAMESPACE NetworkEditor
	BASENAME NetworkEditorVersion
	PRODUCT NetworkEditor
	CHANGES_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CHANGES"
)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Xml Svg SvgWidgets OpenGL OpenGLWidgets PrintSupport)
set(CLAM_QT_LIBRARIES
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets
	Qt6::Xml
	Qt6::Svg
	Qt6::SvgWidgets
	Qt6::OpenGL
	Qt6::OpenGLWidgets
	Qt6::PrintSupport
)

clam_generate_qt_compat_headers(CLAM_QT_COMPAT_INCLUDE_DIR)
find_library(CLAM_GLU_LIBRARY NAMES GLU)
if(NOT CLAM_GLU_LIBRARY)
	message(FATAL_ERROR "NetworkEditor OpenGL widgets require GLU but libGLU was not found")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CLAM_NETWORKEDITOR_COMMON_INCLUDE_DIRS
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/widgets"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/monitors"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/binders"
)

set(CLAM_QTMONITOR_DIRS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/widgets"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/monitors"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/binders"
)

set(CLAM_QTMONITOR_SOURCES)
set(CLAM_QTMONITOR_HEADERS)
foreach(_dir IN LISTS CLAM_QTMONITOR_DIRS)
	file(GLOB _sources CONFIGURE_DEPENDS "${_dir}/*.cxx")
	file(GLOB _headers CONFIGURE_DEPENDS "${_dir}/*.hxx")
	list(APPEND CLAM_QTMONITOR_SOURCES ${_sources})
	list(APPEND CLAM_QTMONITOR_HEADERS ${_headers})
endforeach()

add_library(clam_qtmonitors
	${CLAM_QTMONITOR_SOURCES}
	${CLAM_QTMONITOR_HEADERS}
	"${CMAKE_CURRENT_SOURCE_DIR}/src/images.qrc"
)
add_library(CLAM::qtmonitors ALIAS clam_qtmonitors)
clam_set_legacy_cxx(clam_qtmonitors)
set_target_properties(clam_qtmonitors PROPERTIES
	OUTPUT_NAME clam_qtmonitors
	VERSION "${NETWORKEDITOR_VERSION}"
	SOVERSION "1"
	AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_compile_definitions(clam_qtmonitors
	PRIVATE
		QT_PLUGIN
		QDESIGNER_EXPORT_WIDGETS
		_REENTRANT
)
target_include_directories(clam_qtmonitors
	BEFORE PRIVATE
		"${CLAM_QT_COMPAT_INCLUDE_DIR}"
)
target_include_directories(clam_qtmonitors
	PUBLIC
		"$<BUILD_INTERFACE:${CLAM_NETWORKEDITOR_COMMON_INCLUDE_DIRS}>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CLAM/qtmonitors>"
)
target_link_libraries(clam_qtmonitors
	PUBLIC
		CLAM::core
		CLAM::processing
		CLAM::audioio
		PRIVATE
			${CLAM_QT_LIBRARIES}
			"${CLAM_GLU_LIBRARY}"
	)

install(TARGETS clam_qtmonitors
	EXPORT CLAMTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

install(FILES ${CLAM_QTMONITOR_HEADERS}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/CLAM/qtmonitors"
)

option(CLAM_NETWORKEDITOR_BUILD_APPS "Build NetworkEditor executable" ON)
if(CLAM_NETWORKEDITOR_BUILD_APPS)
	set(CLAM_NETWORKEDITOR_APP_DIRS
		"${CMAKE_CURRENT_SOURCE_DIR}/src"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/processing"
	)
	set(CLAM_NETWORKEDITOR_APP_SOURCES ${NETWORKEDITOR_VERSION_SOURCE})
	set(CLAM_NETWORKEDITOR_APP_HEADERS)
	foreach(_dir IN LISTS CLAM_NETWORKEDITOR_APP_DIRS)
		file(GLOB _sources CONFIGURE_DEPENDS "${_dir}/*.cxx")
		file(GLOB _headers CONFIGURE_DEPENDS "${_dir}/*.hxx")
		list(APPEND CLAM_NETWORKEDITOR_APP_SOURCES ${_sources})
		list(APPEND CLAM_NETWORKEDITOR_APP_HEADERS ${_headers})
	endforeach()
	list(REMOVE_ITEM CLAM_NETWORKEDITOR_APP_SOURCES
		"${CMAKE_CURRENT_SOURCE_DIR}/src/main.cxx"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/MonoOfflinePlayerMain.cxx"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/OfflinePlayerMain.cxx"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/CheckClamNetworkMain.cxx"
	)

	add_executable(NetworkEditor
		"${CMAKE_CURRENT_SOURCE_DIR}/src/main.cxx"
		${CLAM_NETWORKEDITOR_APP_SOURCES}
		${CLAM_NETWORKEDITOR_APP_HEADERS}
		"${CMAKE_CURRENT_SOURCE_DIR}/src/About.ui"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/DummyProcessingConfig.ui"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/LadspaMetadataEditor.ui"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/MainWindow.ui"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/images.qrc"
	)
	clam_set_legacy_cxx(NetworkEditor)
	set_target_properties(NetworkEditor PROPERTIES
		AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/src"
	)
	target_include_directories(NetworkEditor
		BEFORE PRIVATE
			"${CLAM_QT_COMPAT_INCLUDE_DIR}"
	)
	target_include_directories(NetworkEditor
		PRIVATE
			${CLAM_NETWORKEDITOR_COMMON_INCLUDE_DIRS}
			"${CMAKE_CURRENT_SOURCE_DIR}/src/processing"
			"${NETWORKEDITOR_VERSION_INCLUDE_DIR}"
			"${NETWORKEDITOR_VERSION_INCLUDE_DIR}/CLAM"
	)
	target_compile_definitions(NetworkEditor
		PRIVATE
			DATA_EXAMPLES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/example-data"
			CLAM_PREFIX="${CMAKE_INSTALL_PREFIX}"
			I18N_PATH="${CMAKE_INSTALL_PREFIX}/"
	)
	target_link_libraries(NetworkEditor
		PRIVATE
			CLAM::qtmonitors
			${CLAM_QT_LIBRARIES}
	)
	install(TARGETS NetworkEditor
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
	)
endif()
