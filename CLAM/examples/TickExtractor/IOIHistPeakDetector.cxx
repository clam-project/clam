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
#include <list>

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
			TData* arr = input.GetBins().GetPtr();


			//int maxpeaks = out.Size(); //mConfig.GetnPeaks();
			int nPeaks=1;
			TData max=0;
			
			std::list<TimeIndex> detectedPeaks;
			typedef std::list<TimeIndex>::iterator LI;

			TimeIndex newPeak;
			newPeak.SetPosition(0);
			newPeak.SetWeight(0);

			detectedPeaks.push_back( newPeak );

			bool sameSlope=false;
			TData fs = input.GetBinRate();

			// MRJ: 3 ms is the minimum allowed space between peaks
			const int peaksMinDist = std::max(1,(int)(0.003*fs)); 
			const int twicePeaksMinDist = 2 * peaksMinDist;
			const int maxPeakPos = size - twicePeaksMinDist;

			// MRJ: Actual peak detection loop. Peaks are stored onto
			// a list for O(k) insertion
			for( int i = twicePeaksMinDist;
			     i < maxPeakPos;
			     i++)
			{
				if ( (arr[i+peaksMinDist]>arr[i])
				     && (!sameSlope) ) 
					sameSlope=true; 
		
				if ( (arr[i-twicePeaksMinDist] < arr[i-peaksMinDist]) 
				     && (arr[i-peaksMinDist] < arr[i]) 
				     && (arr[i] >  arr[i+peaksMinDist]) 
				     && (arr[i+peaksMinDist] > arr[i+twicePeaksMinDist]) 
				     && (sameSlope) )
				{
					sameSlope=false;
					
					newPeak.SetPosition( i );
					newPeak.SetWeight( arr[i] );
					
					detectedPeaks.push_back( newPeak );
					
					if (arr[i] > max) 
						max=arr[i];
				}
			}



			int nActualPeaks=1;
			const TData invMax = 1.0 / (double)max;
			const TData minPeakMagThreshold = mConfig.GetThreshold();
			const bool mustNormalize = mConfig.GetNormalizeWeights();


			// MRJ: Unlikely peaks removal and weight normalization

			LI prev = detectedPeaks.begin();
			LI i = detectedPeaks.begin();
			i++;
			
			for ( ; i != detectedPeaks.end();
			      i++, prev++ )
			{
				const TData tmpWeight = i->GetWeight();
				const TData tmpPosition = i->GetPosition();
				const TData normWeight = tmpWeight * invMax;
				
				if ( (normWeight > minPeakMagThreshold)
				     && std::fabs( tmpPosition - prev->GetPosition() ) > peaksMinDist )
				{
					if ( mustNormalize )
						i->SetWeight( normWeight );
				}
				else
					i = detectedPeaks.erase( i );
			}

			out.Resize( detectedPeaks.size() );
			out.SetSize( detectedPeaks.size() );

			int j = 0;

			for ( LI k = detectedPeaks.begin();
			      k != detectedPeaks.end(); k++, j++ )
				out[j] = *k;
			
			return true;
		}

	} // namespace RhythmDescription
} // namespace CLAM
