#ifndef __FL_SPECTRUM_ANALYZER__
#define __FL_SPECTRUM_ANALYZER__

#include <FL/Fl_Window.H>
#include "SpectrumPresentation.hxx"
#include "SpectrumRenderingManager.hxx"

namespace CLAMVM
{
		// forwards declarations
		class Fl_Gl_Single_Display;
		class Fl_GridLayout;

		class Fl_SpectrumAnalyzer : public SpectrumPresentation, public Fl_Window
		{
		private:
				Fl_GridLayout*              mLayout;
				Fl_Gl_Single_Display*       mDisplay;
				SpectrumRenderingManager    mDrawMgr;

		protected:
				
				virtual void OnNewSpectrum( const DataArray&, TData );

		public:
				Fl_SpectrumAnalyzer( int X, int Y, int W, int H, const char* label = 0 );
				~Fl_SpectrumAnalyzer();

				void resize( int X, int Y, int W, int H );
				void Show();
		};
}

#endif // Fl_SpectrumAnalyzer.hxx
