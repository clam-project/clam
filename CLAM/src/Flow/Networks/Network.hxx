#ifndef _Network_hxx_
#define _Network_hxx_

#include "Processing.hxx"
#include <string>
#include <map>

namespace CLAM
{


class Network
{
public:
	typedef std::map< std::string, Processing* > ProcessingsMap;
	// destructor
	~Network();

	Processing& GetProcessing( std::string & name );
	void AddProcessing( std::string &, Processing* );
	bool HasProcessing( std::string & name );

private:
	ProcessingsMap _processings;
};

}// namespace
#endif
