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

#ifndef _DESCRIPTOR_COMPUTATION_H_
#define _DESCRIPTOR_COMPUTATION_H_

#include "DataTypes.hxx"
#include "Processing.hxx"
#include "Descriptor.hxx"
#include "DescriptorComputationConfig.hxx"
#include "InControl.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"

namespace CLAM {

	
	/** This Processing class only calls the Compute operation of Descriptor passed to the Do 
	 */
	class DescriptorComputation
		: public Processing {
	
	protected:


		const char *GetClassName() const {return "CircularShift";}

		/** Config change method
		 */
		bool ConcreteConfigure(const ProcessingConfig& c){CopyAsConcreteConfig(mConfig,c);}

	public:
		DescriptorComputation(){Configure(DescriptorComputationConfig());}

		DescriptorComputation(const DescriptorComputationConfig &c){Configure(c);}

		~DescriptorComputation(){};

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void){/*not implemented*/ return false;}

		bool Do(Descriptor& input)
		{
			input.Compute();
			return true;
		}
	
		
		void StoreOn(Storage &s) {};
	protected:
		DescriptorComputationConfig mConfig;


		
	};

}

#endif 
