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

#include "MultiChannelAudioFileReader.hxx"
#include <sstream>

namespace CLAM
{
	MultiChannelAudioFileReader::MultiChannelAudioFileReader()
	{
	}

	MultiChannelAudioFileReader::MultiChannelAudioFileReader( const ProcessingConfig& cfg )
	{
		Configure( cfg );
	}

	MultiChannelAudioFileReader::~MultiChannelAudioFileReader()
	{
		DestroyOldOutputs();
	}

	const char* MultiChannelAudioFileReader::GetClassName() const
	{
		return "MultiChannelAudioFileReader";
	}

	const ProcessingConfig& MultiChannelAudioFileReader::GetConfig() const
	{
		return mConfig;
	}

	bool MultiChannelAudioFileReader::Do()
	{
		if ( !AbleToExecute() )
			return false;

		if ( mEOFReached )
			return false;
		
		// Check all outputs sizes
		bool allOutputsSameSize = true;
		
		TSize sizeTmp = 0;

		OutputVector::iterator i = mOutputs.begin();

		sizeTmp = (*i)->GetData().GetSize();	
		
		while ( i != mOutputs.end() && allOutputsSameSize )
		{
			allOutputsSameSize = ( sizeTmp == (*i++)->GetData().GetSize() );
		}

		CLAM_ASSERT( allOutputsSameSize, "Outputs sizes differ!" );

		// build the samples matrix

		i = mOutputs.begin();
		int j = 0;
		while ( i != mOutputs.end() )
			mSamplesMatrix[ j++ ] = (*i++)->GetData().GetBuffer().GetPtr();

		// read the data
		
		mEOFReached = mNativeStream.ReadData( mConfig.GetSelectedChannels().GetPtr(),
							  mConfig.GetSelectedChannels().Size(),
							  mSamplesMatrix.GetPtr(),
							  sizeTmp );

		// Audio 'simple meta-data' setup
		
		i = mOutputs.begin();

		while( i != mOutputs.end() )
		{
			(*i)->GetData().SetSampleRate( mConfig.GetSourceFile().GetSampleRate() );
			(*i)->GetData().SetBeginTime( mCurrentBeginTime );
			i++;
		}

		mDeltaTime = TData(sizeTmp) / mConfig.GetSourceFile().GetSampleRate();
		mCurrentBeginTime += mDeltaTime;

		return true;
	}

	bool MultiChannelAudioFileReader::ConcreteConfigure( const ProcessingConfig& cfgObj )
	{
		CopyAsConcreteConfig( mConfig, cfgObj );

		if ( !mConfig.HasSourceFile() )			
		{
			mStatus = "No 'source file' was specified in the configuration!";

			return false;
		}

		if ( !mConfig.GetSourceFile().CanRead() )
		{
			mStatus = "The source file could not be opened!";

			return false;
		}

		mConfig.GetSourceFile().RetrieveHeaderInfo();

		if ( !mConfig.HasSelectedChannels() )
		{
			mConfig.AddSelectedChannels();
			mConfig.UpdateData();

			Array< TIndex >& channelsToRead  = mConfig.GetSelectedChannels();

			channelsToRead.Resize( mConfig.GetSourceFile().GetChannels() );
			channelsToRead.SetSize( mConfig.GetSourceFile().GetChannels() );

			DestroyOldOutputs();

			for ( int i = 0; i < channelsToRead.Size(); i++ )
			{
				std::stringstream sstr;
				sstr << i;

				mOutputs.push_back(
					new OutPortTmpl<Audio>( "Channel #" + sstr.str() , this, 1  )
					);

				channelsToRead[ i ] = i;
			}

			mSamplesMatrix.Resize( channelsToRead.Size() );
			mSamplesMatrix.SetSize( channelsToRead.Size() );

		}
		else
		{
			// Checking selected channels validity
			Array< TIndex >& channelsToRead =
				mConfig.GetSelectedChannels();

			if ( channelsToRead.Size() != mConfig.GetSourceFile().GetChannels() )
			{
				mStatus = "There are not so many channels in the source file ";
				mStatus += "check configuration";

				return false;
			}

			int maxChannels = mConfig.GetSourceFile().GetChannels();

			for ( int i = 0; i < channelsToRead.Size(); i++ )
				if ( channelsToRead[i] < 0
				     || channelsToRead[i] >= maxChannels )
				{
					mStatus = "Invalid channel index in configuration!";
					return false;
				}
				     

			DestroyOldOutputs();

			for ( int i = 0; i < channelsToRead.Size(); i++ )
			{
				std::stringstream sstr;
				sstr << channelsToRead[i];

				mOutputs.push_back(
					new OutPortTmpl<Audio>( "Channel #" + sstr.str(), this, 1 )
					);
			}

			mSamplesMatrix.Resize( maxChannels );
			mSamplesMatrix.SetSize( maxChannels );
		}

		mNativeStream.SetFOI( mConfig.GetSourceFile() );

		return true;
	}

	void MultiChannelAudioFileReader::DestroyOldOutputs()
	{
		for( OutputVector::iterator i = mOutputs.begin();
		     i != mOutputs.end(); i++ )
			if( *i ) delete *i;

		mOutputs.clear();
	}

	bool MultiChannelAudioFileReader::ConcreteStart()
	{
		mNativeStream.PrepareReading();
		mCurrentBeginTime = 0.0;
		mEOFReached = false;

		return true;
	}

	bool MultiChannelAudioFileReader::ConcreteStop()
	{
		mNativeStream.Dispose();

		return true;
	}
}
