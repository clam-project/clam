#include "SMSMorphConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_Morph_Control.hxx"

namespace CLAMVM
{
	const char* SMSMorphConfigurator::mHelpText = 
		"<html><body>"
		"<h2>Morph</h2>"
		"<p>Allows to 'morph' two sounds, so the resulting one possesses a smooth blending of timbre, pitch and harmonicity, "
		"and texture of the original ones. Control on the actual blending is provided to the user through a set of "
		"envelopes that define the evolution through time and across several dimensions. These dimensions are:</p>"
		"<p><strong>Sinusoidal component amplitude hybridization:</strong> "
		"Allows to control an aspect of the timbre blending performed, concretely the sinusoidal component amplitude "
		"envelopes blending. The X axis is time, expressed in a parametric form,  and the Y axis is the interpolation factor between "
		"the two sinusoidal envelopes. An interpolation factor of 0.0 means that the resulting sound sinusoidal envelope matches "
		"source's one, and a factor of 1.0 means that the resulting sound sinusoidal envelope matches exactly the target's one."
		"</p>"
		"<p><strong>Sinusoidal component frequency hybridization: </strong>"
		"Allows to control another aspect of timbre blending, the sinusoidal component frequencies blending. The X axis represents time, "
		"expressed in a parametric form, and the Y axis is the interpolation factor between the two sinusoidal frequency contents. An "
		"interpolation factor of 0.0 means that the resulting sound sinusoidal frequency content matches source's one,  and a factor of "
		"1.0 means that the resulting sound sinusoidal frequency content matches exactly the target's one. "
		"</p>"
		"<p><strong>Pitch Hybridization:</strong> "
		"Allows to control sounds' pitches blending. Note that this involves not only to blend the fundamental frequency, but also any"
		"accompanying harmonics detected. The X axis is time, expressed in a parametric form, and the Y axis is the interpolation factor"
		"between the two sounds pitches. An interpolation factor of 0.0 means that the resulting sound harmonicity matches source's one, "
		"and a factor of 1.0 means that the resulting sound harmonicity matches exactly the target's one. "
		"</p>"
		"<p><strong>Residual component amplitude hybridization:</strong> "
		"This widget allows you to control the blending of the two sounds hoarseness, or residual component amplitude envelope of the two "
		"sounds. The X axis represents transformation time, in a parametric form, and the Y axis represents the interpolation factor "
		"between the two sounds hoarseness. So a interpolation factor of 0.0 means that the resulting sound hoarseness should match "
		"source's one, and an interpolation factor of 1.0 means that the resulting sound hoarseness should match target's one. "
		"</p>"
		"<p>Finally, in the sake of comodity, a 'global' envelope control is provided, that allows to define morph behaviour across "
		"all control dimensions in the same way.</p>"
		"<p>The option to interpolate 'intermediate' frames is given so to define SMS Tools behaviour when source and target morph "
		"sounds' lenghts differ. If it is deactivated, Tools will resample shorter sound replicating missing frames. Otherwise, the "
		"resampling will imply a linear interpolation between existing frames. Transformation result will sound better with linear interpolation "
		"but also will be a bit slower.</p>"
		"</body></html>";
		
		
	SMSMorphConfigurator::UserDefinedParams::UserDefinedParams(  )
		: mUserActivatedFrameInterpolation ( false ),
		  mUserDefinedGlobalEnvelope( false ),
		  mUserDefinedSinAmpEnvelope( false ),
		  mUserDefinedSinFreqEnvelope( false ),
		  mUserDefinedResAmpEnvelope( false ),
		  mUserDefinedPitchHybEnvelope( false ),
		  mUserDefinedTimeSyncEnvelope( false ),
		  mUserDefinedSinShapeW1Envelope( false ),
		  mUserDefinedSinShapeW2Envelope( false ),
		  mUserDefinedResShapeW1Envelope( false ),
		  mUserDefinedResShapeW2Envelope( false ),
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
		TimeSyncEnvelopeListener.Wrap( this, 
					       &SMSMorphConfigurator::UserDefinedParams::OnTimeSyncEnvelopeChanged );

		SinShapeW1EnvelopeListener.Wrap( this, 
						 &SMSMorphConfigurator::UserDefinedParams::OnSinShapeW1EnvelopeChanged );

		SinShapeW2EnvelopeListener.Wrap( this, 
						 &SMSMorphConfigurator::UserDefinedParams::OnSinShapeW2EnvelopeChanged );

		ResShapeW1EnvelopeListener.Wrap( this, 
						 &SMSMorphConfigurator::UserDefinedParams::OnResShapeW1EnvelopeChanged );
		
		ResShapeW2EnvelopeListener.Wrap( this, 
						 &SMSMorphConfigurator::UserDefinedParams::OnResShapeW2EnvelopeChanged );

		SinShapeEnvelopeListener.Wrap( this, 
					       &SMSMorphConfigurator::UserDefinedParams::OnSinShapeEnvelopeChanged );
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

