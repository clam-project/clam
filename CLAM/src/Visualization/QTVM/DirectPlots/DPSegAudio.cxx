#include "QtAppWrapper.hxx"
#include "QtSegAudioPlot.hxx"
#include "DPSegAudio.hxx"

namespace CLAM
{
	namespace VM
	{
		void PlotSegAudio(	const Audio& audio,
							std::list<unsigned>& marks,
							const std::string& label,
							int x, int y, int w, int h )
		{
			QtAppWrapper::Init();

			QtSegAudioPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetData(audio,marks);
			plot.SwitchDisplayColors(true);
			plot.Show();

			QtAppWrapper::Run();
		}
	}
}

// END

