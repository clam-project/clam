
#include "ProcessingDefinitionAdapter.hxx"
#include "Assert.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "Factory.hxx"
#include "XMLAdapter.hxx"

namespace CLAM
{

	typedef Factory<CLAM::Processing> ProcessingFactory;

	ProcessingDefinitionAdapter::ProcessingDefinitionAdapter( Processing * adaptee, const std::string & name )
		:  mAdaptee(adaptee), mName(name)
	{
	}

	ProcessingDefinitionAdapter::~ProcessingDefinitionAdapter()
	{
	}

	void ProcessingDefinitionAdapter::StoreOn (Storage & store)	
	{
		XMLAdapter<Text> nameAdapter( mName, "id");
		Text className(mAdaptee->GetClassName());
		XMLAdapter<Text> classNameAdapter( className, "type");
		store.Store(&nameAdapter);
		store.Store(&classNameAdapter);

		XMLComponentAdapter configAdapter((ProcessingConfig&)mAdaptee->GetConfig());
		store.Store(&configAdapter);
	}

	void ProcessingDefinitionAdapter::LoadFrom (Storage & store) 
	{	
		XMLAdapter<Text> nameAdapter( mName, "id");
		store.Load(&nameAdapter);
		Text className("");
		XMLAdapter<Text> classNameAdapter( className, "type");
		store.Load(&classNameAdapter);

		mAdaptee = ProcessingFactory::GetInstance().Create(className);
		ProcessingConfig&  cfg = (ProcessingConfig&)mAdaptee->GetConfig();
		XMLComponentAdapter configAdapter( cfg );
		store.Load(&configAdapter);
		mAdaptee->Configure(cfg);
	}
} // namespace CLAM
