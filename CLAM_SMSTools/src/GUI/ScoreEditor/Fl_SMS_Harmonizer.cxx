#include "Fl_SMS_Harmonizer.hxx"
#include <FL/Fl_Box.H>
#include <FL/Fl_Light_Button.H>
#include "Fl_X_Axis.hxx"
#include "Fl_Y_Axis.hxx"
#include "Fl_SMS_Grid_Surface.hxx"

namespace CLAMVM
{
	Fl_SMS_Harmonizer::Fl_SMS_Harmonizer( int X, int Y, int W, int H, const char* label )
		: Fl_Group( X, Y, W, H, label ), mpDisplay( NULL )
	{
		int buttonWidth = (W-30)/3;

		mAddButton = new Fl_Light_Button( X+30, Y+5, buttonWidth, 20 );
		mAddButton->label( "Add" );
		mAddButton->labelsize( 10 );
		mAddButton->value(1);
		
		mRemoveButton = new Fl_Light_Button( X+30+buttonWidth, Y+5, buttonWidth, 20 );
		mRemoveButton->label("Remove");
		mRemoveButton->labelsize( 10 );
		mRemoveButton->value(0);

		mMoveButton = new Fl_Light_Button( X+30+(2*buttonWidth), Y+5, buttonWidth, 20 );
		mMoveButton->label( "Move" );
		mMoveButton->labelsize( 10 );
		mMoveButton->value(0);

		mpYAxis = new Fl_Y_Axis( X, Y+25, 30, H-55 );
		mpYAxis->label( "P.T.F." );
		mpYAxis->scale( FL_AXIS_LIN );
		mpYAxis->minimum( 0.5 );
		mpYAxis->maximum( 2.0 );
		mpYAxis->label_format( "%g" );
		mpYAxis->label_step( 15 );
		mpYAxis->label_size( 9 );
		mpYAxis->axis_color( FL_BLACK );
		mpYAxis->axis_align( FL_AXIS_LINE|FL_AXIS_LEFT );

		mpXAxis = new Fl_X_Axis( X+30, Y+H-30, W-30, 30 );
		mpXAxis->label( "Gain Ratio (dB)" );
		mpXAxis->scale( FL_AXIS_LIN );
		mpXAxis->minimum( -9 );
		mpXAxis->maximum( 9 );
		mpXAxis->label_format( "%g" );
		mpXAxis->label_step( 10 );
		mpXAxis->label_size( 9 );
		mpXAxis->axis_color( FL_BLACK );
		mpXAxis->axis_align( FL_AXIS_BOTTOM|FL_AXIS_LINE);

		mpImposter = new Fl_Box( X+30, Y+25, W-30, H-30 );
		mpImposter->label( "Wait..." );

		end();
		
		mWSC.mLeft = -9;
		mWSC.mRight = 9;
		mWSC.mTop = 2.0;
		mWSC.mBottom = 0.5;

	}

	Fl_SMS_Harmonizer::~Fl_SMS_Harmonizer()
	{
	}

	int Fl_SMS_Harmonizer::handle( int event )
	{
		if ( event == FL_SHOW )
		{
			mpDisplay = new Fl_SMS_Grid_Surface( x()+30, y()+25, w()-30, h()-30 );
			mpDisplay->EnableDoubleBuffering();
			mpDisplay->SetWorldSpace( mWSC.mRight, mWSC.mLeft, mWSC.mTop, mWSC.mBottom );
			mpDisplay->end();
			mpDisplay->SetGridWidth( 3, 0.1 );
			add(mpDisplay);
			remove(*mpImposter);
		}
		else if ( event == FL_HIDE )
		{
			if ( mpDisplay )
			{
				remove( mpDisplay );
				delete mpDisplay;
				mpDisplay = NULL;
			}
			add( mpImposter );
		}

		return Fl_Group::handle( event );
	}

	void Fl_SMS_Harmonizer::Show()
	{
	}

	void Fl_SMS_Harmonizer::Hide()
	{
	}

}
