/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "PCMStreamCodec.hxx"
#include "Assert.hxx"

#ifdef CLAM_DOUBLE
#define CLAM_sf_read sf_readf_double
#define CLAM_sf_write sf_writef_double
#else
#define CLAM_sf_read sf_readf_float
#define CLAM_sf_write sf_writef_float
#endif

namespace CLAM
{
	PCMStreamCodec::PCMStreamCodec()
		: mFileHandle( NULL ), mStrictStreaming( true )
	{
		mNativeFileParams.format = 0;
	}

	PCMStreamCodec::PCMStreamCodec( const AudioFile& file )
		: mFOI( file ), mFileHandle( NULL ), mStrictStreaming( true )
	{
		AudioFileToNative();
	}

	PCMStreamCodec::~PCMStreamCodec()
	{
		if ( mFileHandle )
		{
			sf_close( mFileHandle );
		}
	}

	void PCMStreamCodec::AudioFileToNative()
	{
		mNativeFileParams.channels = mFOI.GetChannels();
		mNativeFileParams.samplerate = (int) mFOI.GetSampleRate();
		mNativeFileParams.format = mFOI.GetFormat() | mFOI.GetEncoding() | mFOI.GetEndianess();
	}

	void PCMStreamCodec::SetFOI( const AudioFile& file )
	{
		Dispose();
		mFOI = file;
		AudioFileToNative();
	}

	void PCMStreamCodec::PrepareReading()
	{
		CLAM_ASSERT( mFOI.CanRead(),
			     "Cannot open file for reading!!!" );

		mFileHandle = sf_open( mFOI.GetName().c_str(), 
				       SFM_READ, 
				       &mNativeFileParams );

		CLAM_ASSERT( mFileHandle != NULL,
			     "Cannot open file for reading!!!" );

		mChannelsConsumed.resize( mNativeFileParams.channels );
		MarkAllChannelsAsConsumed();
		mEOFReached = false;
	}

	void PCMStreamCodec::PrepareWriting()
	{
		
		mFileHandle = sf_open( mFOI.GetName().c_str(),
				       SFM_WRITE,
				       &mNativeFileParams );

		CLAM_ASSERT( mFileHandle != NULL,
			     "Cannot open file for writing!!!" );

		mChannelsProduced.resize( mNativeFileParams.channels );
		MarkAllChannelsAsProduced();
	}

	void PCMStreamCodec::PrepareReadWrite()
	{
		CLAM_ASSERT( mFOI.CanRead(),
			     "Cannot open file for reading!!!" );

		mFileHandle = sf_open( mFOI.GetName().c_str(),
				       SFM_RDWR,
				       &mNativeFileParams );

		CLAM_ASSERT( mFileHandle != NULL,
			     "Cannot open file for reading/writing!!!" );

		mChannelsConsumed.resize( mNativeFileParams.channels );
		mChannelsProduced.resize( mNativeFileParams.channels );
		MarkAllChannelsAsConsumed();
		MarkAllChannelsAsProduced();
		mEOFReached = false;
	}

	void PCMStreamCodec::Dispose()
	{
		if ( mFileHandle )
		{
			sf_close( mFileHandle );
			mFileHandle = NULL;
		}
	}

	inline bool PCMStreamCodec::AllChannelsProduced()
	{

		std::vector<bool>::iterator i = mChannelsProduced.begin();

		while ( i != mChannelsProduced.end() && *i )
			i++;

		return i == mChannelsProduced.end();
	}

	inline void PCMStreamCodec::MarkAllChannelsAsProduced()
	{

		for ( std::vector<bool>::iterator i = mChannelsProduced.begin();
		      i != mChannelsProduced.end();
		      i++ )
			*i = true;

	}

	inline void PCMStreamCodec::ResetProducedChannels()
	{
		for ( std::vector<bool>::iterator i = mChannelsProduced.begin();
		      i != mChannelsProduced.end();
		      i++ )
			*i = false;
	}

	inline bool PCMStreamCodec::AllChannelsConsumed()
	{
		std::vector<bool>::iterator i = mChannelsConsumed.begin();

		while ( i != mChannelsConsumed.end() && *i )
			i++;

		return i == mChannelsConsumed.end();

	}

