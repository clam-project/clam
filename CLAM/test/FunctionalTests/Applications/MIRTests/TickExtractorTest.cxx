#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "RythmDescriptionHelpers.hxx"
#include "TickExtractor.hxx"
#include "XMLStorage.hxx"

namespace CLAMTest
{
	class TickExtractorTest;
	
	CPPUNIT_TEST_SUITE_REGISTRATION( TickExtractorTest );

	class TickExtractorTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( TickExtractorTest );

		CPPUNIT_TEST( test_02_alles_Ticks );
		CPPUNIT_TEST( test_12_cubic_Ticks );
		CPPUNIT_TEST( test_blue_monday_Ticks );

		CPPUNIT_TEST_SUITE_END();

	protected:

		std::string mPathToTests;

	public:

		void setUp()
		{
			mPathToTests = "../../../../../CLAM-TestData/RythmDescription";
		}

		void tearDown()
		{
		}

	private:
		
		void test_02_alles_Ticks()
		{
			RythmDescriptionTests::RythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/02_alles_Ticks.xml" );
			
			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RythmDescription/Tick/02_alles_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			
			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
		}

		void test_12_cubic_Ticks()
		{
			RythmDescriptionTests::RythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/12_cubic_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE(	"Error loading back to back test data: problem with RythmDescription/Tick/12_cubic_Ticks.xml",
						 inst.IsValid() == true );

			CLAM::DataArray extractedEvents;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
		}

		void test_blue_monday_Ticks()
		{
			RythmDescriptionTests::RythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/blue_monday_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RythmDescription/Tick/blue_monday_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );

		}
	};
}
