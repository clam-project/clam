#include "Fl_SMS_PitchShift_Editor.hxx"
#include "Fl_Envelope_Scroll.H"
#include <FL/Fl_Check_Button.H>

#include "BPF.hxx"

namespace CLAMVM
{
	Fl_SMS_PitchShift_Editor::Fl_SMS_PitchShift_Editor( int X, int Y, int W, int H )
		: Fl_Group( X, Y, W, H), mpFunctionEditor( NULL )
	{
		mpFunctionEditor = new Fl_Envelope_Scroll( X+15, Y+15, W-30, H-30 );
		mpFunctionEditor->envelope.grid( 0.1f, 0.1f );
		mpFunctionEditor->control.hvalue( 0.0, 1.0, 0.0, 1.0 );
		mpFunctionEditor->control.vvalue( 2.0, 1.5, 0.5, 1.5 );
		mpFunctionEditor->envelope.margin( 20, 10, 0, 0 );
		mpFunctionEditor->margin_adjust();
		mpFunctionEditor->end();

		mpSnapToGridBtn = new Fl_Check_Button( X+10, H-15, W-40, 15 );
		mpSnapToGridBtn->label( "Snap points to grid" );
		mpSnapToGridBtn->labelsize( 10 );
		//mpSnapToGridBtn->align( FL_ALIGN_RIGHT );
		mpSnapToGridBtn->callback( (Fl_Callback*)cbSnapToGrid, this );

		end();
		resizable( mpFunctionEditor );

	}
	
	void Fl_SMS_PitchShift_Editor::cbSnapToGrid( Fl_Check_Button* btn, void* data )
	{
		Fl_SMS_PitchShift_Editor* pThis = (Fl_SMS_PitchShift_Editor*)data;

		if ( btn->value() )
			pThis->mpFunctionEditor->envelope.snap( SNAPBOTH );
		else
			pThis->mpFunctionEditor->envelope.snap( 0 );

	}

	Fl_SMS_PitchShift_Editor::~Fl_SMS_PitchShift_Editor()
	{
	}

	void Fl_SMS_PitchShift_Editor::InitPoints()
	{
		mpFunctionEditor->envelope.add_point( 0.0, 1.0 );
		mpFunctionEditor->envelope.add_point( 1.0, 1.0 );
	}

	void Fl_SMS_PitchShift_Editor::InsertPointsIntoBPF( CLAM::BPF& editedBPF )
	{
		for ( int i = 0; i < editedBPF.Size(); i++ )
			editedBPF.DeleteIndex( i );

		FLPOINT* p;
		int i = 0;
		while( ( p =mpFunctionEditor->envelope.point(i++))!=0 )
			editedBPF.Insert( p->x, p->y );
	}
}
