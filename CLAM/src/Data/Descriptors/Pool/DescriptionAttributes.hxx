#ifndef _DescriptionAttributes_hxx_
#define _DescriptionAttributes_hxx_

#include <typeinfo>
#include "Assert.hxx"
#include "Storage.hxx"
#include "XMLAdapter.hxx"
#include "XMLArrayAdapter.hxx"
#include "XMLComponentAdapter.hxx"
#include "Component.hxx"

/**
 * @group Descriptors Pool
 */



namespace CLAM
{
	class AbstractAttribute
	{
	public:
		AbstractAttribute(const std::string & attributeName) : _attributeName(attributeName) {}
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
		const std::string & GetName()
		{
			return _attributeName;
		}
	protected:
		virtual const std::type_info & TypeInfo() const = 0;
	private:
		std::string _attributeName;
	};

	template <typename AttributeType>
	class Attribute : public AbstractAttribute
	{
	public:
		Attribute(const std::string & attributeName) : AbstractAttribute(attributeName) {}
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
			XMLAdapter<std::string> nameAdapter(GetName(),"name",false);
			storage.Store(nameAdapter);
			XmlDumpConcreteData(storage,(AttributeType*)data,size,(AttributeType*)0);
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
			for (unsigned i=0 ; i < size ; i++ )
			{
				XMLComponentAdapter componentAdapter(data[i],data[i].GetClassName(),true);
				storage.Store(componentAdapter);
			}
		}
	protected:
		virtual const std::type_info & TypeInfo() const
		{
			return typeid(AttributeType);
		}
	};

}



#endif// _DescriptionAttributes_hxx_
