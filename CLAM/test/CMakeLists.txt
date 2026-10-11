# Added from CLAM/CMakeLists.txt when BUILD_TESTING is on, which also finds
# cppunit (CLAM_CPPUNIT_TARGET).

set(CLAM_UNIT_TEST_EXTRA_LIBS)
if(CLAM_XML_BACKEND STREQUAL "xercesc")
	# Some XML tests talk to the Xerces DOM directly.
	list(APPEND CLAM_UNIT_TEST_EXTRA_LIBS PkgConfig::XERCESC)
endif()

set(CLAM_TEST_DATA_PATH "" CACHE PATH "CLAM test data checkout used by the data-driven unit test suites (they are skipped when empty)")

set(_unit_dir "${CMAKE_CURRENT_SOURCE_DIR}/UnitTests")
set(CLAM_UNIT_TEST_FOLDERS
	CommonHelpers
	ControlsTests
	DescriptorsTests
	DynamicTypeTests
	FactoryTest
	FlowControlTests
	NonComponentData
	PortsTests
	ProcessingBaseTests
	ProcessingDataTests
	ProcessingsTests
	StandardTests
	TonalAnalysis
	ToolsTests
	XmlTests
	VisualizationTests
)
file(GLOB CLAM_UNIT_TEST_SOURCES CONFIGURE_DEPENDS "${_unit_dir}/*.cxx")
foreach(_folder IN LISTS CLAM_UNIT_TEST_FOLDERS)
	file(GLOB _sources CONFIGURE_DEPENDS "${_unit_dir}/${_folder}/*.cxx")
	list(APPEND CLAM_UNIT_TEST_SOURCES ${_sources})
endforeach()
# Same exclusions as UnitTests/SConscript (see the reasons there).
list(REMOVE_ITEM CLAM_UNIT_TEST_SOURCES
	"${_unit_dir}/TestRunnerQt.cxx"
	"${_unit_dir}/PortsTests/PortsTest.cxx"
	"${_unit_dir}/FactoryTest/PolymorphicTest.cxx"
	"${_unit_dir}/ProcessingsTests/AudioMixerTest.cxx"
	"${_unit_dir}/TonalAnalysis/ChordCorrelatorTest.cxx"
	"${_unit_dir}/TonalAnalysis/ChordExtractorTest.cxx"
	"${_unit_dir}/DescriptorsTests/ProofOfConceptTest.cxx"
	"${_unit_dir}/FlowControlTests/TestsRandomStream.cxx"
	"${_unit_dir}/FlowControlTests/TestsStlBasedStream.cxx"
)

add_executable(CLAMUnitTests ${CLAM_UNIT_TEST_SOURCES})
clam_set_legacy_cxx(CLAMUnitTests)
target_include_directories(CLAMUnitTests PRIVATE
	"${_unit_dir}"
	"${_unit_dir}/CommonHelpers"
	"${CMAKE_CURRENT_SOURCE_DIR}/FunctionalTests/CommonHelpers"
)
target_compile_definitions(CLAMUnitTests PRIVATE
	TEST_DATA_PATH="${CLAM_TEST_DATA_PATH}"
	CLAM_MODULE="tests"
)
target_link_libraries(CLAMUnitTests PRIVATE CLAM::core CLAM::processing CLAM::audioio ${CLAM_CPPUNIT_TARGET} ${CLAM_UNIT_TEST_EXTRA_LIBS})

# These suites read files from the external CLAM test data checkout, so they
# only run when CLAM_TEST_DATA_PATH points at one.
set(CLAM_DATA_DRIVEN_TEST_SUITES
	CLAMTest::AudioDescriptorsTest
	CLAMTest::AudioFileTest
	CLAMTest::NetworkSerializationTest
	CLAMTest::SpectralDescriptorsTest
	CLAMTest::SpectralPeakDescriptorsTest
	CLAMTest::SpectralPeakDetectTest
)
set(_clam_unit_test_args)
if(NOT CLAM_TEST_DATA_PATH)
	foreach(_suite IN LISTS CLAM_DATA_DRIVEN_TEST_SUITES)
		list(APPEND _clam_unit_test_args --skip "${_suite}")
	endforeach()
endif()
add_test(NAME clam_unit_tests
	COMMAND CLAMUnitTests ${_clam_unit_test_args}
	WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
