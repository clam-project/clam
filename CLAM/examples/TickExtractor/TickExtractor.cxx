#include "TickExtractor.hxx"
#include "TickExtractorConfig.hxx"
#include "Err.hxx"
#include "Pulse.hxx"
#include "Audio.hxx"
#include "XMLStorage.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileReaderConfig.hxx"
#include "AudioFile.hxx"
#include "SimacOnsetDetection.hxx"
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
				  CLAM::DataArray& extractedTicksSequence )
	{
		TickExtractorConfig configuration; // we will be using default config
		configuration.SetFromAudio( false );
		
		CLAM::Audio readAudio;

		try
		{
			LoadAudioFile( readAudio, pathToFile );
		}
		catch( Err& e )
		{
			Err propErr("Error opening file: \n" );
			propErr.Embed(e);
			
			throw propErr;
		}
		
		TData sampleRate = readAudio.GetSampleRate();
		TTime duration = readAudio.GetSize()/sampleRate;
		
		Array< TimeIndex > transients;

		if ( !configuration.GetFromAudio() )
		{
			Segment seg;
			seg.AddAudio();
			seg.UpdateData();
			seg.SetAudio(readAudio);
			seg.SetHoldsData(true);
			seg.SetEndTime(duration);

			OnsetDetectionConfig onsetconfig;
			OnsetDetection onset(onsetconfig);
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

		}

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


		if (!configuration.GetFromAudio()) 
		{
			//Use the transients computed in this main
			myTickFromOnsets.Do(transients, allTicks, allBeats, globalTick, globalTempo, IOIHist);
		}
		else 
		{
			//Compute the transients in the Do
			myTickFromOnsets.Do(readAudio, allTicks, allBeats, globalTick, globalTempo, IOIHist);
		}

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
		
		Array<TimeIndex>& ticks = tick.GetIndexes();

		for ( int i = 0 ; i < ticks.Size(); i++ )
			extractedTicksSequence.AddElem( ticks[i].GetPosition() );

	}
}
