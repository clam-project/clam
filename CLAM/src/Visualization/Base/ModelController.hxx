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

#ifndef __MODELCONTROLLER__
#define __MODELCONTROLLER__

#include "ModelAdapter.hxx"

namespace CLAMVM
{

/**
 *   This class is just a way to accomplish finer grain control
 *   for defining concrete view interface since many views won't
 *   be able to modify the model ( the components of applications )
 *   whatsoever.
 * 
 *   @see ProcessingModelAdapter
 */
class ModelController
		: public ModelAdapter
{
public:
		virtual ~ModelController()
		{
		}

		/**
		 *  @see ModelAdapter::GetClassName
		 */
		virtual const char* GetClassName() const = 0;

		/**
		 *  ModelControllers are also able to Publish
		 *  @see ModelAdapter::Publish
		 */
		virtual bool Publish() = 0;

		/**
		 *  This pure virtual method expresses a view ability to
		 *  to modify the model as sees fit.
		 *  @returns A boolean indicating wether it has been possible 
		 *           to update the model object associated to this controller
		 */
		virtual bool Update() = 0;
};

}
#endif // ModelController.hxx
