#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "BPF.hxx"


namespace CLAMTest
{
	class BPFTest;
	
	CPPUNIT_TEST_SUITE_REGISTRATION( BPFTest );

	class BPFTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( BPFTest );

		CPPUNIT_TEST( test_After_SetValue_i_Y_GetValue_i_Yields_Y );
		CPPUNIT_TEST( test_After_SetValue_0_Y_GetValueFromIndex_0_Yields_Y );


		CPPUNIT_TEST_SUITE_END();

	public:

		void setUp()
		{
		}

		void tearDown()
		{
		}

	private:

		void test_After_SetValue_i_Y_GetValue_i_Yields_Y()
		{
			CLAM::BPF bpf;

			bpf.Resize(2);
			bpf.SetSize(2);

			bpf.SetXValue( 0, CLAM::TData(0) );
			bpf.SetValue( 0, CLAM::TData(1) );
			bpf.SetXValue( 1, CLAM::TData(1) );
			bpf.SetValue( 1, CLAM::TData(0) );

			CPPUNIT_ASSERT( bpf.GetValue(CLAM::TData(0)) == CLAM::TData(1) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex(0) == CLAM::TData(1) );
			CPPUNIT_ASSERT( bpf.GetValue(CLAM::TData(1)) == CLAM::TData(0) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex(1) == CLAM::TData(0) );
		}

		void test_After_SetValue_0_Y_GetValueFromIndex_0_Yields_Y()
		{
			CLAM::BPF bpf;

			bpf.Resize(1);
			bpf.SetSize(1);

			bpf.SetValue( 0, CLAM::TData(1) );

			CPPUNIT_ASSERT( bpf.GetValueFromIndex(0) == CLAM::TData(1) );

		}
	};
}
