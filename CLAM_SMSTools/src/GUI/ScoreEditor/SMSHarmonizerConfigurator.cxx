#include "SMSHarmonizerConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_Harmonizer.hxx"

namespace CLAMVM
{
	const char* SMSHarmonizerConfigurator::mHelpText = "<html><body><p>Here should go some nice description about what does this SMS transformation do.</p></body></html>";

	SMSHarmonizerConfigurator::SMSHarmonizerConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );
		
		mEditorWidget = new Fl_SMS_Harmonizer( 0, 0, 100, 100 );
		mEditorWidget->end();

		SetHelpWidgetText();
		mConfig.AddType();
		mConfig.UpdateData();
		mConfig.SetType( "SMSHarmonizer" );
	}

	SMSHarmonizerConfigurator::~SMSHarmonizerConfigurator()
	{
		if ( mHelpWidget->parent() == NULL )
			delete mHelpWidget;
		if ( mEditorWidget->parent() == NULL )
			delete mEditorWidget;
	}

	void SMSHarmonizerConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSHarmonizerConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
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


