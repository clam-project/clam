#include "SMSSineFilterConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>


namespace CLAMVM
{
	const char* SMSSineFilterConfigurator::mHelpText = "<html><body><p>The <bf>Frequency Shift</bf> SMS transformation, is an operator over the results of an SMS Analysis, that allows to offset all detected partials by the same frequency differential.</p></body></html>";

	SMSSineFilterConfigurator::SMSSineFilterConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );
		
		SetHelpWidgetText();
		mConfig.AddType();
		mConfig.UpdateData();
		mConfig.SetType( "SMSSineFilter" );
	}

	SMSSineFilterConfigurator::~SMSSineFilterConfigurator()
	{
	}

	void SMSSineFilterConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSSineFilterConfigurator::GetParametersWidget()
	{
		return NULL;
	}

	const CLAM::ProcessingConfig& SMSSineFilterConfigurator::GetConfig()
	{
		return mConfig;
	}

	Fl_Widget* SMSSineFilterConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	typedef CLAM::Factory<SMSConfigurator> SMSConfiguratorFactory;

	static SMSConfiguratorFactory::Registrator<CLAMVM::SMSSineFilterConfigurator> regtPitchShiftCfg( "SMSSineFilter" );

}
