#ifndef _Extractor_hxx_
#define _Extractor_hxx_

#include <string>
#include "Pool.hxx"

namespace CLAM
{

template <typename AttributeType>
class Binder 
{
public:
	void Init(DescriptionDataPool & pool, 
			const std::string & scope,
			const std::string & attribute)
	{
		_pool = &pool;
		_scope = scope;
		_attribute = attribute;
		_current = 0;
		_data = _pool->template GetAttributePool<AttributeType>(_scope,_attribute);
	}

	const AttributeType & GetForReading()
	{
		return _data [_current];
	}

	void Next()
	{
		_current++;
	}

	bool IsInsideScope()
	{
		return _current < _pool->GetNumberOfContexts(_scope);
	}
private:
	unsigned _current;
	DescriptionDataPool * _pool;
	std::string _scope;
	std::string _attribute;
	AttributeType * _data;

};


}





#endif // _Extractor_hxx_

