/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "AudioFile.hxx"
#include "Assert.hxx"
#include "PCMCodec.hxx"
#include "OggVorbisCodec.hxx"

namespace CLAM
{
	AudioFile::AudioFile()
		: mLocation( "" ), mKind( EAudioFileKind::eUnknown ), mActiveCodec( NULL )
	{
	}

	AudioFile::AudioFile( const AudioFile& obj )
	{
		mLocation = obj.mLocation;
		mActiveCodec = obj.mActiveCodec;
		mKind = obj.mKind;
		mHeaderData = obj.mHeaderData;
	}

	const AudioFile& AudioFile::operator=( const AudioFile& obj )
	{
		mLocation = obj.mLocation;
		mActiveCodec = obj.mActiveCodec;
		mKind = obj.mKind;
		mHeaderData = obj.mHeaderData;

		return *this;
	}

	AudioFile::~AudioFile()
	{
	}

	void AudioFile::SetLocation( std::string uri )
	{
		mLocation = uri;
		
		if ( AudioCodecs::Codec::FileExists( uri ) )
		{		
			if ( AudioCodecs::PCMCodec::Instantiate().IsReadable( uri ) )
				SetKind( EAudioFileKind::ePCM );
			else if ( AudioCodecs::OggVorbisCodec::Instantiate().IsReadable( uri ) )
				SetKind( EAudioFileKind::eOggVorbis );
			else
			{
				SetKind( EAudioFileKind::eUnknown );
				return;
			}
			
			mActiveCodec->RetrieveHeaderData( mLocation, GetHeader() );
		}
	}

	AudioFileHeader& AudioFile::GetHeader()
	{
		return mHeaderData;
	}

	const AudioFileHeader& AudioFile::GetHeader() const
	{
		return mHeaderData;
	}

	const std::string& AudioFile::GetLocation() const
	{
		return mLocation;
	}

	void AudioFile::SetKind( EAudioFileKind kind )
	{
		if ( kind == EAudioFileKind::ePCM )
		{
			mKind = kind;
			mActiveCodec = &AudioCodecs::PCMCodec::Instantiate();
		}
		else if ( kind == EAudioFileKind::eOggVorbis )
		{
			mKind = kind;
			mActiveCodec = &AudioCodecs::OggVorbisCodec::Instantiate();
		}
		else
		{
			mKind = EAudioFileKind::eUnknown;
			mActiveCodec = NULL;
		}
	}

	EAudioFileKind AudioFile::GetKind() const
	{
		return mKind;
	}

	bool AudioFile::IsValid() const
	{
		return mActiveCodec == NULL;
	}

	bool AudioFile::IsReadable() const
	{
		if (mActiveCodec == NULL )
			return false;
		
		return mActiveCodec->IsReadable(mLocation);
	}

	bool AudioFile::IsWritable() const
	{
		if ( mActiveCodec == NULL )
			return false;

		return mActiveCodec->IsWritable(mLocation, GetHeader() );
	}

	AudioCodecs::Stream*  AudioFile::GetStream()
	{
		if ( !mActiveCodec )
			return NULL;
		
		return mActiveCodec->GetStreamFor( *this );
	}

	const char* AudioFile::GetClassName() const
	{
		return "AudioFile";
	}

	void AudioFile::LoadFrom( Storage& storage )
	{
	}

	void AudioFile::StoreOn( Storage& storage )
	{
	}

}
