#include "QtPlayer.hxx"
#include "PlayableList.hxx"
#include "PlayablePlot.hxx"

namespace CLAM
{
	namespace VM
	{
		PlayablePlot::PlayablePlot(QWidget* parent) 
			: QtPlot(parent)
		{
			_player = NULL;
			PlayableList::Add(this);
		}

		PlayablePlot::~PlayablePlot()
		{
		}
				
		void PlayablePlot::Play()
		{
			if(_player) _player->play();
		}

		void PlayablePlot::Pause()
		{
			if(_player) _player->pause();
		}

		void PlayablePlot::Stop()
		{
			if(_player) _player->stop();
		}
				
		bool PlayablePlot::IsPlaying()
		{
			if(_player) return _player->IsPlaying();
			return false;
		}

		void PlayablePlot::closeEvent(QCloseEvent *e)
		{
			PlayableList::Remove(this);
			QtPlot::closeEvent(e);
		}
	}
}

// END

