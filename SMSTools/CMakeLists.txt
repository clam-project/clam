if(NOT TARGET CLAM::core OR NOT TARGET CLAM::processing OR NOT TARGET CLAM::audioio)
	message(FATAL_ERROR "SMSTools CMake currently expects CLAM_BUILD_LIBRARIES=ON so it can link the in-tree CLAM libraries.")
endif()

clam_parse_version(SMSTOOLS_VERSION SMSTools "${CMAKE_CURRENT_SOURCE_DIR}/CHANGES")
clam_generate_version_sources(SMSTOOLS_VERSION_SOURCE SMSTOOLS_VERSION_INCLUDE_DIR
	NAMESPACE SMSTools
	BASENAME SMSToolsVersion
	PRODUCT SMSTools
	CHANGES_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CHANGES"
)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Xml OpenGL OpenGLWidgets PrintSupport)
set(CLAM_SMSTOOLS_QT_LIBRARIES
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets
	Qt6::Xml
	Qt6::OpenGL
	Qt6::OpenGLWidgets
	Qt6::PrintSupport
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Common Qt-free SMSTools sources (SMS engine glue, serializers, config)
set(CLAM_SMSTOOLS_COMMON_SOURCES
	${SMSTOOLS_VERSION_SOURCE}
	src/Progress.cxx
	src/SDIFSerializer.cxx
	src/SMSAnalysisSynthesisConfig.cxx
	src/SMSBase.cxx
	src/SerializationController.cxx
	src/WaitMessage.cxx
	src/XMLSerializer.cxx
)
set(CLAM_SMSTOOLS_COMMON_HEADERS
	src/Progress.hxx
	src/SDIFSerializer.hxx
	src/Serializer.hxx
	src/SMSAnalysisSynthesisConfig.hxx
	src/SMSAppState.hxx
	src/SMSBase.hxx
	src/WaitMessage.hxx
	src/XMLSerializer.hxx
)
set(CLAM_SMSTOOLS_STDOUT_SOURCES
	src/batch/StdOutProgress.cxx
	src/batch/StdOutWaitMessage.cxx
)
set(CLAM_SMSTOOLS_STDOUT_HEADERS
	src/batch/StdOutProgress.hxx
	src/batch/StdOutWaitMessage.hxx
)

# Qt visual-monitor library bundled with SMSTools (its own copy of vmqt/CLAM/*)
file(GLOB CLAM_SMSTOOLS_VMQT_SOURCES CONFIGURE_DEPENDS "vmqt/CLAM/*.cxx")
file(GLOB CLAM_SMSTOOLS_VMQT_HEADERS CONFIGURE_DEPENDS "vmqt/CLAM/*.hxx")

# Qt-dependent GUI sources
set(CLAM_SMSTOOLS_QT_GUI_DIRS
	src/QtImpl
	src/QtImpl/Application
	src/QtImpl/System
	src/QtImpl/GUI/misc
	src/QtImpl/GUI/Dialogs/AboutBox
	src/QtImpl/GUI/Dialogs/Configurator
	src/QtImpl/GUI/Dialogs/ScoreEditor
)
set(CLAM_SMSTOOLS_QT_GUI_SOURCES)
set(CLAM_SMSTOOLS_QT_GUI_HEADERS)
foreach(_dir IN LISTS CLAM_SMSTOOLS_QT_GUI_DIRS)
	file(GLOB _sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.cxx")
	file(GLOB _headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.hxx")
	list(APPEND CLAM_SMSTOOLS_QT_GUI_SOURCES ${_sources})
	list(APPEND CLAM_SMSTOOLS_QT_GUI_HEADERS ${_headers})
endforeach()

# main.cxx is included with the GUI sources but is the entry point for SMSTools only
set(CLAM_SMSTOOLS_GUI_MAIN "${CMAKE_CURRENT_SOURCE_DIR}/src/QtImpl/main.cxx")

set(CLAM_SMSTOOLS_UI_FILES
	src/QtImpl/Application/QtSMSTools.ui
	src/QtImpl/GUI/Dialogs/AboutBox/AboutBox.ui
	src/QtImpl/GUI/Dialogs/Configurator/SMSConfigDlg.ui
	src/QtImpl/GUI/Dialogs/ScoreEditor/ScoreEditorDlg.ui
)

# ------------------------------------------------------------------------------
# Qt-free common static library shared by every SMSTools-derived executable.
# ------------------------------------------------------------------------------
add_library(clam_smstools_common STATIC
	${CLAM_SMSTOOLS_COMMON_SOURCES}
	${CLAM_SMSTOOLS_COMMON_HEADERS}
)
clam_set_legacy_cxx(clam_smstools_common)
target_include_directories(clam_smstools_common
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/src"
		"${SMSTOOLS_VERSION_INCLUDE_DIR}"
		"${SMSTOOLS_VERSION_INCLUDE_DIR}/CLAM"
)
target_link_libraries(clam_smstools_common
	PUBLIC
		CLAM::core
		CLAM::processing
		CLAM::audioio
)

# StdOut helpers are only needed by the console / batch executables.
add_library(clam_smstools_stdout STATIC
	${CLAM_SMSTOOLS_STDOUT_SOURCES}
	${CLAM_SMSTOOLS_STDOUT_HEADERS}
)
clam_set_legacy_cxx(clam_smstools_stdout)
target_include_directories(clam_smstools_stdout
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/src"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/batch"
)
target_link_libraries(clam_smstools_stdout
	PUBLIC clam_smstools_common
)

# ------------------------------------------------------------------------------
# Qt visual-monitor library used by the GUI executable.
# ------------------------------------------------------------------------------
add_library(clam_smstools_vmqt STATIC
	${CLAM_SMSTOOLS_VMQT_SOURCES}
	${CLAM_SMSTOOLS_VMQT_HEADERS}
)
clam_set_legacy_cxx(clam_smstools_vmqt)
target_include_directories(clam_smstools_vmqt
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/vmqt"
		"${CMAKE_CURRENT_SOURCE_DIR}/vmqt/CLAM"
)
target_link_libraries(clam_smstools_vmqt
	PUBLIC
		clam_smstools_common
		${CLAM_SMSTOOLS_QT_LIBRARIES}
)

# ------------------------------------------------------------------------------
# Console SMS tool (no Qt).
# ------------------------------------------------------------------------------
add_executable(SMSConsole src/SMS.cxx)
clam_set_legacy_cxx(SMSConsole)
target_link_libraries(SMSConsole
	PRIVATE
		clam_smstools_common
		clam_smstools_stdout
)

# ------------------------------------------------------------------------------
# Batch SMS tool (no Qt).
# ------------------------------------------------------------------------------
add_executable(SMSBatch src/batch/SMSBatch.cxx)
clam_set_legacy_cxx(SMSBatch)
target_link_libraries(SMSBatch
	PRIVATE
		clam_smstools_common
		clam_smstools_stdout
)

# ------------------------------------------------------------------------------
# Qt GUI executable.
# ------------------------------------------------------------------------------
add_executable(SMSTools
	${CLAM_SMSTOOLS_GUI_MAIN}
	${CLAM_SMSTOOLS_QT_GUI_SOURCES}
	${CLAM_SMSTOOLS_QT_GUI_HEADERS}
	${CLAM_SMSTOOLS_UI_FILES}
	src/images.qrc
)
clam_set_legacy_cxx(SMSTools)
set_target_properties(SMSTools PROPERTIES
	AUTOUIC_SEARCH_PATHS
		"${CMAKE_CURRENT_SOURCE_DIR}/src/QtImpl/Application;${CMAKE_CURRENT_SOURCE_DIR}/src/QtImpl/GUI/Dialogs/AboutBox;${CMAKE_CURRENT_SOURCE_DIR}/src/QtImpl/GUI/Dialogs/Configurator;${CMAKE_CURRENT_SOURCE_DIR}/src/QtImpl/GUI/Dialogs/ScoreEditor"
)
target_include_directories(SMSTools
	PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/src"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/QtImpl"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/QtImpl/Application"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/QtImpl/System"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/QtImpl/GUI/misc"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/QtImpl/GUI/Dialogs/AboutBox"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/QtImpl/GUI/Dialogs/Configurator"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/QtImpl/GUI/Dialogs/ScoreEditor"
)
target_compile_definitions(SMSTools
	PRIVATE
		DATA_EXAMPLES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/example-data"
)
target_link_libraries(SMSTools
	PRIVATE
		clam_smstools_common
		clam_smstools_vmqt
		${CLAM_SMSTOOLS_QT_LIBRARIES}
)

install(TARGETS SMSTools SMSBatch SMSConsole
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
