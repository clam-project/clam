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

#ifndef __PRESENTATION__
#define __PRESENTATION__

#include <exception>

namespace CLAMGUI
{

class View;
class Aspect;

/**
 *   This is the interface required to all Visualization Module presentations.
 *   Presentations are the entities in charge of applying some kind of transformation
 *   to data received from attached views in order to make it more knowledgeable
 *   to users. Views and Presentations talk with each other through the Aspect supported
 *   by each View.
 *
 *   @see View
 *   @see Aspect
 */
class Presentation
{

public:
		/**
		 *  Presentation users should use this method to attach a given presentation
		 *  with an arbitrary View. The concrete association between the concrete
		 *  presentation and the concrete view is resolved in the Presentation::Bind
		 *  method.
		 */
		virtual void AttachTo( View& );

		virtual ~Presentation()
		{
		}

protected:
		/**
		 *  The Presentation base class forces all concrete classes derived from it
		 *  to implement this method, which should perform the concrete binding
		 *  between the Presentation and the attached view. 
		 *  @param Reference to the Aspect supported by the attached view
		 *  @throw This method must throw a bad_cast exception if the Aspect provided
		 *  by the view is not the one the Presentation expects
		 */
		virtual void Bind( Aspect& ) = 0;

};

}

#endif // Presentation.hxx
