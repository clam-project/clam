/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in thFl_Smart_Tile.e hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "Fl_SMS_Spectrum.hxx"
#include "Fl_X_Axis.hxx"
#include "Fl_Y_Axis.hxx"
#include "Fl_ZoomSlider.hxx"
#include "Fl_GridLayout.hxx"
#include "Fl_Gl_Single_Display.hxx"
#include <algorithm>

namespace CLAMVM
{
	Fl_SMS_Spectrum::Fl_SMS_Spectrum( int X, int Y, int W, int H, const char* label )
		: Fl_Window( X, Y, W, H )
	{
		mXAxis = new Fl_X_Axis( X, H-40, W-40, 20 );
		mXAxis->align( FL_ALIGN_BOTTOM );
		mXAxis->scale( FL_AXIS_LIN );
		mXAxis->minimum( 0.0f );
		mXAxis->maximum( 1.0f );
		mXAxis->label_format( "%g" );
		mXAxis->label_step( 10 );
		mXAxis->label_size( 9 );
		mXAxis->axis_color( FL_BLACK );
		mXAxis->axis_align( FL_AXIS_BOTTOM | FL_AXIS_LINE );

		mYAxis = new Fl_Y_Axis( W-50, Y, 30, H-40 );
		mYAxis->align( FL_ALIGN_LEFT );
		mYAxis->scale( FL_AXIS_LIN );
		mYAxis->minimum( -1.0 );
		mYAxis->maximum( 1.0 );
		mYAxis->label_format( "%g" );
		mYAxis->label_step( 10 );
		mYAxis->label_size( 9 );
		mYAxis->axis_color( FL_BLACK );
		mYAxis->axis_align( FL_AXIS_RIGHT | FL_AXIS_LINE );

		mXSlider = new Fl_ZoomSlider( X, H-20, W-40, 20, FL_HORIZONTAL );
		mYSlider = new Fl_ZoomSlider( W-20, Y, 20, H-40, FL_VERTICAL );

		mDisplay = new Fl_Gl_Single_Display( X, Y, W-50, H-40 );
		resizable( mDisplay );
		mDisplay->SetRenderer( mDrawMgr );
		mDisplay->EnableDoubleBuffering();

		// Signal and Slot connections
		mXSlider->SpanChanged.Connect( mXAxis->AdjustRange );
		mXSlider->SpanChanged.Connect( mDisplay->AdjustXAxis );				
		mYSlider->SpanChanged.Connect( mYAxis->AdjustRange );
		mYSlider->SpanChanged.Connect( mDisplay->AdjustYAxis );
				
		end();
		mDrawMgr.SetDetailThreshold( 500 );
	}

	Fl_SMS_Spectrum::~Fl_SMS_Spectrum( )
	{
	}

	void Fl_SMS_Spectrum::OnNewSpectrum( const DataArray& array, TData spectralRange )
	{
		mDrawMgr.CacheData( array );
		const TData offsetPercentil = 0.2f; // 20%
		mDisplay->SetWorldSpace( array.Size() - 2, 0, 0, -150 );
		mXAxis->minimum( 0 );
		mXAxis->maximum( spectralRange );
		mYAxis->minimum( -150 );
		mYAxis->maximum( 0 );
		mDisplay->invalidate();
		redraw();
	}

	void Fl_SMS_Spectrum::Show()
	{
		show();
	}

	void Fl_SMS_Spectrum::Hide()
	{
		hide();
	}
}
