#include "Segment.hxx"
#include "SinTracksView.hxx"
#include "StdioSinTracksPresentation.hxx"
#include "XMLStorage.hxx"
#include "Err.hxx"
#include <iostream>
#include <exception>
#include <string>

using CLAM::Segment;
using CLAM::XMLStorage;
using CLAMGUI::SinTracksView;
using CLAMGUI::StdioSinTracksPresentation;

static const char* sPathToData = "./DataSets/";

bool TestBasicUseCase( SinTracksView& view, StdioSinTracksPresentation& presentation )
{
		XMLStorage x;
		Segment    segmentObj;
		
		std::string filename = "Segment.xml";
		std::string pathToFile = sPathToData;

		pathToFile+=filename;

		x.Restore( segmentObj, pathToFile );

		std::cout << "SEGMENT RESTORED" << std::endl;

		view.BindTo( &segmentObj );

		view.Refresh();
		std::cout << "DATA ACQUISITION DONE!" << std::endl;
		presentation.Show();

		return true;
}

int main( int argc, char** argv )
{
		try
		{
				SinTracksView                view;
				StdioSinTracksPresentation   presentation;

				presentation.AttachTo( view );

				std::cerr << "BASIC SPECTRAL PEAK ARRAY USE CASE TEST LAUNCHED" << std::endl;
				if ( !TestBasicUseCase( view, presentation ) )
						std::cerr << "Basic Use case Test...... FAILED!" << std::endl;
				else
						std::cerr << "Basic Use case Test...... Passed!" << std::endl;

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
