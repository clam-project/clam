#ifndef __AUDIODISPLAY__
#define __AUDIODISPLAY__

#include <FL/Fl_Window.H>
#include "AudioPresentation.hxx"

namespace CLAMGUI
{
		class FLDisplayContainer;
		class GLPort;
		class GLWaveRenderer;
}

namespace CLAMVM
{
		using CLAMGUI::FLDisplayContainer;
		using CLAMGUI::GLPort;
		using CLAMGUI::GLWaveRenderer;
		
		class AudioDisplay
				: public AudioPresentation, public Fl_Window
		{
		public:
				
				AudioDisplay( int X, int Y, int W, int H, const char* label = 0 );
				
				~AudioDisplay();
				
				void Show();

				void Hide();

		protected:
			
				void OnNewAudio( const DataArray& samples, TTime begin, TTime end, TData srate );
	
				void Init( int W, int H );
				
		private:
				
				
				GLPort*                             mPort;
				GLWaveRenderer*                     mRenderer;
				FLDisplayContainer*                 mDispContainer;

		};
}

#endif // AudioDisplay.hxx
