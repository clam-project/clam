#ifndef __FL_SMS_BPF_EDITOR__
#define __FL_SMS_BPF_EDITOR__

#include <FL/Fl_Group.H>
#include "BPF.hxx"

class Fl_Envelope_Scroll;
class Fl_Check_Button;

namespace CLAMVM
{
	class Fl_SMS_BPF_Editor : public Fl_Group
	{
	public:
		Fl_SMS_BPF_Editor( int X, int Y, int W, int H );
		virtual ~Fl_SMS_BPF_Editor();

		void InitPoints( double yvalue );
		void InitPoints( const CLAM::BPF& );
		void InsertPointsIntoBPF( CLAM::BPF& editedBPF );
		void Clear();

		void SetGridWidth( double xwidth, double ywidth );
		void SetHorizontalRange( double xmin, double xmax );
		void SetVerticalRange( double ymin, double ymax );

	protected:

		static void cbSnapToGrid( Fl_Check_Button* btn, void* data );

	protected:
		Fl_Envelope_Scroll* mpFunctionEditor;
		Fl_Check_Button*    mpSnapToGridBtn;
	};

}

#endif // Fl_SMS_BPF_Editor.hxx
