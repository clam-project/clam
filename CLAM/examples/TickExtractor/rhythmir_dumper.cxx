#include "rhythmir_dumper.hxx"
#include "XMLStorage.hxx"
#include "XmlScoped.hxx"
#include "Array.hxx"
#include <fstream>
#include <iostream>
#include "Pulse.hxx"

using namespace CLAM;

std::ostream& operator<<( std::ostream& os, const DataArray& array )
{
	for ( int i = 0; i < array.Size(); i++ )
		os << array[i] << " ";

	return os;
}

void DumpExtractedData( std::string filename,
			std::string analyzedFile,
			const CLAM::Pulse& ticksData,
			const CLAM::Pulse& beatsData )
{
	std::ofstream fileStream( filename.c_str() );

	DataArray listTicks;
	DataArray listBeats;

	listTicks.Resize( ticksData.GetIndexes().Size() );
	listTicks.SetSize( ticksData.GetIndexes().Size() );
	
	for ( int i = 0; i < listTicks.Size(); i++ )
	{
		listTicks[i] = ticksData.GetIndexes()[i].GetPosition();
	}

	listBeats.Resize( beatsData.GetIndexes().Size() );
	listBeats.SetSize( beatsData.GetIndexes().Size() );
	
	for ( int i = 0; i < listBeats.Size(); i++ )
	{
		listBeats[i] = beatsData.GetIndexes()[i].GetPosition();
	}
	

	XmlFragment frag( fileStream );
	{
		XmlElement element( "Simac:RhythmDescription" );
		{
			XmlElement element( "Simac:MediaDigitalFile" );
			XmlAttribute attrib( "path", analyzedFile );
		}		
		{
			XmlElement element( "Simac:BeatsPerMinute" );
			XmlAttribute attrib( "value", beatsData.GetRate() );
		}
		{
			XmlElement   element( "Simac:BeatSequence" );
			XmlAttribute attrib( "size", beatsData.GetIndexes().Size() );
			XmlAttribute attrib1( "timeunit", "seconds" );
			XmlContent   content( listBeats );
			
		}
		{
			XmlElement element( "Simac:TicksPerMinute" );
			XmlAttribute attrib( "value", ticksData.GetRate() );
		}
		{
			XmlElement   element( "Simac:TicksSequence" );
			XmlAttribute attrib( "size", ticksData.GetIndexes().Size() );
			XmlAttribute attrib1( "timeunit", "seconds" );
			XmlContent   content( listTicks );
			
		}

	}


	fileStream.close();
	
}
