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


#ifndef __AUDIOPRESENTATION__
#define __AUDIOPRESENTATION__

// Includes of New Visualization Module
#include "Presentation.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Slotv4.hxx"
// Includes of Old Visualization Module
#include "ProcDataPresentation.hxx"
#include "FLDisplayContainer.hxx"
#include "GLPortNew.hxx"
#include "GLWaveRenderer.hxx"
#include "GLState.hxx"
#include "AudioGView.hxx"
#include "ViewConfiguration.hxx"
#include "CLAMGL.hxx"
#include "CBL.hxx"
#include "GeometryKit.hxx"
#include "PresentationWindow.hxx" // TODO this should be a forward declaration!

namespace CLAMVM
{
		using SigSlot::Slotv4;
		using CLAM::DataArray;
		using CLAM::TData;
		using CLAM::TTime;

		class AudioPresentation : public Presentation
		{
		private:
				TData mMaximum;
				TData mMinimum;
				
		protected:

				virtual void Bind( Aspect& ) throw( std::bad_cast );

//slots functions:
				virtual void OnNewAudio( const DataArray&, TTime, TTime, TData ) = 0;
// Accessors to received data
				
		public:

				AudioPresentation();

				~AudioPresentation();

				virtual void Show() = 0;

				Slotv4<const DataArray&, TTime, TTime, TData> SetAudio;
		};

}

namespace CLAM
{
	class Audio;
}
namespace CLAMGUI
{
		using CLAM::DataArray;
		using CLAM::TData;

		using CLAM::Audio;

	class AudioPresGLState : public GLState
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

	template<>
	class ProcDataPresentation<Audio>
	: public Presentation
	{
	public:

		ProcDataPresentation( const char* l = 0)
			: Presentation()
		{
			Geometry g( 100, 100, 400, 300 );

			Init( g, l );
		}


		ProcDataPresentation( const Geometry& g, const char* label = 0 )
		{
			Init ( g, label );
		}

		~ProcDataPresentation()
		{
			delete mWindow;
			delete mRenderer;
		}

		void PublishCallbacks();

		void Show();

		void Do()
		{
			mPort->redraw();
		}

		// TODO: there should be a class in between Presentation and the 
		//  FLTK Presentations, where this method goes.
		Fl_Window* GetWindow() { return mWindow; } 

	protected:

		void Init(const Geometry& g, const char* label );

		void UpdateBufferData( const DataArray& array );

		void UpdateSampleRateData( TData value );

		void UpdateDurationData( TData value );

	private:

		CBL::Functor1< const DataArray& >   mDrawBufferCb;
		CBL::Functor1< int >                mDrawSampleRateCb;
		CBL::Functor1< TData >              mDrawDurationCb;

		GLPort*                             mPort;
		GLWaveRenderer*                     mRenderer;
		FLDisplayContainer*                 mDispContainer;
		PresentationWindow*                 mWindow;
	};

}

#endif // AudioPresentation.hxx
