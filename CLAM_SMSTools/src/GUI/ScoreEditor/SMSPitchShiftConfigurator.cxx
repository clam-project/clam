#include "SMSPitchShiftConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_BPF_Editor.hxx"

namespace CLAMVM
{
	const char* SMSPitchShiftConfigurator::mHelpText = "<html><body><p>Here should go a description of the Pitch Shift transformation.</p></body></html>";

	SMSPitchShiftConfigurator::SMSPitchShiftConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );
		
		mEditorWidget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		mEditorWidget->end();
		mEditorWidget->SetHorizontalRange( 0.0, 1.0 );
		mEditorWidget->SetVerticalRange( 0.5, 2.0 );
		mEditorWidget->SetGridWidth( 0.1, 0.1 );
		SetHelpWidgetText();
		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
		mConfig.SetType( "SMSPitchShift" );
		mConfig.GetBPFAmount().Insert( 0.0, 1.0 );
		mConfig.GetBPFAmount().Insert( 1.0, 1.0 );
		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );
	}

	SMSPitchShiftConfigurator::~SMSPitchShiftConfigurator()
	{
		if ( mHelpWidget->parent() == NULL )
			delete mHelpWidget;
		if ( mEditorWidget->parent() == NULL )
			delete mEditorWidget;
	}

	void SMSPitchShiftConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSPitchShiftConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	void SMSPitchShiftConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast< const CLAM::SMSTransformationConfig& >( cfg );
		mEditorWidget->Clear();
		if ( mConfig.HasBPFAmount() )
		{
			mEditorWidget->InitPoints( mConfig.GetBPFAmount() );
		}
		else
		{
			if ( mConfig.GetAmount() >= 0.5 )
				mEditorWidget->InitPoints( mConfig.GetAmount() );
			else
				mEditorWidget->InitPoints( 1.0 );
			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();
		}

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


