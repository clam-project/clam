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

#include "TraverseDirectory.hxx"
#include <filesystem>
#include <system_error>

namespace fs = std::filesystem;

void TraverseDirectory::Traverse(const std::string& rootname, int maxdepth)
{
	const fs::path root = rootname.empty() ? fs::path(".") : fs::path(rootname);
	std::error_code ec;
	if (!fs::is_directory(root, ec))
		return;

	OnDirectory(rootname);

	fs::recursive_directory_iterator it(root, fs::directory_options::skip_permission_denied, ec);
	const fs::recursive_directory_iterator end;
	while (!ec && it != end)
	{
		const bool isDir = it->is_directory(ec);
		if (ec) { ec.clear(); it.increment(ec); continue; }

		const std::string path = it->path().string();
		if (isDir)
		{
			OnDirectory(path);
			if (maxdepth >= 0 && it.depth() >= maxdepth)
				it.disable_recursion_pending();
		}
		else
		{
			OnFile(path);
		}

		it.increment(ec);
	}
}

std::string TraverseDirectory::GetExtension(const std::string& filename)
{
	const auto ext = fs::path(filename).extension().string();
	return ext.empty() ? std::string{} : ext.substr(1);
}
