#ifndef _Extractor_hxx_
#define _Extractor_hxx_

#include <string>

namespace CLAM
{
	template <typename AttributeType> class ReadBinder;
	class AbstractReadBinder
	{
	public:
		virtual ~AbstractReadBinder() {}
		template <typename AttribType>
		const AttribType & TakeValue()
		{
			return ((ReadBinder<AttribType>*)this)->TakeValue();
		}
	};

	template <typename AttributeType>
	class ReadBinder : public AbstractReadBinder
	{
		const AttributeType * _value;
	public:
		ReadBinder()
		{
		}
		void BindTo(const AttributeType & value)
		{
			_value = &value;
		}
		const AttributeType & TakeValue()
		{
			return *_value;
		}
	};

}





#endif // _Extractor_hxx_

