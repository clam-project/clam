/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "SpectralPeakDescriptors.hxx"
#include "SpectralPeakArray.hxx"
#include "SpectralPeakDetect.hxx"
#include "Spectrum.hxx"
#include "FFT.hxx"
#include "SpecTypeFlags.hxx"
#include "AudioFile.hxx"
#include "MonoAudioFileReader.hxx"



#include <iostream>

namespace CLAMTest
{


class SpectralPeakDescriptorsTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SpectralPeakDescriptorsTest );

class SpectralPeakDescriptorsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SpectralPeakDescriptorsTest );
	CPPUNIT_TEST( testMagnitudeMean );

	CPPUNIT_TEST_SUITE_END();

private:
	

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
		char* pathToTestData = getenv("CLAM_TEST_DATA");

		if ( !pathToTestData )
			mPathToTestData = "../../../../CLAM-TestData/";
		else
			mPathToTestData = pathToTestData;

		mPathToTestData += "descriptorsData/frames/";

		mDescriptors = new CLAM::SpectralPeakDescriptors();
		mDescriptors->RemoveAll();
	}

	/// Common clean up, executed after each test method
	void tearDown() 
	{ 
		delete mDescriptors;
	}

private:
	CLAM::SpectralPeakDescriptors *mDescriptors;
	std::string mPathToTestData;

	CLAM::Spectrum ComputeSpectrum(const CLAM::Audio& audioData, CLAM::TSize spectrumSize)
	{
		// Configure and create the spectrum
		CLAM::SpecTypeFlags specFlags;
		specFlags.bMagPhase = 1;
			
		CLAM::Spectrum mySpectrum;
		mySpectrum.SetType(specFlags);
		mySpectrum.SetSize(spectrumSize);

		// Configure the transform
		CLAM::FFTConfig FFTCfg;
		FFTCfg.SetAudioSize(audioData.GetSize());

		// Compute the transform
		CLAM::FFT myFFT;
		myFFT.Configure(FFTCfg);
		myFFT.Start();
		myFFT.Do(audioData, mySpectrum);
		myFFT.Stop();

		// Return the result
		return mySpectrum;
	}

	CLAM::SpectralPeakArray ComputeSpectralPeaks(const CLAM::Spectrum& spectrum)
	{
		// Define peak detection class
		CLAM::SpectralPeakDetect peakDetect;
		// Define spectral peak data class
		CLAM::SpectralPeakArray  myPeakArray;
		myPeakArray.SetScale(CLAM::EScale(CLAM::EScale::eLog));
		// Convert spectrum to dB
		CLAM::Spectrum auxSpectrum(spectrum);
		auxSpectrum.ToDB();
		// Perform peak detection
		peakDetect.Do(auxSpectrum,myPeakArray);
		// Convert peaks to linear
		myPeakArray.ToLinear();

		return myPeakArray;
	}
	
	CLAM::SpectralPeakArray helperGetData(const std::string & fileName)
	{
		CLAM::AudioFile audioFile;
		audioFile.SetLocation(mPathToTestData+fileName);
		CPPUNIT_ASSERT_MESSAGE( 
			"Unable to load file "+ audioFile.GetLocation(),
			audioFile.IsReadable());
		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile(audioFile);
		CLAM::MonoAudioFileReader reader;
		CPPUNIT_ASSERT_MESSAGE(
			"configuration failed " + reader.GetConfigErrorMessage(),
			reader.Configure(cfg));

		CLAM::Audio buf;
		buf.SetSize(1025);


		reader.Start();
		reader.Do( buf );
		reader.Stop();

		return ComputeSpectralPeaks(
			ComputeSpectrum(buf, buf.GetSize()/2 + 1));
	}

	void assertDescriptorExtractionInsideTolerance(const std::map<std::string, CLAM::TData> & expected, 
		CLAM::TData tolerance, CLAM::TData & (CLAM::SpectralPeakDescriptors::*getter)() const )
	{
		std::stringstream log;
		bool success = true;
		mDescriptors->UpdateData();
		CLAM::SpectralPeakArray peaks;
		std::map<std::string, CLAM::TData>::const_iterator it;
		for (it = expected.begin(); it != expected.end(); it++) {
			peaks = helperGetData((*it).first);
			mDescriptors->SetpSpectralPeakArray(&peaks);
			mDescriptors->Compute();
			if (
				(std::isnan((mDescriptors->*getter)()) && !std::isnan(it->second)) ||
				(!std::isnan((mDescriptors->*getter)()) && std::isnan(it->second)) ||
				(mDescriptors->*getter)() > (*it).second + tolerance ||
				(mDescriptors->*getter)() < (*it).second - tolerance
				)
			{
				log << (*it).first
				<< ": expected " << (*it).second
				<< ", received " << (mDescriptors->*getter)()
				<< std::endl;

				success = false;
			}
		}

		CPPUNIT_ASSERT_MESSAGE(std::string("Error margins surpassed:\n")+log.str()
			,success);
	}

private:

	void testMagnitudeMean()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = 473.268293;
		data["Balance000.600.wav"] = 150.585366;
		data["Balance000.992.wav"] = 236.634146;
		data["Balance001.988.wav"] = 925.024390;
		data["Balance010.910.wav"] = 150.585366;
		data["Cello_A2.wav"] = 1656.439024;
		data["Cello_C2.wav"] = -1.000000;
		data["Disco_Rojo001.008.wav"] = 4904.780488;
		data["Disco_Rojo002.327.wav"] = 537.804878;
		data["Geiger_Counter005.020.wav"] = 258.146341;
		data["SaxBritHorns12.wav"] = 2732.048780;
		data["Time002.624.wav"] = 9874.097561;
		data["bell_A3.wav"] = 2646.000000;
		data["gamelan-gong.wav"] = 139.829268;
		data["gt_E4.wav"] = 86.048780;
		data["pno_Eb1.wav"] = -1.000000;
		data["silence.wav"] = -1.000000;
		data["vln_A3.wav"] = -1.000000;
		data["vln_D5.wav"] = -1.000000;
		data["whitenoise.wav"] = 18672.585366;

		mDescriptors->AddMagnitudeMean();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralPeakDescriptors::GetMagnitudeMean);
	}


};


} // namespace CLAMTest
