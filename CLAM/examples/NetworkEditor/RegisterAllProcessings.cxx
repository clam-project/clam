
#include "Factory.hxx"
#include "Oscillator.hxx"
#include "AudioMixer.hxx"
#include "AudioMultiplier.hxx"
#include "AudioFileOut.hxx"
#include "AudioFileIn.hxx"

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
typedef CLAM::AudioMixer< 2 > MixerTwoInPorts;

static ProcessingFactory::Registrator<CLAM::Oscillator> regtOscil( "Oscillator" );
static ProcessingFactory::Registrator<MixerTwoInPorts> regtMixer( "Mixer 2" );
static ProcessingFactory::Registrator<CLAM::AudioMultiplier> regtMulti( "Multiplier" );



