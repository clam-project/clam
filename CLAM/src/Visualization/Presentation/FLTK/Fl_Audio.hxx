#ifndef __FL_AUDIO__
#define __FL_AUDIO__

#include <FL/Fl_Window.H>
#include "AudioPresentation.hxx"
#include "AudioRenderingManager.hxx"

namespace CLAMGUI
{
		// forward declarations
		class Fl_X_Axis;
		class Fl_Y_Axis;
		class Fl_ZoomSlider;
		class Fl_Gl_Single_Display;
		class Fl_GridLayout;

		class Fl_Audio : public AudioPresentation, public Fl_Window
		{

		private:
				Fl_X_Axis*              mXAxis;
				Fl_Y_Axis*              mYAxis;
				Fl_ZoomSlider*          mXSlider;
				Fl_ZoomSlider*          mYSlider;
				Fl_GridLayout*          mLayout;
				Fl_Gl_Single_Display*   mDisplay;
				AudioRenderingManager   mDrawMgr;

		protected:

				virtual void OnNewAudio( const DataArray&, TTime, TTime, TData );

		public:
				Fl_Audio( int X, int Y, int W, int H, const char* label = 0 );
				~Fl_Audio();
				
				void resize( int X, int Y, int W, int H );
				void Show();
			

		};

}


#endif // Fl_Audio.hxx
