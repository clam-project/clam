#include <typeinfo>
#include "Assert.hxx"
#include "Storage.hxx"
#include "XMLArrayAdapter.hxx"
#include "Component.hxx"

/**
 * @group Descriptors Pool
 */



namespace CLAM
{
	class AbstractAttribute
	{
	public:
		virtual ~AbstractAttribute() {}
		virtual void * Allocate(unsigned size) = 0;
		virtual void Deallocate(void * data) = 0;
		virtual void XmlDumpData(Storage & storage, const void * data, unsigned size ) = 0;
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
	class Attribute : public AbstractAttribute
	{
	public:
		typedef AttributeType DataType;
		virtual void * Allocate(unsigned size)
		{
			return new AttributeType[size];
		}
		virtual void Deallocate(void * data)
		{
			delete [] (AttributeType*)data;
		}
		virtual void XmlDumpData(Storage & storage, const void * data, unsigned size )
		{
			XmlDumpConcreteData(storage,data,size,(AttributeType*)0);
		}
	private:
		template <typename T>
		void XmlDumpConcreteData(Storage & storage, const T * data, unsigned size, void * discriminator )
		{
			XMLArrayAdapter<AttributeType> dataAdapter((AttributeType*)data, size);
			storage.Store(dataAdapter);
		}
		template <typename T>
		void XmlDumpConcreteData(Storage & storage, const T * data, unsigned size, Component * discriminator )
		{
		}
	protected:
		virtual const std::type_info & TypeInfo() const
		{
			return typeid(AttributeType);
		}
	};

}



