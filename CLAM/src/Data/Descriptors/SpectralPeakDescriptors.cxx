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




#include "ProcessingData.hxx"
#include "SpectralPeakDescriptors.hxx"
#include "SpectralPeakArray.hxx"

using namespace CLAM;


SpectralPeakDescriptors::SpectralPeakDescriptors(SpectralPeakArray* pSpectralPeakArray): Descriptor(eNumAttr)
{
	MandatoryInit();
	mpSpectralPeakArray=pSpectralPeakArray;
}

void SpectralPeakDescriptors::SetpSpectralPeakArray(SpectralPeakArray* pSpectralPeakArray) {
	mpSpectralPeakArray=pSpectralPeakArray;
    //TODO: it may give problems because pointer passed
	InitStats(&mpSpectralPeakArray->GetMagBuffer());

}


void SpectralPeakDescriptors::ConcreteCompute()
{

	if (HasMagnitudeMean())
		SetMagnitudeMean(mpStats->GetMean());
	if (HasHarmonicCentroid())
		SetHarmonicCentroid(mCentroid(mpSpectralPeakArray->GetMagBuffer(),
							mpSpectralPeakArray->GetFreqBuffer()));
/*
		DYN_ATTRIBUTE (2, public, TData, SpectralTilt);
		DYN_ATTRIBUTE (3, public, TData, HarmonicDeviation);
		DYN_ATTRIBUTE (4, public, TData, FirstTristimulus);
		DYN_ATTRIBUTE (5, public, TData, SecondTristimulus);
		DYN_ATTRIBUTE (6, public, TData, ThirdTristimulus);
		DYN_ATTRIBUTE (7, public, TData, Brightness);
		DYN_ATTRIBUTE (8, public, TData, OddHarmonics);
		DYN_ATTRIBUTE (9, public, TData, EvenHarmonics);
		DYN_ATTRIBUTE (10,public, TData, OddToEvenRatio);
*/
}