#include <string>
#include <map>
#include "DataTypes.hxx"

#include "Assert.hxx"


namespace CLAM
{
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
			bool inserted = 
				GetNames().insert(std::make_pair(name,GetNames().size())).second;
			CLAM_ASSERT(inserted,"ScopeSpec::Add, Attribute already present");
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
			for (unsigned i = 0; i<_spec.GetNAttributes(); i++)
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



