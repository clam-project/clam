#include "OggVorbisAudioStream.hxx"
#include "AudioFile.hxx"
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <vorbis/codec.h>
#include <iostream>

#if defined ( __powerpc__ ) || defined ( __POWERPC__ )
#define HOST_ENDIANESS 1
#else
#define HOST_ENDIANESS 0
#endif

namespace CLAM
{

namespace AudioCodecs
{
	OggVorbisAudioStream::OggVorbisAudioStream()
		: mFileHandle( NULL ), mValidFileParams( false ), mEncoding( false )
	{
	}

	OggVorbisAudioStream::OggVorbisAudioStream( const AudioFile& file )
		: mFileHandle( NULL ), mValidFileParams( false ), mEncoding( false )
	{
		SetFOI( file );
	}

	OggVorbisAudioStream::~OggVorbisAudioStream()
	{
		if ( mValidFileParams )
			Dispose();
	}

	void OggVorbisAudioStream::SetFOI( const AudioFile& file )
	{
		if ( mValidFileParams )
			Dispose();
		AudioFileToNative( file );
		
	}

	void OggVorbisAudioStream::AudioFileToNative( const AudioFile& file )
	{
		mName = file.GetLocation();
		mEncodedSampleRate = (int)file.GetHeader().GetSampleRate();
		mEncodedChannels = (int)file.GetHeader().GetChannels();
	}


	void OggVorbisAudioStream::PrepareReading()
	{
		if ( ( mFileHandle = fopen( mName.c_str(), "rb" ) ) == NULL )
		{
			std::string msgString = "Could not open ";
			msgString += mName;
			msgString +=" for reading!";
			CLAM_ASSERT( false, msgString.c_str() );
		}

		if ( ov_open( mFileHandle, &mNativeFileParams, NULL, 0 ) < 0 )		
		{
			fclose( mFileHandle );			
			std::string msgString = mName;
			msgString += " is not a valid Ogg/Vorbis file!";
			
			CLAM_ASSERT( false, msgString.c_str() );
		}

		vorbis_info* info = ov_info( &mNativeFileParams, -1 );
		
		SetChannels( info->channels );
		MarkAllChannelsAsConsumed();

		mValidFileParams = true;
		mCurrentSection = 0;
		
		// MRJ: Seen on Audacity sources. It seems that
		// not all encoders respect the specs right: sometimes
		// one might stumble on a file with poorly encoded headers
		// having this the effect of reading several frames of zeros
		// at the beginning
		ov_pcm_seek( &mNativeFileParams, 0 );		
	}

	void OggVorbisAudioStream::PrepareWriting()
	{
		if ( ( mFileHandle = fopen( mName.c_str(), "wb" ) ) == NULL )
		{
			std::string msgString = "Could not open ";
			msgString += mName;
			msgString +=" for writing!";
			CLAM_ASSERT( false, msgString.c_str() );
		}
		
		VorbisI_EncoderSetup();
		MarkAllChannelsAsProduced();
		mEncoding = true;
	}

	void OggVorbisAudioStream::VorbisI_EncoderSetup()
	{

		vorbis_info_init( &mStreamInfo );

		// encoding mode choosing

		int retValue = vorbis_encode_init_vbr( &mStreamInfo, 
						       mEncodedChannels,
						       mEncodedSampleRate,
						       0.5 );

		CLAM_ASSERT( retValue == 0, "Error trying to initialize Vorbis encoder!" );

		// We add to the comment section who we are
		vorbis_comment_init( &mFileComments );
		vorbis_comment_add_tag( &mFileComments, "ENCODER", "CLAM" );

		// analysis state and auxiliary encoding state storage setup
		vorbis_analysis_init( &mDSPState, &mStreamInfo );
		vorbis_block_init( &mDSPState, &mVorbisBlock );
		
		// packet->stream encoder setup
		// pick random serial number
		srand( time(NULL) );
		ogg_stream_init( &mOggStreamState, rand() );

		WriteBitstreamHeader();

		SetChannels( mEncodedChannels );
	}

	void OggVorbisAudioStream::WriteBitstreamHeader()
	{
		// Every Vorbis stream begins with 3 headers:
		//   + the initial header ( with codec setup params )
		//   + the header with the comment fields
		//   + the header with the code books
		
		ogg_packet header_codec_setup;
		ogg_packet header_comments;
		ogg_packet header_codebooks;

		// We make the headers from the current Vorbis DSP module state
		// and file comments
		vorbis_analysis_headerout( &mDSPState, &mFileComments, 
					   &header_codec_setup,
					   &header_comments,
					   &header_codebooks );

		// We 'push' each header one at a time into the stream
		ogg_stream_packetin( &mOggStreamState, &header_codec_setup );
		ogg_stream_packetin( &mOggStreamState, &header_comments );
		ogg_stream_packetin( &mOggStreamState, &header_codebooks );

		// Now we ensure that the audio data will begin on a new
		// 'page' as the specs require
		int eos = 0;

		while( !eos )
		{
			int res = ogg_stream_flush( &mOggStreamState, &mOggPage );
			if ( res == 0 )
				break;

			fwrite( mOggPage.header, 1, mOggPage.header_len, mFileHandle );
			fwrite( mOggPage.body, 1, mOggPage.body_len, mFileHandle );
		}

	}

