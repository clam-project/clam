#include "TraverseDirectory.hxx"
#include <sys/types.h>

TraverseDirectory::TraverseDirectory(void)
{

}
// Helper method for TraverseHelper
bool TraverseDirectory::IsCurrentOrParentDir(dirent* dirEntry) const
{
	return !strcmp(dirEntry->d_name,".") || !strcmp(dirEntry->d_name,"..");
}

std::string TraverseDirectory::CompleteName(const std::string& currentDirName, dirent* dirEntry) const
{
	bool noDirName = currentDirName == "";
	return noDirName? dirEntry->d_name : currentDirName+"/"+dirEntry->d_name;
}

void TraverseDirectory::TraverseHelper( DIR* dir, const std::string& currentDirname,
	int curdepth, int maxdepth )
{
	dirent* dirEntry;
	while ((dirEntry = readdir(dir)))
	{
		if (IsCurrentOrParentDir(dirEntry))
			continue;

		std::string currentItemName = CompleteName(currentDirname, dirEntry);
		DIR* subdir = opendir(currentItemName.c_str());
		if (subdir)
		{
			OnDirectory(currentItemName); // 'template method'
			if (curdepth<maxdepth || maxdepth==-1)
			{
				TraverseHelper(subdir, currentItemName, curdepth+1, maxdepth);
			}
			closedir(subdir);
		}else
		{
			OnFile(currentItemName); // 'template method'
		}
	}
}

void TraverseDirectory::Traverse(const std::string& rootname,int maxdepth)
{
	DIR* dir;

	dir = opendir(rootname == "" ? "." : rootname.c_str());

	if (dir)
	{
		OnDirectory(rootname);
		TraverseHelper(dir,rootname,0,maxdepth);
		closedir(dir);
	}
}


//Auxiliary function to return the extension of a given filename
std::string TraverseDirectory::GetExtension(const std::string& filename)
{
   	return filename.substr(filename.rfind('.')+1);	
}

