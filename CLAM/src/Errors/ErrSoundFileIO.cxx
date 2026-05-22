/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "ErrSoundFileIO.hxx"

namespace CLAM
{
	ErrSoundFileIO::ErrSoundFileIO() noexcept
		: ErrProcessingObj( )
	{
	}

	ErrSoundFileIO::ErrSoundFileIO( const char* msg ) noexcept
		: ErrProcessingObj( msg )
	{
	}

	ErrSoundFileIO::ErrSoundFileIO( const ErrSoundFileIO& obj ) noexcept
		: ErrProcessingObj( obj )
	{
	}

	ErrSoundFileIO::~ErrSoundFileIO() noexcept
	{
	}

	UnavailableSoundFile::UnavailableSoundFile() noexcept
		: ErrSoundFileIO()
	{
	}

	UnavailableSoundFile::UnavailableSoundFile( const char* msg ) noexcept
		: ErrSoundFileIO( msg )
	{
	}

	UnavailableSoundFile::UnavailableSoundFile( const UnavailableSoundFile& obj ) noexcept
		: ErrSoundFileIO( obj )
	{
	}
	
	UnavailableSoundFile::~UnavailableSoundFile() noexcept
	{
	}

	UnsupportedSoundFileSampleEncoding::UnsupportedSoundFileSampleEncoding() noexcept
		: ErrSoundFileIO()
	{
	}

	UnsupportedSoundFileSampleEncoding::UnsupportedSoundFileSampleEncoding( const char* msg ) noexcept
		: ErrSoundFileIO( msg )
	{
	}

	UnsupportedSoundFileSampleEncoding::UnsupportedSoundFileSampleEncoding( const UnsupportedSoundFileSampleEncoding& obj ) noexcept
		: ErrSoundFileIO( obj )
	{
	}
	
	UnsupportedSoundFileSampleEncoding::~UnsupportedSoundFileSampleEncoding() noexcept
	{
	}


	UnsupportedSoundFileFormat::UnsupportedSoundFileFormat() noexcept
		: ErrSoundFileIO()
	{
	}

	UnsupportedSoundFileFormat::UnsupportedSoundFileFormat( const char* msg ) noexcept
		: ErrSoundFileIO( msg )
	{
	}

	UnsupportedSoundFileFormat::UnsupportedSoundFileFormat( const UnsupportedSoundFileFormat& obj ) noexcept
		: ErrSoundFileIO( obj )
	{
	}
	
	UnsupportedSoundFileFormat::~UnsupportedSoundFileFormat() noexcept
	{
	}

		
}

