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

#ifndef _MeterEstimator_
#define _MeterEstimator_

#include "ProcessingComposite.hxx"
#include "TimeIndex.hxx"
#include "TickFromOnsets.hxx"
#include "Segment.hxx"
#include "SegmentDescriptors.hxx"
#include "AutocorrelationTD.hxx"
#include "DescriptorComputation.hxx"

namespace CLAM
{

	class Audio;

	namespace RhythmDescription
	{
		class Meter;

		class MeterEstimatorConfig : public ProcessingConfig
		{
		public:
			DYNAMIC_TYPE_USING_INTERFACE (MeterEstimatorConfig, 5 ,ProcessingConfig);
			DYN_ATTRIBUTE ( 0, public, TData, TempoLimInf);
			DYN_ATTRIBUTE ( 1, public, TData, TempoLimSup);
			DYN_ATTRIBUTE ( 2, public, int, ACFUpperLimit);
			DYN_ATTRIBUTE ( 3, public, bool , AutomaticIntegTime);
			DYN_ATTRIBUTE ( 4, public, int, ACFIntegrationTime);
		protected:
			void DefaultInit();
		};

		class MeterEstimator : public ProcessingComposite
		{
		public:
			MeterEstimator();
			MeterEstimator(const MeterEstimatorConfig &c);
			~MeterEstimator() {}

			const ProcessingConfig &GetConfig() const { return mConfig;}

			bool Do(void);
			bool Do(Audio& audioIn, const Array<TimeIndex>& beats, Meter& dataOut);

			bool Compute(Audio& audioIn, const Array<TimeIndex>& allBeats, 
				     const TData& globalTempo, const TData& globalTick, Meter& dataOut);

		private:
			MeterEstimatorConfig mConfig;
			const char *GetClassName() const {return "MeterEstimator";}
			bool ConcreteConfigure(const ProcessingConfig&);

			//Internal POs
			AutocorrelationTD mACF;

			DescriptorComputation mAudioDescGen;

			//PDs
			Segment mSegment;
			SegmentDescriptors mSegmentD;
	
			// Internal convenience methods
			void AttachChildren();
			bool ConfigureChildren();
			void ConfigureData();

		};

	} // namespace RhythmDescription

} // namespace CLAM

#endif // _MeterEstimator_

