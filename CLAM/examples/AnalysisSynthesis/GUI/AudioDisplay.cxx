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

#include "AudioDisplay.hxx"
#include "FLDisplayContainer.hxx"
#include "GLPortNew.hxx"
#include "GLWaveRenderer.hxx"
#include "GLState.hxx"
#include "CLAMGL.hxx"
#include "Viewport.hxx"
#include "GeometryKit.hxx"

namespace CLAMVM
{
		using CLAMGUI::GLState;
		using CLAMGUI::Viewport;
		using CLAMGUI::Rect;

		class AudioDisplayGLState : public GLState
		{
		public:
				
				void Apply()
				{
						glClearColor( 0.0, 0.0, 0.0, 0.0 );
						glDisable( GL_CULL_FACE );
						glDisable( GL_DEPTH_TEST );
						glShadeModel( GL_FLAT );
				}
				
		};

		AudioDisplay::AudioDisplay( int X, int Y, int W, int H, const char* label )
				: AudioPresentation(), Fl_Window( X, Y, W, H, label )
		{
				Init( W, H);
		}
		

		AudioDisplay::~AudioDisplay()
		{
				delete mRenderer;
		}
		
		
		void AudioDisplay::Show()
		{
				show();
		}

		void AudioDisplay::Hide()
		{
			hide();
		}

		void AudioDisplay::OnNewAudio( const DataArray& array, TTime begin, TTime end, TData srate )
		{
				Viewport view_specs;
				
				mRenderer->DefineViewport( array, view_specs );
				
				mDispContainer->SetHorRange( view_specs.left, view_specs.right );
				mDispContainer->SetVerRange( view_specs.top, -fabs(view_specs.top-view_specs.bottom));
				mDispContainer->mpHorRuler->mInteger = view_specs.isIntX;
				mDispContainer->mpVerRuler->mInteger = view_specs.isIntY;
				
				mRenderer->CacheData( array );

				mRenderer->SetSamplingRate( srate );

		}
						
		void AudioDisplay::Init( int W, int H)
		{
				// FLTK thingies initialization
				
				mDispContainer = new FLDisplayContainer( 0, 0, W, H );
				
				mPort = new GLPort( Rect<int>( 0, 0, W, H ) );
				mRenderer = new GLWaveRenderer( 0, 200, 0 );
				
				mDispContainer->Add( mPort );
				
				resizable( mDispContainer );
				
				end();
				
				// Linking the port and the renderer...
				
				mPort->SetCullingCallback( 
						makeMemberFunctor4( float, float, unsigned, unsigned, *mRenderer, GLWaveRenderer, PerformCulling ) );

				mPort->SetDrawingCallback( makeMemberFunctor0( *mRenderer, GLWaveRenderer, Draw) );
				mPort->Configure( new AudioDisplayGLState, makeMemberFunctor0( *mRenderer, GLWaveRenderer, Draw) );
				
		}

} // namespace CLAMVM
