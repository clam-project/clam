#ifndef _Extractor_hxx_
#define _Extractor_hxx_

#include <string>
#include "Pool.hxx"

namespace CLAM
{

template <typename AttributeType>
class Hook 
{
public:
	void Bind(
			const std::string & scope,
			const std::string & attribute)
	{
		_scope = scope;
		_attribute = attribute;
	}

	void Next()
	{
		_current++;
	}

	bool IsInsideScope() const
	{
		return _current < _scopeSize;
	}

protected:
	std::string _attribute;
	std::string _scope;
	unsigned _current;
	unsigned _scopeSize;
};

template <typename AttributeType>
class ReadHook : public Hook<AttributeType>
{
public:
	void Init(const DescriptionDataPool & pool) 
	{
		_pool = &pool;
		_current = 0;
		_data = _pool->template GetReadAttributePool<AttributeType>(_scope,_attribute);
		_scopeSize = _pool->GetNumberOfContexts(_scope);
	}

	const AttributeType & GetForReading() const
	{
		return _data [_current];
	}
private:
	const DescriptionDataPool * _pool;
	const AttributeType * _data;
};


template <typename AttributeType>
class ReadIndirectHook : public Hook<AttributeType>
{
public:
	void Indirect(
		const std::string & scope, 
		const std::string & attribute)
	{
		_indirectionScope = scope;
		_indirectionAttribute = attribute;
	}

	void Init(const DescriptionDataPool & pool) 
	{
		_pool = &pool;
		_current = 0;
		_data = _pool->template GetReadAttributePool<AttributeType>(_scope,_attribute);
		_indirections = _pool->template GetReadAttributePool<unsigned>(_indirectionScope,_indirectionAttribute);
		_scopeSize = _pool->GetNumberOfContexts(_indirectionScope);
		_targetScopeSize = _pool->GetNumberOfContexts(_scope);
	}

	const AttributeType & GetForReading() const
	{
		unsigned indirection = _indirections[_current];
		CLAM_ASSERT(indirection<_targetScopeSize,
			"Invalid cross-scope reference");
		return _data[indirection];
	}
	
private:
	const DescriptionDataPool * _pool;
	std::string _indirectionScope;
	std::string _indirectionAttribute;
	const unsigned * _indirections;
	const AttributeType * _data;
	unsigned _targetScopeSize;
};


template <typename AttributeType>
class WriteHook : public Hook<AttributeType>
{
public:
	void Init(DescriptionDataPool & pool) 
	{
		_pool = &pool;
		_current = 0;
		_data = _pool->template GetAttributePool<AttributeType>(_scope,_attribute);
		_scopeSize = _pool->GetNumberOfContexts(_scope);
	}

	AttributeType & GetForWriting() const
	{
		return _data [_current];
	}

private:
	DescriptionDataPool * _pool;
	AttributeType * _data;
};



}





#endif // _Extractor_hxx_

