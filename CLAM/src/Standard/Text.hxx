#ifndef _TEXT_HXX_
#define _TEXT_HXX_

#include <string>
#include <sstream>
#include <iterator>
#include "TypeInfo.hxx"

namespace CLAM
{
	/**
	 * This class represents an string that can contain any kind of separators.
	 * Use it instead std::string if you want to deserialize
	 * It inherits all the std::string methods.
	 */
	// MRJ: I was obliged to change the alias usage std::string by the long and quite unreadable
	// basic_string<char> thing ( thanks Pau! ) in order to get Text to compile under VisualC++ 6.0. The compiler
	// was issuing this error ( pure nonsense BTW ):
	// src\Standard\Text.hxx(29) : error C2614: 'Text' : illegal member initialization: 'string' is not a base or member
	// src\Standard\Text.hxx(33) : error C2614: 'Text' : illegal member initialization: 'string' is not a base or member
	// I don't like this much, but AFAIK there's no way of 'undoing' a using statement.

	class Text : public std::basic_string<char>
	{
		public:
			Text()
			{
			}
			Text(const std::string & value)
				: std::basic_string<char>(value)
			{
			}
			Text(const char * value)
				: std::basic_string<char>(value)
			{
			}
	};

	inline std::istream & operator >> (std::istream & stream, Text & text)
	{
		
#if 0
		stream.unsetf(std::ios::skipws);
		text = std::string(
			(std::istream_iterator<char>(stream)),
			std::istream_iterator<char>());
#elif 0
		text.assign(
			(std::istreambuf_iterator<char>(stream)),
			std::istreambuf_iterator<char>());
#elif 0
		text="";
		char c;
		while (stream.get(c))
			text.push_back(c);
//			text.append(1,c);
#elif 1
		text = "";
		char buffer[1024];
		stream.read( buffer, 1023 );
		do 
		{
			text.append( buffer, stream.gcount() );
		}
		while( stream.read( buffer, 1023 ) );
#endif
		return stream;

	}

#if 1
	/**
	* This is a kludge to make windows work
	*/
	inline std::istream& operator>>( std::istringstream& stream, CLAM::Text& text )
	{
		std::istream & basestream = stream;
		return basestream >> text;
	}
#endif

	CLAM_TYPEINFOGROUP(BasicCTypeInfo, Text);
}


#endif // _TEXT_HXX_
