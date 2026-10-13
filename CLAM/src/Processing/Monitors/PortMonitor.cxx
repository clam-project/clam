#include "PortMonitor.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

    template <>
	bool PortMonitor<Audio,AudioInPort>::Do()
	{
		if(!AbleToExecute()) return true;
		unsigned whichDataToWrite = mWhichDataToRead?0:1;
		mData[whichDataToWrite] = mInput.GetAudio();
		mSigNewData.Emit();
		{
			// The old ScopedTryLock(..., true) path blocked here; now a frozen
			// reader makes us skip publishing this buffer instead.
			std::unique_lock<std::mutex> lock(mSwitchMutex, std::try_to_lock);
			if (lock.owns_lock())
				mWhichDataToRead = whichDataToWrite;
		}
		mInput.Consume();
		return true;
	}

	template <>
	PortMonitor<Audio,AudioInPort>::PortMonitor(const Config& cfg)
	    : mInput("Input", this)
		, mWhichDataToRead(0)
	{
		Configure(cfg);
	}
#ifndef QT_PLUGIN
	static const char * metadataPeaks[] = {
		"key", "PeaksPortMonitor",
	//	"category", "Monitors",
	//	"description", "PeaksPortMonitor",
		0
	};
	static FactoryRegistrator<ProcessingFactory, PeaksPortMonitor> regPeaks = metadataPeaks;

	static const char * metadataSpectrum[] = {
		"key", "SpectrumPortMonitor",
	//	"category", "Monitors",
	//	"description", "SpectrumPortMonitor",
		0
	};
	static FactoryRegistrator<ProcessingFactory, SpectrumPortMonitor> regSpectrum = metadataSpectrum;

	static const char * metadataAudio[] = {
		"key", "AudioPortMonitor",
	//	"category", "Monitors",
	//	"description", "AudioPortMonitor",
		0
	};
	static FactoryRegistrator<ProcessingFactory, AudioPortMonitor> regAudio = metadataAudio;

	static const char * metadataFundamental[] = {
		"key", "FundamentalPortMonitor",
	//	"category", "Monitors",
	//	"description", "FundamentalPortMonitor",
		0
	};
	static FactoryRegistrator<ProcessingFactory, FundamentalPortMonitor> regFundamental = metadataFundamental;

	static const char * metadataAudioBuff[] = {
		"key", "AudioBuffPortMonitor",
	//	"category", "Monitors",
	//	"description", "AudioBuffPortMonitor",
		0
	};
	static FactoryRegistrator<ProcessingFactory, AudioBuffPortMonitor> regAudioBuff = metadataAudioBuff;

	static const char * metadataSpecgram[] = {
		"key", "SpecgramPortMonitor",
	//	"category", "Monitors",
	//	"description", "SpecgramPortMonitor",
		0
	};
	static FactoryRegistrator<ProcessingFactory, SpecgramPortMonitor> regSpecgram = metadataSpecgram;

	static const char * metadataFundTrack[] = {
		"key", "FundTrackPortMonitor",
	//	"category", "Monitors",
	//	"description", "FundTrackPortMonitor",
		0
	};
	static FactoryRegistrator<ProcessingFactory, FundTrackPortMonitor> regFundTrack = metadataFundTrack;

	static const char * metadataSinTracks[] = {
		"key", "SinTracksPortMonitor",
	//	"category", "Monitors",
	//	"description", "SinTracksPortMonitor",
		0
	};
	static FactoryRegistrator<ProcessingFactory, SinTracksPortMonitor> regSinTracks = metadataSinTracks;
#endif
	
} // namespace CLAM
