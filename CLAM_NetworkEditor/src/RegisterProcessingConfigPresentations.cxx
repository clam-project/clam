
#include "Factory.hxx"
#include "ProcessingConfigPresentation.hxx"
#include "Qt_ProcessingConfigPresentation.hxx"

typedef CLAM::Factory<NetworkGUI::ProcessingConfigPresentation> ProcessingConfigPresentationFactory;

static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::Qt_ProcessingConfigPresentation> regAudioOutWrapperCfg( "AudioOutWrapperConfig" );
static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::Qt_ProcessingConfigPresentation> regAutoPannerCfg( "AutoPannerConfig" );
static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::Qt_ProcessingConfigPresentation> regControllerCfg( "ControllerConfig" );
static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::Qt_ProcessingConfigPresentation> regAudioFileInCfg( "AudioFileConfig" );
static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::Qt_ProcessingConfigPresentation> regAudioOutCfg( "AudioIOConfig" );
static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::Qt_ProcessingConfigPresentation> regOscillatorCfg( "OscillatorConfig" );
static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::Qt_ProcessingConfigPresentation> regAudioMixerCfg( "AudioMixerConfig" );
static ProcessingConfigPresentationFactory::Registrator<NetworkGUI::Qt_ProcessingConfigPresentation> regMultiplierCfg( "BinaryAudioOpConfig" );
