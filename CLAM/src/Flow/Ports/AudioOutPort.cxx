
#include "AudioOutPort.hxx"

namespace CLAM
{

AudioOutPort::AudioOutPort( const std::string & name, Processing * proc )
	: OutPort<TData>(name,proc)
{
}

AudioOutPort::~AudioOutPort()
{
}

Audio & AudioOutPort::GetAudio()
{
	mAudio.GetBuffer().SetPtr( &(mRegion[0]), mRegion.Size() );
	return mAudio;
}

void AudioOutPort::SetSampleRate( TData sampleRate )
{
	mAudio.SetSampleRate( sampleRate );
}

TData AudioOutPort::GetSampleRate()
{
	return mAudio.GetSampleRate();
}

} // namespace CLAM

