#include "PlotSinTracks.hxx"

#include "Segment.hxx"
#include "SinTracksAdapter.hxx"
#include "Fl_SinTracks.hxx"
#include "WidgetTKWrapper.hxx"

namespace CLAMVM
{

	void plot( const CLAM::Segment& model, const char* label )
	{
		CLAMVM::SinTracksAdapter ma;

		ma.BindTo( model );

		// create the widget
		CLAMVM::Fl_SinTracks presWidget( 100, 100, 640, 480, label );
		
		presWidget.AttachTo( ma );

		ma.Publish();

		presWidget.Show();

		// Run widget toolkit wrapper:
		CLAMVM::WidgetTKWrapper& tk = CLAMVM::WidgetTKWrapper::GetWrapperFor("FLTK");
		tk.Run();
		
	}

}
