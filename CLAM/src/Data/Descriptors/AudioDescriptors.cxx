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




#include "AudioDescriptors.hxx"
#include "Audio.hxx"

using namespace CLAM;

AudioDescriptors::AudioDescriptors(Audio* pAudio): Descriptor(eNumAttr)
{
	MandatoryInit();
	mpAudio=pAudio;
}


void AudioDescriptors::SetpAudio(Audio* pAudio) {
	mpAudio=pAudio;
    //TODO: it may give problems because pointer passed
	InitStats(&mpAudio->GetBuffer());
	
}

void AudioDescriptors::ConcreteCompute()
{
	if (HasMean())
		SetMean(mpStats->GetMean());
	if (HasTemporalCentroid())
		SetTemporalCentroid(mpStats->GetCentroid()*mpAudio->GetDuration()/mpAudio->GetSize());
	if (HasEnergy())
		SetEnergy(mpStats->GetEnergy());
	if(HasVariance())
		SetVariance(mpStats->GetVariance());

/*		DYN_ATTRIBUTE (3, public, TData, Attack);
		DYN_ATTRIBUTE (4, public, TData, Decay);
		DYN_ATTRIBUTE (5, public, TData, Sustain);
		DYN_ATTRIBUTE (6, public, TData, Release);
		DYN_ATTRIBUTE (7, public, TData, LogAttackTime);
		DYN_ATTRIBUTE (9, public, TData, ZeroCrossingRate);
		DYN_ATTRIBUTE (10,public, TData, RiseTime);
*/
}
