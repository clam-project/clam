#include "Fl_SMS_SpectrumAndPeaks.hxx"
#include "Fl_X_Axis.hxx"
#include "Fl_Y_Axis.hxx"
#include "Fl_ZoomSlider.hxx"
#include "Fl_Gl_Multi_Display.hxx"
#include <algorithm>

namespace CLAMVM
{

	Fl_SMS_SpectrumAndPeaks::Fl_SMS_SpectrumAndPeaks( int X, int Y, int W, int H, const char* label )
		: Fl_Window( X, Y, W, H, label )
	{

		mXAxis = new Fl_X_Axis( X,H-50,W-50, 30  );
		mXAxis->align( FL_ALIGN_BOTTOM );
		mXAxis->scale( FL_AXIS_LIN );
		mXAxis->minimum( 0.0f );
		mXAxis->maximum( 1.0f );
		mXAxis->label_format( "%g" );
		mXAxis->label_step( 10 );
		mXAxis->label_size( 9 );
		mXAxis->axis_color( FL_BLACK );
		mXAxis->axis_align( FL_AXIS_BOTTOM|FL_AXIS_LINE);

		mYAxis = new Fl_Y_Axis( W-50,Y,30,H-50 );
		mYAxis->align( FL_ALIGN_LEFT );
		mYAxis->scale( FL_AXIS_LIN );
		mYAxis->minimum( -1.0 );
		mYAxis->maximum( 1.0 );
		mYAxis->label_format( "%g" );
		mYAxis->label_step( 10 );
		mYAxis->label_size( 9 );
		mYAxis->axis_color( FL_BLACK );
		mYAxis->axis_align( FL_AXIS_RIGHT | FL_AXIS_LINE );

		mXSlider = new Fl_ZoomSlider( X,H-20,W-50,20, FL_HORIZONTAL );
		mYSlider = new Fl_ZoomSlider( W-20,Y,20,H-50, FL_VERTICAL );

		mDisplay = new Fl_Gl_Multi_Display( X,Y,W-50,H-50 );
		mDisplay->AddRenderer( mSpectrumDrawMgr );
		mDisplay->AddRenderer( mPeaksDrawMgr );
		mDisplay->EnableDoubleBuffering();
		
		resizable( mDisplay );

		// Signal and Slot connections

		mXSlider->SpanChanged.Connect( mXAxis->AdjustRange );
		mXSlider->SpanChanged.Connect( mDisplay->AdjustXAxis );				
		mYSlider->SpanChanged.Connect( mYAxis->AdjustRange );
		mYSlider->SpanChanged.Connect( mDisplay->AdjustYAxis );
				
		end();
				
		mSpectrumDrawMgr.SetDetailThreshold( 50 );				

	}

	Fl_SMS_SpectrumAndPeaks::~Fl_SMS_SpectrumAndPeaks( )
	{
	}

	void Fl_SMS_SpectrumAndPeaks::OnNewSpectrum( const DataArray& array, TData spectralRange )
	{
		mSpectrumDrawMgr.CacheData( array );
		mDisplay->SetWorldSpace( array.Size() - 2, 0, 0, -150 );
		mXAxis->minimum( 0 );
		mXAxis->maximum( spectralRange );

		// We check here for -1.0INFs, clamping the magnitude to -200 dB
		mYAxis->minimum( -150 );
				
		mYAxis->maximum( 0 );
		mPeaksDrawMgr.SetBinNumber( array.Size() );
		mPeaksDrawMgr.SetSpectralRange ( spectralRange );
		redraw();
	}

	void Fl_SMS_SpectrumAndPeaks::OnNewPeakArray( const Array<Partial>& partArray )
	{
		mPeaksDrawMgr.CacheData( partArray );
		redraw();
	}

	void Fl_SMS_SpectrumAndPeaks::Show()
	{
		show();
	}

	void Fl_SMS_SpectrumAndPeaks::Hide()
	{
		hide();
	}
}
