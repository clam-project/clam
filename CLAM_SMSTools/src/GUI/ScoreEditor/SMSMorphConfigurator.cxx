#include "SMSMorphConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>


namespace CLAMVM
{
	const char* SMSMorphConfigurator::mHelpText = "<html><body><p>The <bf>Frequency Shift</bf> SMS transformation, is an operator over the results of an SMS Analysis, that allows to offset all detected partials by the same frequency differential.</p></body></html>";

	SMSMorphConfigurator::SMSMorphConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );
		
		SetHelpWidgetText();
	}

	SMSMorphConfigurator::~SMSMorphConfigurator()
	{
	}

	void SMSMorphConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSMorphConfigurator::GetParametersWidget()
	{
		return NULL;
	}

	const CLAM::ProcessingConfig& SMSMorphConfigurator::GetConfig()
	{
		return mConfig;
	}

	Fl_Widget* SMSMorphConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	static SMSConfiguratorFactory::Registrator<SMSMorphConfigurator> regtPitchShiftCfg( "SMSMorph" );

}