	void OggVorbisAudioStream::PrepareReadWrite()
	{
		CLAM_ASSERT( false, "Cannot be done!" );
	}

	void OggVorbisAudioStream::Dispose()
	{
		if ( !mEncoding )
		{
			ov_clear( &mNativeFileParams );
			mValidFileParams = false;
		}
		else
		{
			// We tell the Vorbis encoder that we are 
			// finished with encoding frames
			vorbis_analysis_wrote( &mDSPState, 0 );
			
			// Encoder cleaning up
			ogg_stream_clear( &mOggStreamState );
			vorbis_block_clear( &mVorbisBlock );
			vorbis_dsp_clear( &mDSPState );
			vorbis_comment_clear( &mFileComments );
			vorbis_info_clear( &mStreamInfo );

			fclose( mFileHandle );
			
			mEncoding = false;
		}
	}


	void OggVorbisAudioStream::DiskToMemoryTransfer()
	{
		static const double norm = 1.0/32768.0;

		if ( mIntegerSamples.Size() < mInterleavedData.Size() )
		{
			mIntegerSamples.Resize( mInterleavedData.Size() );
			mIntegerSamples.SetSize( mInterleavedData.Size() );
		}		

		int bytesRead = ov_read( &mNativeFileParams, (char*)mIntegerSamples.GetPtr(), 
					mIntegerSamples.Size()*sizeof(TInt16), HOST_ENDIANESS,
					2, 1, &mCurrentSection );



		CLAM_ASSERT( bytesRead >= 0, "Malformed OggVorbis file!" );

		// We get the number of samples we have extracted from the file
		int samplesRead = bytesRead / sizeof(TInt16);


		TData* pSamplesFP = mInterleavedData.GetPtr();
		TInt16* pSamplesInt = mIntegerSamples.GetPtr();

		CLAM_ASSERT( samplesRead <= mIntegerSamples.Size(), "Too many samples!" );

		const TData* pSamplesFPEnd = pSamplesFP + mInterleavedData.Size();
		const TInt16* pSamplesIntEnd = pSamplesInt + samplesRead;

		while ( pSamplesInt != pSamplesIntEnd )
		{
			*pSamplesFP++ = TData(*pSamplesInt++)*norm;
		}

		// If the number of samples read is less than expected
		// then we zero the rest of the fp buffer
		while ( pSamplesFP != pSamplesFPEnd )
		{
			*pSamplesFP++ = 0.0;
		}

		mEOFReached = (bytesRead == 0 ) || ( mCurrentSection != 0 );

	}

	void OggVorbisAudioStream::MemoryToDiskTransfer()
	{
		// Yahoo! The vorbis encoder wants the samples
		// as floats!
		
		// We expose the buffer for submitting data to the encoder

		int frameSize = mInterleavedDataOut.Size()/mEncodedChannels;

		float** encBuffer = vorbis_analysis_buffer( &mDSPState, 
							    frameSize );
		
		int samplesWrote = 0;

		for ( int j = 0; j < mEncodedChannels; j++ )
		{
			int k = j;
			for( int i = 0; i < frameSize; i++, k+=mEncodedChannels )
			{
				encBuffer[j][i] = mInterleavedDataOut[ k ];
			}
		}
			
		vorbis_analysis_wrote( &mDSPState, frameSize );

		while( vorbis_analysis_blockout( &mDSPState, &mVorbisBlock ) == 1 )
		{
			// we assume we want bitrate management

			vorbis_analysis( &mVorbisBlock, NULL );
			vorbis_bitrate_addblock( &mVorbisBlock );

			while( vorbis_bitrate_flushpacket( &mDSPState, &mOggPacket ) )
			{
				// We push the packet into the bitstream
				ogg_stream_packetin( &mOggStreamState, &mOggPacket );				
				
				
				// page writeout
				int eos = 0;
			
				while( !eos )
				{
					int res = ogg_stream_pageout( &mOggStreamState, &mOggPage );
					if ( res == 0 )
						break;
					fwrite( mOggPage.header, 1, mOggPage.header_len, mFileHandle );
					fwrite( mOggPage.body, 1, mOggPage.body_len, mFileHandle );
					
					eos = ( ogg_page_eos( &mOggPage ) )? 1 : 0;
				}

			}
		}


	}
}	

}
