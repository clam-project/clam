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

#ifndef __AUDIOBROWSER__
#define __AUDIOBROWSER__

#include <FL/Fl_Window.H>
#include "AudioPresentation.hxx"
#include "Slotv1.hxx"
#include "Signalv1.hxx"

namespace CLAMGUI
{
		class GLPortSigSlot;
		class GLWaveRenderer;
		class FLDisplayContainer;
}


namespace CLAMVM
{
		using SigSlot::Slotv1;
		using SigSlot::Signalv1;
		using CLAMGUI::GLPortSigSlot;
		using CLAMGUI::GLWaveRenderer;
		using CLAMGUI::FLDisplayContainer;
	
	class AudioBrowser
			: public AudioPresentation, public Fl_Window
	{
	public:
		
		AudioBrowser( int X, int Y, int W, int H, const char* label = 0 );
		
		~AudioBrowser();
		

		virtual void Show();
		virtual void Hide();
				
			void setPainting(bool painting); 
			Slotv1<CLAM::TData>* GetFrameSlot();
			Slotv1<bool>* GetPaintSlot(); 
			Signalv1<double>* GetSignal();
		
	protected:
		
		void Init( int W, int H );

		
		void OnNewAudio( const DataArray& samples, TTime begin, TTime end, TData srate );
	private:

		GLWaveRenderer*         mRenderer;
		GLPortSigSlot*          mGLsurface;
		FLDisplayContainer*     mDispContainer;

};

}

#endif // AudioBrowser.hxx
