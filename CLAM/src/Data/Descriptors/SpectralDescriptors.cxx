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

#include "SpectralDescriptors.hxx"
#include "Spectrum.hxx"

using namespace CLAM;

SpectralDescriptors::SpectralDescriptors(Spectrum* pSpectrum):Descriptor(eNumAttr)
{
	MandatoryInit();
	mpSpectrum=pSpectrum;
}

// Implementation

void SpectralDescriptors::DefaultInit() {
	mpSpectrum=0;
	mpStats=0;
	//Warning: no attributes are added by default, the user is in charge of adding the ones he is interested in
}

void SpectralDescriptors::CopyInit(const SpectralDescriptors & copied) {
	mpSpectrum=copied.mpSpectrum;
	mpStats=0;
}

inline const Spectrum* SpectralDescriptors::GetpSpectrum() const {
	return mpSpectrum;
}

void SpectralDescriptors::SetpSpectrum(Spectrum* pSpectrum) {
	mpSpectrum=pSpectrum;
	//TODO: we are asuming Spectrum is in MagBuffer
    //TODO: it may give problems because pointer passed
	InitStats(&mpSpectrum->GetMagBuffer());

	mDeltaFreq=mpSpectrum->GetSpectralRange()/mpSpectrum->GetSize();
	
}

void SpectralDescriptors::ConcreteCompute()
{
	if (HasMean())
		SetMean(mpStats->GetMean());
	if (HasGeometricMean())
		SetGeometricMean(mpStats->GetGeometricMean());
	if (HasEnergy())
		SetEnergy(mpStats->GetEnergy());
	if (HasCentroid())
		SetCentroid(mpStats->GetCentroid()*mDeltaFreq);
	if(HasMoment2())
		SetMoment2(mpStats->GetMoment(SecondOrder)*mDeltaFreq);
	if(HasMoment3())
		SetMoment3(mpStats->GetMoment(ThirdOrder)*mDeltaFreq);	
	if(HasMoment4())
		SetMoment4(mpStats->GetMoment(FourthOrder)*mDeltaFreq);
	if(HasMoment5())
		SetMoment5(mpStats->GetMoment(FifthOrder)*mDeltaFreq);
	if(HasMoment6())
		SetMoment6(mpStats->GetMoment((O<6>*)(0))*mDeltaFreq);
	if(HasKurtosis())	
		SetKurtosis(mpStats->GetKurtosis());
	if(HasSkewness())
	   SetSkewness(mpStats->GetSkew());

/*The computation for the following descriptors is still to be implemented

		DYN_ATTRIBUTE (9, public, TData, Irregularity);
		DYN_ATTRIBUTE (10,public, TData, Tilt);
		DYN_ATTRIBUTE (11,public, TData, Flatness);
		DYN_ATTRIBUTE (13,public, TData, StrongPeak);
		DYN_ATTRIBUTE (14,public, TData, HFC);
		DYN_ATTRIBUTE (15,public, Array<TData>, MFCC);
		DYN_ATTRIBUTE (16,public, Array<TData>, BandEnergy);
		//for this I may have to use the CLAM::Search
		  DYN_ATTRIBUTE (17,public, TData, MaxMagFreq); 
		// Frequency of the maximun of the spectrum normalized by the spectral range
		DYN_ATTRIBUTE (18,public, TData, LowFreqEnergyRelation); 
		DYN_ATTRIBUTE (20,public, TData, Rolloff); 
*/
}

void SpectralDescriptors::SetPrototype(const SpectralDescriptors& p)
{
	*this=p;
}