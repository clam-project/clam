#include "SMSMorphConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_BPF_Editor.hxx"

namespace CLAMVM
{
	const char* SMSMorphConfigurator::mHelpText = "<html><body><p>Here should go a nice description of what does the SMS Morph do</p></body></html>";

	SMSMorphConfigurator::SMSMorphConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );
		mEditorWidget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		mEditorWidget->end();
		mEditorWidget->SetHorizontalRange( 0.0, 1.0 );
		mEditorWidget->SetVerticalRange( 0.0, 1.0 );
		mEditorWidget->SetGridWidth( 0.1, 0.1 );
		
		SetHelpWidgetText();
		mConfig.AddHybBPF();
		mConfig.UpdateData();
		
		mConfig.GetHybBPF().Insert( 0.0, 0.0 );
		mConfig.GetHybBPF().Insert( 1.0, 1.0 );
		mEditorWidget->InitPoints( mConfig.GetHybBPF() );
		
	}

	SMSMorphConfigurator::~SMSMorphConfigurator()
	{
		if ( mHelpWidget->parent() == NULL )
			delete mHelpWidget;
		if ( mEditorWidget->parent() == NULL )
			delete mEditorWidget;
	}

	void SMSMorphConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSMorphConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	void SMSMorphConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast<const CLAM::SMSMorphConfig& >(cfg);
		mEditorWidget->Clear();
		if ( !mConfig.HasHybBPF() )
		{
			mConfig.AddHybBPF();
			mConfig.UpdateData();
			mConfig.GetHybBPF().Insert( 0.0, 0.0 );
			mConfig.GetHybBPF().Insert( 1.0, 1.0 );

		}
		
		mEditorWidget->InitPoints( mConfig.GetHybBPF() );

	}
	
	void SMSMorphConfigurator::SetupConfigObject()
	{
		mEditorWidget->InsertPointsIntoBPF( mConfig.GetHybBPF() );

		if ( !mConfig.HasInterpolateFrame() )
		{
			mConfig.AddInterpolateFrame();
			mConfig.UpdateData();
		}
		
		mConfig.SetInterpolateFrame( false );

		if ( !mConfig.HasHybSinFreq() )
		{
			mConfig.AddHybSinFreq();
			mConfig.UpdateData();
		}
		mConfig.SetHybSinFreq( mConfig.GetHybBPF() );

		if ( !mConfig.HasHybSinAmp() )
		{
			mConfig.AddHybSinAmp();
			mConfig.UpdateData();			
		}
		mConfig.SetHybSinAmp( mConfig.GetHybBPF() );

		if ( !mConfig.HasHybPitch() )
		{
			mConfig.AddHybPitch();
			mConfig.UpdateData();
		}

		mConfig.SetHybPitch( mConfig.GetHybBPF() );

		if ( !mConfig.HasHybResAmp() )
		{
			mConfig.AddHybResAmp();
			mConfig.UpdateData();
		}

		mConfig.SetHybResAmp( mConfig.GetHybBPF() );

		if ( !mConfig.HasSynchronizeTime() )
		{
			mConfig.AddSynchronizeTime();
			mConfig.UpdateData();			
		}

		CLAM::BPF tmpBPF(2);
		tmpBPF.Insert( 0, 0 );
		tmpBPF.Insert( 1, 1 );
		mConfig.SetSynchronizeTime( tmpBPF );
	}


	const CLAM::ProcessingConfig& SMSMorphConfigurator::GetConfig()
	{
		SetupConfigObject();
		return mConfig;
	}

	Fl_Widget* SMSMorphConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	static SMSConfiguratorFactory::Registrator<SMSMorphConfigurator> regtPitchShiftCfg( "SMSMorph" );

}


