#include "APlayer.hxx"
#include "QtAudioPlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		QtAudioPlayer::QtAudioPlayer(QWidget* parent) : QtPlayer(parent)
		{
			SetPlayer(new APlayer());
		}
		
		QtAudioPlayer::~QtAudioPlayer()
		{
		}

		void QtAudioPlayer::SetData(const Audio& audio)
		{
			((APlayer*)_player)->SetData(audio);
		}
	}
}

// END

