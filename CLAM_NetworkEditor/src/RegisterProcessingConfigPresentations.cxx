
#include "Factory.hxx"
#include "ProcessingConfigPresentation.hxx"

// concrete configs
#include "OscillatorConfigPresentation.hxx"
#include "ConfigPresentationTmpl.hxx"
#include "AudioOutWrapper.hxx"
#include "Controller.hxx"
#include "AudioFile.hxx"
#include "AutoPanner.hxx"
#include "AudioIO.hxx"
#include "AudioMixer.hxx"
#include "BinaryAudioOp.hxx"
#include "Oscillator.hxx"

typedef CLAM::Factory<NetworkGUI::ProcessingConfigPresentation> ProcessingConfigPresentationFactory;

typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AudioOutWrapperConfig> AudioOutWrapperConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::ControllerConfig> ControllerConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AutoPannerConfig> AutoPannerConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AudioFileConfig> AudioFileConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AudioIOConfig> AudioIOConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::AudioMixerConfig> AudioMixerConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::BinaryAudioOpConfig> BinaryAudioOpConfigPresentation;
typedef NetworkGUI::ConfigPresentationTmpl<CLAM::OscillatorConfig> OscillatorConfigPresentationTmpl;

static ProcessingConfigPresentationFactory::Registrator<AudioOutWrapperConfigPresentation> regAudioOutWrapperCfg( "AudioOutWrapperConfig" );
static ProcessingConfigPresentationFactory::Registrator<AutoPannerConfigPresentation> regAutoPannerCfg( "AutoPannerConfig" );
static ProcessingConfigPresentationFactory::Registrator<ControllerConfigPresentation> regControllerCfg( "ControllerConfig" );
static ProcessingConfigPresentationFactory::Registrator<AudioFileConfigPresentation> regAudioFileInCfg( "AudioFileConfig" );
static ProcessingConfigPresentationFactory::Registrator<AudioIOConfigPresentation> regAudioOutCfg( "AudioIOConfig" );
static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::OscillatorConfigPresentation> regOscillatorCfg( "OscillatorConfig" );
//static ProcessingConfigPresentationFactory::Registrator<OscillatorConfigPresentationTmpl> regOscillatorCfg( "OscillatorConfig" );
static ProcessingConfigPresentationFactory::Registrator<AudioMixerConfigPresentation> regAudioMixerCfg( "AudioMixerConfig" );
static ProcessingConfigPresentationFactory::Registrator<BinaryAudioOpConfigPresentation> regMultiplierCfg( "BinaryAudioOpConfig" );
