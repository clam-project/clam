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
	void Init(const DescriptionDataPool & pool, 
			const std::string & scope,
			const std::string & attribute)
	{
		_pool = &pool;
		_scope = scope;
		_attribute = attribute;
		_current = 0;
		_data = _pool->template GetReadAttributePool<AttributeType>(_scope,_attribute);
	}

	const AttributeType & GetForReading() const
	{
		return _data [_current];
	}

	void Next()
	{
		_current++;
	}

	bool IsInsideScope() const
	{
		return _current < _pool->GetNumberOfContexts(_scope);
	}

private:
	std::string _scope;
	std::string _attribute;
	unsigned _current;
	const DescriptionDataPool * _pool;
	const AttributeType * _data;
};


}





#endif // _Extractor_hxx_

