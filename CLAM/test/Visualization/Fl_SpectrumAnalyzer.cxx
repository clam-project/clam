#include "Fl_SpectrumAnalyzer.hxx"
#include "Fl_X_Axis.hxx"
#include "Fl_Y_Axis.hxx"
#include "Fl_ZoomSlider.hxx"
#include "Fl_GridLayout.hxx"
#include "Fl_Gl_Single_Display.hxx"
#include <algorithm>

namespace CLAMVM
{
	Fl_SpectrumAnalyzer::Fl_SpectrumAnalyzer( int X, int Y, int W, int H, const char* label )
		: Fl_Window( X, Y, W, H, label )
	{
		mLayout = new Fl_GridLayout( 6, 4 );

		mLayout->MakeHeightFixed();

		Fl_Box * aLabelBox = new Fl_Box( 0,0,0,0, label );
		aLabelBox->align( FL_ALIGN_CENTER | FL_ALIGN_INSIDE );
		aLabelBox->labelsize(10);
		mLayout->AddMultiCellWidget( aLabelBox, 0,0, 4, 1 );

		mDisplay = new Fl_Gl_Single_Display( 0,0,0,0 );
		mDisplay->SetRenderer( mDrawMgr );
		mDisplay->EnableDoubleBuffering();

		mLayout->AddMultiCellWidget( mDisplay, 0,1, 4, 5 );

		mLayout->end();

		mLayout->box( FL_EMBOSSED_BOX );

		add_resizable( *mLayout );

		end();
		mDrawMgr.SetDetailThreshold( 500 );
	}

	void Fl_SpectrumAnalyzer::resize( int x, int y, int w, int h )
	{
		Fl_Widget::resize( x, y, w, h );

		mLayout->resize( x, y, w, h );

		redraw();
	}

	Fl_SpectrumAnalyzer::~Fl_SpectrumAnalyzer( )
	{
	}

	void Fl_SpectrumAnalyzer::OnNewSpectrum( const DataArray& array, TData spectralRange )
	{
		mDrawMgr.CacheData( array );
		TData maxMag = *std::max_element( array.GetPtr(), array.GetPtr()+array.Size() );
		TData minMag = *std::min_element( array.GetPtr(), array.GetPtr()+array.Size() );
		const TData offsetPercentil = 0.2f; // 20%
		TData maxOffset = fabs( maxMag - minMag ) * offsetPercentil;
		mDisplay->SetWorldSpace( array.Size() - 2, 0, maxMag + maxOffset, minMag );
	}

	void Fl_SpectrumAnalyzer::Show()
	{
		show();
	}
}
