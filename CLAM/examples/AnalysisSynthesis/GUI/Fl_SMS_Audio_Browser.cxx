#include "Fl_SMS_Audio_Browser.hxx"
#include "Fl_SMS_Gl_Single_Browsable_Display.hxx"
#include <iostream>

namespace CLAMVM
{
	Fl_SMS_Audio_Browser::Fl_SMS_Audio_Browser( int X, int Y, int W, int H, const char* label )
		: Fl_SMS_Browsable_Playable_Audio( X, Y, W, H, label )
	{
		Init();

		mDisplay->SelectedXValue.Connect( HandleDisplaySelection );
		ChangeSelectedXValue.Connect( mDisplay->SetSelectedXValue );
		
		mDisplay->SetPainting();
	}

	Fl_SMS_Audio_Browser::~Fl_SMS_Audio_Browser()
	{
	}
	
	void Fl_SMS_Audio_Browser::Init()
	{
		HandleDisplaySelection.Wrap( this, &Fl_SMS_Audio_Browser::OnDisplaySelectedXValue );
		SetSelectedXValue.Wrap( this, &Fl_SMS_Audio_Browser::OnSetSelectedXValue );
	}

	void Fl_SMS_Audio_Browser::OnDisplaySelectedXValue( double value )
	{
		// towards the outer world
		double sampleTime = (value / mSampleRate ) + mAudioOffset;
		SelectedXValue.Emit( sampleTime );
	}

	void Fl_SMS_Audio_Browser::OnSetSelectedXValue( double value )
	{
		// towards the display
		double sampleIndex = ( value * mSampleRate );
		ChangeSelectedXValue.Emit( sampleIndex );
	}


}
