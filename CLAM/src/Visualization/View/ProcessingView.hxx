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

#ifndef __PROCESSINGVIEW__
#define __PROCESSINGVIEW__

#include "ModelUpdatingView.hxx"

namespace CLAM
{
		class Processing;
}

namespace CLAMGUI
{

using CLAM::Processing;

/**
 *  This abstract class expresses the required interface
 *  for all Views that observe CLAM Processing's.
 */

class ProcessingView : public ModelUpdatingView
{

public:
		virtual ~ProcessingView()
		{
		}

		/**
		 *  @see View::GetClassName
		 */
		virtual const char* GetClassName() const = 0;

		/**
		 *  @see View::GetAspect
		 */
		virtual Aspect& GetAspect() = 0;

		/**
		 *  @see View::Refresh
		 */
		virtual bool Refresh() = 0;
		
		/**
		 *  @see View::UpdateModel
		 */
		virtual bool UpdateModel() = 0;

		/**
		 *  This method stores a reference to the observed model object,
		 *  and checks the provided object consistency (i.e. determined buffers
		 *  are instantiated, etc. ), forcing it depending on the cost of such
		 *  an operation.
		 *  @param   Pointer to the processing to be observed
		 *  @returns A boolean telling us wether it was possible to bind the view
		 *           with the Processing provided
		 */
		virtual bool BindTo( Processing* procObj ) = 0;
};
	
}

#endif // ProcessingView.hxx
