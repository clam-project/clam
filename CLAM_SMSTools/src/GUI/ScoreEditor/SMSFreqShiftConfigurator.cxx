#include "SMSFreqShiftConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>


namespace CLAMVM
{
	const char* SMSFreqShiftConfigurator::mHelpText = "<html><body><p>The <bf>Frequency Shift</bf> SMS transformation, is an operator over the results of an SMS Analysis, that allows to offset all detected partials by the same frequency differential.</p></body></html>";

	SMSFreqShiftConfigurator::SMSFreqShiftConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );
		
		SetHelpWidgetText();
		mConfig.AddType();
		mConfig.UpdateData();
		mConfig.SetType( "SMSFreqShift" );
	}

	SMSFreqShiftConfigurator::~SMSFreqShiftConfigurator()
	{
	}

	void SMSFreqShiftConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText );
	}

	Fl_Widget* SMSFreqShiftConfigurator::GetParametersWidget()
	{
		return NULL;
	}

	const CLAM::ProcessingConfig& SMSFreqShiftConfigurator::GetConfig()
	{
		return mConfig;
	}

	Fl_Widget* SMSFreqShiftConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	typedef CLAM::Factory<SMSConfigurator> SMSConfiguratorFactory;

	static SMSConfiguratorFactory::Registrator<CLAMVM::SMSFreqShiftConfigurator> regtFreqShiftCfg( "SMSFreqShift" );

}
