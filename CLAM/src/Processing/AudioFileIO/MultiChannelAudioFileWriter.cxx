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

#include "MultiChannelAudioFileWriter.hxx"
#include <sstream>

namespace CLAM
{
	MultiChannelAudioFileWriter::MultiChannelAudioFileWriter()
	{
	}

	MultiChannelAudioFileWriter::MultiChannelAudioFileWriter( const ProcessingConfig& cfg )
	{
		Configure( cfg );
	}

	MultiChannelAudioFileWriter::~MultiChannelAudioFileWriter()
	{
		DestroyOldInputs();
	}

	const char* MultiChannelAudioFileWriter::GetClassName() const
	{
		return "MultiChannelAudioFileWriter";
	}

	const ProcessingConfig& MultiChannelAudioFileWriter::GetConfig() const
	{
		return mConfig;
	}

	bool MultiChannelAudioFileWriter::Do()
	{
		if ( !AbleToExecute() )
			return false;

		// Checking that all inputs have the same size
		bool  allInputsSameSize = true;
		TSize inputsSize = 0;

		VectorOfInputs::iterator i = mInputs.begin();

		inputsSize = (*i)->GetData().GetSize();

		while( i != mInputs.end() && allInputsSameSize )
			allInputsSameSize = ( inputsSize == (*i++)->GetData().GetSize() );

		CLAM_ASSERT( allInputsSameSize, "Input sizes differ!" );

		// Now, let's build the samples matrix

		i = mInputs.begin();
		int j = 0;

		while( i != mInputs.end() )
			mSamplesMatrix[ j++ ] = (*i++)->GetData().GetBuffer().GetPtr();

		mNativeStream.WriteData( mChannelsToWrite.GetPtr(), mChannelsToWrite.Size(),
					 mSamplesMatrix.GetPtr(), inputsSize );

		return true;
	}

	bool MultiChannelAudioFileWriter::ConcreteConfigure( const ProcessingConfig& cfg )
	{
		CopyAsConcreteConfig( mConfig, cfg );

		AudioFile& targetFile = mConfig.GetTargetFile();

		if ( !targetFile.HasSampleRate() )
		{
			mStatus = "Target file sample rate was not specified!";
			return false;
		}
		if ( !targetFile.HasChannels() )
		{
			mStatus = "Number of channels in target file was not specified!";
			return false;
		}
		if ( !targetFile.HasFormat() )
		{
			mStatus = "Target file format was not specified!";
			return false;
		}
		if ( !targetFile.HasEncoding() )
		{
			mStatus = "Target file sample encoding scheme not specified!";
			return false;
		}
		if ( !targetFile.HasEndianess() )
		{
			targetFile.AddEndianess();
			targetFile.UpdateData();
			targetFile.SetEndianess( EAudioFileEndianess::eHost );
		}

		if ( targetFile.GetChannels() < 2 )
		{
			mStatus = "Too few channels. This processing is meant for handling ";
			mStatus+= "files with two or more channels.";
			return false;
		}

		if ( !targetFile.AreValidEncodingSettings() )
		{
			mStatus = "Settings were not supported by selected output format. ";
			mStatus += "Check that the sample rate, endianess and number of ";
			mStatus += "channels conform the format specification.";

			return false;
		}

		if ( !mInputs.empty() )
			DestroyOldInputs();

		mChannelsToWrite.Resize( targetFile.GetChannels() );
		mChannelsToWrite.SetSize( targetFile.GetChannels() );
		mSamplesMatrix.Resize( targetFile.GetChannels() );
		mSamplesMatrix.SetSize( targetFile.GetChannels() );

		for ( int i = 0; i < targetFile.GetChannels(); i++ )
		{
			mChannelsToWrite[ i ] = i;
			mSamplesMatrix[ i ] = NULL;
			std::stringstream sstr;
			sstr << i;
			
			mInputs.push_back( 
				new InPortTmpl<Audio>( "Channel #" + sstr.str(), this, 1 ) );
		}

		mNativeStream.SetFOI( mConfig.GetTargetFile() );

		return true;
	}

	bool MultiChannelAudioFileWriter::ConcreteStart()
	{
		mNativeStream.PrepareWriting();

		return true;
	}

	bool MultiChannelAudioFileWriter::ConcreteStop()
	{
		mNativeStream.Dispose();

		return true;
	}

	void MultiChannelAudioFileWriter::DestroyOldInputs()
	{
		for ( VectorOfInputs::iterator i = mInputs.begin();
		      i != mInputs.end(); i++ )
		{
			if ( *i ) delete *i;
		}

		mInputs.clear();
	}
}
