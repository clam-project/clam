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

#include "SpectrumDisplay.hxx"
#include "FLDisplayContainer.hxx"
#include "GLPortNew.hxx"
#include "GLLinearSpRenderer.hxx"
#include "GLState.hxx"
#include "CLAMGL.hxx"
#include "CBL.hxx"
#include "GlobalEnums.hxx"
#include "GLLogSpRenderer.hxx"
#include "GeometryKit.hxx"
#include "Viewport.hxx"
using CLAM::DB;


namespace CLAMVM
{
		using CLAMGUI::GLState;
		using CLAMGUI::Rect;
		using CLAMGUI::Viewport;
		using CLAMGUI::GLLogSpRenderer;

		class SpecDisplayGLState : public GLState
		{
		public:
				void Apply()
				{
						glClearColor( 0.0, 0.0, 0.0, 0.0 );
						glShadeModel( GL_FLAT );
				}
		};

		SpectrumDisplay::SpectrumDisplay( int X, int Y, int W, int H, const char* label )
			: SpectrumPresentation(), Fl_Window( X, Y, W, H, label )
		{
			Init( W, H);
		}
				
		SpectrumDisplay::~SpectrumDisplay()
		{
			delete mRenderer;
		}	
		void SpectrumDisplay::Show()
		{
				show();
		}

		void SpectrumDisplay::Hide()
		{
			hide();
		}

		void SpectrumDisplay::OnNewSpectrum( const DataArray& samples, TData specRange )
		{
				
				Viewport                          mSpecPresMetrix;
				mRenderer->CacheData( samples );
				mRenderer->SetSamplingRate( specRange );
			
				// :MRJ: We discard any estimation. this is a ugly hack that
				// whose need I hopefully will vanquish soon
				mSpecPresMetrix.top_es = false;
				mRenderer->DefineViewport( samples, mSpecPresMetrix );
				
				mDispContainer->SetHorRange( mSpecPresMetrix.left, mSpecPresMetrix.right );
				mDispContainer->SetVerRange( 20 , -120 );
				mDispContainer->mpHorRuler->mInteger = mSpecPresMetrix.isIntX;
				mDispContainer->mpVerRuler->mInteger = mSpecPresMetrix.isIntY;



		}
		
		
		void SpectrumDisplay::Init( int W, int H )
		{
				
				// FLTK thingies initialization
				
				mDispContainer = new FLDisplayContainer( 0, 0, W, H );
				
				mPort = new GLPort( Rect<int>( 0, 0, W, H ) );
				mRenderer = new GLLinearSpRenderer( 0, 200, 0 );
				
				mDispContainer->Add( mPort );
				
				resizable( mDispContainer );
	
				end();

				// Linking the port and the renderer...
				
				mPort->SetDrawingCallback( makeMemberFunctor0( *mRenderer, GLLinearSpRenderer, Draw ) );
				
				mPort->Configure( new SpecDisplayGLState , makeMemberFunctor0( *mRenderer, GLLinearSpRenderer, Draw ) );
				

}

} // namespace CLAMVM
