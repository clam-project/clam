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

#include "AudioBrowser.hxx"
#include "FLDisplayContainer.hxx"
#include "GLPortSigSlot.hxx"
#include "GLWaveRenderer.hxx"
#include "GeometryKit.hxx"
#include "CLAMGL.hxx"
#include "CLAMGL.hxx"
#include "GLState.hxx"
#include "CBL.hxx"

namespace CLAMVM
{
		using CLAMGUI::GLState;
		using CLAMGUI::Viewport;
		using CLAMGUI::Rect;

		class AudioBrowserGLState : public GLState
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


		AudioBrowser::AudioBrowser( int X, int Y, int W, int H, const char* label )
				: AudioPresentation(), Fl_Window( X,Y,W,H, label )
		{
				Init( W, H );
		}
		
		AudioBrowser::~AudioBrowser()
		{
				
		}
		
		void AudioBrowser::setPainting(bool painting) 
		{ 
				mGLsurface->setPainting(painting); 
		}
		
		Slotv1<TData>* AudioBrowser::GetFrameSlot()
		{
				return mGLsurface->getFrameSlot();

		}

		Slotv1<bool>* AudioBrowser::GetPaintSlot() 
		{ 
				return mGLsurface->getPaintSlot(); 
		} 

		Signalv1<double>* AudioBrowser::GetSignal() 
		{ 
				return mGLsurface->getSignal(); 
		}
 		
		void AudioBrowser::Show()
		{
				show();
		}
		
		void AudioBrowser::OnNewAudio( const DataArray& array, TTime begin, TTime end, TData srate )
		{
				Viewport view_specs;
				mRenderer->SetSamplingRate( srate );
			
				mRenderer->DefineViewport( array, view_specs );
				
				mDispContainer->SetHorRange( view_specs.left, view_specs.right );
				mDispContainer->SetVerRange( view_specs.top, -fabs(view_specs.top-view_specs.bottom));//-( view_specs.top - view_specs.bottom ) );
				
				mDispContainer->mpHorRuler->mInteger = view_specs.isIntX;
				mDispContainer->mpVerRuler->mInteger = view_specs.isIntY;
				
				mRenderer->CacheData( array );
		}
		
		void AudioBrowser::Init( int W, int H )
		{
				// FLTK thingies initialization			   
				mDispContainer = new FLDisplayContainer( 0, 0, W, H);
				

				mGLsurface = new GLPortSigSlot( Rect<int>( 0, 0, W, H ) );
				mRenderer = new GLWaveRenderer( 0, 200, 0 );
				
				mDispContainer->Add( mGLsurface );
				
				resizable( mDispContainer );
				
				end();
				
				// Linking the port and the renderer...
				
				mGLsurface->SetCullingCallback( 
						makeMemberFunctor4( float, float, unsigned, unsigned, *mRenderer, GLWaveRenderer, PerformCulling ) );
				mGLsurface->SetDrawingCallback( makeMemberFunctor0( *mRenderer, GLWaveRenderer, Draw) );
				mGLsurface->Configure( new AudioBrowserGLState, makeMemberFunctor0( *mRenderer, GLWaveRenderer, Draw) );
}

} // namespace CLAMVM
