#include "SMSMorphConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_Morph_Control.hxx"

namespace CLAMVM
{
	const char* SMSMorphConfigurator::mHelpText = "<html><body><h2>Morph</h2><p><strong>Usage:</strong> Morphing between two different sounds. Introduce interpolation value, 0 meaning original sound and 1 sound to morph (X axis = time). Note: More parameters can be controlled from the xml score. You must have a morph file specified in your configuration file in order to use this transformation.</p><p><strong>Explanation:</strong> All frames of the sound (sinusoidal and residual components plus pitch) are interpolated in the frequency domain.</p></body></html>";

	SMSMorphConfigurator::SMSMorphConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );

		mpMorphEditor = new Fl_SMS_Morph_Control( 0, 0, 100, 100 );
		
		SetHelpWidgetText();
		mConfig.AddHybBPF();
		mConfig.UpdateData();
		
		mConfig.GetHybBPF().Insert( 0.0, 0.0 );
		mConfig.GetHybBPF().Insert( 1.0, 1.0 );

	
	}

	SMSMorphConfigurator::~SMSMorphConfigurator()
	{
		if ( mHelpWidget->parent() == NULL )
			delete mHelpWidget;
		if ( mpMorphEditor->parent() == NULL )
			delete mpMorphEditor;
	}

	void SMSMorphConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSMorphConfigurator::GetParametersWidget()
	{
		return mpMorphEditor;
	}

	void SMSMorphConfigurator::Initialize( CLAM::ProcessingConfig& cfg )
	{
		CLAM::SMSMorphConfig& conCfg = static_cast< CLAM::SMSMorphConfig& >( cfg );

		if ( conCfg.HasHybBPF() )
		{
			conCfg.RemoveHybBPF();
			conCfg.UpdateData();
		}
		conCfg.AddHybBPF();
		conCfg.UpdateData();
		conCfg.GetHybBPF().Insert( 0.0, 0.0 );
		conCfg.GetHybBPF().Insert( 1.0, 1.0 );
				
	}

	void SMSMorphConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast<const CLAM::SMSMorphConfig& >(cfg);

		if ( !mConfig.HasHybBPF() )
		{
			mConfig.AddHybBPF();
			mConfig.UpdateData();
			mConfig.GetHybBPF().Insert( 0.0, 0.0 );
			mConfig.GetHybBPF().Insert( 1.0, 1.0 );

		}

	}
	
	void SMSMorphConfigurator::SetupConfigObject()
	{

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


