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

#ifndef __MODEL_ADAPTER__
#define __MODEL_ADAPTER__

namespace CLAM
{
class Mutex;
}

namespace CLAMVM
{

using CLAM::Mutex;

class Aspect;

/**
 *  This class defines the interface common to all model object adapters available in CLAM
 *  Visualization module. The ModelAdapter encodes the interface required by the Observable
 *  concept of the GOF Observer pattern. We chose to write an Adapter, since we didn't feel
 *  that tainting the model object interface satisfied our goal of separating effectively
 *  the Model objects from its representation.
 */

class ModelAdapter
{
public:
		virtual ~ModelAdapter()
		{
		}

		/**
		 *   Returns the class name of the view. It's purpose as of today is
		 *   to provide run-time debugging information.
		 *   @returns A human-readable class name
		 */

		virtual const char* GetClassName() const = 0;

		/**
		 *   Returns a non-const reference to the aspect supported by concrete
		 *   views, so a Presentation class can use it to select which kind of
		 *   events wants to be notified.
		 *   @returns A reference to the concrete aspect
		 *   @see Aspect
		 *   @see Presentation
		 */

		virtual Aspect& GetAspect() = 0;
		
		/**
		 *   The main message to be send to any view. Its purpose is the same
		 *   as the Observer::broadcast() method found in the Observer pattern.
		 *   @returns A boolean indicating wether it has been possible to perform the refresh
		 */

		virtual bool Publish() = 0;

		/**
		 *   This is a proxy method that ensures that ModelAdapter::Refresh() is an
		 *   atomic operation. Users must provide a CLAM::Mutex object when
		 *   necessary.
		 */

		void Publish( Mutex& );
};

}

#endif // ModelAdapter.hxx
