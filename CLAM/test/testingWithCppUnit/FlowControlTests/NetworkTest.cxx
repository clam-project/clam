/*
* Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
*                         UNIVERSITAT POMPEU FABRA
*
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*/

#include <cppunit/extensions/HelperMacros.h>
#include "Processing.hxx"
#include "Network.hxx"
#include <string>
#include "BaseLoggable.hxx" // also includes <sstream>

namespace CLAMTest {

class NetworkTest;
CPPUNIT_TEST_SUITE_REGISTRATION( NetworkTest );


class NetworkTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( NetworkTest );
		
	CPPUNIT_TEST( testGetProcessing_WhenNoProcessings );
	CPPUNIT_TEST( testGetProcessing_WhenProcessingAdded );
	CPPUNIT_TEST( testGetProcessing_WithTwoProcessings );
	CPPUNIT_TEST( testHasProcessing_WhenHasIt );
	CPPUNIT_TEST( testHasProcessing_WhenHasntIt );
	CPPUNIT_TEST( testDestructor_DeletesChildrenProcessings );
	CPPUNIT_TEST_SUITE_END();


	void testGetProcessing_WhenNoProcessings()
	{
		CLAM::Network net;

		try{
			net.GetProcessing( std::string("not existing processing") );
			CPPUNIT_FAIL("Assert expected, but no exception was thrown");
		}
		catch( CLAM::ErrAssertionFailed& )
		{}
	}

	class DummyProcessing : public CLAM::Processing
	{
		const char* GetClassName() const
		{
			return "DummyProcessing";
		}
		const CLAM::ProcessingConfig & GetConfig() const
		{
			CLAM_ASSERT( false, "This is a dummy processing and doesn't have config" );
		}
		bool ConcreteConfigure( const CLAM::ProcessingConfig & cfg)
		{
			return false;
		}
		bool Do()
		{
			return false;
		}

	};

	void testGetProcessing_WhenProcessingAdded()
	{
		CLAM::Network net;
		
		CLAM::Processing* proc = new DummyProcessing;
		std::string name( "dummy-processing" );
		net.AddProcessing( name, proc );

		CPPUNIT_ASSERT_EQUAL ( proc, &net.GetProcessing( name ) );
	}

	void testGetProcessing_WithTwoProcessings()
	{
		CLAM::Network net;
		
		CLAM::Processing* proc = new DummyProcessing;
		net.AddProcessing( std::string( "the first" ), proc );

		CLAM::Processing* proc2 = new DummyProcessing;
		net.AddProcessing( std::string( "the second" ), proc2 );

		CPPUNIT_ASSERT_EQUAL( proc, &net.GetProcessing( std::string("the first") ) );		
	}

	void testHasProcessing_WhenHasIt()
	{
		CLAM::Network net;
		
		std::string name("the name");
		net.AddProcessing( name,  new DummyProcessing );

		CPPUNIT_ASSERT_EQUAL( true, net.HasProcessing( name ) );
	}

	void testHasProcessing_WhenHasntIt()
	{
		CLAM::Network net;
		
		CPPUNIT_ASSERT_EQUAL( false, net.HasProcessing( std::string("non-existing") ) );
	}

	class LoggableDummyProcessing : public DummyProcessing
	{
		BaseLoggable& _log;
	public:
		LoggableDummyProcessing( BaseLoggable& log ) :
			_log(log)
		{}
		~LoggableDummyProcessing() {
			_log.ToLog() << this << " deleted\n";
		}
	};

	void testDestructor_DeletesChildrenProcessings()
	{
		BaseLoggable log;
		LoggableDummyProcessing* proc1 = new LoggableDummyProcessing(log);
		LoggableDummyProcessing* proc2 = new LoggableDummyProcessing(log);
		
		CLAM::Network* net = new CLAM::Network;
		net->AddProcessing( std::string("first"), proc1 );
		net->AddProcessing( std::string("second"), proc2 );
		delete net;

		std::ostringstream expected;
		expected << proc1 << " deleted\n" << proc2 << " deleted\n";

		CPPUNIT_ASSERT_EQUAL(expected.str(), log.GetLog() );
	}


};

} // namespace 