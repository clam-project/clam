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
#include "ControlSender.hxx"
#include "InControl.hxx"

namespace CLAMTest {

class ControlSenderTest;
CPPUNIT_TEST_SUITE_REGISTRATION( ControlSenderTest );

class ControlSenderTest : public CppUnit::TestFixture
{
public:

	void setUp()
	{
		_controlSender._outControl1.AddLink(&_receiverLeft);
		_controlSender._outControl2.AddLink(&_receiverRight);		
	}
	void tearDown(){}

private:

	CPPUNIT_TEST_SUITE( ControlSenderTest );
	CPPUNIT_TEST( testDo_WhenFreqEqualSamplingRateAndNoPhase );
	CPPUNIT_TEST( testDo_WhenFreqEqualSamplingRateWithPhase );
	CPPUNIT_TEST( testDo_WhenFreqIsQuarterOfSamplingRate );
	CPPUNIT_TEST_SUITE_END();
	
	//fixture attributes
	CLAM::InControl _receiverLeft;
	CLAM::InControl _receiverRight;
	ControlSender _controlSender;
	const double _delta;

	ControlSenderTest()
		: _receiverLeft("Receiver Left"),
		  _receiverRight("Receiver Right"),
		  _controlSender(44100, 44100, 0),
		  _delta(0.00001)
		// ControlSender needs a constructor but we won't use
		// this configuration
	{
	}

	void testDo_WhenFreqEqualSamplingRateAndNoPhase()
	{
		CLAM::TData freq = 44100;
		CLAM::TData samplingRate = 44100;
		CLAM::TData phase = 0.0;

		_controlSender.Configure( freq , samplingRate, phase );

		_controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , _receiverLeft.GetLastValue(),_delta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , _receiverRight.GetLastValue(),_delta);

		_controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , _receiverLeft.GetLastValue(),_delta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , _receiverRight.GetLastValue(),_delta);
	}

	void testDo_WhenFreqEqualSamplingRateWithPhase()
	{
		CLAM::TData freq = 44100;
		CLAM::TData samplingRate = 44100;
		CLAM::TData phase = M_PI/2;

		_controlSender.Configure( freq , samplingRate, phase );

		_controlSender.Do();


		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , _receiverLeft.GetLastValue(),_delta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , _receiverRight.GetLastValue(),_delta);

		_controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , _receiverLeft.GetLastValue(),_delta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , _receiverRight.GetLastValue(),_delta);
	}

	void testDo_WhenFreqIsQuarterOfSamplingRate()
	{
		CLAM::TData samplingRate = 44100;
		CLAM::TData freq = samplingRate/4;
		CLAM::TData phase = 0;

		_controlSender.Configure( freq , samplingRate, phase );

		_controlSender.Do();
		//first Do gives the initial state, already tested
		_controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , _receiverLeft.GetLastValue(),_delta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , _receiverRight.GetLastValue(),_delta); 
	}

};

} //namespace CLAMTest


