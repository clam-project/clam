
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

} // namespace CLAM

