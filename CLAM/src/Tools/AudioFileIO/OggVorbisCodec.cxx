#include "AudioFileFormats.hxx"
#include "AudioFile.hxx"
#include "OggVorbisCodec.hxx"
#include "OggVorbisAudioStream.hxx"
#include <cstdio>
#include <vorbis/vorbisfile.h>

namespace CLAM
{

namespace AudioCodecs
{
	OggVorbisCodec::OggVorbisCodec()
	{
	}

	OggVorbisCodec::~OggVorbisCodec()
	{
	}

	OggVorbisCodec& OggVorbisCodec::Instantiate()
	{
		static OggVorbisCodec theInstance;

		return theInstance;
	}

	bool OggVorbisCodec::IsReadable( std::string uri ) const
	{
		FILE*          fileHandle;
		OggVorbis_File vorbisFile;
		
		if ( ( fileHandle = fopen( uri.c_str(), "rb" ) ) == NULL )
		     return false;

		if ( ov_open( fileHandle, &vorbisFile, NULL, 0 ) < 0 )		
		{
			fclose( fileHandle );
			
			return false;
		}

		// MRJ: No need to close the fileHandle since libvorbisfile takes
		// its ownership if the ov_open call is successful
		ov_clear( &vorbisFile );

		return true;

	}

	bool OggVorbisCodec::IsWritable( std::string uri, const AudioFileHeader& hdr ) const
	{
		// MRJ: These values were documented ( sort of ) in
		// the Vorbis I specification document

		if ( (hdr.GetChannels() < 0) || ( hdr.GetChannels() > 255) )
			return false;
		if ( (hdr.GetSampleRate() < 8000.) || ( hdr.GetSampleRate() > 192000.) )
			return false;
		
		return true;	
	}

	Stream* OggVorbisCodec::GetStreamFor( const AudioFile& file )
	{
		return new OggVorbisAudioStream(file);
	}

	void OggVorbisCodec::RetrieveHeaderData( std::string uri, AudioFileHeader& hdr )
	{
		FILE*          fileHandle;
		OggVorbis_File vorbisFile;
		
		if ( ( fileHandle = fopen( uri.c_str(), "rb" ) ) == NULL )
		     return;

		if ( ov_open( fileHandle, &vorbisFile, NULL, 0 ) < 0 )		
		{
			fclose( fileHandle );			
			return;
		}

		vorbis_info* fileInfo = ov_info( &vorbisFile, -1 );
		
		if ( !fileInfo ) // File was encoded improperly
			return;

		hdr.AddSampleRate();
		hdr.AddChannels();
		hdr.AddFormat();
		hdr.AddEncoding();
		hdr.AddEndianess();
		hdr.AddLength();
		hdr.UpdateData();

		hdr.SetSampleRate( (TData)fileInfo->rate );
		hdr.SetChannels( (TSize)fileInfo->channels );
		hdr.SetLength( (TTime)ov_time_total( &vorbisFile, -1) * 1000. );
		hdr.SetFormat( EAudioFileFormat::eVorbisMk1 );
		hdr.SetEncoding( EAudioFileEncoding::eDefault );
		hdr.SetEndianess( EAudioFileEndianess::eDefault );

		// MRJ: No need to close the fileHandle since libvorbisfile takes
		// its ownership if the ov_open call is successful
		ov_clear( &vorbisFile );
	}
}

}
