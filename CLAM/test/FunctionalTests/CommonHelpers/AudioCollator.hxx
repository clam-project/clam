#ifndef AudioCollator_hxx
#define AudioCollator_hxx

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "Audio.hxx"
#include "AudioFileIn.hxx"
#include "AudioFileOut.hxx"

#include <string>
#include <fstream> // used for open(..) : we want to check if some file exists

namespace CLAMTest
{
	/// checks that a given filename exists
	bool helperFileExist(const std::string& filename)
	{
		std::fstream file;
		file.open( filename.c_str() );
		return file.is_open();
	}
	
	/// Loads audio from file using processing AudioFileIn
	void helperLoadAudioFromFile(const std::string filename, CLAM::Audio& audioToWrite )
	{
		CPPUNIT_ASSERT( helperFileExist(filename) );
		CLAM::AudioFileConfig conf;
		conf.SetFilename(filename);
		conf.SetFiletype(CLAM::EAudioFileType::eWave);

		CLAM::AudioFileIn in;
		in.Configure(conf);
		in.Start();
		
		audioToWrite.SetSize( in.Size() );
		in.Do( audioToWrite );
		in.Stop();
	}
	
	/// Saves audio to file using processig AudioFileOut
	void helperSaveAudioToFile( const CLAM::Audio& audio, const std::string filename )
	{
		CLAM::AudioFileConfig conf;
		conf.SetFilename( filename );
		conf.SetFiletype(CLAM::EAudioFileType::eWave);
		CLAM::AudioFileOut out;
		out.Configure(conf);
		out.Start();
		out.Do( audio );
		out.Stop();
	}

	
	/// this helper func. should result in the future in a processing or assert. 
	bool helperAudiosAreEqual(
			CLAM::Audio& first, 
			CLAM::Audio& second, 
			std::string& whyDifferents, 
			double delta = 0.001)
	{
		if (first.GetSize() != second.GetSize() )
		{	
			std::stringstream formatter;
			formatter << "different audio sizes: first " << 
				first.GetSize() << " second " << second.GetSize();
			whyDifferents += formatter.str();
			return false;
		}

		CLAM::DataArray& samplesFirst = first.GetBuffer();
		CLAM::DataArray& samplesSecond = second.GetBuffer();
		for (int i=0; i<samplesFirst.Size(); i++)
		{
			if( fabs(samplesFirst[i] - samplesSecond[i]) > delta )
			{
				std::stringstream formatter;
				formatter << "found a different sampler in position " << 
					i << " first value is "<< samplesFirst[i] <<
					" second value is " << samplesSecond[i] << 
					" with delta = " << delta;
				whyDifferents += formatter.str();
				return false;
			}
		}
		return true;
	}
	/// shorthand for loading an audio and then call
	/// helperAudiosAreEqual
	bool helperAudioIsEqualToAudioFile( 
		CLAM::Audio& audio, 
		const std::string& audioFile, 
		std::string& whyDifferents,
		double delta=0.0001 )
	{
		whyDifferents = "comparing with file: "+ audioFile+" ";
		CLAM::Audio loadedAudio;
		helperLoadAudioFromFile( audioFile, loadedAudio);
		bool result = helperAudiosAreEqual(audio, loadedAudio, whyDifferents, delta);
				
		return result;
	}


	
} // namespace CLAMTest

#endif

