#ifndef __FL_SMS_PITCHSHIFT_EDITOR__
#define __FL_SMS_PITCHSHIFT_EDITOR__

#include <FL/Fl_Group.H>
#include "BPF.hxx"

class Fl_Envelope_Scroll;
class Fl_Check_Button;

namespace CLAMVM
{
	class Fl_SMS_PitchShift_Editor : public Fl_Group
	{
	public:
		Fl_SMS_PitchShift_Editor( int X, int Y, int W, int H );
		virtual ~Fl_SMS_PitchShift_Editor();

		void InitPoints();
		void InsertPointsIntoBPF( CLAM::BPF& editedBPF );

	protected:

		static void cbSnapToGrid( Fl_Check_Button* btn, void* data );

	protected:
		Fl_Envelope_Scroll* mpFunctionEditor;
		Fl_Check_Button*    mpSnapToGridBtn;
	};

}

#endif // Fl_SMS_PitchShift_Editor.hxx
