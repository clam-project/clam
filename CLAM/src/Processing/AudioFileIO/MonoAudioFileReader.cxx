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

#include "MonoAudioFileReader.hxx"

namespace CLAM
{
	MonoAudioFileReader::MonoAudioFileReader()
		: mOutput( "Samples read", this, 1 )
	{
	}

	MonoAudioFileReader::MonoAudioFileReader( const ProcessingConfig& cfg )
		: mOutput( "Samples read", this, 1 )
	{
		Configure( cfg );
	}

	const char* MonoAudioFileReader::GetClassName() const
	{
		return "MonoAudioFileReader";
	}

	const ProcessingConfig& MonoAudioFileReader::GetConfig() const
	{
		return mConfig;
	}

	bool MonoAudioFileReader::ConcreteConfigure( const ProcessingConfig& cfgObject )
	{
		CopyAsConcreteConfig( mConfig, cfgObject );

		if ( !mConfig.HasSourceFile() )
		{
			mStatus = "The provided config object lacked the field ";
			mStatus += "'SourceFile'";

			return false;
		}

		// Check that the given file can be opened
		if ( ! mConfig.GetSourceFile().CanRead() )
		{
			mStatus = "The Source file could not be opened";
			return false;
		}

		mConfig.GetSourceFile().RetrieveHeaderInfo();

		if ( mConfig.GetSelectedChannel() < 0
		     || mConfig.GetSelectedChannel() >= mConfig.GetSourceFile().GetChannels() )
		{
			mStatus = "The channel selected for reading does not exist";
			return false;
		}

		mNativeStream.SetFOI( mConfig.GetSourceFile() );
		mNativeStream.DeactivateStrictStreaming();

		return true;
	}

	bool MonoAudioFileReader::ConcreteStart()
	{
		mNativeStream.PrepareReading();
		mCurrentBeginTime = 0.0;
		mEOFReached = false;
		
		return true;
	}

	bool MonoAudioFileReader::ConcreteStop()
	{
		mNativeStream.Dispose();
		
		return true;
	}

	bool MonoAudioFileReader::Do()
	{
		if ( !AbleToExecute() )
			return false;

		if ( mEOFReached )
			return false;

		Audio& outputSamples = mOutput.GetData();

		mEOFReached = mNativeStream.ReadData( mConfig.GetSelectedChannel(),
						      outputSamples.GetBuffer().GetPtr(),
						      outputSamples.GetSize() );

		outputSamples.SetBeginTime( mCurrentBeginTime );
		mDeltaTime = outputSamples.GetSize() / mConfig.GetSourceFile().GetSampleRate();
		mCurrentBeginTime += mDeltaTime;
		outputSamples.SetSampleRate( mConfig.GetSourceFile().GetSampleRate() );

		return true;
	}
	
}
