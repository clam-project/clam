#include "SMSSpectralShapeShiftConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>


namespace CLAMVM
{
	const char* SMSSpectralShapeShiftConfigurator::mHelpText = "<html><body><p>The <bf>Frequency Shift</bf> SMS transformation, is an operator over the results of an SMS Analysis, that allows to offset all detected partials by the same frequency differential.</p></body></html>";

	SMSSpectralShapeShiftConfigurator::SMSSpectralShapeShiftConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );
		
		SetHelpWidgetText();
		mConfig.AddType();
		mConfig.UpdateData();
		mConfig.SetType( "SMSSpectralShapeShift" );
	}

	SMSSpectralShapeShiftConfigurator::~SMSSpectralShapeShiftConfigurator()
	{
	}

	void SMSSpectralShapeShiftConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSSpectralShapeShiftConfigurator::GetParametersWidget()
	{
		return NULL;
	}

	const CLAM::ProcessingConfig& SMSSpectralShapeShiftConfigurator::GetConfig()
	{
		return mConfig;
	}

	Fl_Widget* SMSSpectralShapeShiftConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	static SMSConfiguratorFactory::Registrator<SMSSpectralShapeShiftConfigurator> regtPitchShiftCfg( "SMSSpectralShapeShift" );
	
}


