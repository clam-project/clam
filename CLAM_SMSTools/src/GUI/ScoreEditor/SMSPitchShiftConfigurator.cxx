#include "SMSPitchShiftConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_PitchShift_Editor.hxx"

namespace CLAMVM
{
	const char* SMSPitchShiftConfigurator::mHelpText = "<html><body><p>Here should go a description of the Pitch Shift transformation.</p></body></html>";

	SMSPitchShiftConfigurator::SMSPitchShiftConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );
		
		mEditorWidget = new Fl_SMS_PitchShift_Editor( 0, 0, 100, 100 );
		mEditorWidget->end();
		mEditorWidget->InitPoints();

		SetHelpWidgetText();
		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
		mConfig.SetType( "SMSPitchShift" );
	}

	SMSPitchShiftConfigurator::~SMSPitchShiftConfigurator()
	{
	}

	void SMSPitchShiftConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSPitchShiftConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	const CLAM::ProcessingConfig& SMSPitchShiftConfigurator::GetConfig()
	{
		mEditorWidget->InsertPointsIntoBPF( mConfig.GetBPFAmount() );

		return mConfig;
	}

	Fl_Widget* SMSPitchShiftConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}
	
	static SMSConfiguratorFactory::Registrator<SMSPitchShiftConfigurator> regtPitchShiftCfg( "SMSPitchShift" );


}


