#ifndef _TEXT_HXX_
#define _TEXT_HXX_

#include <string>
#include <sstream>
#include "TypeInfo.hxx"

namespace CLAM
{
	/**
	 * This class represents an string that can contain any kind of separators.
	 * Use it instead std::string if you want to deserialize
	 * Inherit all the std::string methods.
	 */
	class Text : public std::string
	{
		public:
			Text()
			{
			}
			Text(const std::string & value)
				: std::string(value)
			{
			}
			Text(const char * value)
				: std::string(value)
			{
			}
	};

	inline std::istream & operator >> (std::istream & stream, Text & text)
	{
		
#if 0
		stream.unsetf(std::ios::skipws);
		text.assign(
			(std::istream_iterator<char>(stream)),
			std::istream_iterator<char>());
#elif 0
		text.assign(
			(std::istreambuf_iterator<char>(stream)),
			std::istreambuf_iterator<char>());
#elif 1
		text="";
		char c;
		while (stream.get(c))
			text.push_back(c);
#endif
		return stream;

	}

	CLAM_TYPEINFOGROUP(BasicCTypeInfo, Text);

}


#endif // _TEXT_HXX_
