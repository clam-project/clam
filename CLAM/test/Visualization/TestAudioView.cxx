#include "Audio.hxx"
#include "AudioFileIn.hxx"
#include "AudioView.hxx"
#include "StdioAudioPresentation.hxx"
#include "Err.hxx"
#include <iostream>
#include <exception>

using CLAM::Audio;
using CLAM::EAudioFileType;
using CLAM::AudioFileConfig;
using CLAM::AudioFileIn;
using CLAMGUI::AudioView;
using CLAMGUI::StdioAudioPresentation;

bool TestBasicUseCase()
{

		AudioFileConfig audioLoadCfg;
		
		audioLoadCfg.SetFilename( "./TestWav.wav" );
		audioLoadCfg.SetFiletype( EAudioFileType::eWave );
		
		Audio       audioObj;
		AudioFileIn audioLoad;
		
		audioLoad.Configure( audioLoadCfg );
		audioLoad.Start();
		audioObj.SetSize( audioLoad.Size() );
		audioLoad.Do( audioObj );
		
		audioLoad.Stop();

		// View and Presentation now talk with each other

		AudioView                view;
		StdioAudioPresentation   presentation;

		view.BindTo( &audioObj );
		presentation.AttachTo( view );

		view.Refresh();
		presentation.Show();
		
		return true;
}


int main( int argc, char** argv )
{
		try
		{
				if ( !TestBasicUseCase() )
						std::cerr << "Basic Use case Test...... FAILED!" << std::endl;
				else
						std::cerr << "Basic Use case Test...... PASSED!" << std::endl;
				
		}
		catch ( CLAM::Err& e )
		{
				std::cerr << "A CLAM controlled error has occured" << std::endl;
				e.Print();
				std::cerr << "<==== END OF ERROR MESSAGE" << std::endl;

				return -1;
		}
		catch( std::exception& e )
		{
				std::cerr << "An standard library error has occured" << std::endl;
				std::cerr << e.what() << std::endl;
				std::cerr << "<==== END OF ERROR MESSAGE" << std::endl;

				return -1;
		}
		catch( ... )
		{
				std::cerr << "An unknown error has occured!" << std::endl;
				return -1;
		}

		return 0;
}
