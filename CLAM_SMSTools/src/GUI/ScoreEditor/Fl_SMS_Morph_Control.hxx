#ifndef __FL_SMS_MORPH_CONTROL__
#define __FL_SMS_MORPH_CONTROL__

#include <FL/Fl_Group.H>
#include <string>
#include <map>
#include "BPF.hxx"
#include "Signalv0.hxx"
#include "Signalv1.hxx"
#include "Slotv0.hxx"

class  Fl_Choice;
class  Fl_Check_Button;
class  Fl_Group;

namespace CLAMVM
{

	class Fl_SMS_Morph_Control 
		: public Fl_Group 
	{
	public:
		Fl_SMS_Morph_Control( int X, int Y, int W, int H, const char* label = 0 );
		
		virtual ~Fl_SMS_Morph_Control();

		virtual void resize( int X, int Y, int W, int H );

		virtual int handle( int event );

		SigSlot::Signalv0        GlobalEnvelopeChanged;
		SigSlot::Signalv0        SinAmpEnvelopeChanged;
		SigSlot::Signalv0        PitchHybEnvelopeChanged;
		SigSlot::Signalv0        SinFreqEnvelopeChanged;
		SigSlot::Signalv0        ResAmpEnvelopeChanged;
		SigSlot::Signalv1<bool>  FrameInterpolationChanged;   

		void RetrieveGlobalEnvelope( CLAM::BPF& );
		void SetGlobalEnvelope( const CLAM::BPF& );

		void RetrieveSinAmpEnvelope( CLAM::BPF& );
		void SetSinAmpEnvelope( const CLAM::BPF& );

		void RetrievePitchHybEnvelope( CLAM::BPF& );
		void SetPitchHybEnvelope( const CLAM::BPF& );

		void RetrieveSinFreqEnvelope( CLAM::BPF & );
		void SetSinFreqEnvelope( const CLAM::BPF& );

		void RetrieveResAmpEnvelope( CLAM::BPF& );
		void SetResAmpEnvelope( const CLAM::BPF& );

		void ActivateFrameInterpolation();
		void DeactivateFrameInterpolation();

	protected:

		
		SigSlot::Slotv0       GlobalEnvelopeEdited;
		SigSlot::Slotv0       SinAmpEnvelopeEdited;
		SigSlot::Slotv0       PitchHybEnvelopeEdited;
		SigSlot::Slotv0       SinFreqEnvelopeEdited;
		SigSlot::Slotv0       ResAmpEnvelopeEdited;

		void OnGlobalEnvelopeEdition();
		void OnSinAmpEnvelopeEdition();
		void OnPitchHybEnvelopeEdition();
		void OnSinFreqEnvelopeEdition();
		void OnResAmpEnvelopeEdition();

		void InitEnvelopeSelectorContents();

		static void sMenuItemSelectedCb( Fl_Choice*, Fl_SMS_Morph_Control* );
		static void sFrameInterpolationCb( Fl_Check_Button*, Fl_SMS_Morph_Control* );

		void CreateEnvelopeEditors();
		
		void ShowEnvelopeEditorFor( const std::string& name );
		
		void ShowEnvelopeEditorFor( const char* name );
		
		void FrameInterpolationActivated( );
		
		void FrameInterpolationDeactivated();

		void DestroyEnvelopeEditors();

		void SetVisibleEnvelopeWidget( Fl_Widget* w );

		bool FirstTimeShown( ) const {
			return mFirstTimeShown;
		}

		void ShownOnce( ) {
			mFirstTimeShown = false;
		}

	protected: // envelope editors builder methods
		
		Fl_Widget* BuildGlobalEditor();
		Fl_Widget* BuildSinAmpEditor();
		Fl_Widget* BuildPitchHybEditor();
		Fl_Widget* BuildSinFreqEditor();
		Fl_Widget* BuildResAmpEditor();
		
	protected: // "dynamic widgets"

		typedef std::map< std::string, Fl_Widget* >   EnvelopeWidgetRepository;
		
		EnvelopeWidgetRepository                      mEnvelopeEditors;

	protected: // "static" widgets
		
		Fl_Choice*        mpEnvelopeSelector;
		Fl_Check_Button*  mpFrameInterpSelector;
		Fl_Group*         mpEnvelopeContainer;
		
		const std::string mGlobalControlKey;
		const std::string mSinAmpControlKey;
		const std::string mPitchHybControlKey;
		const std::string mSinFreqControlKey;
		const std::string mResAmpControlKey;
		bool  mFirstTimeShown;
	};

}

#endif //Fl_SMS_Morph_Control.hxx
