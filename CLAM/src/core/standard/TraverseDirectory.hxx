/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _TraverseDirectory_hxx_
#define _TraverseDirectory_hxx_

#include <string>

class TraverseDirectory
{
public:
	TraverseDirectory() = default;
	virtual ~TraverseDirectory() = default;

	/**
	* Walks the tree rooted at @p rootname, calling OnDirectory() / OnFile()
	* for each entry. @p maxdepth limits recursion depth (immediate children
	* live at depth 0); -1 means unlimited.
	*/
	void Traverse(const std::string& rootname = "", int maxdepth = -1);

protected:
	virtual void OnFile(const std::string& filename) {}
	virtual void OnDirectory(const std::string& dirname) {}

	/** Helper method for client classes: returns the extension without the leading dot. */
	std::string GetExtension(const std::string& filename);
};

#endif