	void SMSMorphConfigurator::UserDefinedParams::OnTimeSyncEnvelopeChanged() 
	{
		mUserDefinedTimeSyncEnvelope = true;
		mOnlyGlobalEnvelope = false;
		UserHasActed.Emit();
	}

	void SMSMorphConfigurator::UserDefinedParams::OnSinShapeW1EnvelopeChanged()
	{
		mUserDefinedSinShapeW1Envelope = true;
		mOnlyGlobalEnvelope = false;
		UserHasActed.Emit();
	}

	void SMSMorphConfigurator::UserDefinedParams::OnSinShapeW2EnvelopeChanged()
	{
		mUserDefinedSinShapeW2Envelope = true;
		mOnlyGlobalEnvelope = false;
		UserHasActed.Emit();
	}

	void SMSMorphConfigurator::UserDefinedParams::OnResShapeW1EnvelopeChanged()
	{
		mUserDefinedResShapeW1Envelope = true;
		mOnlyGlobalEnvelope = false;
		UserHasActed.Emit();
	}

	void SMSMorphConfigurator::UserDefinedParams::OnResShapeW2EnvelopeChanged()
	{
		mUserDefinedResShapeW2Envelope = true;
		mOnlyGlobalEnvelope = false;
		UserHasActed.Emit();
	}

	void SMSMorphConfigurator::UserDefinedParams::OnSinShapeEnvelopeChanged()
	{
		mUserDefinedSinShapeEnvelope = true;
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
		mUserDefinedTimeSyncEnvelope = false;
		mUserDefinedSinShapeW1Envelope = false;
		mUserDefinedSinShapeW2Envelope = false;
		mUserDefinedResShapeW1Envelope = false;
		mUserDefinedResShapeW2Envelope = false;
		mUserDefinedSinShapeEnvelope = false;
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
		mpMorphEditor->TimeSyncEnvelopeChanged.Connect( UserListener().TimeSyncEnvelopeListener );
		mpMorphEditor->SinShapeW1EnvelopeChanged.Connect( UserListener().SinShapeW1EnvelopeListener );
		mpMorphEditor->SinShapeW2EnvelopeChanged.Connect( UserListener().SinShapeW2EnvelopeListener );
		mpMorphEditor->ResShapeW1EnvelopeChanged.Connect( UserListener().ResShapeW1EnvelopeListener );
		mpMorphEditor->ResShapeW2EnvelopeChanged.Connect( UserListener().ResShapeW2EnvelopeListener );
		mpMorphEditor->SinShapeEnvelopeChanged.Connect( UserListener().SinShapeEnvelopeListener );
	
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

		mpMorphEditor->SetTimeSyncEnvelope( mConfig.GetSynchronizeTime() );

		mpMorphEditor->SetSinShapeW1Envelope( mConfig.GetHybSinShapeW1() );

		mpMorphEditor->SetSinShapeW2Envelope( mConfig.GetHybSinShapeW2() );
		
		mpMorphEditor->SetResShapeW1Envelope( mConfig.GetHybResShapeW1() );
		
		mpMorphEditor->SetResShapeW2Envelope( mConfig.GetHybResShapeW2() );

		mpMorphEditor->SetSinShapeEnvelope( mConfig.GetHybSinSpectralShape() );

	}
	
	void SMSMorphConfigurator::SetupConfigObject()
	{
		mConfig.SetInterpolateFrame( UserListener().UserActivatedFrameInterpolation() );

		// HybBPF ( Global envelope ) sync
		
		mpMorphEditor->RetrieveGlobalEnvelope( mConfig.GetHybBPF() );
		
		mpMorphEditor->RetrieveSinFreqEnvelope( mConfig.GetHybSinFreq() );
			
		mpMorphEditor->RetrieveSinAmpEnvelope( mConfig.GetHybSinAmp() );
		
		// Pitch hybridization control envelope sync
		
		mpMorphEditor->RetrievePitchHybEnvelope( mConfig.GetHybPitch() );
			
		// Residual amplitude blending envelope sync
			
		mpMorphEditor->RetrieveResAmpEnvelope( mConfig.GetHybResAmp() );
		
		mpMorphEditor->RetrieveTimeSyncEnvelope( mConfig.GetSynchronizeTime() );

		mpMorphEditor->RetrieveSinShapeW1Envelope( mConfig.GetHybSinShapeW1() );
		
		mpMorphEditor->RetrieveSinShapeW2Envelope( mConfig.GetHybSinShapeW2() );
		
		mpMorphEditor->RetrieveResShapeW1Envelope( mConfig.GetHybResShapeW1() );
		
		mpMorphEditor->RetrieveResShapeW2Envelope( mConfig.GetHybResShapeW2() );

		mpMorphEditor->RetrieveSinShapeEnvelope( mConfig.GetHybSinSpectralShape() );

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


