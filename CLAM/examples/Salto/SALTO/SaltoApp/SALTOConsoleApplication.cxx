#include "SALTOConsoleApplication.hxx"
#include "SALTOKernel.hxx"
#include <cstdlib>
#include <iostream>


namespace SALTO
{

	ConsoleApplication::ConsoleApplication()
		: melodyFile("None.xml"), audioFile("None")
	{
	}

	ConsoleApplication::~ConsoleApplication()
	{
		mKernel.ProcessingCleanup();
	}

	void ConsoleApplication::Run( int argc, char** argv )
	{
		
		ParseArgs( argc, argv );
		mKernel.StartProcessing( melodyFile, audioFile );
		std::cout << "Press Ctrl+C to terminate..." << std::endl;
		mKernel.ProcessingLoop();
	}

	void ConsoleApplication::ParseArgs( int argc, char** argv ) throw ( Err )
	{
		if ( argc > 1 )
			melodyFile = argv[1];
		if ( argc > 2 )
			audioFile = argv[2];
		if ( argc > 3 )
			throw Err("Bad syntax: expected Salto <melodyfile>.xml <audioFile>.wav");
	}
}
