#ifndef __QTAUDIOPLAYER__
#define __QTAUDIOPLAYER__

#include "Audio.hxx"
#include "QtPlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		class QtAudioPlayer : public QtPlayer
		{
			public:
				QtAudioPlayer(QWidget* parent=0);
				virtual ~QtAudioPlayer();

				void SetData(const Audio& audio);
		};
	}
}

#endif

