/*
 * Author: fabien gouyon 
 * http://www.iua.upf.es/~fgouyon
 * Description:
 *
 * Syntax: C++
 *
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

#include "IOIHistPeakDetector.hxx"
#include "Audio.hxx"
#include "CLAM_Math.hxx"

namespace CLAM
{
	namespace RhythmDescription
	{

	void IOIHistPeakDetectorConfig::DefaultInit()
	{
		/* the dynamic type takes care if we add an existing attr .. */
		AddThreshold();
		AddNormalizeWeights();

		/* All Attributes are added */
		UpdateData();
		SetThreshold(0);
		SetNormalizeWeights(true);

	}


/* Processing  object Method  implementations */

	IOIHistPeakDetector::IOIHistPeakDetector()
	{
		Configure(IOIHistPeakDetectorConfig());
	}

	IOIHistPeakDetector::IOIHistPeakDetector(const IOIHistPeakDetectorConfig &c)
	{
		Configure(c);
	}

/* Configure the Processing Object according to the Config object */

	bool IOIHistPeakDetector::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig( mConfig, c );
		return true;
	}

/* The supervised Do() function */
	bool  IOIHistPeakDetector::Do(void) 
	{
		return false;
	}

/* The  unsupervised Do() function */
	bool  IOIHistPeakDetector::Do( IOIHistogram& input, Array<TimeIndex>& out)
	{
		int  size = input.GetBins().Size();
		int i;
		TData* arr = input.GetBins().GetPtr();


		//Add a peak of weight 0 at 0
		out.Resize(1);
		out.SetSize(1);
		out[0].SetPosition(0);
		out[0].SetWeight(0);

		//int maxpeaks = out.Size(); //mConfig.GetnPeaks();
		int nPeaks=1;
		TData max=0;
		Array<TimeIndex> tmpArray;
		tmpArray.Resize(1);
		tmpArray.SetSize(1);
		tmpArray[0].SetPosition(0);
		tmpArray[0].SetWeight(0);

		bool sameSlope=false;
		TData fs = input.GetBinRate();
		int toto=std::max(1,(int)(0.003*fs));

//		int toto=4;
		for(i=2*toto;i<size-2*toto;i++)
		{
			if ((arr[i+toto]>arr[i])&&(sameSlope==false)) 
				sameSlope=true; 
			if ((arr[i-2*toto] < arr[i-toto]) &&
			    (arr[i-toto] < arr[i]) && (arr[i] >  arr[i+toto]) &&
			    (arr[i+toto] > arr[i+2*toto]) &&
			    (sameSlope==true))
			{
				sameSlope=false;
				tmpArray.Resize(nPeaks+1);
				tmpArray.SetSize(nPeaks+1);
				tmpArray[nPeaks].SetPosition(i);
				tmpArray[nPeaks].SetWeight(arr[i]); 			  
				nPeaks++;
				if (arr[i] > max) 
					max=arr[i];
			}
		}


		//int peaksDistMin = 100;
		int peaksDistMin = std::max(1,(int)(0.003*fs));
		TData tmpWeight, tmpPosition;
		int nActualPeaks=1;
		for (i=0;i<nPeaks;i++)
		{
			tmpWeight=tmpArray[i].GetWeight();
			tmpPosition=tmpArray[i].GetPosition();
			if (tmpWeight/max > mConfig.GetThreshold() &&
			    std::fabs(tmpPosition-out[nActualPeaks-1].GetPosition()) > peaksDistMin)
			{
				out.Resize(nActualPeaks+1);
				out.SetSize(nActualPeaks+1);
				if (mConfig.GetNormalizeWeights())
					out[nActualPeaks].SetWeight(tmpWeight/max);
				else
					out[nActualPeaks].SetWeight(tmpWeight);
				out[nActualPeaks].SetPosition(tmpPosition);
				nActualPeaks++;
			}
		}
		return true;
	}

	} // namespace RhythmDescription
} // namespace CLAM
