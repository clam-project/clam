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
#ifndef __AUDIOPRESENTATION__
#define __AUDIOPRESENTATION__

#include "Presentation.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Slotv4.hxx"

namespace CLAMVM
{
		using SigSlot::Slotv4;
		using CLAM::DataArray;
		using CLAM::TData;
		using CLAM::TTime;

		class AudioPresentation : public Presentation
		{
		private:
				TData mMaximum;
				TData mMinimum;
				
		protected:

				virtual void Bind( Aspect& ) throw( std::bad_cast );

//slots functions:
				virtual void OnNewAudio( const DataArray&, TTime, TTime, TData ) = 0;
// Accessors to received data
				
		public:

				AudioPresentation();

				~AudioPresentation();

				virtual void Show() = 0;

				Slotv4<const DataArray&, TTime, TTime, TData> SetAudio;
		};

}


#endif // AudioPresentation.hxx
