#ifndef __PLAYABLEPLOT__
#define __PLAYABLEPLOT__

#include "QtPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class QtPlayer;
		
		class PlayablePlot : public QtPlot
		{
			public:
				PlayablePlot(QWidget* parent=0);
				virtual ~PlayablePlot();
				
				void Play();
				void Pause();
				void Stop();
				
				bool IsPlaying();
			
			protected:
				QtPlayer* _player;

				virtual void closeEvent(QCloseEvent* e);

				virtual void SetPlotController()=0;
				virtual void Connect()=0;

				virtual void DisplayBackgroundBlack()=0;
				virtual void DisplayBackgroundWhite()=0;

		};
	}
}

#endif

