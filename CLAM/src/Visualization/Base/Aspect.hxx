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
#ifndef __ASPECT__
#define __ASPECT__

namespace CLAMVM
{

/**
 *  This class is the base class for all view aspects. An aspect
 *  is the interface used by Presentations to interact with views,
 *  and expresses what can do a view for a presentation. This way
 *  coupling between views and presentations is quite low, and many
 *  views can share the same aspect, without needing the presentation
 *  to know it.
 *  BTW, base classes declaring just a virtual destructor aren't 
 *  "outstanding" solutions, but they sometimes come in handy.
 *
 *  @see View
 *  @see Presentation
 */
class Aspect
{
public:

		/**
		 *   This pure-virtual method allows Presentations to force
		 *   the view behind the Aspect to do a Refresh().
		 *
		 *   @see View
		 */
		virtual void ForceViewRefresh() = 0;

		virtual ~Aspect()
		{
		}
};

}

#endif // Aspect.hxx
