
#ifndef __AudioOutPort_hxx__
#define __AudioOutPort_hxx__

#include "OutPort.hxx"
#include "Audio.hxx"

namespace CLAM
{

class AudioOutPort : public OutPort<TData>
{
public:
	AudioOutPort();
	virtual ~AudioOutPort();
	Audio & operator[](int offset);
	void SetAudioSize( int newFrameSize );
	int GetAudioSize();
	void Produce();
	bool CanProduce();
	int GetHop();
	void SetHop( int hop );
protected:
	/*
	 * Audio out port has always 1 Audio Token, so this method is not useful. To change the size of audio, use SetAudioSize.?
	 */
	void SetSize( int newSize );	
	/*
	 * Audio out port has always 1 Audio Token, so this method is not useful. To get the size of audio, use GetAudioSize.?
	 */
	int GetSize();

	Audio mAudio;		
};

} // namespace CLAM

#endif // __AudioOutPort_hxx__

