#include <string>
#include <map>
#include "DataTypes.hxx"

#include "Assert.hxx"


namespace CLAM
{
	/*

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

	class Scope
	{
		ScopeSpec & _spec;
	public:
		Scope(const ScopeSpec & specification) : 
			_spec(specification) 
		{
			
		}
	};

	*/

	class PoolSpec
	{
	public:
		typedef std::map<std::string, unsigned> NamesMap;
	private:
		NamesMap _nameMap;
	public:
		NamesMap & GetNames()
		{
			return _nameMap;
		}

		unsigned GetIndex(const std::string & name) const
		{
			NamesMap::const_iterator it = _nameMap.find(name);
			CLAM_ASSERT(it!=_nameMap.end(),
				"Not such descriptor name on this descriptor scope");
			return it->second;
		}
		unsigned GetNAttributes() const
		{
			return _nameMap.size();
		}

		void Add(const std::string & name)
		{
			GetNames().insert(std::make_pair(name,GetNames().size()));
		}
	};

	class Pool
	{
		typedef TData AttributeType;
		typedef std::vector<AttributeType*> Attributes;
	
		unsigned _size;
		Attributes _attributes;
		const PoolSpec & _spec;
	public:
		Pool(const PoolSpec & spec, unsigned size)
			: _size(size), _spec(spec)
		{
			for (unsigned i = 0; i<size; i++)
				_attributes.push_back(new AttributeType[_size]);
		}
		~Pool()
		{
			Attributes::iterator it = _attributes.begin();
			Attributes::iterator end = _attributes.end();
			for (; it!=end; it++)
				delete [] *it;
		}
		unsigned GetNAttributes()
		{
			return _spec.GetNAttributes();
		}
		unsigned GetSize()
		{
			return _size;
		}

		const AttributeType * Get(const std::string & name) const
		{
			return _attributes[_spec.GetIndex(name)];
		}
		AttributeType * Get(const std::string & name)
		{
			return _attributes[_spec.GetIndex(name)];
		}
	};

}



