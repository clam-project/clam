#ifndef __FL_SMS_MORPH_CONTROL__
#define __FL_SMS_MORPH_CONTROL__

#include <FL/Fl_Group.H>
#include <string>
#include <map>

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

	protected:

		void InitEnvelopeSelectorContents();

		static void sMenuItemSelectedCb( Fl_Choice*, Fl_SMS_Morph_Control* );

		void CreateEnvelopeEditors();
		
		void ShowEnvelopeEditorFor( const std::string& name );
		
		void ShowEnvelopeEditorFor( const char* name );
		

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
