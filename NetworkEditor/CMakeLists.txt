if(NOT TARGET CLAM::core)
	find_package(CLAM CONFIG REQUIRED)
endif()

clam_parse_version(NETWORKEDITOR_VERSION NetworkEditor "${CMAKE_CURRENT_SOURCE_DIR}/CHANGES")
clam_abi_version(NETWORKEDITOR_SOVERSION "${NETWORKEDITOR_VERSION}")
clam_generate_version_sources(NETWORKEDITOR_VERSION_SOURCE NETWORKEDITOR_VERSION_INCLUDE_DIR
	NAMESPACE NetworkEditor
	BASENAME NetworkEditorVersion
	PRODUCT NetworkEditor
	CHANGES_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CHANGES"
)

option(CLAM_NETWORKEDITOR_BUILD_APPS "Build NetworkEditor, Prototyper and the offline command line tools" ON)
option(CLAM_NETWORKEDITOR_BUILD_DESIGNER_PLUGIN "Build the CLAMWidgets plugin for Qt Designer" ON)

set(CLAM_NETWORKEDITOR_QT_COMPONENTS Core Gui Widgets Xml Svg SvgWidgets OpenGL OpenGLWidgets PrintSupport UiPlugin Qml Quick QuickControls2)
if(CLAM_NETWORKEDITOR_BUILD_APPS)
	list(APPEND CLAM_NETWORKEDITOR_QT_COMPONENTS UiTools)
endif()
if(CLAM_NETWORKEDITOR_BUILD_DESIGNER_PLUGIN)
	list(APPEND CLAM_NETWORKEDITOR_QT_COMPONENTS Designer)
endif()
find_package(Qt6 REQUIRED COMPONENTS ${CLAM_NETWORKEDITOR_QT_COMPONENTS})
find_package(Qt6 QUIET COMPONENTS LinguistTools)
# The monitors call GL directly (glBegin, glVertex, ...), and Qt6::OpenGL does
# not link the GL library on every platform (opengl32.lib on Windows).
find_package(OpenGL REQUIRED)

# Python scripting: the ipyclam console and the upgrade of networks saved by
# older CLAM versions. NetworkEditor embeds Python (python3-embed) through
# Boost.Python, and the console wraps the Qt objects with shiboken6. Off by
# default: CI does not build it.
option(CLAM_NETWORKEDITOR_USE_PYTHON
	"Enable Python scripting support (needs python3-embed, Boost.Python and shiboken6)" OFF)
if(CLAM_NETWORKEDITOR_USE_PYTHON)
	include(FindPkgConfig)
	pkg_check_modules(Python3 REQUIRED IMPORTED_TARGET python3-embed)
	pkg_check_modules(shiboken6 REQUIRED IMPORTED_TARGET shiboken6)
	# The Boost.Python library is named after the Python version (python312)
	string(REGEX MATCH "^[0-9]+\\.[0-9]+" _python_version "${Python3_VERSION}")
	string(REPLACE "." "" _python_tag "${_python_version}")
	find_package(Boost REQUIRED COMPONENTS "python${_python_tag}")
	set(CLAM_NETWORKEDITOR_PYTHON_LIBRARIES
		PkgConfig::Python3 PkgConfig::shiboken6 "Boost::python${_python_tag}")
	# Set by the CLAM tree or by the installed CLAM package
	if(NOT CLAM_REFACTOR_SCRIPT)
		message(FATAL_ERROR "CLAM_REFACTOR_SCRIPT is not set: clamrefactor.py, which NetworkEditor embeds, was not found")
	endif()
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

