
#include "AudioInPort.hxx"

namespace CLAM
{

AudioInPort::AudioInPort( const std::string & name, Processing * proc )
	: InPort<TData>(name,proc)
{
}

AudioInPort::~AudioInPort()
{
}

const Audio & AudioInPort::GetAudio()
{
	mAudio.GetBuffer().SetPtr( &(mRegion[0]), mRegion.Size() );
	return mAudio;
}

} // namespace CLAM

