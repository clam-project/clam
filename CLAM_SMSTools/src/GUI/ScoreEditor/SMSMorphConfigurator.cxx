#include "SMSMorphConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_Morph_Control.hxx"

namespace CLAMVM
{
	const char* SMSMorphConfigurator::mHelpText = "<html><body><h2>Morph</h2><p><strong>Usage:</strong> Morphing between two different sounds. Introduce interpolation value, 0 meaning original sound and 1 sound to morph (X axis = time). Note: More parameters can be controlled from the xml score. You must have a morph file specified in your configuration file in order to use this transformation.</p><p><strong>Explanation:</strong> All frames of the sound (sinusoidal and residual components plus pitch) are interpolated in the frequency domain.</p></body></html>";

	SMSMorphConfigurator::UserDefinedParams::UserDefinedParams(  )
		: mUserActivatedFrameInterpolation ( false ),
		  mUserDefinedGlobalEnvelope( false ),
		  mUserDefinedSinAmpEnvelope( false ),
		  mUserDefinedSinFreqEnvelope( false ),
		  mUserDefinedResAmpEnvelope( false ),
		  mUserDefinedPitchHybEnvelope( false ),
		  mOnlyGlobalEnvelope( true )
	{
		FrameInterpolationListener.Wrap( this, 
						 &SMSMorphConfigurator::UserDefinedParams::OnFrameInterpolationChanged );
		GlobalEnvelopeListener.Wrap( this,
					     &SMSMorphConfigurator::UserDefinedParams::OnGlobalEnvelopeChanged );
		PitchHybEnvelopeListener.Wrap( this,
					       &SMSMorphConfigurator::UserDefinedParams::OnPitchHybEnvelopeChanged );
		SinAmpEnvelopeListener.Wrap( this,
					     &SMSMorphConfigurator::UserDefinedParams::OnSinAmpEnvelopeChanged );
		SinFreqEnvelopeListener.Wrap( this,
					      &SMSMorphConfigurator::UserDefinedParams::OnSinFreqEnvelopeChanged );
		ResAmpEnvelopeListener.Wrap( this,
					     &SMSMorphConfigurator::UserDefinedParams::OnResAmpEnvelopeChanged );
	}

	SMSMorphConfigurator::UserDefinedParams::~UserDefinedParams()
	{
	}

	void SMSMorphConfigurator::UserDefinedParams::OnFrameInterpolationChanged( bool newState )
	{
		mUserActivatedFrameInterpolation = newState;
		UserHasActed.Emit();
	}

	void SMSMorphConfigurator::UserDefinedParams::OnGlobalEnvelopeChanged()
	{
		mUserDefinedGlobalEnvelope = true;
		UserHasActed.Emit();
	}

	void SMSMorphConfigurator::UserDefinedParams::OnPitchHybEnvelopeChanged()
	{
		mUserDefinedPitchHybEnvelope = true;
		mOnlyGlobalEnvelope = false;
		UserHasActed.Emit();
	}

	void SMSMorphConfigurator::UserDefinedParams::OnSinAmpEnvelopeChanged()
	{
		mUserDefinedSinAmpEnvelope = true;
		mOnlyGlobalEnvelope = false;
		UserHasActed.Emit();
	}

	void SMSMorphConfigurator::UserDefinedParams::OnSinFreqEnvelopeChanged()
	{
		mUserDefinedSinFreqEnvelope = true;
		mOnlyGlobalEnvelope = false;
		UserHasActed.Emit();
	}

	void SMSMorphConfigurator::UserDefinedParams::OnResAmpEnvelopeChanged()
	{
		mUserDefinedResAmpEnvelope = true;
		mOnlyGlobalEnvelope = false;
		UserHasActed.Emit();
	}

	void SMSMorphConfigurator::UserDefinedParams::Reset()
	{
		mUserActivatedFrameInterpolation = false;
		mUserDefinedGlobalEnvelope = false;
		mUserDefinedSinAmpEnvelope = false;
		mUserDefinedSinFreqEnvelope = false;
		mUserDefinedResAmpEnvelope = false;
		mUserDefinedPitchHybEnvelope = false;
	}

