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
#include "CLAM_Math.hxx"

namespace CLAM{

	DataArray Add(DataArray &a, DataArray &b);
	DataArray Multiply(TData &factor, DataArray &a);

SpectralPeakDescriptors::SpectralPeakDescriptors(SpectralPeakArray* pSpectralPeakArray): Descriptor(eNumAttr)
{
	MandatoryInit();
	mpSpectralPeakArray=pSpectralPeakArray;
}

SpectralPeakDescriptors::SpectralPeakDescriptors(TData initVal):Descriptor(eNumAttr)
{
	MandatoryInit();
	AddAll();
	UpdateData();
	
	SetMagnitudeMean(initVal);
	SetHarmonicCentroid(initVal);
	SetSpectralTilt(initVal);
	SetFirstTristimulus(initVal);
	SetSecondTristimulus(initVal);
	SetThirdTristimulus(initVal);
	SetHarmonicDeviation(initVal);
	SetOddHarmonics(initVal);
	SetEvenHarmonics(initVal);
	SetOddToEvenRatio(initVal);
}

void SpectralPeakDescriptors::DefaultInit() {
	mpSpectralPeakArray=0;
	mpStats=0;
}

void SpectralPeakDescriptors::CopyInit(const SpectralPeakDescriptors & copied) {
	mpSpectralPeakArray=copied.mpSpectralPeakArray;
	mpStats=0;
}

const SpectralPeakArray* SpectralPeakDescriptors::GetpSpectralPeakArray() const {
	return mpSpectralPeakArray;
}

void SpectralPeakDescriptors::SetpSpectralPeakArray(SpectralPeakArray* pSpectralPeakArray) {
	mpSpectralPeakArray=pSpectralPeakArray;
    //TODO: it may give problems because pointer passed
	InitStats(&mpSpectralPeakArray->GetMagBuffer());
	mCentroid.Reset();
}


void SpectralPeakDescriptors::ConcreteCompute()
{

	if (HasMagnitudeMean())
		SetMagnitudeMean(mpStats->GetMean());
	if (HasHarmonicCentroid())
		SetHarmonicCentroid(ComputeCentroid());
	if(HasSpectralTilt())
		SetSpectralTilt(ComputeSpectralTilt());
	if(HasFirstTristimulus())
		SetFirstTristimulus(ComputeFirstTristimulus());
	if(HasSecondTristimulus())
		SetSecondTristimulus(ComputeSecondTristimulus());
	if(HasThirdTristimulus())
		SetThirdTristimulus(ComputeThirdTristimulus());
	if(HasHarmonicDeviation())
		SetHarmonicDeviation(ComputeHarmonicDeviation());
	if(HasOddHarmonics())
		SetOddHarmonics(ComputeOddHarmonics());
	if(HasEvenHarmonics())
		SetEvenHarmonics(ComputeEvenHarmonics());
	if(HasOddToEvenRatio())
		SetOddToEvenRatio(ComputeOddToEvenRatio());
}

TData SpectralPeakDescriptors::ComputeCentroid()
{
	int size = mpSpectralPeakArray->GetnPeaks();
	if(size<=0) return 0;
	const Array<TData> & magnitudes = mpSpectralPeakArray->GetMagBuffer();
	const Array<TData> & frequencies = mpSpectralPeakArray->GetFreqBuffer();
	TData crossProduct=0.0;
	for (unsigned i = 0; i < size; i++)
	{
		crossProduct += magnitudes[i]*frequencies[i];
	}
	return crossProduct/(mpStats->GetMean()*size);
}

/*this has been mostly copied and pasted from cuidado and should be checked and some of
it promoted into basicOps*/
TData SpectralPeakDescriptors::ComputeSpectralTilt()
{
	if(mpSpectralPeakArray->GetnPeaks()<=1) return 0;

	/* TODO check me , this computation does not seem to work*/
	TData m1;
	int i;

	TData d1=0;
	TData d2=0;
	TData ti=0;
	TData SumTi2 = 0;
	TData Tilt = 0;

	SpectralPeakArray tmpSpectralPeakArray=*mpSpectralPeakArray;
	tmpSpectralPeakArray.ToLinear();
	DataArray& mag=tmpSpectralPeakArray.GetMagBuffer();
	DataArray& pos=mpSpectralPeakArray->GetFreqBuffer();

	TData size=mag.Size();

	m1 = Mean()(mag);

	for (i=0;i<size;i++)
	  {
	d1 += pos[i]/mag[i];
	d2 += 1/mag[i];
	  }

	/* ti = m1/ai *(n - (d1/d2)) */
	/* SpecTilt = m1²/ti² * SUM[1/ai *(i-d1/d2)]  */

	for (i=0;i<size;i++) {
	  Tilt += (1/mag[i] *(pos[i]-d1/d2));
	  ti = m1/mag[i]*(pos[i] - (d1/d2));
	  SumTi2 += ti*ti;
	}

	Tilt*= (m1*m1/SumTi2);
	return Tilt;
}

TData SpectralPeakDescriptors::ComputeFirstTristimulus()
{
	if(mpSpectralPeakArray->GetnPeaks()<=0) return 0;
	TData firstHarmonicMag=mpSpectralPeakArray->GetMagBuffer()[0];
	return firstHarmonicMag*firstHarmonicMag/mpStats->GetEnergy();
}

TData SpectralPeakDescriptors::ComputeSecondTristimulus()
{
	if(mpSpectralPeakArray->GetnPeaks()<=3) return 0;

	TData secondHarmonicMag=mpSpectralPeakArray->GetMagBuffer()[1];
	TData thirdHarmonicMag=mpSpectralPeakArray->GetMagBuffer()[2];
	TData fourthHarmonicMag=mpSpectralPeakArray->GetMagBuffer()[3];
	
	return (secondHarmonicMag*secondHarmonicMag+thirdHarmonicMag*thirdHarmonicMag+
			fourthHarmonicMag*fourthHarmonicMag)/mpStats->GetEnergy();
}

TData SpectralPeakDescriptors::ComputeThirdTristimulus()
{
	if(mpSpectralPeakArray->GetnPeaks()<=4) return 0;
	DataArray& a=mpSpectralPeakArray->GetMagBuffer();
	return accumulate(a.GetPtr()+4,a.GetPtr()+a.Size(),0.,Power<2,false,TData>())/mpStats->GetEnergy();	
}

TData SpectralPeakDescriptors::ComputeHarmonicDeviation()
{
	int size=mpSpectralPeakArray->GetnPeaks();
	if(size<4) return 0;//is it really necessary to have 4 or with 2 is enough
	DataArray& data=mpSpectralPeakArray->GetMagBuffer();

	DataArray SE;
	SE.Resize(size);
	SE.SetSize(size);

	SE[0] = log10((data[0]+data[1])/2);
	
	for (int i=1; i<size-1; i++)
	{
		SE[i]=log10((data[i-1]+data[i]+data[i+1])/3);
		data[i-1] = log10(data[i-1]);
	}

	SE[size-1]=log10((data[size-2]+data[size-1])/2);

	data[size-2] = log10(data[size-2]);
	data[size-1] = log10(data[size-1]);

	TData nom = 0;
	TData denom = 0;

	for (int i=0;i<size;i++)
	{
		nom +=	abs(data[i] - SE[i]);
        denom += data[i];
	}

	return nom/denom;	
}

TData SpectralPeakDescriptors::ComputeOddHarmonics()
{
	int size=mpSpectralPeakArray->GetnPeaks();
	if(size<3) return 0;
	DataArray& data=mpSpectralPeakArray->GetMagBuffer();
	int i;
	DataArray odd;
	for (i=2;i<size;i+=2)
	{
		odd.AddElem(data[i]);
	}
	return Energy()(odd)/mpStats->GetEnergy();
	
}

TData SpectralPeakDescriptors::ComputeEvenHarmonics()
{
	int size=mpSpectralPeakArray->GetnPeaks();
	if(size<2) return 0;
	DataArray& data=mpSpectralPeakArray->GetMagBuffer();
	int i;
	DataArray even;
	for (i=1;i<size;i+=2)
	{
		even.AddElem(data[i]);
	}
	return Energy()(even)/mpStats->GetEnergy();

}

TData SpectralPeakDescriptors::ComputeOddToEvenRatio()
{
	if(mpSpectralPeakArray->GetnPeaks()<=1) return 0.5;
	TData odd,even;
	if(HasOddHarmonics()) odd=GetOddHarmonics();
	else odd=ComputeOddHarmonics();
	if(HasEvenHarmonics()) even=GetEvenHarmonics();
	else even=ComputeEvenHarmonics();

	return odd/(even+odd);
}

SpectralPeakDescriptors operator * (const SpectralPeakDescriptors& a,TData mult) 
{
	SpectralPeakDescriptors tmpD(a);

	if (a.HasMagnitudeMean())
	{
		tmpD.SetMagnitudeMean(a.GetMagnitudeMean()*mult);
	}
	if (a.HasHarmonicCentroid())
	{
		tmpD.SetHarmonicCentroid(a.GetHarmonicCentroid()*mult);
	}
	if (a.HasSpectralTilt())
	{
		tmpD.SetSpectralTilt(a.GetSpectralTilt()*mult);
	}
	if (a.HasFirstTristimulus())
	{
		tmpD.SetFirstTristimulus(a.GetFirstTristimulus()*mult);
	}
	if (a.HasSecondTristimulus())
	{
		tmpD.SetSecondTristimulus(a.GetSecondTristimulus()*mult);
	}
	if (a.HasThirdTristimulus())
	{
		tmpD.SetThirdTristimulus(a.GetThirdTristimulus()*mult);
	}
	if (a.HasHarmonicDeviation())
	{
		tmpD.SetHarmonicDeviation(a.GetHarmonicDeviation()*mult);
	}
	if (a.HasOddHarmonics())
	{
		tmpD.SetOddHarmonics(a.GetOddHarmonics()*mult);
	}
	if (a.HasEvenHarmonics())
	{
		tmpD.SetEvenHarmonics(a.GetEvenHarmonics()*mult);
	}
	if (a.HasOddToEvenRatio())
	{
		tmpD.SetOddToEvenRatio(a.GetOddToEvenRatio()*mult);
	}
	if(a.HasHPCP())
		tmpD.SetHPCP(Multiply(mult,a.GetHPCP()));

	return tmpD;
}

SpectralPeakDescriptors operator * (const SpectralPeakDescriptors& a,const SpectralPeakDescriptors& b) 
{
	SpectralPeakDescriptors tmpD;

	if (a.HasMagnitudeMean() && b.HasMagnitudeMean())
	{
		tmpD.AddMagnitudeMean();
		tmpD.UpdateData();
		tmpD.SetMagnitudeMean(a.GetMagnitudeMean()*b.GetMagnitudeMean());
	}
	if (a.HasHarmonicCentroid() && b.HasHarmonicCentroid())
	{
		tmpD.AddHarmonicCentroid();
		tmpD.UpdateData();
		tmpD.SetHarmonicCentroid(a.GetHarmonicCentroid()*b.GetHarmonicCentroid());
	}
	if (a.HasSpectralTilt() && b.HasSpectralTilt())
	{
		tmpD.AddSpectralTilt();
		tmpD.UpdateData();
		tmpD.SetSpectralTilt(a.GetSpectralTilt()*b.GetSpectralTilt());
	}
	if (a.HasFirstTristimulus() && b.HasFirstTristimulus())
	{
		tmpD.AddFirstTristimulus();
		tmpD.UpdateData();
		tmpD.SetFirstTristimulus(a.GetFirstTristimulus()*b.GetFirstTristimulus());
	}
	if (a.HasSecondTristimulus() && b.HasSecondTristimulus())
	{
		tmpD.AddSecondTristimulus();
		tmpD.UpdateData();
		tmpD.SetSecondTristimulus(a.GetSecondTristimulus()*b.GetSecondTristimulus());
	}
	if (a.HasThirdTristimulus() && b.HasThirdTristimulus())
	{
		tmpD.AddThirdTristimulus();
		tmpD.UpdateData();
		tmpD.SetThirdTristimulus(a.GetThirdTristimulus()*b.GetThirdTristimulus());
	}
	if (a.HasHarmonicDeviation() && b.HasHarmonicDeviation())
	{
		tmpD.AddHarmonicDeviation();
		tmpD.UpdateData();
		tmpD.SetHarmonicDeviation(a.GetHarmonicDeviation()*b.GetHarmonicDeviation());
	}
	if (a.HasOddHarmonics() && b.HasOddHarmonics())
	{
		tmpD.AddOddHarmonics();
		tmpD.UpdateData();
		tmpD.SetOddHarmonics(a.GetOddHarmonics()*b.GetOddHarmonics());
	}
	if (a.HasEvenHarmonics() && b.HasEvenHarmonics())
	{
		tmpD.AddEvenHarmonics();
		tmpD.UpdateData();
		tmpD.SetEvenHarmonics(a.GetEvenHarmonics()*b.GetEvenHarmonics());
	}
	if (a.HasOddToEvenRatio() && b.HasOddToEvenRatio())
	{
		tmpD.AddOddToEvenRatio();
		tmpD.UpdateData();
		tmpD.SetOddToEvenRatio(a.GetOddToEvenRatio()*b.GetOddToEvenRatio());
	}
	
	return tmpD;
}

SpectralPeakDescriptors operator + (const SpectralPeakDescriptors& a,const SpectralPeakDescriptors& b)
{
	SpectralPeakDescriptors tmpD;

	if (a.HasMagnitudeMean() && b.HasMagnitudeMean())
	{
		tmpD.AddMagnitudeMean();
		tmpD.UpdateData();
		tmpD.SetMagnitudeMean(a.GetMagnitudeMean()+b.GetMagnitudeMean());
	}
	if (a.HasHarmonicCentroid() && b.HasHarmonicCentroid())
	{
		tmpD.AddHarmonicCentroid();
		tmpD.UpdateData();
		tmpD.SetHarmonicCentroid(a.GetHarmonicCentroid()+b.GetHarmonicCentroid());
	}
	if (a.HasSpectralTilt() && b.HasSpectralTilt())
	{
		tmpD.AddSpectralTilt();
		tmpD.UpdateData();
		tmpD.SetSpectralTilt(a.GetSpectralTilt()+b.GetSpectralTilt());
	}
	if (a.HasFirstTristimulus() && b.HasFirstTristimulus())
	{
		tmpD.AddFirstTristimulus();
		tmpD.UpdateData();
		tmpD.SetFirstTristimulus(a.GetFirstTristimulus()+b.GetFirstTristimulus());
	}
	if (a.HasSecondTristimulus() && b.HasSecondTristimulus())
	{
		tmpD.AddSecondTristimulus();
		tmpD.UpdateData();
		tmpD.SetSecondTristimulus(a.GetSecondTristimulus()+b.GetSecondTristimulus());
	}
	if (a.HasThirdTristimulus() && b.HasThirdTristimulus())
	{
		tmpD.AddThirdTristimulus();
		tmpD.UpdateData();
		tmpD.SetThirdTristimulus(a.GetThirdTristimulus()+b.GetThirdTristimulus());
	}
	if (a.HasHarmonicDeviation() && b.HasHarmonicDeviation())
	{
		tmpD.AddHarmonicDeviation();
		tmpD.UpdateData();
		tmpD.SetHarmonicDeviation(a.GetHarmonicDeviation()+b.GetHarmonicDeviation());
	}
	if (a.HasOddHarmonics() && b.HasOddHarmonics())
	{
		tmpD.AddOddHarmonics();
		tmpD.UpdateData();
		tmpD.SetOddHarmonics(a.GetOddHarmonics()+b.GetOddHarmonics());
	}
	if (a.HasEvenHarmonics() && b.HasEvenHarmonics())
	{
		tmpD.AddEvenHarmonics();
		tmpD.UpdateData();
		tmpD.SetEvenHarmonics(a.GetEvenHarmonics()+b.GetEvenHarmonics());
	}
	if (a.HasOddToEvenRatio() && b.HasOddToEvenRatio())
	{
		tmpD.AddOddToEvenRatio();
		tmpD.UpdateData();
		tmpD.SetOddToEvenRatio(a.GetOddToEvenRatio()+b.GetOddToEvenRatio());
	}
	if(a.HasHPCP() && b.HasHPCP() )
	{
		tmpD.AddHPCP();
		tmpD.UpdateData();
		tmpD.SetHPCP(Add(a.GetHPCP(),b.GetHPCP()));
	}

	
	return tmpD;
}

SpectralPeakDescriptors operator / (const SpectralPeakDescriptors& a,TData div) 
{
	return a*(1/div);
}

SpectralPeakDescriptors operator * (TData mult, const SpectralPeakDescriptors& a) 
{
	return a*mult;
}

}//CLAM
