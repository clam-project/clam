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
#include "AudioMixer.hxx"
#include "Processing.hxx"
#include "Audio.hxx"
#include "OutControl.hxx"

namespace CLAMTest{

class AudioMixerTest;
CPPUNIT_TEST_SUITE_REGISTRATION( AudioMixerTest );

class AudioMixerTest : public CppUnit::TestFixture
{
public:

	void setUp()
	{

		//add link to in controls of mixer
		_out1.AddLink(&_mixer.mGain[0]);
		_out2.AddLink(&_mixer.mGain[1]);

		_inAudio1.SetSize(1);
		_inAudio2.SetSize(1);
		_outAudio.SetSize(1);

		CLAM::AudioMixerConfig mixerCfg;
		mixerCfg.SetFrameSize(1);
		_mixer.Configure(mixerCfg);

		_mixer.mInput[0].Attach(_inAudio1);
		_mixer.mInput[1].Attach(_inAudio2);
		_mixer.mOutput.Attach(_outAudio);
		

	}
	void tearDown(){}

private:

	CPPUNIT_TEST_SUITE( AudioMixerTest );

	CPPUNIT_TEST( testDo_WhenControlsGivesEqualValue );

	CPPUNIT_TEST_SUITE_END();
	
	//processing
	CLAM::OutControl _out1;
	CLAM::OutControl _out2;
	CLAM::AudioMixer<2> _mixer;

	//data
	CLAM::Audio _inAudio1;
	CLAM::Audio _inAudio2;
	CLAM::Audio _outAudio;

	AudioMixerTest()
		: _out1("Sender left"),
		  _out2("Sender right")
	{		
	}

	void testDo_WhenControlsGivesEqualValue()
	{
		_inAudio1.GetBuffer()[0] = 1;
		_inAudio2.GetBuffer()[0] = 1;
		_out1.SendControl(0.5);
		_out2.SendControl(0.5);
		_mixer.Do();
		
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.5) , _outAudio.GetBuffer()[0],0.000001);
	}
};
	
}
