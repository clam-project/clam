#ifndef _Extractor_hxx_
#define _Extractor_hxx_

#include <string>
#include "Pool.hxx"

namespace CLAM
{

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
	}
	template <typename AttributeType>
	const AttributeType & GetForReading()
	{
		return _pool->template GetAttributePool<AttributeType>(_scope,_attribute)[_current];
	}
	void Next()
	{
		_current++;
	}
private:
	unsigned _current;
	DescriptionDataPool * _pool;
	std::string _scope;
	std::string _attribute;
		
};


}





#endif // _Extractor_hxx_

