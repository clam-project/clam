#ifndef __SALTOCONSOLEAPPLICATION__
#define __SALTOCONSOLEAPPLICATION__

#include <string>
#include "SALTOKernel.hxx"
#include "Err.hxx"

namespace SALTO
{

using CLAM::Err;

class ConsoleApplication
{
public:
	ConsoleApplication();

	~ConsoleApplication();

	void Run( int argc, char** argv );

protected:
	
	void ParseArgs( int argc, char** argv ) throw (Err);


private:

	Kernel      mKernel;
	std::string melodyFile;
	std::string audioFile;
};

}

#endif
