#include "MpegAudioStream.hxx"
#include "AudioFile.hxx"
#include "Assert.hxx"

namespace CLAM
{

namespace AudioCodecs
{
	// A reasonable multiple of four
	const TSize MpegAudioStream::mMaxDecodedBlockSize = 8192;

	MpegAudioStream::MpegAudioStream()
		: mpHandle( NULL )
	{
	}

	MpegAudioStream::MpegAudioStream( const AudioFile& file )
		: mpHandle( NULL )
	{
		SetFOI( file );
	}

	MpegAudioStream::~MpegAudioStream()
	{
	}

	void MpegAudioStream::SetFOI( const AudioFile& file )
	{
		AudioFileToNative( file );		
	}

	void MpegAudioStream::AudioFileToNative( const AudioFile& file )
	{
		mName = file.GetLocation();
		mEncodedSampleRate = (int)file.GetHeader().GetSampleRate();
		mEncodedChannels = (int)file.GetHeader().GetChannels();

		mDecodeBuffer.resize( mEncodedChannels );
	}

	void MpegAudioStream::PrepareReading()
	{
		mpHandle = fopen( mName.c_str(), "rb");

		if ( !mpHandle )
		{
			std::string msgString = "Could not open ";
			msgString += mName;
			msgString += " for reading!";

			CLAM_ASSERT( false, msgString.c_str() );
			
		}

		mBitstream.Init( mpHandle );

		SetChannels( mEncodedChannels );
		MarkAllChannelsAsConsumed();
	}

	void MpegAudioStream::PrepareWriting()
	{
		CLAM_ASSERT( false, "CLAM does not encode Mpeg Audio!!!");
	}

	void MpegAudioStream::PrepareReadWrite()
	{
		CLAM_ASSERT( false, "CLAM does not encode Mpeg Audio!!!");
	}

	void MpegAudioStream::Dispose()
	{
		mBitstream.Finish();
	}

	void MpegAudioStream::DiskToMemoryTransfer()
	{
		TSize samplesToRead = mInterleavedData.Size()/mEncodedChannels;

		while( mDecodeBuffer[0].size() < samplesToRead
		       && !mBitstream.EOS() )
		{
			if ( mBitstream.NextFrame() )
			{
			
				mBitstream.SynthesizeCurrent();
				
				for( int i = 0; i < mEncodedChannels; i++ )
				{
					mDecodeBuffer[i].insert( mDecodeBuffer[i].end(),
								 mBitstream.CurrentSynthesis().pcm.samples[i],
								 mBitstream.CurrentSynthesis().pcm.samples[i]+
								 mBitstream.CurrentSynthesis().pcm.length );
				}
			}
		}

		// Checking zero padding
		for ( int i = 0; i < mEncodedChannels; i++ )
			if ( mDecodeBuffer[i].size() < samplesToRead )
				mDecodeBuffer[i].insert( mDecodeBuffer[i].end(),
							 samplesToRead - mDecodeBuffer[i].size(),
							 mad_fixed_t(0) );

		ConsumeDecodedSamples();

		mEOFReached = mBitstream.EOS() && mDecodeBuffer[0].empty();
	}

	void MpegAudioStream::ConsumeDecodedSamples()
	{
		TSize samplesToRead = mInterleavedData.Size()/mEncodedChannels;

		for ( int i = 0; i < mEncodedChannels; i++ )
		{
			TIndex currOffset = 0;

			for ( std::deque<mad_fixed_t>::iterator j = mDecodeBuffer[i].begin();
			      currOffset < mInterleavedData.Size(); 
			      j++, currOffset+=mEncodedChannels )
				mInterleavedData[ currOffset + i ] = mad_f_todouble(*j);
			
			mDecodeBuffer[i].erase( mDecodeBuffer[i].begin(),
						mDecodeBuffer[i].begin() + samplesToRead );
		}

		

	}

	void MpegAudioStream::MemoryToDiskTransfer()
	{
		CLAM_ASSERT( false, "CLAM does not encode Mpeg Audio!!!");
	}
}
	
}
