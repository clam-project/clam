
#ifndef __AudioInPort_hxx__
#define __AudioInPort_hxx__

#include "InPort.hxx"
#include "Audio.hxx"

namespace CLAM
{

class AudioInPort : public InPort<TData>
{
public:
	AudioInPort();
	virtual ~AudioInPort();
	const Audio & operator[](int offset);
	void SetAudioSize( int newFrameSize );
	int GetAudioSize();
	void Consume();
	bool CanConsume();
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

#endif // __AudioInPort_hxx__

