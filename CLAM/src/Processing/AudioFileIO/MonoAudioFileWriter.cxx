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

#include "MonoAudioFileWriter.hxx"

namespace CLAM
{
	MonoAudioFileWriter::MonoAudioFileWriter()
		: mInput( "Samples to write", this, 1 )
	{
	}

	MonoAudioFileWriter::MonoAudioFileWriter( const ProcessingConfig& cfg )
		: mInput( "Samples to write", this, 1 )
	{
		Configure( cfg );
	}

	const char* MonoAudioFileWriter::GetClassName() const
	{
		return "MonoAudioFileWriter";
	}
	
	const ProcessingConfig& MonoAudioFileWriter::GetConfig() const
	{
		return mConfig;
	}

	bool MonoAudioFileWriter::Do()
	{
		Audio& data = mInput.GetData();

		mOutStream.WriteData( 0, data.GetBuffer().GetPtr(), data.GetSize() );

		return true;
	}

	bool MonoAudioFileWriter::ConcreteConfigure( const ProcessingConfig& cfg )
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
		
		if ( targetFile.GetChannels() != 1 ) // this is the 'mono' file writer...
		{
			mStatus = "Too many channels!";
			return false;
		}

		if ( !targetFile.AreValidEncodingSettings() )
		{
			mStatus = "There is an incompatibility between the ";
			mStatus += "'Format', 'Encoding' and 'Endianess'  ";
			mStatus += "configuration parameter values";
			
			return false;
		}

		mOutStream.SetFOI( mConfig.GetTargetFile() );

		return true;
	}

	bool MonoAudioFileWriter::ConcreteStart()
	{
		mOutStream.PrepareWriting();
		
		return true;
	}

	bool MonoAudioFileWriter::ConcreteStop()
	{
		mOutStream.Dispose();

		return true;
	}
}
