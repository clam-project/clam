#ifndef __FL_SINTRACKS__
#define __FL_SINTRACKS__

#include <FL/Fl_Window.H>
#include "Base/SinTracksPresentation.hxx"
#include "SinTracksRenderingManager.hxx"

namespace CLAMVM
{

		class Fl_X_Axis;
		class Fl_Y_Axis;
		class Fl_ZoomSlider;
		class Fl_GridLayout;
		class Fl_Gl_Single_Display;

		class Fl_SinTracks : public SinTracksPresentation, public Fl_Window
		{
		private:
				Fl_GridLayout*            mLayout;
				Fl_X_Axis*                mXAxis;
				Fl_Y_Axis*                mYAxis;
				Fl_ZoomSlider*            mXSlider;
				Fl_ZoomSlider*            mYSlider;
				Fl_Gl_Single_Display*     mDisplay;
				SinTracksRM               mDrawMgr;

		protected:
				virtual void OnNewPartials( const Array<Partial>&, TIndex ); 
				
				virtual void OnNewRange( TData );

				virtual void OnNewDuration( TTime begin, TTime end );

		public:

				Fl_SinTracks( int X, int Y, int W, int H, const char* label = 0 );
				~Fl_SinTracks();

				virtual void resize( int X, int Y, int W, int H );
				virtual void Show();

		};

}

#endif // Fl_SinTracks.hxx
