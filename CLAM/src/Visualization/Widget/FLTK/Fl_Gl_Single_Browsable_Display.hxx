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

#ifndef __FL_GL_SINGLE_BROWSABLE_DISPLAY__
#define __FL_GL_SINGLE_BROWSABLE_DISPLAY__

#include "Fl_Gl_Single_Display.hxx"
#include "GLRenderingManager.hxx"
#include "DataTypes.hxx"
#include "Slotv1.hxx"
#include "Signalv1.hxx"


namespace CLAMVM
{
	using SigSlot::Signalv1;
	using SigSlot::Slotv1;

	class Fl_Gl_Single_Browsable_Display : public Fl_Gl_Single_Display
	{
	public:
		Fl_Gl_Single_Browsable_Display( int X, int Y, int W, int H, const char* label = 0);

		~Fl_Gl_Single_Browsable_Display();

		void DrawOverlay(  );
		
 		int handle( int event ); // Fl_Gl_Window required interface

		void SetPainting( bool painting );
		void SetPos( CLAM::TData pos );
		Slotv1<CLAM::TData>* GetFrameSlot(  ) { return &mFrameSlot; }
		Slotv1<bool>& GetPaintSlot(  ) { return mPaintSlot; }
		Signalv1<double>* GetSignal(  ) { return &mSignal; }
		
	protected:
		void DrawContents();

		bool mPainting;
		double mPos;

		Signalv1<double> mSignal;
		Slotv1<CLAM::TData> mFrameSlot;
		Slotv1<bool> mPaintSlot;

	private:
		GLRenderingManager* mRenderer;
	};
}

#endif // Fl_Gl_Single_Display.hxx