	inline void PCMStreamCodec::MarkAllChannelsAsConsumed()
	{
		for ( std::vector<bool>::iterator i = mChannelsConsumed.begin();
		      i != mChannelsConsumed.end();
		      i++ )
			*i = true;
	}

	inline void PCMStreamCodec::ResetConsumedChannels()
	{
		for ( std::vector<bool>::iterator i = mChannelsConsumed.begin();
		      i != mChannelsConsumed.end();
		      i++ )
			*i = false;
	}

	inline bool PCMStreamCodec::HandleReAllocation( DataArray& buffer, TSize newSize )
	{
		if ( newSize  > buffer.Size() )
		{
			buffer.Resize( 2 * newSize );
			buffer.SetSize( newSize );
			return true;
		}
		else
		{
			buffer.SetSize( newSize );		
			return false;
		}
	}

	inline void PCMStreamCodec::CheckForFileReading( TSize howmany )
	{
		if ( !StrictStreaming() || AllChannelsConsumed() )
		{
			ResetConsumedChannels();
			
			if ( HandleReAllocation( mInterleavedData, howmany*mNativeFileParams.channels ) )
				mFramesToRead = howmany;

			// Acquire samples from file 

			DiskToMemoryTransfer();
		}
	}

	inline void PCMStreamCodec::DiskToMemoryTransfer()
	{
		int channelCount = mNativeFileParams.channels;			
		
		sf_count_t framesRead = CLAM_sf_read( mFileHandle, 
						       mInterleavedData.GetPtr(),
						       mFramesToRead );
		
		if ( framesRead < mFramesToRead ) // EOF reached
		{
			// We set the remainder to zero

			const TData* end = mInterleavedData.GetPtr() + mInterleavedData.Size();

			for ( TData* i = mInterleavedData.GetPtr() + ( framesRead * channelCount );
			      i != end;
			      i++ )
				*i = 0.0;

			/*
			for ( int i = samplesRead * channelCount;
			      i < mInterleavedData.Size();
			      i += channelCount )
				for ( int j = 0; j < channelCount; j++ )
					mInterleavedData[ i + j ] = 0.0;
			*/
			
			mEOFReached = true;
		}		
	}

	bool PCMStreamCodec::ReadData( int channel, TData* ptr, TSize howmany )
	{
		CheckForFileReading( howmany );
		// Actual data reading
		int channelCount = mNativeFileParams.channels;

		const TData* end = mInterleavedData.GetPtr() + mInterleavedData.Size();
		const int stride = channelCount + channel;

		for ( TData* i = mInterleavedData.GetPtr();
		      i < end; i+=stride, ptr++ )
			*ptr = *i;

		mChannelsConsumed[ channel ] = true;
		
		return mEOFReached;
		       
	}

	bool PCMStreamCodec::ReadData( int* channels, int nchannels,
				       TData** samples, TSize howmany )
	{
		CheckForFileReading( howmany );
		// Actual data reading
		int channelCount = mNativeFileParams.channels;

		const TData*  end = mInterleavedData.GetPtr() + mInterleavedData.Size();
		TData** const samplesEnd = samples + nchannels;
		const int* endChannels = channels + nchannels;
		std::vector<bool>::iterator cIt = mChannelsConsumed.begin();

		for( int* currentChannel = channels;
		     currentChannel != endChannels;
		     currentChannel++ )
		{
			const int channelIndex = *currentChannel;
			// mark channel as consumed
			*(cIt + channelIndex ) = true;
			const int stride = channelCount + channelIndex;
			TData* pSamples = *(samples+channelIndex);

			for ( const TData* i = mInterleavedData.GetPtr();
			      i<end;
			      i+=stride, pSamples++ )
			{
				*pSamples = *i;
			}
		}

/* Bad memory access pattern
		for ( TData* i = mInterleavedData.GetPtr();
		      i != end;
		      i+=channelCount )
		{
			int* channelIdx = channels;
			for ( TData**  data = samples;
			      data != samplesEnd;
			      data++, channelIdx++ )
			{
				// read sample value
				**data = *(i + *channelIdx ); 
				// advance pointer
				(*data)++;
				// mark channel as consumed
				*(cIt + *channelIdx ) = true;
			}

		}
*/

/* First naive approach
		for ( int i = 0; i < mInterleavedData.Size(); i+=channelCount )
		{
			for ( int j = 0; j < nchannels; j++ )
			{
				*samples[j] = mInterleavedData[i + channels[j] ];
				samples[j]++;
			}	
		}

		for ( int i = 0; i < nchannels; i++ )
			mChannelsConsumed[ channels[i] ] = true;
*/
		return mEOFReached;
		
	}

