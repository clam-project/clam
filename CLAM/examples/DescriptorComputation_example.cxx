
#include "Segment.hxx"
#include "XMLStorage.hxx"
#include "SegmentDescriptors.hxx"
#include "DescriptorComputation.hxx"

using namespace CLAM;


int main()
{
		XMLStorage s;
		Segment segment;
		std::cout<<"Please enter name of your xml analysis file"<<"\n";
		std::string filename;
		std::cin>>filename;
		std::cout<<"Loading Analysis File. Please Wait."<<"\n";
		s.Restore(segment,filename);
		std::cout<<"Analysis File Loaded Successfully"<<"\n";

		std::cout<<"Computing Descriptors. Please Wait."<<"\n";
		
		
		
		SegmentDescriptors segmentDescriptors;

		SpectralDescriptors specProto;
		specProto.AddMean();
		specProto.AddCentroid();
		specProto.AddEnergy();
		specProto.UpdateData();

		SpectralPeakDescriptors peakProto;
		peakProto.AddMagnitudeMean();
		peakProto.AddHarmonicCentroid();
		peakProto.UpdateData();

		FrameDescriptors frameProto;
		frameProto.AddSpectralPeakD();
		frameProto.AddResidualSpecD();
		frameProto.UpdateData();
		
		frameProto.GetSpectralPeakD().SetPrototype(peakProto);
		frameProto.GetResidualSpecD().SetPrototype(specProto);
		
		segmentDescriptors.SetFramePrototype(frameProto,segment.GetnFrames());
		segmentDescriptors.SetpSegment(&segment);
		
		//we can directly call the compute operation on the descriptor
		//segmentDescriptors.Compute();
		
		//or use an intermediate DescriptorComputation processing
		DescriptorComputation processing;
		processing.Do(segmentDescriptors);
		
		std::cout<<"Descriptors Computed Successfully"<<"\n";
		
		std::cout<<"Please enter name of where you want your output descriptors to be stored"<<"\n";
		std::string outFilename;
		std::cin>>outFilename;

		std::cout<<"Storing Results into xml file. Please Wait."<<"\n";
		s.Dump(segmentDescriptors,"segmentDescriptors",outFilename);
		
		std::cout<<"Program finished Successfully"<<"\n";
		return 0;
}

  
