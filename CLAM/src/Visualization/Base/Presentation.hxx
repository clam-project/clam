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

namespace CLAMVM
{

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
		
		//@todo: the destructor should not be present in the header, so I should move it
		//       down to the implementation file ( broking several makefiles and dsp's :/ )
		virtual ~Presentation();

		/**
		 *   Calling this method makes the widget to appear on the screen
		 */
		virtual void Show() = 0;

		/**
		 *   Calling this method makes the widget to disappear from the screen,
		 *   so it should not be confused with concepts such as 'minimize' or
		 *   'iconify'.
		 */
		virtual void Hide() = 0;

};

}



#endif // Presentation.hxx
