#ifndef __QTSPECTRUMPLOT__
#define __QTSPECTRUMPLOT__
#include "Spectrum.hxx"
#include "QtPlot.hxx"
namespace CLAM
{
	namespace VM
	{
		class SingleLabel;		
		class QtSpectrumPlot : public QtPlot
		{
			Q_OBJECT
			public:
				QtSpectrumPlot(QWidget* parent=0);
				virtual ~QtSpectrumPlot();

				virtual void SetData(const Spectrum& spec);
				void SetForegroundColor(Color c);
				void SetVLineColor(Color c);
			protected slots:
				void updateMagLabel(TData);
				void updateFreqLabel(TData);

			protected:
				virtual void SetPlotController();
				virtual void Connect();

				virtual void DisplayBackgroundBlack();
				virtual void DisplayBackgroundWhite();
			private:
				SingleLabel *_magLabel,*_freqLabel;

				void InitSpectrumPlot();
		};
	}
}

#endif

