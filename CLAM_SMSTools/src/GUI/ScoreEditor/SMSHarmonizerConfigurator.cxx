#include "SMSHarmonizerConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>


namespace CLAMVM
{
	const char* SMSHarmonizerConfigurator::mHelpText = "<html><body><p>The <bf>Frequency Shift</bf> SMS transformation, is an operator over the results of an SMS Analysis, that allows to offset all detected partials by the same frequency differential.</p></body></html>";

	SMSHarmonizerConfigurator::SMSHarmonizerConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );
		
		SetHelpWidgetText();
		mConfig.AddType();
		mConfig.UpdateData();
		mConfig.SetType( "SMSHarmonizer" );
	}

	SMSHarmonizerConfigurator::~SMSHarmonizerConfigurator()
	{
	}

	void SMSHarmonizerConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSHarmonizerConfigurator::GetParametersWidget()
	{
		return NULL;
	}
	
	void SMSHarmonizerConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast<const CLAM::SMSTransformationConfig& >(cfg);
	}

	const CLAM::ProcessingConfig& SMSHarmonizerConfigurator::GetConfig()
	{
		return mConfig;
	}

	Fl_Widget* SMSHarmonizerConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	static SMSConfiguratorFactory::Registrator<SMSHarmonizerConfigurator> regtPitchShiftCfg( "SMSHarmonizer" );


}


