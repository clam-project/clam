#include "WritingRegion.hxx"
#include "ReadingRegion.hxx"

#include "MiniCppUnit.hxx"
#include <sstream>
#include <fstream>
#include <list>
#include <cstdlib>
#include <ctime>

template <template <class> class DataStructure>
class TestsRandomStream : public GrupDeTests< TestsRandomStream<DataStructure> >
{
	typedef WritingRegion<char, DataStructure> WRegion;
	typedef typename WritingRegion<char, DataStructure>::ProperReadingRegion RRegion;

	WRegion _writer;
	RRegion _outputReader;
	std::list< RRegion * > _otherReaders;
	std::ifstream _inputFile;
	std::string _inputString;
	std::ostringstream _output;
	std::ostringstream _buff;

public:
	// GRUP_DE_TESTS( TestsStream<DataStructure> )
	TestsRandomStream() :
		GrupDeTests< TestsRandomStream<DataStructure> >( "TestsRandomStream<DataStructure>" ),
		_inputFile( "testFile.txt" ),
		_inputString( (std::istreambuf_iterator<char>(_inputFile)), std::istreambuf_iterator<char>() )
	{
		CAS_DE_TEST( test );

		_inputFile.seekg( std::ios::beg );
		std::srand(std::time(0));
	}
		
	~TestsRandomStream()
	{
		typename std::list< RRegion* >::iterator it;
		for(it=_otherReaders.begin(); it!=_otherReaders.end(); it++)
			delete *it;
	}

private:
	bool fillWriterFromInputFile()
	{
		int inchar;
		for (int i=0; i<_writer.size(); i++)
		{
			if (i<_writer.hop())
			{
				inchar = _inputFile.get();
				if (inchar<0)
				{
					_writer[i] ='\0';
					return false;
				}
				_writer[i] = inchar;
			}
			else
				_writer[i] = 'X';
		}
		return true;
	}
	void dumpOutputReader()
	{
		for (int i=0; i<_outputReader.size(); i++)
		{
			char actualToken = _outputReader[i];
			if (actualToken =='\n')
			{
				_output << _buff.str() << '\n';
				_buff.str("");
			}
			else
				_buff << actualToken;
		}
	}

	bool chanceOf(double chance)
	{
		double rand = random() / float(RAND_MAX);
		return rand < chance;
	}

	int randomIntFromTo(int from, int to)
	{
		int dist = to-from;
		int result = from + int((random() / double(RAND_MAX)) * dist);

		assert(result>=from && result <=to);
		return result;
	}


	void maybeChangeSizeWriter()
	{
		if (chanceOf(1-0.3))
			return;

		int newSize = randomIntFromTo(1, 40);
		_writer.size(newSize);
		_writer.hop(newSize);
	}
	
	void maybeChangeHopWriter()
	{
		if (chanceOf(1-0.3))
			return;

		int newSize = randomIntFromTo(1, _writer.size());
		_writer.hop(newSize);
	}

	void maybeChangeHopAndSizeOutputReader()
	{	
		if (chanceOf(1-0.3))
			return;

		int newSize = randomIntFromTo(1, 40);
		_outputReader.size(newSize);
		_outputReader.hop(newSize);
	}

	void maybeCreateOtherReader()
	{
		if(chanceOf(1-0.3))
			return;

		RRegion * newRegion = new RRegion; 
		_writer.linkRegions( *newRegion );
		_otherReaders.push_back( newRegion );
	}

	void maybeRemoveOtherReader()
	{	
		typename std::list< RRegion * >::iterator it;
		for(it=_otherReaders.begin(); it!=_otherReaders.end(); it++)
		{
			if(chanceOf(0.1))
			{
				RRegion * toDelete = *it;
				_writer.removeRegion( *toDelete );
				_otherReaders.remove( toDelete );
				delete toDelete;
				return;
			}
		}
	}
	
	void maybeModifyHopAndSizeOfOtherReaders()
	{	
		typename std::list< RRegion * >::iterator it;
		for(it=_otherReaders.begin(); it!=_otherReaders.end(); it++)
		{
			if(chanceOf(0.2))
			{
				RRegion * otherRegion = *it;
				int newSize = randomIntFromTo(1, 40);
				otherRegion->size( newSize );
				otherRegion->hop( newSize );

			}		
			if(chanceOf(0.2))
			{
				RRegion * otherRegion = *it;
				int newHop = randomIntFromTo(1, otherRegion->size() );
				otherRegion->hop( newHop );
			}

		}
	}

	void maybeChangeOtherReaders()
	{
		maybeCreateOtherReader();
		maybeRemoveOtherReader();
		maybeModifyHopAndSizeOfOtherReaders();
	}

	void OtherReadersConsume()
	{
		typename std::list< RRegion * >::iterator it;
		for(it=_otherReaders.begin(); it!=_otherReaders.end(); it++)
			if( (*it)->canConsume() )
				(*it)->consume();
	}

	void test()
	{
		_writer.size(2);
		_writer.hop(2);
		_writer.linkRegions(_outputReader);
		bool endOfFile = false;

		while(true)
		{	
			if ( _writer.canProduce() && !endOfFile )
			{
				endOfFile =  !fillWriterFromInputFile();
				_writer.produce();
			}
								
			maybeChangeSizeWriter();
			maybeChangeHopWriter();
		//	maybeChangeHopAndSizeOutputReader();
			maybeChangeOtherReaders();

			OtherReadersConsume();
			
			if( _outputReader.canConsume() )
			{
				dumpOutputReader();
				_outputReader.consume();
			}
			else
				if( endOfFile ) break;
		}

		ASSERT_IGUALS( _inputString, _output.str() );
	}
};




