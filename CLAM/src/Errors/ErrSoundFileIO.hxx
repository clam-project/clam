#ifndef __SoundFileIOError__
#define __SoundFileIOError__

#include "Err.hxx"

namespace CLAM {

	class ErrSoundFileIO : public Err
	{
	public:
		ErrSoundFileIO() throw();
		ErrSoundFileIO( const char* msg ) throw();
		ErrSoundFileIO( const ErrSoundFileIO& ) throw();

		virtual ~ErrSoundFileIO() throw();
	};

	class UnavailableSoundFile : public ErrSoundFileIO
	{
	public:
		UnavailableSoundFile() throw();
		UnavailableSoundFile( const char* msg ) throw();
		UnavailableSoundFile( const UnavailableSoundFile& ) throw();

		virtual ~UnavailableSoundFile() throw();
		
	};

	class UnsupportedSoundFileSampleEncoding : public ErrSoundFileIO
	{
	public:
		UnsupportedSoundFileSampleEncoding() throw();
		UnsupportedSoundFileSampleEncoding( const char* msg ) throw();
		UnsupportedSoundFileSampleEncoding( const UnsupportedSoundFileSampleEncoding& ) throw();

		virtual ~UnsupportedSoundFileSampleEncoding() throw();
		
	};
	
	class UnsupportedSoundFileFormat : public ErrSoundFileIO
	{
	public:
		UnsupportedSoundFileFormat() throw();
		UnsupportedSoundFileFormat( const char* msg ) throw();
		UnsupportedSoundFileFormat( const UnsupportedSoundFileFormat& ) throw();

		virtual ~UnsupportedSoundFileFormat() throw();

	};

};//CLAM

#endif
