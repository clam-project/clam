#include "SMSOddEvenHarmonicRatioConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>

namespace CLAMVM
{
	const char* SMSOddEvenHarmonicRatioConfigurator::mHelpText = "<html><body><p>The <bf>Frequency Shift</bf> SMS transformation, is an operator over the results of an SMS Analysis, that allows to offset all detected partials by the same frequency differential.</p></body></html>";

	SMSOddEvenHarmonicRatioConfigurator::SMSOddEvenHarmonicRatioConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );
		
		SetHelpWidgetText();
		mConfig.AddType();
		mConfig.UpdateData();
		mConfig.SetType( "SMSOddEvenHarmonicRatio" );
	}

	SMSOddEvenHarmonicRatioConfigurator::~SMSOddEvenHarmonicRatioConfigurator()
	{
	}

	void SMSOddEvenHarmonicRatioConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSOddEvenHarmonicRatioConfigurator::GetParametersWidget()
	{
		return NULL;
	}

	const CLAM::ProcessingConfig& SMSOddEvenHarmonicRatioConfigurator::GetConfig()
	{
		return mConfig;
	}

	Fl_Widget* SMSOddEvenHarmonicRatioConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	typedef CLAM::Factory<SMSConfigurator> SMSConfiguratorFactory;

	static SMSConfiguratorFactory::Registrator<CLAMVM::SMSOddEvenHarmonicRatioConfigurator> regtPitchShiftCfg( "SMSOddEvenHarmonicRatio" );

}
