#include "MpegBitstream.hxx"
#include <cstring>

namespace CLAM
{

namespace AudioCodecs
{
	const int MpegBitstream::mInputBufferSize = 5*8192;
	
	MpegBitstream::MpegBitstream( FILE* bitstream )
		: mpFile( bitstream )
	{
		mInputBuffer = new unsigned char[mInputBufferSize];
	}
	
	MpegBitstream::~MpegBitstream()
	{
		if ( mInputBuffer )
			delete [] mInputBuffer;
	}

	void MpegBitstream::Init()
	{
		mad_stream_init( &mStreamBuffer );
		mad_frame_init( &mCurrentFrame );
		mad_synth_init( &mMpegSynth );
		mad_timer_reset( &mStreamTimer );
		mFatalError = false;
	}

	TTime MpegBitstream::Finish()
	{
		mad_synth_finish( &mMpegSynth );
		mad_frame_finish( &mCurrentFrame );
		mad_stream_finish( &mStreamBuffer );

		return (TTime)mad_timer_count( mStreamTimer, MAD_UNITS_MILLISECONDS );
	}

	bool MpegBitstream::EOS()
	{
		if ( feof( mpFile ) ) // no more frames
			return true;
		return false;
				
	}

	bool MpegBitstream::NextFrame()
	{
		bool validFrameFound = false;


		while( !validFrameFound )
		{
			if ( mStreamBuffer.buffer == NULL || mStreamBuffer.error == MAD_ERROR_BUFLEN )
			{
				TSize readSize, remaining;
				unsigned char*  readStart;
				
				if ( mStreamBuffer.next_frame != NULL )
				{
					remaining = mStreamBuffer.bufend - mStreamBuffer.next_frame;
					memmove( mInputBuffer, mStreamBuffer.next_frame, remaining );
					readStart = mInputBuffer+remaining;
					readSize = mInputBufferSize - remaining;
				}
				else
				{
					readSize = mInputBufferSize;
					readStart = mInputBuffer;
					remaining = 0;
				}
				
				readSize = fread( readStart, 1, readSize, mpFile );
				
				if ( readSize <= 0 )
				{
					if ( ferror( mpFile ) ) // File error
						return false;
					if ( feof( mpFile ) ) // end of input stream
						break;
					return false;
				}
				
				mad_stream_buffer( &mStreamBuffer, mInputBuffer, readSize+remaining );
				mStreamBuffer.error = mad_error(0);
			}

			if (mad_frame_decode( &mCurrentFrame, &mStreamBuffer ) ) // error
			{
				if ( MAD_RECOVERABLE( mStreamBuffer.error ) )
					return false;
				if ( mStreamBuffer.error == MAD_ERROR_BUFLEN )
					return false;
				else
				{
					mFatalError = true;
					break;
				}
			}
			else
			{
				validFrameFound = true;
				mad_timer_add( &mStreamTimer, mCurrentFrame.header.duration );
			}

		}


		return validFrameFound;

	}

	bool MpegBitstream::FatalError()
	{
		return mFatalError;
	}

	bool MpegBitstream::SynthesizeCurrent()
	{
		mad_synth_frame( &mMpegSynth, &mCurrentFrame );

		return true;
	}

	struct mad_frame& MpegBitstream::CurrentFrame()
	{
		return mCurrentFrame;
	}

	struct mad_synth& MpegBitstream::CurrentSynthesis()
	{
		return mMpegSynth;
	}
	
}

}
