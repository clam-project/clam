
namespace CLAMTest
{

	class DummyProduct
	{
	public:
		DummyProduct() {}
		virtual ~DummyProduct() {}
		virtual void dummyVirtual() =0;

	};

	class DummyProductFoo
		: public DummyProduct
	{
	public:
		DummyProductFoo() {}
		virtual ~DummyProductFoo() {}
		virtual void dummyVirtual() {}
	};

	class DummyProductBar
		: public DummyProduct
	{
	public:
		DummyProductBar() {}
		virtual ~DummyProductBar() {}
		virtual void dummyVirtual() {}
	};

}
