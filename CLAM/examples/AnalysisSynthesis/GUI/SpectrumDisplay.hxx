//  Includes of old Visualization Module
/** @todo: Remove this **/
#include "SpectrumPresentation.hxx"
#include "Viewport.hxx"
#include <FL/Fl_Window.H>

/** @todo: remove this **/

namespace CLAMGUI
{
		class GLPort;
		class FLDisplayContainer;
		class GLLinearSpRenderer;
}

namespace CLAMVM
{

		using CLAMGUI::GLPort;
		using CLAMGUI::FLDisplayContainer;
		using CLAMGUI::GLLinearSpRenderer;
		

	class SpectrumDisplay
			: public SpectrumPresentation, public Fl_Window
	{
	public:
		
			SpectrumDisplay( int X, int Y, int W, int H, const char* label = 0);
		
		
			virtual ~SpectrumDisplay();
			void Show();
	
	protected:

			virtual void OnNewSpectrum( const DataArray&, TData ); 
		
		
			virtual void Init( int W, int H);

	protected:
		
   		GLPort*                           mPort;
		GLLinearSpRenderer*               mRenderer;
		FLDisplayContainer*               mDispContainer;
		
	};
	
}
