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

#ifndef __SoundFileIOError__
#define __SoundFileIOError__

#include "ErrProcessingObj.hxx"

namespace CLAM {

	class ErrSoundFileIO : public ErrProcessingObj
	{
	public:
		ErrSoundFileIO() noexcept;
		ErrSoundFileIO( const char* msg ) noexcept;
		ErrSoundFileIO( const ErrSoundFileIO& ) noexcept;

		virtual ~ErrSoundFileIO() noexcept;
	};

	class UnavailableSoundFile : public ErrSoundFileIO
	{
	public:
		UnavailableSoundFile() noexcept;
		UnavailableSoundFile( const char* msg ) noexcept;
		UnavailableSoundFile( const UnavailableSoundFile& ) noexcept;

		virtual ~UnavailableSoundFile() noexcept;
		
	};

	class UnsupportedSoundFileSampleEncoding : public ErrSoundFileIO
	{
	public:
		UnsupportedSoundFileSampleEncoding() noexcept;
		UnsupportedSoundFileSampleEncoding( const char* msg ) noexcept;
		UnsupportedSoundFileSampleEncoding( const UnsupportedSoundFileSampleEncoding& ) noexcept;

		virtual ~UnsupportedSoundFileSampleEncoding() noexcept;
		
	};
	
	class UnsupportedSoundFileFormat : public ErrSoundFileIO
	{
	public:
		UnsupportedSoundFileFormat() noexcept;
		UnsupportedSoundFileFormat( const char* msg ) noexcept;
		UnsupportedSoundFileFormat( const UnsupportedSoundFileFormat& ) noexcept;

		virtual ~UnsupportedSoundFileFormat() noexcept;

	};

};//CLAM

#endif

