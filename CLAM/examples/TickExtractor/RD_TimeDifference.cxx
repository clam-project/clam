/*
 * Author: Günter Geiger
 *			fabien gouyon 
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
#include "Complex.hxx"
#include "RD_TimeDifference.hxx"
#include <cmath>

namespace CLAM 
{

namespace RhythmDescription
{

/* The  Configuration object has at least to have a name */
	void TimeDifferenceConfig::DefaultInit()
	{
		/* the dynamic type takes care if we add an existing attr .. */
		AddGaussianSize();

		/* All Attributes are added */
		UpdateData();
		SetGaussianSize(8192);
	}


/* Processing  object Method  implementations */
	TimeDifference::TimeDifference()
	{
		Configure(TimeDifferenceConfig());
	}

	TimeDifference::TimeDifference(const TimeDifferenceConfig &c)
	{
		Configure(c);
	}

	TimeDifference::~TimeDifference()
	{}


/* Configure the Processing Object according to the Config object */

	bool TimeDifference::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig( mConfig, c );
		WindowGeneratorConfig winconf;
		winconf.AddType();
		winconf.UpdateData();
		winconf.SetType(EWindowType::eGaussian);
		winconf.SetSize(mConfig.GetGaussianSize());

		mWindowGen.Configure(winconf);

		mWindow.SetSize(mConfig.GetGaussianSize());

		mWindowGen.Start();

//		cerr << "audiosize " <<  mConfig.GetGaussianSize();
//		cerr << " windowsize " <<  mWindowGen.GetSize() << endl;

		mWindowGen.Do(mWindow);
		mWindowGen.Stop();	    

		//CLAM::ShowSnapshot(mWindow,"GGGaussian");
	
		//NB: full-width of gaussian at half max= s*2*sqrt(2)
		//	where s is the standard dev
		//	in WindowGenerator::Gaussian, s=0.15

		return true;
	}

/* The supervised Do() function */
	bool  TimeDifference::Do(void) 
	{
		return false;
	}

/* The  unsupervised Do() function */
	bool  TimeDifference::Do(Array<TimeIndex>& in, Audio& out)
	{
		int gsize = mConfig.GetGaussianSize();
	
		TData* outp = out.GetBuffer().GetPtr();
		TData* end = outp + out.GetSize();
		TData* win = mWindow.GetBuffer().GetPtr();
		int  size = in.Size();
    
		for (int i=0;i<out.GetSize();i++)
			outp[i] = 0.;
	
		for (int j=0;j < size-1;j++) {
			int apos = (int) in[j].GetPosition();
			for (int k = j+1; k<size;k++) {
				int pos = labs((int)apos - in[k].GetPosition()); 
				if (pos+gsize<out.GetSize()) {
#if 1
					TData* outw = outp + pos - gsize/2;
					TData weight = std::min(in[k].GetWeight(),in[j].GetWeight());
					// add the gaussian 
					for (int i=0;i<gsize;i++) {
						if (outw >= outp && outw < end) {
							*outw+=(win[i]*weight);
						}
						outw++;
					}
#else
					TData* outw = outp + pos;
					TData weight = std::min(in[k].GetWeight(),in[j].GetWeight());
					if (outw >= outp && outw < end) {
						*outw=weight;
					}
#endif
				}
			}
		}
		return true;
	}

} // namespace RhythmDescription
  
} // namespace CLAM

