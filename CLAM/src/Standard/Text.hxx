#ifndef _TEXT_HXX_
#define _TEXT_HXX_

#include <string>
#include <sstream>

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

	std::istream operator >> (std::istream & stream, Text & text)
	{
		std::string temp(
			(std::istream_iterator<char>(stream)),
			std::istream_iterator<char>());
		text = temp;
	}
}


#endif // _TEXT_HXX_
