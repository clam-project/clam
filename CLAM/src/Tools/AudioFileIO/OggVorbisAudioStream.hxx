#ifndef __OGGVORBISAUDIOSTREAM__
#define __OGGVORBISAUDIOSTREAM__

#include "AudioCodecs_Stream.hxx"
#include <string>
#include <vorbis/vorbisfile.h>
#include <vorbis/vorbisenc.h>
#include "DataTypes.hxx"
#include "Array.hxx"
#include "CircularBuffer.hxx"
#include <deque>

namespace CLAM
{

namespace AudioCodecs
{
	class OggVorbisAudioStream : public Stream
	{

	public:
		OggVorbisAudioStream();
		OggVorbisAudioStream( const AudioFile& file );

		~OggVorbisAudioStream();

		void SetFOI( const AudioFile& file );

		void PrepareReading();
		void PrepareWriting();
		void PrepareReadWrite();
		void Dispose();

	protected:
		void AudioFileToNative( const AudioFile& file );
		void DiskToMemoryTransfer();
		void MemoryToDiskTransfer();

		void VorbisI_EncoderSetup();
		void WriteBitstreamHeader();

		
	protected:
		std::string      mName;
		FILE*            mFileHandle;
		OggVorbis_File   mNativeFileParams;
		Array<TInt16>    mIntegerSamples;
		int              mCurrentSection;
		bool             mValidFileParams;

		// Structs needed for Vorbis encoding
		vorbis_info        mStreamInfo;
		vorbis_comment     mFileComments;

		ogg_stream_state   mOggStreamState; // os
		ogg_page           mOggPage;        // og
		ogg_packet         mOggPacket;      // op
		vorbis_dsp_state   mDSPState;       // vd
		vorbis_block       mVorbisBlock;    // vb
		
		int                mEncodedSampleRate;
		int                mEncodedChannels;
		bool               mEncoding;
		int                mOffset;

		static const TSize      mMaxBlockSize;
		Array<TInt16>           mBlockBuffer;
		Array<TInt16>           mMiddleBuffer;
		CircularBuffer<TInt16>  mCBuffer;
		std::deque<TInt16>      mAltBuffer;
		TSize                   mRemainderOffset;
  	};
}

}

#endif // OggVorbisAudioStream.hxx
