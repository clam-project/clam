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

	Processing& GetProcessing( const std::string & name );
	void AddProcessing( const std::string &, Processing* );
	bool HasProcessing( const std::string & name );

protected:
	InPort & GetInPortByCompleteName( const std::string& );
private:
	// fields
	ProcessingsMap _processings;
	// helpers
	static std::size_t PositionOfLastIdentifier( const std::string& );
	static std::size_t PositionOfProcessingIdentifier( const std::string& );
	std::string GetLastIdentifier( const std::string& );
	std::string GetProcessingIdentifier( const std::string& );
	static const char NamesIdentifiersSeparator();
	
};

}// namespace
#endif
