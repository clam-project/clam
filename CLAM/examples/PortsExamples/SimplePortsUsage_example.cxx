
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


/**
 *  The goals of this example is to show you how to:
 *      -> Create a CLAM Processing with Ports and Controls
 *      -> Connect them to pass control messages and data to process
 *      -> Illustrate how to use Do() instead of the version with parameters.
 */

#include <iostream>
#include "Err.hxx"
#include "SimpleOscillator.hxx"
#include "AudioManager.hxx"
#include "AudioOut.hxx"
#include "FFT_rfftw.hxx"
#include "IFFT_rfftw.hxx"

int main( int argc, char** argv )
{	
	try
	{
		// we won't comment the code related to audio I/O and processing configuration, because they are questions discussed
		// in another examples (like FilePlayback_example.cxx  and ProcessingLifeCycle_example.cxx).
		int sampleRate = 44100;
		int size = 128;

		CLAM::AudioManager manager(sampleRate, size);
		manager.Start();

		// In this example, we will produce a sine signal with an oscillator, sending the sine to the audio out. 


		CLAM::SimpleOscillator osc;
		CLAM::AudioOut audioOut;
	
		CLAM::FFTConfig fconfig;
		fconfig.SetAudioSize(size);	
		CLAM::IFFTConfig ifconfig;
		ifconfig.SetAudioSize(size);

		CLAM::FFT_rfftw myfft;
		myfft.Configure( fconfig );
		CLAM::IFFT_rfftw myifft;
		myifft.Configure( ifconfig );
		
		// we need to configure the ports with the correct size in order to get the data automatically.
		// the way of accessing the ports is by name, specifying before which kind of port you need.
		osc.GetOutPorts().Get("Audio Output").SetSize( size );
		osc.GetOutPorts().Get("Audio Output").SetHop( size );

		audioOut.GetInPorts().Get("Audio Input").SetSize( size );
		audioOut.GetInPorts().Get("Audio Input").SetHop( size );

		myfft.GetInPorts().Get("Audio Input").SetSize( size );
		myfft.GetInPorts().Get("Audio Input").SetHop( size );
		myifft.GetOutPorts().Get("Audio Output").SetSize( size );
		myifft.GetOutPorts().Get("Audio Output").SetHop( size );

		// after this, is needed to attach the different ports to their respective nodes.
//		osc.GetOutPorts().Get("Audio Output").ConnectToIn( audioOut.GetInPorts().Get("Audio Input") );
		osc.GetOutPorts().Get("Audio Output").ConnectToIn( myfft.GetInPorts().Get("Audio Input") );
		myfft.GetOutPorts().Get("Spectrum Output").ConnectToIn( myifft.GetInPorts().Get("Spectrum Input") );
		myifft.GetOutPorts().Get("Audio Output").ConnectToIn( audioOut.GetInPorts().Get("Audio Input") );
		
		osc.Start();
		myifft.Start();
		myfft.Start();
		audioOut.Start();

		for(int i=0;i<500;i++)
		{
			osc.Do();
			myfft.Do();
			myifft.Do();
			audioOut.Do();
		}

		osc.Stop();
		myfft.Stop();
		myifft.Stop();
		audioOut.Stop();
		
		std::cout << "ok, now I know how to use CLAM ports!" << std::endl;
	}
	catch ( CLAM::Err& e )
	{
		e.Print();
		exit(-1);
	}
	catch( std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		exit(-1);		
	}

	return 0;
}
