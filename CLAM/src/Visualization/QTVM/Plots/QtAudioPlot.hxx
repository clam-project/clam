#ifndef __QTAUDIOPLOT__
#define __QTAUDIOPLOT__

#include "Audio.hxx"
#include "MediaTime.hxx"
#include "PlayablePlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class TimeSegmentLabelsGroup;
		class SingleLabel;
	
		class QtAudioPlot : public PlayablePlot
		{
			Q_OBJECT

			public:

				QtAudioPlot(QWidget* parent=0);
				virtual ~QtAudioPlot();

				virtual void SetData(const Audio& audio);

				void SetForegroundColor(Color c);
				void SetDialColor(Color c);
				void SetRegionColor(Color c);

			protected slots:
				void updateRegion(MediaTime);

			protected:
				virtual void keyPressEvent(QKeyEvent* e);
				virtual void keyReleaseEvent( QKeyEvent* e);

				virtual void SetPlotController();
				virtual void Connect();

				virtual void DisplayBackgroundBlack();
				virtual void DisplayBackgroundWhite();

				void SetPData(const Audio& audio);
				
			private:
				TimeSegmentLabelsGroup* _labelsGroup;
				SingleLabel *_leftAmpLab, *_rightAmpLab;
				
				void UpdateAmpLabels(MediaTime time);
				void InitAudioPlot();
		};
	}
}

#endif

