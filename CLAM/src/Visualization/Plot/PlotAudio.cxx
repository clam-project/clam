#include "PlotAudio.hxx"

#include "Audio.hxx"
#include "AudioAdapter.hxx"
#include "Fl_Audio.hxx"
#include "WidgetTKWrapper.hxx"

void CLAMVM::plot(const CLAM::Audio &model, const char *label)
{
	// Create model adaptor:
	AudioAdapter ma;

	// Bind model to the model adapter:
	ma.BindTo( model);

	// Create widget to serve as the presentation of the model:
	Fl_Audio presentation(100, 100, 640, 480, label);

	// Attach model adapter to presentation:
	presentation.AttachTo(ma);

	// Let the model adaptor publish it's data:
	ma.Publish();

	// Force presentation (widget) to refresh:	
	presentation.Show();

	// Run widget toolkit wrapper:
	WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");
	tk.Run();
}