	SMSMorphConfigurator::SMSMorphConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );

		mpMorphEditor = new Fl_SMS_Morph_Control( 0, 0, 100, 100 );
	
		SetHelpWidgetText();

		UserListener().UserHasActed.Connect( UserEditedParameters );

		mpMorphEditor->FrameInterpolationChanged.Connect( UserListener().FrameInterpolationListener );
		mpMorphEditor->GlobalEnvelopeChanged.Connect( UserListener().GlobalEnvelopeListener );
		mpMorphEditor->PitchHybEnvelopeChanged.Connect( UserListener().PitchHybEnvelopeListener );
		mpMorphEditor->SinAmpEnvelopeChanged.Connect( UserListener().SinAmpEnvelopeListener );
		mpMorphEditor->SinFreqEnvelopeChanged.Connect( UserListener().SinFreqEnvelopeListener );
		mpMorphEditor->ResAmpEnvelopeChanged.Connect( UserListener().ResAmpEnvelopeListener );
	
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
		conCfg.GetHybBPF().Insert( 0.0, 0.5 );
		conCfg.GetHybBPF().Insert( 1.0, 0.5 );
				
	}

	void SMSMorphConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast<const CLAM::SMSMorphConfig& >(cfg);

		if ( mConfig.GetInterpolateFrame() ) 
			mpMorphEditor->ActivateFrameInterpolation();
		else 
			mpMorphEditor->DeactivateFrameInterpolation();

		mpMorphEditor->SetGlobalEnvelope( mConfig.GetHybBPF() );

		mpMorphEditor->SetSinAmpEnvelope( mConfig.GetHybSinAmp() );

		mpMorphEditor->SetSinFreqEnvelope( mConfig.GetHybSinFreq() );

		mpMorphEditor->SetPitchHybEnvelope( mConfig.GetHybPitch() );

		mpMorphEditor->SetResAmpEnvelope( mConfig.GetHybResAmp() );

	}
	
	void SMSMorphConfigurator::SetupConfigObject()
	{
		mConfig.SetInterpolateFrame( UserListener().UserActivatedFrameInterpolation() );

		// HybBPF ( Global envelope ) sync
		
		if ( UserListener().UserOnlyDefinedGlobalEnvelope() )
		{
			mpMorphEditor->RetrieveGlobalEnvelope( mConfig.GetHybBPF() );
			mpMorphEditor->RetrieveGlobalEnvelope( mConfig.GetHybSinFreq() );
			mpMorphEditor->RetrieveGlobalEnvelope( mConfig.GetHybSinAmp() );
			mpMorphEditor->RetrieveGlobalEnvelope( mConfig.GetHybPitch() );
			mpMorphEditor->RetrieveGlobalEnvelope( mConfig.GetHybResAmp() );
		}
		else
		{
			mpMorphEditor->RetrieveGlobalEnvelope( mConfig.GetHybBPF() );
			
			mpMorphEditor->RetrieveSinFreqEnvelope( mConfig.GetHybSinFreq() );
			
			mpMorphEditor->RetrieveSinAmpEnvelope( mConfig.GetHybSinAmp() );
			
			// Pitch hybridization control envelope sync
			
			mpMorphEditor->RetrievePitchHybEnvelope( mConfig.GetHybPitch() );
			
			// Residual amplitude blending envelope sync
			
			mpMorphEditor->RetrieveResAmpEnvelope( mConfig.GetHybResAmp() );
		}

		CLAM::BPF tmpBPF;
		tmpBPF.Insert( 0, 0 );
		tmpBPF.Insert( 1, 1 );
		mConfig.SetSynchronizeTime( tmpBPF );
	}


	const CLAM::ProcessingConfig& SMSMorphConfigurator::GetConfig()
	{
		SetupConfigObject();
		UserListener().Reset();
		return mConfig;
	}

	Fl_Widget* SMSMorphConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	static SMSConfiguratorFactory::Registrator<SMSMorphConfigurator> regtPitchShiftCfg( "SMSMorph" );

}


