#include "Spectrum.hxx"
#include "LogMagSpectrumAdapter.hxx"
#include "Fl_Spectrum.hxx"
#include "XMLStorage.hxx"
#include "Err.hxx"
#include "WidgetTKWrapper.hxx"
#include <iostream>
#include <exception>
#include <string>

using CLAM::Spectrum;
using CLAMGUI::WidgetTKWrapper;
using CLAMGUI::LogMagSpectrumAdapter;
using CLAMGUI::Fl_Spectrum;
using CLAM::XMLStorage;

static const char* sPathToData= "./DataSets/";

bool TestWindowedUseCase(LogMagSpectrumAdapter& view, Fl_Spectrum& presentation )
{
		XMLStorage  x;
		Spectrum    specObj;

		std::string filename = "ResidualSpectrum.xml";
		std::string pathToFile = sPathToData;

		pathToFile+=filename;


		x.Restore( specObj, pathToFile );  // Spectrum has been incorporated into the system
		
		view.BindTo( &specObj );

		view.Publish();
		presentation.Show();

		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");

		tk.Run();


		return true;

}


bool TestPureComplexUseCase(LogMagSpectrumAdapter& view, Fl_Spectrum& presentation )
{
		XMLStorage  x;
		Spectrum    specObj;

		std::string filename = "SpectrumOnlyComplex.xml";
		std::string pathToFile = sPathToData;

		pathToFile+=filename;


		x.Restore( specObj, pathToFile );  // Spectrum has been incorporated into the system
		
		view.BindTo( &specObj );

		view.Publish();
		presentation.Show();

		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");

		tk.Run();


		return true;

}

bool TestPurePolarUseCase(LogMagSpectrumAdapter& view, Fl_Spectrum& presentation)
{
		XMLStorage  x;
		Spectrum    specObj;

		std::string filename = "SpectrumOnlyPolar.xml";
		std::string pathToFile = sPathToData;

		pathToFile+=filename;


		x.Restore( specObj, pathToFile );  // Spectrum has been incorporated into the system
		
		view.BindTo( &specObj );

		view.Publish();

		presentation.Show();

		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");

		tk.Run();


		return true;

}

bool TestPureBPFUseCase( LogMagSpectrumAdapter& view, Fl_Spectrum& presentation )
{
		XMLStorage  x;
		Spectrum    specObj;

		std::string filename = "SpectrumOnlyBPF.xml";
		std::string pathToFile = sPathToData;

		pathToFile+=filename;


		x.Restore( specObj, pathToFile );  // Spectrum has been incorporated into the system
		
		view.BindTo( &specObj );

		view.Publish();
		presentation.Show();

		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");

		tk.Run();

		return true;
}

bool TestBasicUseCase( LogMagSpectrumAdapter& view, Fl_Spectrum& presentation )
{
		XMLStorage  x;
		Spectrum    specObj;

		std::string filename = "Spectrum.xml";
		std::string pathToFile = sPathToData;

		pathToFile+=filename;


		x.Restore( specObj, pathToFile );  // Spectrum has been incorporated into the system
		
		view.BindTo( &specObj );

		view.Publish();
		presentation.Show();

		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");

		tk.Run();

		return true;
}

int main( int argc, char** argv )
{
		try
		{
				LogMagSpectrumAdapter        view;
				Fl_Spectrum presentation( 100, 100, 640, 480, "An Spectrum Presentation");

				presentation.AttachTo( view );

				std::cerr << "BASIC SPECTRUM VIEW USE CASE TEST LAUNCHED" << std::endl;
				if ( !TestBasicUseCase( view, presentation ) )
						std::cerr << "Basic Use case Test...... FAILED!" << std::endl;
				else
						std::cerr << "Basic Use case Test...... Passed!" << std::endl;

				std::cerr << "ONLY COMPLEX BUFFER ON SPECTRUM OBJECT SPECTRUM VIEW USE CASE TEST LAUNCHED" << std::endl;
				if ( !TestPureComplexUseCase( view, presentation ) )
						std::cerr << "Observing spectrum with only complex buffer use case Test...... FAILED!" << std::endl;
				else
						std::cerr << "Observing spectrum with only complex buffer use case Test...... Passed!" << std::endl;

				std::cerr << "ONLY POLAR BUFFER ON SPECTRUM OBJECT SPECTRUM VIEW USE CASE TEST LAUNCHED" << std::endl;
				if ( !TestPurePolarUseCase( view, presentation ) )
						std::cerr << "Observing spectrum with only polar buffer use case Test...... FAILED!" << std::endl;
				else
						std::cerr << "Observing spectrum with only polar buffer use case Test...... Passed!" << std::endl;

				std::cerr << "ONLY BPF BUFFERS ON SPECTRUM OBJECT SPECTRUM VIEW USE CASE TEST LAUNCHED" << std::endl;
				if ( !TestPureBPFUseCase( view, presentation ) )
						std::cerr << "Observing spectrum with only BPF buffers use case Test...... FAILED!" << std::endl;
				else
						std::cerr << "Observing spectrum with only BPF buffers use case Test...... Passed!" << std::endl;
				if ( !TestWindowedUseCase( view, presentation ) )
						std::cerr << "Observing windowed spectrum  use case Test...... FAILED!" << std::endl;
				else
						std::cerr << "Observing windowed spectrum use case Test...... Passed!" << std::endl;

				

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
