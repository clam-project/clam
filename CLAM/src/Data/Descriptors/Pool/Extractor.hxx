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
	virtual ~Hook(){}
	void Bind(
			const std::string & scope,
			const std::string & attribute)
	{
		_scope = scope;
		_attribute = attribute;
	}

	virtual void Next()
	{
		_current++;
	}

	virtual bool IsInsideScope() const
	{
		return _current < GetIterationSize();
	}
protected:
	virtual unsigned GetIterationSize() const =0;

	std::string _attribute;
	std::string _scope;
	unsigned _current;
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
	}

	const AttributeType & GetForReading() const
	{
		return _data [GetCurrent()];
	}

	virtual unsigned GetIterationSize() const
	{
		return _pool->GetNumberOfContexts(_scope);
	}

private:
	unsigned GetCurrent() const
	{
		return _current;
	}
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
		_chained.Bind(scope,attribute);
	}

	void Init(const DescriptionDataPool & pool) 
	{
		_pool = &pool;
		_current = 0;
		_data = _pool->template GetReadAttributePool<AttributeType>(_scope,_attribute);
		_chained.Init(pool);
		_targetScopeSize = _pool->GetNumberOfContexts(_scope);
	}

	const AttributeType & GetForReading() const
	{
		return _data[GetCurrent()];
	}
	virtual unsigned GetIterationSize() const
	{
		return _chained.GetIterationSize();
	}
	virtual void Next()
	{
		_chained.Next();
	}

	virtual bool IsInsideScope() const
	{
		return _chained.IsInsideScope();
	}
private:
	unsigned GetCurrent() const
	{
		unsigned indirection = _chained.GetForReading();
		CLAM_ASSERT(indirection<_targetScopeSize,
			"Invalid cross-scope reference");
		return indirection;
	}
	
	const DescriptionDataPool * _pool;
	const AttributeType * _data;
	unsigned _targetScopeSize;
	ReadHook<unsigned> _chained;
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
	}

	AttributeType & GetForWriting() const
	{
		return _data [GetCurrent()];
	}

protected:
	virtual unsigned GetIterationSize() const
	{
		return _pool->GetNumberOfContexts(_scope);
	}
private:
	unsigned GetCurrent() const
	{
		return _current;
	}
	DescriptionDataPool * _pool;
	AttributeType * _data;
};



}





#endif // _Extractor_hxx_

