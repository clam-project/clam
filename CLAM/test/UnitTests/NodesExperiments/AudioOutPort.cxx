
#include "AudioOutPort.hxx"

namespace CLAM
{

AudioOutPort::AudioOutPort()
{
}

AudioOutPort::~AudioOutPort()
{
}

Audio & AudioOutPort::operator[](int offset)
{
	mAudio.GetBuffer().SetPtr( &(mRegion[0]), mRegion.Size() );
	return mAudio;
}

void AudioOutPort::SetSize( int newSize)
{	
}

int AudioOutPort::GetSize()
{
	return 1;
}
void AudioOutPort::SetAudioSize( int newFrameSize )
{
	mRegion.Size( newFrameSize );
}

int AudioOutPort::GetAudioSize()
{
	return mRegion.Size();
}

void AudioOutPort::Produce()
{
	mRegion.Produce();
}

bool AudioOutPort::CanProduce()
{
	return mRegion.CanProduce();
}

int AudioOutPort::GetHop()
{
	return mRegion.Hop();
}

void AudioOutPort::SetHop( int hop )
{
	mRegion.Hop(hop);
}


} // namespace CLAM

