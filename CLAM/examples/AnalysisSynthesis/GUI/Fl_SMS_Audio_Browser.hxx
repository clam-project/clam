#ifndef __FL_SMS_AUDIO_BROWSER__
#define __FL_SMS_AUDIO_BROWSER__

#include "Fl_SMS_Browsable_Playable_Audio.hxx"
#include "Signalv1.hxx"
#include "Slotv0.hxx"
#include "Slotv1.hxx"

namespace CLAMVM
{
	using SigSlot::Signalv1;
	using SigSlot::Slotv1;
	using SigSlot::Slotv0;

	class Fl_SMS_Audio_Browser
		: public Fl_SMS_Browsable_Playable_Audio
	{
	public:
		Fl_SMS_Audio_Browser( int X, int Y, int W, int H, const char* label = 0 );
		
		virtual ~Fl_SMS_Audio_Browser();

		Slotv1<double>        SetSelectedXValue;
		Signalv1<double>      SelectedXValue;


	protected:
		virtual void OnDisplaySelectedXValue( double value );
		virtual void OnSetSelectedXValue( double value );

		void Init();

		Signalv1< double > ChangeSelectedXValue;
		Slotv1< double >   HandleDisplaySelection;

	};

}


#endif // Fl_SMS_Audio_Browser.hxx
