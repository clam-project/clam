#include "SDIFOut.hxx"
#include "Spectrum.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "SpectralPeakArray.hxx"
#include "Fundamental.hxx"
#include "SDIFFile.hxx"
#include "SDIFFrame.hxx"
#include "SDIFMatrix.hxx"

using namespace CLAM;


void SDIFOutConfig::DefaultInit()
{
	AddAll();
	UpdateData();

	SetEnableResidual(true);
	SetEnablePeakArray(true);
	SetEnableFundFreq(true);
	SetSpectralRange(22050);
	SetMaxNumPeaks(100);
	SetFileName("nofile");

	SetSamplingRate(44100);
	SetFrameSize(1024);
	SetSpectrumSize(513);
}

SDIFOut::SDIFOut():
mPrevIndexArray(0)
{ 
	mpFile=NULL;
	Configure(SDIFOutConfig());
}

SDIFOut::SDIFOut(const SDIFOutConfig& c):
mPrevIndexArray(0)
{ 
	mpFile=NULL;

	Configure(c);
}
	
SDIFOut::~SDIFOut()
{
	mpFile->Close();
	delete mpFile;
}

bool SDIFOut::ConcreteConfigure(const ProcessingConfig& c)
{
	mConfig = dynamic_cast< const SDIFOutConfig& > ( c );
	if(mpFile) delete mpFile;
	mpFile = new SDIF::File(mConfig.GetFileName().c_str(),DataFileIO::eOutput);
	mpFile->Open();
	return true;
}


const ProcessingConfig& SDIFOut::GetConfig() const
{
	return mConfig;
}

bool SDIFOut::Do(const Frame& frame)
{
	if(!mpFile) return false;
	
//If enabled, first frame will contain fundamental frequency
	if(mConfig.GetEnableFundFreq())
	{
		SDIF::Frame tmpSDIFFrame("1FQ0",frame.GetCenterTime());
		//Note: other Frame Header values could be set but are not available in segment data
				
		//First matrix to add to frame
		SDIF::Matrix* pMatrix=new SDIF::Matrix(SDIF::TypeId::sDefault,SDIF::eFloat32,1,1);
		
		//We add fundamental frequency
		pMatrix->SetValue(0,0,frame.GetFundamental().GetFreq());
		tmpSDIFFrame.Add(pMatrix);
		mpFile->Write(tmpSDIFFrame);
	}
//If enabled, second frame will contain residual spectrum
	if(mConfig.GetEnableResidual())
	{
		SDIF::Frame tmpSDIFFrame("1STF",frame.GetCenterTime());
		
		//First matrix to add to frame
		SDIF::Matrix* pMatrix=new SDIF::Matrix("ISTF",SDIF::eFloat32,1,3);
		pMatrix->SetValue(0,0,mConfig.GetSamplingRate());
		pMatrix->SetValue(0,1,mConfig.GetFrameSize());
		pMatrix->SetValue(0,2,mConfig.GetSpectrumSize());
		tmpSDIFFrame.Add(pMatrix);
		//Next matrix
		pMatrix=new SDIF::Matrix("1STF",SDIF::eFloat32,frame.GetResidualSpec().GetSize(),2);
		//pMatrix->mHeader.mnRows=frame.GetResidualSpec().GetSize();
		
		//We have to convert residual spectrum to complex
  		SpectrumConfig Scfg;
  		SpecTypeFlags sflags;
		frame.GetResidualSpec().GetType(sflags);
		if(!sflags.bComplex )
		{
			sflags.bComplex = 1;
  			frame.GetResidualSpec().SetTypeSynchronize(sflags);
		}
		Array<Complex>& complexBuffer=frame.GetResidualSpec().GetComplexArray();
		for (int r=0;r<pMatrix->mHeader.mnRows;r++)	//Write in complex data
		{
			pMatrix->SetValue(r,0,complexBuffer[r].Real());
			pMatrix->SetValue(r,1,complexBuffer[r].Imag());
		}
		tmpSDIFFrame.Add(pMatrix);
		mpFile->Write(tmpSDIFFrame);
	}
//If enabled, third frame will contain sinusoidal spectral peaks
	if(mConfig.GetEnablePeakArray())
	{
		SpectralPeakArray& tmpPeakArray=frame.GetSpectralPeakArray();
		
		SDIF::Frame tmpSDIFFrame("1TRC",frame.GetCenterTime());
		
		int nElems=tmpPeakArray.GetnPeaks();
		SDIF::Matrix* pMatrix=new SDIF::Matrix(SDIF::TypeId::sDefault,SDIF::eFloat32,nElems,4);
		pMatrix->mHeader.mnRows=nElems;
	
		DataArray& pkfreqBuffer=tmpPeakArray.GetFreqBuffer();
		DataArray& pkmagBuffer=tmpPeakArray.GetMagBuffer();
		DataArray& pkPhaseBuffer=tmpPeakArray.GetPhaseBuffer();
		DataArray& pkBinPosBuffer=tmpPeakArray.GetBinPosBuffer();
		DataArray& pkBinWidthBuffer=tmpPeakArray.GetBinWidthBuffer();
		IndexArray& pkIndexArray=tmpPeakArray.GetIndexArray();
		
		for (int r=0;r<nElems;r++)	
		{
			//write track index
			pMatrix->SetValue(r,0,pkIndexArray[r]+1);	// +1 because SDIF doesnt allow Track 0
			// write frequency , mag and phase
			pMatrix->SetValue(r,1,pkfreqBuffer[r]);
			pMatrix->SetValue(r,2,pkmagBuffer[r]);
			pMatrix->SetValue(r,3,pkPhaseBuffer[r]);
			/* cannot store binpos and binwidth in SDIF?
			pkBinPosBuffer[r];
			pkBinWidthBuffer[r];*/
		}
		tmpSDIFFrame.Add(pMatrix);
		mpFile->Write(tmpSDIFFrame);
	}
	return true;
}
