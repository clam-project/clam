#include <string>
#include <map>
#include <typeinfo>

#include "Assert.hxx"

/**
 * @group Descriptors Pool
 */



namespace CLAM
{
	class AbstractPoolAttribute
	{
	public:
		virtual ~AbstractPoolAttribute() {}
		virtual void * Allocate(unsigned size) = 0;
		virtual void Deallocate(void * data) = 0;
		template <typename TypeToCheck>
		void CheckType() const
		{
			CLAM_ASSERT(typeid(TypeToCheck)==TypeInfo(),
				"Type Missmatch using a pool");
		}
	protected:
		virtual const std::type_info & TypeInfo() const = 0;
	};

	template <typename AttributeType>
	class PoolAttribute : public AbstractPoolAttribute
	{
	public:
		virtual void * Allocate(unsigned size)
		{
			return new AttributeType[size];
		}
		virtual void Deallocate(void * data)
		{
			delete [] (AttributeType*)data;
		}
	protected:
		virtual const std::type_info & TypeInfo() const
		{
			return typeid(AttributeType);
		}
	};

	class PoolSubPoolAttribute : public AbstractPoolAttribute
	{
		std::string _specName;
	public:
		PoolSubPoolAttribute(const std::string & specName):
			_specName(specName)
		{
		}
		virtual void * Allocate(unsigned size)
		{
			return 0;
//			return new Pool[size];
		}
		virtual void Deallocate(void * data)
		{
//			delete [] (Pool*)data;
		}
	protected:
		virtual const std::type_info & TypeInfo() const
		{
			return typeid(int);
		}
	};

	class PoolSpec
	{
	public:
		typedef std::map<std::string, unsigned> NamesMap;
		typedef std::vector<AbstractPoolAttribute *> Attributes;
	private:
		NamesMap _nameMap;
		Attributes _attributes;
	public:
		~PoolSpec()
		{
			Attributes::iterator it = _attributes.begin();
			Attributes::iterator end = _attributes.end();
			for (; it!=end; it++)
				delete *it;
		}

		template <typename AttributeType>
		void Add(const std::string & name)
		{
			unsigned pos = _nameMap.size();
			bool inserted = 
				_nameMap.insert(std::make_pair(name,pos)).second;
			CLAM_ASSERT(inserted,"ScopeSpec::Add, Attribute already present");
			_attributes.push_back(new PoolAttribute<AttributeType>);
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

		void * Allocate(unsigned attribute, unsigned size) const
		{
			return _attributes[attribute]->Allocate(size);
		}
		void Deallocate(unsigned attribute, void * buffer) const
		{
			_attributes[attribute]->Deallocate(buffer);
		}

		template <typename AttributeType>
		void CheckType(unsigned pos, AttributeType *) const
		{
			_attributes[pos]->CheckType<AttributeType>();
		}
	};

	class Pool
	{
	public:
		typedef std::vector<void*> Attributes;
	private:
		unsigned _size;
		Attributes _attributes;
		const PoolSpec & _spec;
	public:
		Pool(const PoolSpec & spec, unsigned size=0)
			: _size(0), _spec(spec)
		{
			_attributes.resize(_spec.GetNAttributes());
			Allocate(size);
		}
		~Pool()
		{
			Deallocate();
		}
	private:
		void Deallocate()
		{
			if (!_size) return;
			Attributes::iterator it = _attributes.begin();
			Attributes::iterator end = _attributes.end();
			for (unsigned i=0; it!=end; i++, it++)
				_spec.Deallocate(i, *it);
			_size=0;
		}
		void Allocate(unsigned newSize)
		{
			if (!newSize) return;
			_size = newSize;
			for (unsigned i = 0; i<_spec.GetNAttributes(); i++)
				_attributes[i]=_spec.Allocate(i,_size);
		}
	public:
		unsigned GetNAttributes()
		{
			return _spec.GetNAttributes();
		}
		unsigned GetSize()
		{
			return _size;
		}
		void SetSize(unsigned newSize)
		{
			Deallocate();
			Allocate(newSize);
		}

		template <typename AttributeType>
		const AttributeType * Get(const std::string & name) const
		{
			CLAM_ASSERT(_size,"Getting an attribute from a zero size pool");
			unsigned attribPos = _spec.GetIndex(name);
			_spec.CheckType(attribPos,(AttributeType*)0);
			return (const AttributeType*) _attributes[attribPos];
		}

		template <typename AttributeType>
		AttributeType * Get(const std::string & name)
		{
			CLAM_ASSERT(_size,"Getting an attribute from a zero size pool");
			unsigned attribPos = _spec.GetIndex(name);
			_spec.CheckType(attribPos,(AttributeType*)0);
			return (AttributeType*) _attributes[attribPos];
		}
	};


	class ScopeRegistry
	{
	public:
		ScopeRegistry()
		{
		}

		template <typename AttributeSpec>
		void AddAttribute(const std::string &scope, const std::string & name)
		{
		}

		void Get(const std::string & name)
		{
			CLAM_ASSERT(false,"No scope registered with that name");
		}

	};
}



