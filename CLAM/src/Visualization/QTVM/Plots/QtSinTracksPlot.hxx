#ifndef __QTSINTRACKSPLOT__
#define __QTSINTRACKSPLOT__

#include "Segment.hxx"
#include "SpectralPeakArray.hxx"
#include "MediaTime.hxx"
#include "QtPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class TimeSegmentLabelsGroup;

		class QtSinTracksPlot : public QtPlot
		{
			Q_OBJECT

			public:
				QtSinTracksPlot(QWidget* parent=0);
				virtual ~QtSinTracksPlot();

				virtual void SetData(const Segment& segment);
				virtual void SetData(const Array< SpectralPeakArray >& peakMtx, 
										const TData& sr, const TData& dur);

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

			private:
				TimeSegmentLabelsGroup* _labelsGroup;

				void InitSinTracksPlot();
		};
	}
}

#endif



