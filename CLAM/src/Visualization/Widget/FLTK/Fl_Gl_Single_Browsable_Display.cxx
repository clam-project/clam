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
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <FL/Fl.H>
#include <FL/gl.h>
#include "Fl_Gl_Single_Browsable_Display.hxx"
#include "Assert.hxx"
#include "AudioRenderingManager.hxx"


#include <iostream>


namespace CLAMVM
{
	Fl_Gl_Single_Browsable_Display::Fl_Gl_Single_Browsable_Display( int X, int Y, int W, int H, const char* label)
		: Fl_Gl_Single_Display( X, Y, W, H, label )
	{
		mPainting = false;
		mPos = -1;
	
		mFrameSlot.Wrap( this, &Fl_Gl_Single_Browsable_Display::SetPos );
		mPaintSlot.Wrap( this, &Fl_Gl_Single_Browsable_Display::SetPainting );
	}

	Fl_Gl_Single_Browsable_Display::~Fl_Gl_Single_Browsable_Display()
	{
	}

	void Fl_Gl_Single_Browsable_Display::SetPainting( bool painting ) {
		mPainting = painting;
	}

	void Fl_Gl_Single_Browsable_Display::SetPos( CLAM::TData pos ) {
		mPos = pos;
		DrawContents();
	}

	int Fl_Gl_Single_Browsable_Display::handle( int event ) {
		if( mPainting ) {
			if ( event == FL_PUSH && Fl::event_button() == FL_LEFT_MOUSE ) {

				DataBoundBox dataBBox;
				QueryDataBoundBox( dataBBox );
				
				mPos = ( Fl::event_x(  ) - x(  ) ) * (dataBBox.mRight-dataBBox.mLeft) / w(  ) + dataBBox.mLeft;

				DrawContents();
				// notify SLOTTED class to act
				mSignal.Emit( mPos );
				return 1;
			}
		}	
		return Fl_Gl_2DSurface::handle( event );
	}

	void Fl_Gl_Single_Browsable_Display::DrawOverlay(  ) {
		DataBoundBox dataBBox;
		QueryDataBoundBox( dataBBox );

		if( dataBBox.mLeft < mPos && mPos < dataBBox.mRight ) 
		{
			int position = (int) ceil( ( ( mPos - dataBBox.mLeft ) * w() ) / (dataBBox.mRight-dataBBox.mLeft) );

			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
			ortho();
			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity(  );
		
			//glClear(GL_COLOR_BUFFER_BIT );
		
			gl_color( FL_RED );
			glLineWidth(2);
		
			glBegin( GL_LINES );
			glVertex2i( position, y() );
			glVertex2i( position, y()+h() );
			glEnd();

			glMatrixMode( GL_PROJECTION );		
			glPopMatrix();		
			glMatrixMode( GL_MODELVIEW );

		}
	}

	void Fl_Gl_Single_Browsable_Display::DrawContents()
	{
		Fl_Gl_Single_Display::DrawContents();

		if( mPainting ) {
			DrawOverlay(  );
		}

	}
}
