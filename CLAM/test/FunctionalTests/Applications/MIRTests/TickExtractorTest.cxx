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
		CPPUNIT_TEST( test_ALoCubano_Ticks );
		CPPUNIT_TEST( test_AmourEnPoudre_Ticks );
		CPPUNIT_TEST( test_Amsterdam_Ticks );
		CPPUNIT_TEST( test_blue_monday_Ticks );
		CPPUNIT_TEST( test_clicseq_Ticks );
		CPPUNIT_TEST( test_drums_electronic_guit_Ticks );
		CPPUNIT_TEST( test_elecpiano_drums_Ticks );
		CPPUNIT_TEST( test_test1_Ticks );
		CPPUNIT_TEST( test_test2_Ticks );
		CPPUNIT_TEST( test_test_Ticks );


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
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );
		}

		void test_12_cubic_Ticks()
		{
			RythmDescriptionTests::RythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/12_cubic_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE(	"Error loading back to back test data: problem with RythmDescription/Tick/12_cubic_Ticks.xml",
						 inst.IsValid() == true );

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;
			
			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );
		}

		void test_blue_monday_Ticks()
		{
			RythmDescriptionTests::RythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/blue_monday_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RythmDescription/Tick/blue_monday_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_ALoCubano_Ticks()
		{
			RythmDescriptionTests::RythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/ALoCubano_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RythmDescription/Tick/ALoCubano_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_AmourEnPoudre_Ticks()
		{
			RythmDescriptionTests::RythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/AmourEnPoudre_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RythmDescription/Tick/AmourEnPoudre_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_Amsterdam_Ticks()
		{
			RythmDescriptionTests::RythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/Amsterdam_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RythmDescription/Tick/Amsterdam_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_clicseq_Ticks()
		{
			RythmDescriptionTests::RythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/clicseq_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RythmDescription/Tick/clicseq_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_drums_electronic_guit_Ticks()
		{
			RythmDescriptionTests::RythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/drums-electronic-guit_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RythmDescription/Tick/drums-electronic-guit_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_elecpiano_drums_Ticks()
		{
			RythmDescriptionTests::RythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/elecpiano-drums_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RythmDescription/Tick/elecpiano-drums_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_test1_Ticks()
		{
			RythmDescriptionTests::RythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/test1_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RythmDescription/Tick/test1_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_test2_Ticks()
		{
			RythmDescriptionTests::RythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/test2_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RythmDescription/Tick/test2_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_test_Ticks()
		{
			RythmDescriptionTests::RythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/test_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RythmDescription/Tick/test_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}


	};
}
