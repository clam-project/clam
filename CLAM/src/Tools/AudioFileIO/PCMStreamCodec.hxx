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

#ifndef __PCMCODEC__
#define __PCMCODEC__

#include "AudioFile.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include <sndfile.h>
#include <vector>

namespace CLAM
{
	class PCMStreamCodec
	{
	public:
		PCMStreamCodec();
		PCMStreamCodec( const AudioFile& file );

		~PCMStreamCodec();

		void SetFOI( const AudioFile& file );

		void PrepareReading();
		void PrepareWriting();
		void PrepareReadWrite();
		void Dispose();

		void DeactivateStrictStreaming();
		void ActivateStrictStreaming();
		bool StrictStreaming() const;


		bool ReadData( int channel, TData* ptr, TSize howmany );
		bool ReadData( int* channels, int nchannels, 
			       TData** samples, TSize howmany );

		void WriteData( int channel, const TData* ptr, TSize howmany );
		void WriteData( int* channels, int nchannels,
				TData** const samples, TSize howmany );

	protected:
		
		void         AudioFileToNative();
	private:
		void CheckForFileReading( TSize samplesToRead );
		void PrepareFileWriting( TSize samplesToWrite );

		void DiskToMemoryTransfer();
		void MemoryToDiskTransfer();

		bool AllChannelsConsumed();
		void ResetConsumedChannels();
		void MarkAllChannelsAsConsumed();

		bool AllChannelsProduced();
		void ResetProducedChannels();
		void MarkAllChannelsAsProduced();
		
		static bool  HandleReAllocation( DataArray& buffer, TSize newSize );

	protected:
		AudioFile           mFOI; // FOI = File Of Interest
		SNDFILE*            mFileHandle;
		SF_INFO             mNativeFileParams;
		DataArray           mInterleavedData;
		DataArray           mInterleavedDataOut;
		std::vector<bool>   mChannelsConsumed;
		std::vector<bool>   mChannelsProduced;
		bool                mStrictStreaming;
		bool                mEOFReached;
		TSize               mFramesToRead;
		TSize               mFramesToWrite;
	};

	// inline methods
	inline void PCMStreamCodec::ActivateStrictStreaming()
	{
		mStrictStreaming = true;
	}

	inline void PCMStreamCodec::DeactivateStrictStreaming()
	{
		mStrictStreaming = false;
	}

	inline bool PCMStreamCodec::StrictStreaming() const
	{
		return mStrictStreaming;
	}
}

#endif // PCMStreamCodec.hxx
