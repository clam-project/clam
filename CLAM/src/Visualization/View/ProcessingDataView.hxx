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

#ifndef __PROCESSINGDATAVIEW__
#define __PROCESSINGDATAVIEW__

#include "View.hxx"

namespace CLAM
{

class ProcessingData;

}

namespace CLAMGUI
{

	using CLAM::ProcessingData;

/**
 *  This abstract class expresses the required interface
 *  for all Views that observe CLAM ProcessingData's.
 */

class ProcessingDataView : public View
{
public:

		virtual ~ProcessingDataView()
		{
		}

		/**
		 *  @see View::GetClassName
		 */	
		virtual const char* GetClassName() const  = 0;

		/**
		 *  @see View::GetAspect
		 */
		virtual Aspect& GetAspect() = 0;

		/**
		 *  @see View::Refresh
		 */
		virtual bool Refresh() = 0;

		/**
		 *  This method stores a reference to the observed model object,
		 *  checking that the provided object is consistent ( from the
		 *  view point of view, of course). Consistency is enforced depending
		 *  on the cost of the operations required to achieve such consistency.
		 *  @param Pointer to the processing data to be observed
		 *  @return A boolean telling us if it was possible to bind 
		 *          the view with the ProcessingData provided
		 */
		virtual bool BindTo( const ProcessingData* procDataObj ) = 0;

};

}

#endif // ProcessingDataView.hxx
