#include "TickExtractor.hxx"
#include "TickExtractorConfig.hxx"
#include "Err.hxx"
#include "Pulse.hxx"
#include "Audio.hxx"
#include "XMLStorage.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileReaderConfig.hxx"
#include "AudioFile.hxx"
#include "OnsetDetector.hxx"
#include "TransientGen.hxx"
#include "TickFromOnsets.hxx"


namespace CLAM
{

	static void LoadAudioFile( CLAM::Audio & audio, const std::string & audioFileName )
	{
		CLAM::AudioFile file;
		file.SetLocation( audioFileName );
		
		if ( !file.IsReadable() )
		{
			std::string errStr = "Error: file " + file.GetLocation() + " cannot be opened ";
			errStr += "or is encoded in an unrecognized format\n";
			
			throw Err( errStr.c_str() );

		}
		
		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile( file );
		CLAM::MonoAudioFileReader reader;
		reader.Configure( cfg );
		
		CLAM::TSize fileSize = file.GetHeader().GetSamples();
		
		audio.SetSize(fileSize);
		audio.SetSampleRate(file.GetHeader().GetSampleRate());
		
		//Read Audio File
		reader.Start();		
		reader.Do(audio);
		reader.Stop();
	}


	void ExtractTicksSequence( std::string pathToFile,
				   CLAM::Pulse& ticksList )
	{
		TickExtractorConfig configuration; // we will be using default config
		configuration.SetFromAudio( false );		

		Segment seg;
		seg.AddAudio();
		seg.UpdateData();
		seg.SetHoldsData(true);


		try
		{
			LoadAudioFile( seg.GetAudio(), pathToFile );
		}
		catch( Err& e )
		{
			Err propErr("Error opening file: \n" );
			propErr.Embed(e);
			
			throw propErr;
		}
		
		TData sampleRate = seg.GetAudio().GetSampleRate();
		TTime duration = seg.GetAudio().GetSize()/sampleRate;
		seg.SetEndTime(duration);
		
		Array< TimeIndex > transients;

		
		OnsetDetectorConfig onsetconfig;
		OnsetDetector onset(onsetconfig);
		onsetconfig.SetComputeOffsets(false);
		onsetconfig.SetGlobalThreshold(25);
		
		onset.Configure(onsetconfig);
		
		onset.Start();
		onset.Do(seg, transients);
		
		transients.Resize(transients.Size()+1);
		transients.SetSize(transients.Size()+1);
		
		for(int i=transients.Size()-1;i>0;i--)
		{
			transients[i].SetPosition(transients[i-1].GetPosition()*sampleRate);
			transients[i].SetWeight(transients[i-1].GetWeight());
		}

		transients[0].SetPosition(0.0);
		transients[0].SetWeight(0.0);


		// Ticks ( and beats ) computation 

		TickFromOnsetsConfig myTickFromOnsetsConfig;
		TickFromOnsets myTickFromOnsets(myTickFromOnsetsConfig);
		myTickFromOnsetsConfig.SetComputeBeats(configuration.GetComputeBeats());
		myTickFromOnsetsConfig.SetThreshold_IOIHistPeaks(configuration.GetThreshold_IOIHistPeaks());
		myTickFromOnsetsConfig.SetTempoLimInf(configuration.GetTempoLimInf());
		myTickFromOnsetsConfig.SetTempoLimSup(configuration.GetTempoLimSup());
		myTickFromOnsetsConfig.SetTickLimInf(configuration.GetTickLimInf());
		myTickFromOnsetsConfig.SetTickLimSup(configuration.GetTickLimSup());
		myTickFromOnsetsConfig.SetDeviationPenalty(configuration.GetDeviationPenalty());
		myTickFromOnsetsConfig.SetOverSubdivisionPenalty(configuration.GetOverSubdivisionPenalty());
		myTickFromOnsetsConfig.SetGaussianWindowSize(configuration.GetGaussianWindowSize());
		myTickFromOnsetsConfig.SetScope(configuration.GetScope());
		myTickFromOnsetsConfig.SetAdjustWithOnsets(configuration.GetAdjustWithOnsets());
		myTickFromOnsetsConfig.SetNTrans(configuration.GetNTrans());
		myTickFromOnsetsConfig.SetTransHop(configuration.GetTransHop());
		
		myTickFromOnsetsConfig.SetSamplingRate(sampleRate);
		
		myTickFromOnsets.Configure(myTickFromOnsetsConfig);
		
		myTickFromOnsets.Start();

		Array<TimeIndex> allTicks; allTicks.Init();
		Array<TimeIndex> allBeats; allBeats.Init();
		TData globalTempo=-1;
		TData globalTick=-1;

		Audio IOIHist;
		IOIHist.SetSampleRate(sampleRate); //Don't really know if it's used, but just in case...


		//Use the transients computed in this main
		myTickFromOnsets.Do(transients, allTicks, allBeats, globalTick, globalTempo, IOIHist);

		myTickFromOnsets.Stop();

		Pulse tick;
		Pulse beat;
		if (globalTick==-1) 
		{
			tick.SetRate(0.0);
			beat.SetRate(0.0);
		}
		else 
		{
			tick.SetRate(60.0/globalTick);
			beat.SetRate(60.0/globalTempo);
		}
		tick.SetIndexes(allTicks);
		beat.SetIndexes(allBeats);

		ticksList = tick;
		

	}
}
