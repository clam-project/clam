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

#include "SpectralDescriptors.hxx"
#include "Spectrum.hxx"
#include "FFT.hxx"
#include "SpecTypeFlags.hxx"
#include "AudioFile.hxx"
#include "MonoAudioFileReader.hxx"
#include "XMLStorage.hxx"



#include <iostream>
#include <iomanip>

namespace CLAMTest
{


class SpectralDescriptorsTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SpectralDescriptorsTest );

class SpectralDescriptorsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SpectralDescriptorsTest );
//	CPPUNIT_TEST( testRolloff );
	CPPUNIT_TEST( testFlatness );
//	CPPUNIT_TEST( testCentroid );
	CPPUNIT_TEST( testSpread );
	CPPUNIT_TEST( testSlope );
//	CPPUNIT_TEST( testMaxMagFreq );

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

		mDescriptors = new CLAM::SpectralDescriptors();
		mDescriptors->RemoveAll();
	}

	/// Common clean up, executed after each test method
	void tearDown() 
	{ 
		delete mDescriptors;
	}

private:
	CLAM::SpectralDescriptors *mDescriptors;
	std::string mPathToTestData;

	CLAM::Spectrum ComputeSpectrum(const CLAM::Audio& audioData)
	{
		const CLAM::TSize spectrumSize = audioData.GetSize()/2 + 1;
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

	CLAM::Audio ReadAudio(const std::string & fileName)
	{
		CLAM::AudioFile audioFile;
		audioFile.SetLocation(fileName);
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

		return buf;

	}

	CLAM::Spectrum helperGetData(const std::string & fileName)
	{
		std::string extension = fileName.substr(fileName.size()-4,fileName.size());
		std::string fullPath = mPathToTestData+fileName;
		if (extension!=".xml")
			return ComputeSpectrum(ReadAudio(fullPath));
		CLAM::Spectrum spectrum;
		CLAM::XMLStorage::Restore(spectrum,fullPath); 
		return spectrum;
	}


	void assertDescriptorExtractionInsideTolerance(const std::map<std::string, CLAM::TData> & expected, 
		CLAM::TData tolerance, CLAM::TData & (CLAM::SpectralDescriptors::*getter)() const )
	{
		std::stringstream log;
		bool success = true;
		mDescriptors->UpdateData();
		CLAM::Spectrum spectrum;
		std::map<std::string, CLAM::TData>::const_iterator it;
		for (it = expected.begin(); it != expected.end(); it++) {
			spectrum = helperGetData((*it).first);
			mDescriptors->SetpSpectrum(&spectrum);
//			CLAM::XMLStorage::Dump(spectrum, "Spectrum", mPathToTestData + it->first + "-Spectrum.xml");
			mDescriptors->Compute();
			if (
				(std::isnan((mDescriptors->*getter)()) != std::isnan(it->second)) ||
				(mDescriptors->*getter)() > (*it).second + tolerance ||
				(mDescriptors->*getter)() < (*it).second - tolerance
				)
			{
				log << (*it).first
				<< ": expected " << std::setprecision(15) << it->second
				<< ", received " << std::setprecision(15) << (mDescriptors->*getter)()
				<< ", difference " << std::setprecision(15) << (it->second - (mDescriptors->*getter)())
				<< std::endl;

				success = false;
			}
		}

		CPPUNIT_ASSERT_MESSAGE(std::string("Error margins surpassed:\n")+log.str()
			,success);
	}

private:

	void testRolloff()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = 473.268293;
		data["Balance000.600.wav"] = 150.585366;
		data["Balance000.992.wav"] = 236.634146;
		data["Balance001.988.wav"] = 925.024390;
		data["Balance010.910.wav"] = 150.585366;
		data["Cello_A2.wav"] = 1656.439024;
		data["Cello_C2.wav"] = 0.000000;
		data["Disco_Rojo001.008.wav"] = 4904.780488;
		data["Disco_Rojo002.327.wav"] = 537.804878;
		data["Geiger_Counter005.020.wav"] = 258.146341;
		data["SaxBritHorns12.wav"] = 2732.048780;
		data["Time002.624.wav"] = 9874.097561;
		data["bell_A3.wav"] = 2646.000000;
		data["gamelan-gong.wav"] = 139.829268;
		data["gt_E4.wav"] = 86.048780;
		data["pno_Eb1.wav"] = 0.000000;
		data["silence.wav"] = 0.000000;
		data["vln_A3.wav"] = 0.000000;
		data["vln_D5.wav"] = 0.000000;
		data["whitenoise.wav"] = 18672.585366;

		mDescriptors->AddRolloff();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralDescriptors::GetRolloff);
	}

	void testFlatness()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = -5.570786;
		data["Balance000.600.wav"] = -14.812896;
		data["Balance000.992.wav"] = -11.349304;
		data["Balance001.988.wav"] = -10.240200;
		data["Balance010.910.wav"] = -4.842938;
		data["Cello_A2.wav"] = -5.525457;
		data["Cello_C2.wav"] = 0.000000;
		data["Disco_Rojo001.008.wav"] = -5.169985;
		data["Disco_Rojo002.327.wav"] = -6.795831;
		data["Geiger_Counter005.020.wav"] = -7.020690;
		data["SaxBritHorns12.wav"] = -4.667628;
		data["Time002.624.wav"] = -3.651583;
		data["bell_A3.wav"] = -5.276683;
		data["gamelan-gong.wav"] = -5.224524;
		data["gt_E4.wav"] = -3.491369;
		data["pno_Eb1.wav"] = 0.000000;
		data["silence.wav"] = 0.000000;
		data["vln_A3.wav"] = 0.000000;
		data["vln_D5.wav"] = 0.000000;
		data["whitenoise.wav"] = -0.795157;

		mDescriptors->AddFlatness();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralDescriptors::GetFlatness);
	}

	void testCentroid()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = 2546.840064;
		data["Balance000.600.wav"] = 416.676904;
		data["Balance000.992.wav"] = 795.851545;
		data["Balance001.988.wav"] = 1588.760453;
		data["Balance010.910.wav"] = 2696.114917;
		data["Cello_A2.wav"] = 3017.010992;
		data["Cello_C2.wav"] = 11025.;
		data["Disco_Rojo001.008.wav"] = 4169.324045;
		data["Disco_Rojo002.327.wav"] = 2579.081038;
		data["Geiger_Counter005.020.wav"] = 1764.977615;
		data["SaxBritHorns12.wav"] = 3750.426935;
		data["Time002.624.wav"] = 7135.560467;
		data["bell_A3.wav"] = 3686.122991;
		data["gamelan-gong.wav"] = 1263.962139;
		data["gt_E4.wav"] = 4403.107081;
		// Silences
		data["pno_Eb1.wav"] = 11025.;
		data["silence.wav"] = 11025.;
		data["vln_A3.wav"] = 11025.;
		data["vln_D5.wav"] = 11025.;
		data["whitenoise.wav"] = 11072.758057;
		data["Constant-Spectrum.xml"]= 11025;

		mDescriptors->AddCentroid();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralDescriptors::GetCentroid);
	}

	CLAM::TData plainSpread(CLAM::TData nBins, CLAM::TData spectralRange)
	{
		CLAM::TData binRange = spectralRange/(nBins-1);
		return binRange*binRange*(nBins+1)*(nBins-1)/12;
	}
	void testSpread()
	{
		CLAM::TData tolerance = 0.0006;  // Due to numerical inaccuracies
		CLAM::TData binRange513 = 22050/(513-1);

		std::map<std::string, CLAM::TData> data;
		data["MaxSpread-Spectrum.xml"] = 22050*22050/4;
		data["MinSpread-Spectrum.xml"] = 0.0;
		data["DeltaAtZeroBin-Spectrum.xml"] = 0.0; // Avoid NaN
		data["Silence-Spectrum.xml"] = plainSpread(513,22050); // Avoid NaN
		data["Constant-Spectrum.xml"] = plainSpread(513,22050);
		data["ConstantDouble-Spectrum.xml"] = plainSpread(513,22050);
		data["ConstantHalfSize-Spectrum.xml"] = plainSpread(257,22050);

		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = 19988729.96601;
		data["Balance000.600.wav"] = 3256552.743;
		data["Balance000.992.wav"] = 6201384.658;
		data["Balance001.988.wav"] = 4687203.759;
		data["Balance010.910.wav"] = 24206724.5216;
		data["Cello_A2.wav"] = 15697222.546;
		data["Cello_C2.wav"] = plainSpread(513,22050);
		data["Disco_Rojo001.008.wav"] = 14805980.3445418;
		data["Disco_Rojo002.327.wav"] = 11706385.6573933;
		data["Geiger_Counter005.020.wav"] = 14443489.7393;
		data["SaxBritHorns12.wav"] = 17017109.5633701;
		data["Time002.624.wav"] = 25246294.0153;
		data["bell_A3.wav"] = 15736636.995407;
		data["gamelan-gong.wav"] = 5662587.598;
		data["gt_E4.wav"] = 34762401.1;
		data["pno_Eb1.wav"] = plainSpread(513,22050);
		data["silence.wav"] = plainSpread(513,22050);
		data["vln_A3.wav"] = plainSpread(513,22050);
		data["vln_D5.wav"] = plainSpread(513,22050);
		data["whitenoise.wav"] = 40201639.326;

		mDescriptors->AddSpread();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralDescriptors::GetSpread);
	}

	void testSlope()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["Constant-Spectrum.xml"]= 0.0;
		data["ConstantDouble-Spectrum.xml"]= 0.0;
		data["ConstantHalfSize-Spectrum.xml"]= 0.0;
		data["MaxSpread-Spectrum.xml"]= 0.0;
		data["MinSpread-Spectrum.xml"]= 0.0;
		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = -4.07655e-07;
		data["Balance000.600.wav"] = -5.10341e-07;
		data["Balance000.992.wav"] = -4.92062e-07;
		data["Balance001.988.wav"] = -4.5384e-07;
		data["Balance010.910.wav"] = -4.00459e-07;
		data["Cello_A2.wav"] = -3.8499e-07;
		data["Cello_C2.wav"] = 0;
		data["Disco_Rojo001.008.wav"] = -3.29443e-07;
		data["Disco_Rojo002.327.wav"] = -4.06101e-07;
		data["Geiger_Counter005.020.wav"] = -4.45345e-07;
		data["SaxBritHorns12.wav"] = -3.49636e-07;
		data["Time002.624.wav"] = -1.86454e-07;
		data["bell_A3.wav"] = -3.52736e-07;
		data["gamelan-gong.wav"] = -8.17134e-07;
		data["gt_E4.wav"] = -3.18173e-07;
		data["pno_Eb1.wav"] = 0;
		data["silence.wav"] = 0;
		data["vln_A3.wav"] = 0;
		data["vln_D5.wav"] = 0;
		data["whitenoise.wav"] = 3.34021e-09;

		mDescriptors->AddSlope();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralDescriptors::GetSlope);
	}

	void testMaxMagFreq()
	{
		CLAM::TData tolerance = 1;  // Due to numerical inaccuracies, 1 Hz

		std::map<std::string, CLAM::TData> data;
		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = 258.3984;
		data["Balance000.600.wav"] = 129.1992;
		data["Balance000.992.wav"] = 215.3320;
		data["Balance001.988.wav"] = 602.9296;
		data["Balance010.910.wav"] = 86.1328;
		data["Cello_A2.wav"] = 861.3280;
		data["Cello_C2.wav"] = 0;
		data["Disco_Rojo001.008.wav"] = 43.06647;
		data["Disco_Rojo002.327.wav"] = 86.1328;
		data["Geiger_Counter005.020.wav"] = 129.1992;
		data["SaxBritHorns12.wav"] =  1981.1;
		data["Time002.624.wav"] = 6373.8;
		data["bell_A3.wav"] = 2153.3;
		data["gamelan-gong.wav"] = 129.199;
		data["gt_E4.wav"] = 43.0664;
		data["pno_Eb1.wav"] = 0;
		data["silence.wav"] = 0;
		data["vln_A3.wav"] = 0;
		data["vln_D5.wav"] = 0;
		data["whitenoise.wav"] = 12791;

		mDescriptors->AddMaxMagFreq();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralDescriptors::GetMaxMagFreq);
	}


};


} // namespace CLAMTest
