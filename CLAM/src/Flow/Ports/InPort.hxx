#ifndef _InPort_hxx_
#define _InPort_hxx_

#include "Port.hxx"
#include <string>

namespace CLAM
{

class Processing;

/** Input port */
class InPort: public Port
{
	/** True if the processing object may use the storage object
	 * in this port to write on it the data of an output port
	 */
	bool mCanDoInplace;

public:
	InPort(const std::string &n, Processing *o, int length, int hop = 0, bool inplace=false);
	virtual ~InPort() {}

	bool CanDoInplace(void)  {return mCanDoInplace;}

};


// Implementation:
//-------------------------------------------------------

inline InPort::InPort(const std::string &n,
			   Processing *o,
			   int length,
			   int hop,
			   bool inplace) : 
	Port(n,o,length,hop),
	mCanDoInplace(inplace)
{}


} // namespace CLAM

#endif