	inline void PCMStreamCodec::PrepareFileWriting( TSize howmany )
	{
		if ( AllChannelsProduced() )
		{
			ResetProducedChannels();
			
			if ( HandleReAllocation( mInterleavedDataOut, howmany * mNativeFileParams.channels ) )
				mFramesToWrite = howmany;

		}
	}

	void PCMStreamCodec::WriteData( int channel, const TData* ptr, TSize howmany )
	{
		PrepareFileWriting( howmany );

		int channelCount = mNativeFileParams.channels;

		const TData* endData = mInterleavedDataOut.GetPtr()+mInterleavedDataOut.Size();

		const int stride = channelCount + channel;

		for ( TData* data = mInterleavedDataOut.GetPtr();
		      data < endData;
		      data += stride, ptr++ )
			*data = *ptr;

		
/*
		for ( TData* data = mInterleavedDataOut.GetPtr();
		      data != endData;
		      data += channelCount, ptr++ )
			*( data + channel ) = *ptr;
*/
/*
		for ( int i = 0; i < mInterleavedDataOut.Size(); i+=channelCount, ptr++ )
			mInterleavedDataOut[ i + channel ] = *ptr;
*/
		mChannelsProduced[channel] = true;

		if ( AllChannelsProduced() )
			MemoryToDiskTransfer();

	}

	void PCMStreamCodec::WriteData( int* channels, int nchannels,
					TData** const samples, TSize howmany )
	{
		PrepareFileWriting( howmany );
	
		int channelCount = mNativeFileParams.channels;

		const TData* end = mInterleavedDataOut.GetPtr() + mInterleavedDataOut.Size();
		const int* endChannels = channels + nchannels;
		TData** const samplesEnd = samples + nchannels;
		std::vector<bool>::iterator cIt = mChannelsProduced.begin();
		
		for( int* currentChannel = channels;
		     currentChannel != endChannels;
		     currentChannel++ )
		{
			const int channelIndex = *currentChannel;
			// mark channel as consumed
			*(cIt + channelIndex ) = true;
			const int stride = channelCount + channelIndex;
			const TData* pSamples = *(samples+channelIndex);

			for ( TData* i = mInterleavedDataOut.GetPtr();
			      i<end;
			      i+=stride, pSamples++ )
			{
				*i = *pSamples; 
			}
		}

/* Bad memory access pattern
		for ( TData* i = mInterleavedDataOut.GetPtr();
		      i != end;
		      i+=channelCount )
		{
			int* channelIdx = channels;
			for ( TData** data = samples;
			      data != samplesEnd;
			      data++, channelIdx++ )
			{
				int channelIdxVal = *channelIdx;
				*(i + channelIdxVal ) = **data;
				*(cIt + channelIdxVal ) = true;
				(*data)++;
			}
		}
		
*/	
/*
		for ( int k = 0; k < mInterleavedDataOut.Size(); k+=nchannels )
		{
			for ( int i = 0; i < nchannels; i++ )
			{
				mInterleavedDataOut[ k + channels[i] ] = *samples[i];
				samples[i]++;
			}
		}

		for ( int i = 0; i < nchannels; i++ )
			mChannelsProduced[ channels[i] ] = true;
*/
		if ( AllChannelsProduced() )
			MemoryToDiskTransfer();

	}

	inline void PCMStreamCodec::MemoryToDiskTransfer()
	{
		sf_count_t samplesWritten = CLAM_sf_write( mFileHandle,
							   mInterleavedDataOut.GetPtr(),
							   mFramesToWrite );

		CLAM_DEBUG_ASSERT( samplesWritten == mFramesToWrite,
			     "Could not write all samples to disk!" );
	}

}