if(BUILD_TESTING AND CLAM_WITH_JACK)
	add_executable(BackendConfigDialogTest test/BackendConfigDialogTest.cxx)
	clam_set_legacy_cxx(BackendConfigDialogTest)
	target_include_directories(BackendConfigDialogTest PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
	target_compile_definitions(BackendConfigDialogTest PRIVATE USE_JACK=1 USE_PIPEWIRE=1)
	target_link_libraries(BackendConfigDialogTest PRIVATE Qt6::Widgets)
	add_test(NAME networkeditor_backend_config COMMAND BackendConfigDialogTest)
	set_tests_properties(networkeditor_backend_config PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endif()

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

# Always shared: the monitors register themselves with the processing factory
# from file-static objects that a static archive would silently drop.
add_library(clam_qtmonitors SHARED
	${CLAM_QTMONITOR_SOURCES}
	${CLAM_QTMONITOR_HEADERS}
	"${CMAKE_CURRENT_SOURCE_DIR}/src/images.qrc"
)
add_library(CLAM::qtmonitors ALIAS clam_qtmonitors)
clam_set_legacy_cxx(clam_qtmonitors)
set_target_properties(clam_qtmonitors PROPERTIES
	OUTPUT_NAME clam_qtmonitors
	EXPORT_NAME qtmonitors
	VERSION "${NETWORKEDITOR_VERSION}"
	SOVERSION "${NETWORKEDITOR_SOVERSION}"
	AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_compile_definitions(clam_qtmonitors
	PRIVATE
		QT_PLUGIN
		QDESIGNER_EXPORT_WIDGETS
		_REENTRANT
)
target_include_directories(clam_qtmonitors
	PUBLIC
		"$<BUILD_INTERFACE:${CLAM_NETWORKEDITOR_COMMON_INCLUDE_DIRS}>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CLAM/qtmonitors>"
)
# The installed headers include Qt (widgets, OpenGL, SVG, QDesignerExportWidget)
# and GL, so those are part of the public interface.
target_link_libraries(clam_qtmonitors
	PUBLIC
		CLAM::core
		CLAM::processing
		CLAM::audioio
		Qt6::Core
		Qt6::Gui
		Qt6::Widgets
		Qt6::Svg
		Qt6::OpenGL
		Qt6::OpenGLWidgets
		Qt6::UiPlugin
		OpenGL::GL
	PRIVATE
		Qt6::Xml
		Qt6::SvgWidgets
		Qt6::PrintSupport
)

install(TARGETS clam_qtmonitors
	EXPORT CLAMQtMonitorsTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
install(EXPORT CLAMQtMonitorsTargets
	NAMESPACE CLAM::
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CLAM"
)

install(FILES ${CLAM_QTMONITOR_HEADERS}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/CLAM/qtmonitors"
)

set(CLAM_NETWORKEDITOR_DATADIR "${CMAKE_INSTALL_DATADIR}/networkeditor")

add_subdirectory(src/qml/CLAM/Widgets)

if(CLAM_NETWORKEDITOR_BUILD_APPS)
	# Processings only NetworkEditor provides (some example networks use
	# them). The command line tools get them too, as in the SCons build.
	file(GLOB CLAM_NETWORKEDITOR_PROCESSING_SOURCES CONFIGURE_DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/src/processing/*.cxx"
	)
	add_library(networkeditor_processings OBJECT ${CLAM_NETWORKEDITOR_PROCESSING_SOURCES})
	clam_set_legacy_cxx(networkeditor_processings)
	target_link_libraries(networkeditor_processings PUBLIC CLAM::processing CLAM::audioio)

	set(CLAM_NETWORKEDITOR_APP_DIRS
		"${CMAKE_CURRENT_SOURCE_DIR}/src"
	)
	set(CLAM_NETWORKEDITOR_APP_SOURCES ${NETWORKEDITOR_VERSION_SOURCE})
	set(CLAM_NETWORKEDITOR_APP_HEADERS)
	foreach(_dir IN LISTS CLAM_NETWORKEDITOR_APP_DIRS)
		file(GLOB _sources CONFIGURE_DEPENDS "${_dir}/*.cxx")
		file(GLOB _headers CONFIGURE_DEPENDS "${_dir}/*.hxx")
		list(APPEND CLAM_NETWORKEDITOR_APP_SOURCES ${_sources})
		list(APPEND CLAM_NETWORKEDITOR_APP_HEADERS ${_headers})
	endforeach()
	set(CLAM_NETWORKEDITOR_TOOLS OfflinePlayer MonoOfflinePlayer CheckClamNetwork)
	list(REMOVE_ITEM CLAM_NETWORKEDITOR_APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cxx")
	foreach(_tool IN LISTS CLAM_NETWORKEDITOR_TOOLS)
		list(REMOVE_ITEM CLAM_NETWORKEDITOR_APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/${_tool}Main.cxx")
	endforeach()

	add_executable(NetworkEditor
		"${CMAKE_CURRENT_SOURCE_DIR}/src/main.cxx"
		${CLAM_NETWORKEDITOR_APP_SOURCES}
		${CLAM_NETWORKEDITOR_APP_HEADERS}
		"${CMAKE_CURRENT_SOURCE_DIR}/src/About.ui"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/DummyProcessingConfig.ui"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/LadspaMetadataEditor.ui"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/MainWindow.ui"
	)
	clam_set_legacy_cxx(NetworkEditor)
	set_target_properties(NetworkEditor PROPERTIES
		AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/src"
	)
	target_include_directories(NetworkEditor
		PRIVATE
			${CLAM_NETWORKEDITOR_COMMON_INCLUDE_DIRS}
			"${CMAKE_CURRENT_SOURCE_DIR}/src/processing"
			"${NETWORKEDITOR_VERSION_INCLUDE_DIR}"
			"${NETWORKEDITOR_VERSION_INCLUDE_DIR}/CLAM"
	)
	# Same locations as the SCons build: the installed example data and
	# translations under <prefix>/share/networkeditor, and (with Python) the
	# clamrefactor script under <prefix>/share/clam.
	target_compile_definitions(NetworkEditor
		PRIVATE
			DATA_EXAMPLES_PATH="${CMAKE_INSTALL_FULL_DATADIR}/networkeditor/example-data"
			CLAM_PREFIX="${CMAKE_INSTALL_PREFIX}"
			I18N_PATH="${CMAKE_INSTALL_PREFIX}/"
	)
	target_link_libraries(NetworkEditor
		PRIVATE
			networkeditor_processings
			CLAM::qtmonitors
			Qt6::Xml
			Qt6::SvgWidgets
			Qt6::PrintSupport
	)
	if(CLAM_NETWORKEDITOR_USE_PYTHON)
		# The network upgrader embeds the scripts at compile time (.incbin
		# needs absolute paths) and must be rebuilt when they change.
		set(_migration_script "${CMAKE_CURRENT_SOURCE_DIR}/migrationScript")
		target_compile_definitions(NetworkEditor
			PRIVATE
				CLAM_NETWORKEDITOR_USE_PYTHON
				NETWORKEDITOR_MIGRATION_SCRIPT="${_migration_script}"
				CLAM_REFACTOR_SCRIPT="${CLAM_REFACTOR_SCRIPT}"
		)
		set_source_files_properties(src/NetworkUpgrader.cxx PROPERTIES
			OBJECT_DEPENDS "${_migration_script};${CLAM_REFACTOR_SCRIPT}"
		)
		target_link_libraries(NetworkEditor PRIVATE ${CLAM_NETWORKEDITOR_PYTHON_LIBRARIES})
	endif()

	# Command line players and checker for networks. They link the monitors
	# too, so that networks containing monitors can still be instantiated.
	# Nothing in them references the monitors directly (the processings
	# register themselves in the factory), so keep --as-needed linkers from
	# dropping the library, as the SCons build did.
	foreach(_tool IN LISTS CLAM_NETWORKEDITOR_TOOLS)
		add_executable(${_tool}
			"${CMAKE_CURRENT_SOURCE_DIR}/src/${_tool}Main.cxx"
			${NETWORKEDITOR_VERSION_SOURCE}
		)
		clam_set_legacy_cxx(${_tool})
		target_include_directories(${_tool}
			PRIVATE
				"${NETWORKEDITOR_VERSION_INCLUDE_DIR}"
				"${NETWORKEDITOR_VERSION_INCLUDE_DIR}/CLAM"
		)
		target_link_libraries(${_tool} PRIVATE networkeditor_processings CLAM::qtmonitors)
		if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
			target_link_options(${_tool} PRIVATE "LINKER:--no-as-needed")
		endif()
	endforeach()

	# Runs QML and legacy Qt Designer interfaces against a network.
	add_executable(Prototyper
		"${CMAKE_CURRENT_SOURCE_DIR}/src/prototyper/main.cxx"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/prototyper/PrototypeLoader.cxx"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/prototyper/PrototypeLoader.hxx"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/prototyper/interfaces.qrc"
		${NETWORKEDITOR_VERSION_SOURCE}
	)
	clam_set_legacy_cxx(Prototyper)
	target_include_directories(Prototyper
		PRIVATE
			"${CMAKE_CURRENT_SOURCE_DIR}/src/prototyper"
			"${NETWORKEDITOR_VERSION_INCLUDE_DIR}"
			"${NETWORKEDITOR_VERSION_INCLUDE_DIR}/CLAM"
	)
	target_link_libraries(Prototyper PRIVATE CLAM::qmlwidgets Qt6::UiTools)
	target_compile_definitions(Prototyper PRIVATE CLAM_QML_INSTALL_DIR="${CMAKE_INSTALL_LIBDIR}/qt6/qml")
	if(BUILD_TESTING)
		add_executable(QmlWidgetsTest
			test/QmlWidgetsTest.cxx
			src/prototyper/PrototypeLoader.cxx
			src/prototyper/PrototypeLoader.hxx
			src/prototyper/interfaces.qrc
		)
		clam_set_legacy_cxx(QmlWidgetsTest)
		target_include_directories(QmlWidgetsTest PRIVATE src/prototyper)
		target_compile_definitions(QmlWidgetsTest PRIVATE
			CLAM_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
			CLAM_QML_BUILD_DIR="${CMAKE_BINARY_DIR}/qml"
			CLAM_QML_INSTALL_DIR="${CMAKE_INSTALL_LIBDIR}/qt6/qml"
		)
		target_link_libraries(QmlWidgetsTest PRIVATE CLAM::qmlwidgets Qt6::UiTools)
		add_test(NAME networkeditor_qml_widgets COMMAND QmlWidgetsTest)
		set_tests_properties(networkeditor_qml_widgets PROPERTIES
			ENVIRONMENT "QT_QPA_PLATFORM=offscreen;QT_QUICK_BACKEND=software"
			WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
			TIMEOUT 30
		)
		add_test(NAME prototyper_help COMMAND Prototyper --help)
		add_test(NAME prototyper_unavailable_backend COMMAND Prototyper -o -b unavailable example.clamnetwork)
		set_tests_properties(prototyper_unavailable_backend PROPERTIES WILL_FAIL TRUE TIMEOUT 10)
	endif()

	install(TARGETS NetworkEditor Prototyper ${CLAM_NETWORKEDITOR_TOOLS}
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
	)

	if(TARGET Qt6::lrelease)
		qt_add_lrelease(NetworkEditor
			TS_FILES
				"${CMAKE_CURRENT_SOURCE_DIR}/src/i18n/NetworkEditor_ca.ts"
				"${CMAKE_CURRENT_SOURCE_DIR}/src/i18n/NetworkEditor_es.ts"
			QM_FILES_OUTPUT_VARIABLE CLAM_NETWORKEDITOR_QM_FILES
		)
		install(FILES ${CLAM_NETWORKEDITOR_QM_FILES}
			DESTINATION "${CLAM_NETWORKEDITOR_DATADIR}/i18n"
		)
	endif()

	install(DIRECTORY example-data/
		DESTINATION "${CLAM_NETWORKEDITOR_DATADIR}/example-data"
		FILES_MATCHING
			PATTERN "*.clamnetwork"
			PATTERN "*.ui"
			PATTERN "*.qml"
			PATTERN "*.wav"
			PATTERN "*.mp3"
			PATTERN "*.ogg"
	)
	install(FILES resources/NetworkEditor.desktop resources/Prototyper.desktop
		DESTINATION "${CMAKE_INSTALL_DATADIR}/applications"
	)
	install(FILES resources/clam-network.xml
		DESTINATION "${CMAKE_INSTALL_DATADIR}/mime/packages"
	)
	install(FILES resources/NetworkEditor-icon.svg
		DESTINATION "${CLAM_NETWORKEDITOR_DATADIR}"
	)
	install(FILES
		resources/man/man1/NetworkEditor.1
		resources/man/man1/Prototyper.1
		resources/man/man1/OfflinePlayer.1
		resources/man/man1/MonoOfflinePlayer.1
		resources/man/man1/CheckClamNetwork.1
		DESTINATION "${CMAKE_INSTALL_MANDIR}/man1"
	)
endif()

if(CLAM_NETWORKEDITOR_BUILD_DESIGNER_PLUGIN)
	file(GLOB CLAM_DESIGNER_PLUGIN_SOURCES CONFIGURE_DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/src/clamWidgetsPlugin/*.cxx"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/clamWidgetsPlugin/*.hxx"
	)
	add_library(CLAMWidgets MODULE ${CLAM_DESIGNER_PLUGIN_SOURCES})
	clam_set_legacy_cxx(CLAMWidgets)
	target_compile_definitions(CLAMWidgets PRIVATE QT_PLUGIN)
	target_link_libraries(CLAMWidgets PRIVATE CLAM::qtmonitors Qt6::Designer)
	install(TARGETS CLAMWidgets
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/qt6/plugins/designer"
	)
endif()
