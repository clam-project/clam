#define SALTO_CONSOLE
#ifndef SALTO_CONSOLE
#include "SALTOApplication.hxx"
typedef SALTO::Application SaltoApplication;
#else 
#include "SALTOConsoleApplication.hxx"
typedef SALTO::ConsoleApplication SaltoApplication;
#endif
#include "Err.hxx"
#include <exception>
using CLAM::Err;

int main( int argc, char** argv )
{
	try
		{
			SaltoApplication app;
			app.Run( argc, argv );
		}
	catch ( Err& e )
		{
			e.Print();
			exit(-1);
		}
	catch( std::exception& e )
		{
			std::cerr << e.what() << std::endl;
			exit(-1);
		}
	catch( ... )
		{
			std::cerr << "FATAL: something nasty has happened..." << std::endl;
			exit(-1);
		}

	return 0;
}
