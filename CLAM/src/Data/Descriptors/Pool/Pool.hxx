#include <string>
#include <map>
#include "DataTypes.hxx"


namespace CLAM
{

	class CalculationSpec
	{
	};

	class ScopeSpec
	{
		typedef std::map<std::string, CalculationSpec> CalculationsMap;
		CalculationsMap _calculations;
	public:
		void Add(const std::string & name, const CalculationSpec & calculation)
		{
			_calculations.insert(make_pair(name,calculation));
		}
		std::string DescriptorsList()
		{
			std::string result;
			CalculationsMap::iterator it = _calculations.begin();
			CalculationsMap::iterator end = _calculations.end();
			for (; it!=end; it++)
			{
				result += it->first;
				result += '\n';
			}
			return result;
		}
		
	};

/*
	class Scope
	{
		ScopeSpec * _spec;
	public:
	};

*/
}



