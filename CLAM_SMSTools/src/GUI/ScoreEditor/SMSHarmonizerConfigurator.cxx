#include "SMSHarmonizerConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_BPF_Editor.hxx"

namespace CLAMVM
{
	const char* SMSHarmonizerConfigurator::mHelpText = "<html><body><p>Here should go some nice description about what does this SMS transformation do.</p></body></html>";

	SMSHarmonizerConfigurator::SMSHarmonizerConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );
		
		mEditorWidget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		mEditorWidget->end();
		mEditorWidget->SetHorizontalRange( -12, 12 );
		mEditorWidget->SetVerticalRange( 0.5, 2.0 );
		mEditorWidget->SetGridWidth( 3, 0.15 );

		SetHelpWidgetText();
		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
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
		mEditorWidget->Clear();
		if ( !mConfig.HasBPFAmount() )
		{
			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();
		}

		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );
	}

	const CLAM::ProcessingConfig& SMSHarmonizerConfigurator::GetConfig()
	{
		mEditorWidget->InsertPointsIntoBPF( mConfig.GetBPFAmount() );
		return mConfig;
	}

	Fl_Widget* SMSHarmonizerConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	static SMSConfiguratorFactory::Registrator<SMSHarmonizerConfigurator> regtPitchShiftCfg( "SMSHarmonizer" );


}


