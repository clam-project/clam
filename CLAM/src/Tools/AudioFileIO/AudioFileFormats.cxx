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

#include "AudioFileFormats.hxx"

namespace CLAM
{
	EAudioFileKind::EAudioFileKind()
		: Enum( sEnumValues, sDefault )
	{
	}

	EAudioFileKind::EAudioFileKind( tValue v )
		: Enum( sEnumValues, v )
	{
	}

	EAudioFileKind::EAudioFileKind( std::string s )
		: Enum( sEnumValues, s )
	{
	}

	Enum::tEnumValue EAudioFileKind::sEnumValues[] = {
		{ ePCM,         "PCM" },
		{ eOggVorbis,   "Ogg/Vorbis" },
		{ eMP3,         "Mpeg-1 Audio Layer 3" },
		{ eUnknown,     "Unknown" },
		{ 0,            NULL }
	};

	Enum::tValue EAudioFileKind::sDefault = EAudioFileKind::ePCM;

	Component* EAudioFileKind::Species() const
	{
		return new EAudioFileKind( "PCM" );
	}


	EAudioFileFormat::EAudioFileFormat()
		: Enum( sEnumValues, sDefault )
	{
	}

	EAudioFileFormat::EAudioFileFormat( tValue v )
		: Enum( sEnumValues, v )
	{
	}

	EAudioFileFormat::EAudioFileFormat( std::string s )
		: Enum( sEnumValues, s )
	{
	}

	Enum::tEnumValue EAudioFileFormat::sEnumValues[] = {
		{ EAudioFileFormat::eWAV,       "WAV" },
		{ EAudioFileFormat::eAIFF,      "AIFF" },
		{ EAudioFileFormat::eAU,        "AU"   },
		{ EAudioFileFormat::eRAW,       "RAW"  },
		{ EAudioFileFormat::ePAF,       "PAF"  },
		{ EAudioFileFormat::eSVX,       "SVX"  },
		{ EAudioFileFormat::eNIST,      "NIST" },
		{ EAudioFileFormat::eVOC,       "VOC"  },
		{ EAudioFileFormat::eIRCAM,     "IRCAM" },
		{ EAudioFileFormat::eW64,       "W64"  },
		{ EAudioFileFormat::eMAT4,      "MAT4" },
		{ EAudioFileFormat::eMAT5,      "MAT5" },
		{ EAudioFileFormat::eVorbisMk1, "VorbisMk1"},
		{ 0,                            NULL }
	};

	Enum::tValue EAudioFileFormat::sDefault = EAudioFileFormat::eWAV;

	Component* EAudioFileFormat::Species() const
	{
		return new EAudioFileFormat( "WAV" );
	}

	EAudioFileEncoding::EAudioFileEncoding()
		: Enum( sEnumValues, sDefault )
	{
	}

	EAudioFileEncoding::EAudioFileEncoding( tValue val )
		: Enum( sEnumValues, val )
	{
	}

	EAudioFileEncoding::EAudioFileEncoding( std::string s )
		: Enum( sEnumValues, s )
	{
	}

	Enum::tEnumValue EAudioFileEncoding::sEnumValues[] = {
		{ ePCM_S8, "signed 8-bit" },
		{ ePCM_16, "signed 16-bit" },
		{ ePCM_24, "signed 24-bit" },
		{ ePCM_32, "signed 32-bit" },
		{ ePCM_U8, "unsigned 8-bit" },
		{ eFLOAT, "float 32-bit"},
		{ eDOUBLE, "float 64-bit"},
		{ eU_LAW, "u-law"},
		{ eA_LAW, "a-law"},
		{ eIMA_ADPCM, "IMA ADPCM" },
		{ eMS_ADPCM, "MS ADPCM "},
		{ eGSM610, "GSM 6.10"},
		{ eVOX_ADPCM, "Oki ADPCM"},
		{ eG721_32, "32k G721 ADPCM"},
		{ eG723_24, "24k G723 ADPCM"},
		{ eG723_40, "40k G723 ADPCM"},
		{ eDWVW_12, "DWVW 12-bit"},
		{ eDWVW_16, "DWVW 16-bit"},
		{ eDWVW_24, "DWVW 24-bit"},
		{ eDWVW_N,  "DWVW N-bit"},
		{ eDefault, "Format Default"},
		{ 0, NULL }
	};

	Enum::tValue EAudioFileEncoding::sDefault = EAudioFileEncoding::ePCM_16;

	Component* EAudioFileEncoding::Species() const
	{
		return new EAudioFileEncoding("signed 16-bit");
	}

	EAudioFileEndianess::EAudioFileEndianess()
		: Enum( sEnumValues, sDefault )
	{
	}

	EAudioFileEndianess::EAudioFileEndianess( tValue val )
		: Enum( sEnumValues, val )
	{
	}

	EAudioFileEndianess::EAudioFileEndianess( std::string s )
		: Enum( sEnumValues, s )
	{
	}

	Enum::tEnumValue EAudioFileEndianess::sEnumValues[] = {
		{ eDefault, "Format Default" },
		{ eLittle,  "Little Endian"},
		{ eBig,     "Big Endian"},
		{ eHost,    "Host CPU Endianess"},
		{ 0, NULL }
	};

	Enum::tValue EAudioFileEndianess::sDefault = EAudioFileEndianess::eDefault;

	Component* EAudioFileEndianess::Species() const
	{
		return new EAudioFileEndianess("Default");
	}

}
