#ifndef __SEGAUDIOPLOTCONTROLLER__
#define __SEGAUDIOPLOTCONTROLLER__

#include <list>
#include "VLineArrayRenderer.hxx"
#include "AudioPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		class SegAudioPlotController : public AudioPlotController
		{
			typedef std::list<unsigned> Marks;

			public:
				SegAudioPlotController();
				virtual ~SegAudioPlotController();

				void SetData(const Audio& audio,std::list<unsigned>& marks);
				void SetMarksColor(Color c);
				void Draw();

			private:
				Marks _marks;
				VLineArrayRenderer _marksRenderer;

				void ProcessMarksData();
		};
	}
}

#endif

