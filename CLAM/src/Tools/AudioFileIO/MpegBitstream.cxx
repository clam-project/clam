#include "MpegBitstream.hxx"
#include "Assert.hxx"
#include <cstring>
#include <iostream>

namespace CLAM
{

namespace AudioCodecs
{
	// MRJ: This number was extracted from Underbit's own
	// mp3 player code ( mad-0.14.2b2, player.c:86 ). As they
	// say: 40000 bytes is worth 2.5 secs of audio on a stream
	// with 128 kpbs, 1s with 320 kbps
	const int MpegBitstream::mInputBufferSize = 5*8192;
	
	MpegBitstream::MpegBitstream( FILE* bitstream )
		: mpFile( bitstream )
	{
		mInputBuffer = new unsigned char[mInputBufferSize];
	}

	MpegBitstream::MpegBitstream()
	{
		mInputBuffer = new unsigned char[mInputBufferSize];
	}
	
	MpegBitstream::~MpegBitstream()
	{
		if ( mInputBuffer )
			delete [] mInputBuffer;
	}

	void MpegBitstream::Init( FILE* fp )
	{
		mpFile = fp;
		Init();
	}

	void MpegBitstream::Init()
	{
		mad_stream_init( &mBitstream );
		mad_frame_init( &mCurrentFrame );
		mad_synth_init( &mMpegSynth );
		mad_timer_reset( &mStreamTimer );
		mFatalError = false;
	}

	TTime MpegBitstream::Finish()
	{
		mad_synth_finish( &mMpegSynth );
		mad_frame_finish( &mCurrentFrame );
		mad_stream_finish( &mBitstream );

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

		while( !validFrameFound && !EOS() && !FatalError() )
		{
			// the first condition ( mStream.buffer == NULL ) handles the first time we 
			// seek a new Mpeg frame since the stream object does not have a buffer attached.			
			// Last time we tried to decode a frame, there wasn't enough data on the buffer,
			// so we must re-read from the file.

			if ( mBitstream.buffer == NULL 
			     || mBitstream.error == MAD_ERROR_BUFLEN ) 
			{

				TSize readSize, remaining;
				unsigned char*  readStart;
			
				if ( mBitstream.next_frame != NULL )
				{
					remaining = mBitstream.bufend - mBitstream.next_frame;
					memmove( mInputBuffer, mBitstream.next_frame, remaining );
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
			
				
				if ( readSize == 0 )
				{
				  // MRJ:Last frame handling. The reason for this can
				  // be traced to libmad-dev mailing list. It seems
				  // that one should add MAD_BUFFER_GUARD bytes worth of
				  // padding if you want to see the last frame decoded...
					CLAM_DEBUG_ASSERT( mInputBufferSize - readSize >= MAD_BUFFER_GUARD,
							   "Whoops! no room left for buffer guard bytes!");
					while ( readSize < MAD_BUFFER_GUARD )
						readStart[ readSize++ ] = 0;
					
				}
				
				// some I/O error occurred
				if ( readSize < 0 )
					continue;
			
			
				mad_stream_buffer( &mBitstream, mInputBuffer, readSize+remaining );
				mBitstream.error = mad_error(0);
			}
			
			
			if (mad_frame_decode( &mCurrentFrame, &mBitstream ) ) // error
			{
				// some *recoverable* error occured
				if ( MAD_RECOVERABLE( mBitstream.error ) )
				{
					switch( mBitstream.error )
					{
					case MAD_ERROR_LOSTSYNC:
						// we try to sync again
						mad_stream_sync( &mBitstream );
						break;
					default:
						std::cerr << "\n " << mad_stream_errorstr( &mBitstream ) << " \n";
					}
					
				}
				// something bad - like a CRC check failure has happened
				else if ( mBitstream.error != MAD_ERROR_BUFLEN )					
					mFatalError = true;
				
				continue;
				
			}
			else // frame was decoded right
			{
				// we signal that a good frame has been decoded
				validFrameFound = true;
				// we add this frame duration to the bitstream internal timer
				mad_timer_add( &mStreamTimer, mCurrentFrame.header.duration );
			}

		}


		return validFrameFound;

	}

	bool MpegBitstream::FatalError()
	{
		return mFatalError || ferror(mpFile)!=0;
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
