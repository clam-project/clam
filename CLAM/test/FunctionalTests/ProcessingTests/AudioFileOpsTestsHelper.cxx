#include "AudioFileOpsTestsHelper.hxx"
#include "similarityHelper.hxx"
#include "PCMStreamCodec.hxx"

namespace CLAMTest
{
	double fileSimilarity( const CLAM::AudioFile& file, int channel,
			       const CLAM::Audio& rhs )
	{
		CLAM::PCMStreamCodec stream;
		CLAM::DataArray samplesExtracted;
		samplesExtracted.Resize( rhs.GetSize() );
		samplesExtracted.SetSize( rhs.GetSize() );
		

		stream.SetFOI( file );
		
		stream.PrepareReading();

		stream.ReadData( channel, 
				 samplesExtracted.GetPtr(), 
				 samplesExtracted.Size() );


		stream.Dispose();

		return evaluateSimilarity( samplesExtracted,
					   rhs.GetBuffer() );
	}

	void fileSimilarity( const CLAM::AudioFile& file,
			     const CLAM::Audio& left, const CLAM::Audio& right,
			     double& similarityLeft, double& similarityRight )
	{
		CLAM::PCMStreamCodec stream;

		CLAM::DataArray samplesExtractedLeft;
		samplesExtractedLeft.Resize( left.GetSize() );
		samplesExtractedLeft.SetSize( left.GetSize() );

		CLAM::DataArray samplesExtractedRight;
		samplesExtractedRight.Resize( right.GetSize() );
		samplesExtractedRight.SetSize( right.GetSize() );

		stream.SetFOI( file );
		
		stream.PrepareReading();

		int* channelBuff = new int[2];
		channelBuff[0] = 0;
		channelBuff[1] = 1;
		
		CLAM::TData** samplesMatrix = new CLAM::TData*[2];
		samplesMatrix[0] = samplesExtractedLeft.GetPtr();
		samplesMatrix[1] = samplesExtractedRight.GetPtr();

		stream.ReadData( channelBuff, 2,
				 samplesMatrix, left.GetSize() );

		stream.Dispose();

		similarityLeft = evaluateSimilarity( samplesExtractedLeft, left.GetBuffer() );
		similarityRight = evaluateSimilarity( samplesExtractedRight, right.GetBuffer() );

		delete channelBuff;
		delete samplesMatrix;
		
	}

}
