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
#include "TickSequenceTracker.hxx"
#include "IOIHistogram.hxx"


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

		if ( transients.Size() > 0 )
		{
			TimeIndex nullTransient;
			nullTransient.SetPosition( 0.0 );
			nullTransient.SetWeight( 0.0 );
			
			transients.InsertElem( 0, nullTransient );
			
			for ( int k = 0; k < transients.Size(); k++ )
			{
				transients[k].SetPosition( transients[k].GetPosition()*sampleRate );
				transients[k].SetWeight( transients[k].GetWeight() );
			}
		}
		// Ticks ( and beats ) computation 

		RhythmDescription::TickSequenceTracker myTickSequenceTracker;
		
		RhythmDescription::TickSequenceTrackerConfig myTickSequenceTrackerConfig;

		myTickSequenceTrackerConfig.SetComputeBeats(configuration.GetComputeBeats());
		myTickSequenceTrackerConfig.SetThreshold_IOIHistPeaks(configuration.GetThreshold_IOIHistPeaks());
		myTickSequenceTrackerConfig.SetTempoLimInf(configuration.GetTempoLimInf());
		myTickSequenceTrackerConfig.SetTempoLimSup(configuration.GetTempoLimSup());
		myTickSequenceTrackerConfig.SetTickLimInf(configuration.GetTickLimInf());
		myTickSequenceTrackerConfig.SetTickLimSup(configuration.GetTickLimSup());
		myTickSequenceTrackerConfig.SetDeviationPenalty(configuration.GetDeviationPenalty());
		myTickSequenceTrackerConfig.SetOverSubdivisionPenalty(configuration.GetOverSubdivisionPenalty());
		myTickSequenceTrackerConfig.SetGaussianWindowSize(configuration.GetGaussianWindowSize());
		myTickSequenceTrackerConfig.SetScope(configuration.GetScope());
		myTickSequenceTrackerConfig.SetAdjustWithOnsets(configuration.GetAdjustWithOnsets());
		myTickSequenceTrackerConfig.SetNTrans(configuration.GetNTrans());
		myTickSequenceTrackerConfig.SetTransHop(configuration.GetTransHop());
		
		myTickSequenceTrackerConfig.SetSamplingRate(sampleRate);
		
		myTickSequenceTracker.Configure(myTickSequenceTrackerConfig);		

		RhythmDescription::IOIHistogram ioiHistogram;

		Pulse tickSequence;
		Pulse beatSequence;

		myTickSequenceTracker.Start();
		
		//Use the transients computed in this main
		myTickSequenceTracker.Do( transients, tickSequence, beatSequence, ioiHistogram );

		myTickSequenceTracker.Stop();


		ticksList = tickSequence;
		

	}
}
