#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "RhythmDescriptionHelpers.hxx"
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
			mPathToTests = "../../../../../CLAM-TestData/RhythmDescription";
		}

		void tearDown()
		{
		}

	private:
		
		void test_02_alles_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/02_alles_Ticks.xml" );
			
			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/02_alles_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );
		}

		void test_12_cubic_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/12_cubic_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE(	"Error loading back to back test data: problem with RhythmDescription/Tick/12_cubic_Ticks.xml",
						 inst.IsValid() == true );

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;
			
			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );
		}

		void test_blue_monday_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/blue_monday_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/blue_monday_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_ALoCubano_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/ALoCubano_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/ALoCubano_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_AmourEnPoudre_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/AmourEnPoudre_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/AmourEnPoudre_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_Amsterdam_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/Amsterdam_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/Amsterdam_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_clicseq_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/clicseq_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/clicseq_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_drums_electronic_guit_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/drums-electronic-guit_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/drums-electronic-guit_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_elecpiano_drums_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/elecpiano-drums_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/elecpiano-drums_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_test1_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/test1_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/test1_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_test2_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/test2_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/test2_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}

		void test_test_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/test_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/test_Ticks.xml",
						inst.IsValid() == true);

			CLAM::DataArray extractedEvents;
			CLAM::TData     tickRate = 0;

			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents, tickRate );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(tickRate) );

		}


	};
}
