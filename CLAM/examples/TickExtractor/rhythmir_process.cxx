#include "rhythmir_process.hxx"
#include "AudioFile.hxx"
#include "MonoAudioFileReader.hxx"
#include "Normalization.hxx"

namespace RhythmIR
{
	void LoadInputAudio( CLAM::DescriptionDataPool& pool,
			     std::string filename )
	{
		CLAM::AudioFile file;
		file.SetLocation( filename );

		if ( !file.IsReadable() )
		{
			std::string errStr = "Error: file " + file.GetLocation() + " cannot be opened ";
			errStr += "or is encoded in an unrecognized format\n";
			
			throw CLAM::Err( errStr.c_str() );
		}

		// Pool setup
		// MRJ: Length is in milliseconds :S
		CLAM::TSize fileSize = int((file.GetHeader().GetLength()/1000.)*file.GetHeader().GetSampleRate());

		pool.SetNumberOfContexts( "Sample",  fileSize );	

		*pool.GetAttributePool<CLAM::TData>("Global", "SampleRate" ) = file.GetHeader().GetSampleRate();

		CLAM::DataArray tempMemory;
		tempMemory.SetPtr( pool.GetAttributePool<CLAM::TData>("Sample","Value"), fileSize );
		CLAM::Audio     tempAudio;
		tempAudio.SetBuffer( tempMemory );
		tempAudio.SetSampleRate( file.GetHeader().GetSampleRate() );

		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile( file );
		CLAM::MonoAudioFileReader reader;
		reader.Configure( cfg );
				
		//Read Audio File
		reader.Start();		
		reader.Do( tempAudio );
		reader.Stop();

	}

	void NormalizeInputAudio( CLAM::DescriptionDataPool& pool, 
				  const CLAM::TickExtractorConfig& cfg )
	{
		if ( cfg.GetOnsetDetection().GetString() == "MTG" )
		{
			CLAM::Normalization       audioNormalizer;
			CLAM::NormalizationConfig audioNormalizerConfig;

			// Scaling factor is computed from the "dominant" energy level
			audioNormalizerConfig.SetType( 3 ); 

			// Building the dummy Audio objects from the pool
			CLAM::DataArray dummyArrayOrig;
			CLAM::DataArray dummyArrayNorm;
			
			dummyArrayOrig.SetPtr( pool.GetAttributePool<CLAM::TData>("Sample","Value"),
					       pool.GetNumberOfContexts( "Sample") );
			dummyArrayNorm.SetPtr( pool.GetAttributePool<CLAM::TData>("Sample","NormalizedValue"),
					       pool.GetNumberOfContexts( "Sample") );

			CLAM::Audio dummyAudioOrig;
			CLAM::Audio dummyAudioNorm;

			dummyAudioOrig.SetBuffer( dummyArrayOrig );
			dummyAudioNorm.SetBuffer( dummyArrayNorm );
			dummyAudioOrig.SetSampleRate( *pool.GetAttributePool<CLAM::TData>("Global","SampleRate") );
			dummyAudioNorm.SetSampleRate( *pool.GetAttributePool<CLAM::TData>("Global","SampleRate") );

			// Configuration and execution
			audioNormalizer.Configure( audioNormalizerConfig );
			audioNormalizer.Start();
			audioNormalizer.Do( dummyAudioOrig, dummyAudioNorm );
			audioNormalizer.Stop();
		}
	}

	
}
