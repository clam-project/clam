
#include "AudioInPort.hxx"

namespace CLAM
{

AudioInPort::AudioInPort()
{
}

AudioInPort::~AudioInPort()
{
}

const Audio & AudioInPort::operator[](int offset)
{
	mAudio.GetBuffer().SetPtr( &(mRegion[0]), mRegion.Size() );
	return mAudio;
}

void AudioInPort::SetSize( int newSize)
{	
	// TODO: this class should have just one audio...how we can avoid people to call this function?
}

int AudioInPort::GetSize()
{
	return 1;
}
void AudioInPort::SetAudioSize( int newFrameSize )
{
	mRegion.Size( newFrameSize );
}

int AudioInPort::GetAudioSize()
{
	return mRegion.Size();
}

void AudioInPort::Consume()
{
	mRegion.Consume();
}

bool AudioInPort::CanConsume()
{
	return mRegion.CanConsume();
}

int AudioInPort::GetHop()
{
	return mRegion.Hop();
}

void AudioInPort::SetHop( int hop )
{
	mRegion.Hop(hop);
}

} // namespace CLAM

