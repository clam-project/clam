#ifndef __QTSEGAUDIOPLOT__
#define __QTSEGAUDIOPLOT__

#include <list>
#include "QtAudioPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class QtSegAudioPlot : public QtAudioPlot
		{
			Q_OBJECT

			public:

				QtSegAudioPlot(QWidget* parent=0);
				virtual ~QtSegAudioPlot();

				virtual void SetData(const Audio& audio,std::list<unsigned>& marks);
				void SetMarksColor(Color c);

			protected:
				virtual void SetData(const Audio& audio);
				virtual void SetPlotController();
				virtual void Connect();

				virtual void DisplayBackgroundBlack();
				virtual void DisplayBackgroundWhite();
		};
	}
}

#endif

