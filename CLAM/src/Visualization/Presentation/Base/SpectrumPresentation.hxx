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

#ifndef __SPECTRUMPRESENTATION__
#define __SPECTRUMPRESENTATION__

// Includes of new Visualization Module
#include "Presentation.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Slotv2.hxx"

namespace CLAMVM
{
		using SigSlot::Slotv2;
		using CLAM::DataArray;
		using CLAM::TData;
		using CLAM::TTime;
		
		class SpectrumPresentation : public Presentation
		{
		private:
				
		protected:
				virtual void Bind( Aspect& ) throw ( std::bad_cast );

				virtual void OnNewSpectrum( const DataArray&, TData ) = 0;
		public:
    
				SpectrumPresentation();

				~SpectrumPresentation();

				virtual void Show() = 0;

				Slotv2< const DataArray&, TData >       SetSpectrum;
				
		};
}


#endif // SpectrumPresentation.hxx
