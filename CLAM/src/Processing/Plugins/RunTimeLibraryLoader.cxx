#include "RunTimeLibraryLoader.hxx"
#include "ProcessingFactory.hxx"

#include <iostream>
#include <cstdlib>
#include <filesystem>
#ifdef WIN32
#	include "CLAM_windows.h"
#else
#	include <dlfcn.h>
#endif

#include <fstream>

#include "ProcessingFactory.hxx"


/// Environment variable to enable verbose plugin loading message
static const char * debugEnvFlag = "CLAM_DEBUG_PLUGINS";

void RunTimeLibraryLoader::ReLoad() 
{
	CLAM::ProcessingFactory& factory = CLAM::ProcessingFactory::GetInstance();
	auto usedLibraries = GetUsedLibraries();
	for (const auto& library : usedLibraries)
	{
		auto keys = factory.GetKeys("library", library);
		for (const auto& key : keys)
		{
			factory.DeleteCreator(key);
		}
		if (needReleaseHandlerOnReload())
		{
			void* handle = GetLibraryHandler(library);
			ReleaseLibraryHandler(handle, library);
		}
	}
	Load();
}


std::list<std::string> RunTimeLibraryLoader::GetUsedLibraries()
{
	CLAM::ProcessingFactory& factory = CLAM::ProcessingFactory::GetInstance();
	std::list<std::string> usedLibraries;
	auto librariesValues = factory.GetSetOfValues("library");
	for (const auto& library : librariesValues)
	{
		const std::string& path = getPathFromFullFileName(library);
		if (IsOnPath(path))
			usedLibraries.push_back(library);
	}
	return usedLibraries;
}

bool RunTimeLibraryLoader::IsOnPath(const std::string & path) const
{
	std::string paths = GetPaths();
	std::vector <std::string> environmentPaths = SplitPathVariable(paths);
	for (unsigned i=0; i<environmentPaths.size(); i++)
	{
		if (environmentPaths[i]==path)
			return true;	
	}
	return false;
}

void RunTimeLibraryLoader::Load() 
{
	std::string path = GetPaths();
	// for each path, load libraries
	std::vector <std::string> environmentPaths = SplitPathVariable(path);
	for (unsigned i=0; i<environmentPaths.size(); i++)
	{
		if (getenv(debugEnvFlag))
			std::cout << "RunTimeLibraryLoader: Scanning for libraries in " << environmentPaths[i] << std::endl;
		LoadLibrariesFromPath(environmentPaths[i]);
	}
}

bool isDynamicLibrary(const std::string & file)
{
	std::size_t found = file.rfind(".");
	if (found == std::string::npos) 
		return true;

	std::string extension = file.substr( file.rfind(".")); 
	bool result = ( false 
		#ifdef WIN32
			|| extension==".dll"
		#else
			|| extension==".so" 
			|| extension==".dylib"
		#endif
		);
	return result;

}

void RunTimeLibraryLoader::LoadLibrariesFromPath(const std::string & path)
{
	std::error_code ec;
	if (!std::filesystem::is_directory(path, ec)) return;

	auto& factory = CLAM::ProcessingFactory::GetInstance();
	for (const auto& entry : std::filesystem::directory_iterator(path, ec))
	{
		const std::string pluginFilename = entry.path().filename().string();
		if (!isDynamicLibrary(pluginFilename))
			continue;
		if (getenv(debugEnvFlag))
			std::cout << "RunTimeLibraryLoader: Found file " << pluginFilename << std::endl;
		const std::string pluginFullFilename = entry.path().string();

		if (factory.isLibraryLoaded(pluginFullFilename))
		{
			if (getenv(debugEnvFlag))
				std::cout << "RunTimeLibraryLoader: Already loaded, skiping..." << std::endl;
			continue;
		}
		factory.setLibraryAsLoaded(pluginFullFilename);

		void* handle = FullyLoadLibrary(pluginFullFilename);

		// TODO: throw exception and have catch in main()
		if (handle == nullptr)
		{
			std::cout << "RunTimeLibraryLoader: Error loading: " << pluginFullFilename
					  << " reason: " << LibraryLoadError()
					  << std::endl;
		}
		else if (getenv(debugEnvFlag))
			std::cout << "RunTimeLibraryLoader: Loaded" << std::endl;

		SetupLibrary(handle, pluginFullFilename);
	}
}


void * RunTimeLibraryLoader::FullyLoadLibrary(const std::string & libraryPath)
{
#ifdef WIN32
//		SetErrorMode(
//			SEM_FAILCRITICALERRORS |
//			SEM_NOALIGNMENTFAULTEXCEPT |
//			SEM_NOGPFAULTERRORBOX |
//			SEM_NOOPENFILEERRORBOX );
	return LoadLibrary(libraryPath.c_str());
#else
	return dlopen( libraryPath.c_str(), RTLD_NOW);
#endif
}


void * RunTimeLibraryLoader::LazyLoadLibrary(const std::string & libraryPath)
{
#ifdef WIN32
//	TODO: if windows allow it, add here a lazy load
	return 0; 
#else
	return dlopen( libraryPath.c_str(), RTLD_LAZY);
#endif
}


void * RunTimeLibraryLoader::GetSymbol(void * libraryHandler, const std::string & symbolName)
{
#ifdef WIN32
	return (void*) GetProcAddress((HMODULE)libraryHandler, symbolName.c_str());
#else
	return dlsym(libraryHandler, symbolName.c_str());
#endif
}


void * RunTimeLibraryLoader::GetLibraryHandler(const std::string & libraryPath) const
{
#ifdef WIN32 //TODO: does windows have an equivalent method to have the handler?
	return 0;
#else
	return dlopen (libraryPath.c_str(), RTLD_NOLOAD);
#endif
}

//TODO: the name argument will be used to check on the plugins map 
// returns false on success, true on fail
bool RunTimeLibraryLoader::ReleaseLibraryHandler(void* handle, const std::string pluginFullFilename)
{
	if (!handle)
	{
		std::cout<<"Cannot release an empty handle!"<<std::endl;
		return true;
	}
#ifdef WIN32
	return !FreeLibrary((HMODULE)handle);
#else 
	return dlclose(handle);
#endif
}

const std::string RunTimeLibraryLoader::LibraryLoadError()
{
#ifdef WIN32
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, nullptr );
	std::string message((char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
	return message;
#else
	return dlerror();
#endif
}

const std::string RunTimeLibraryLoader::FileOfSymbol (void * symbolAddress)
{
#ifdef WIN32
	char pluginpath[1024]; 
	HMODULE module; 
	GetModuleHandleExA(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, 
		(char *)symbolAddress, 
		&module); 
	GetModuleFileNameA(module, pluginpath, 1024); 
	FreeLibrary(module); 
	return pluginpath;
#else
	Dl_info info;
	int ok=dladdr(symbolAddress,&info);
	if (ok)
		return info.dli_fname;
#endif
	return "";
}

std::vector<std::string> RunTimeLibraryLoader::SplitPathVariable(const std::string & pathVariable) const
{
	std::string content=pathVariable;
	std::vector<std::string> result;
	while (!content.empty())
	{
		size_t separatorPos = content.find(pathSeparator());
		if (separatorPos == std::string::npos)
		{
			result.push_back(content);
			break;
		} 
		result.push_back(content.substr(0, separatorPos));
		content = content.substr(separatorPos+1);
	}
	return result;
}

const std::string RunTimeLibraryLoader::GetPaths() const
{
	const char * envPath = getenv(pathEnvironmentVar());
	const char * envHome = getenv("HOME");
	std::string path = envPath ? envPath : "";
	if (envHome)
	{
		path += std::string(path.empty()? "":pathSeparator()) + envHome + homePath();
	}
	// add standardPath to the env path string (i.e. path1:path2)
	for (const char ** standardPath=standardPaths(); *standardPath; standardPath++)
		path += std::string(path.empty()? "":pathSeparator()) + *standardPath;
	return path;
}


const std::string RunTimeLibraryLoader::CompletePathFor(const std::string & subpathAndName) const 
{
	std::string paths=GetPaths();
	std::vector<std::string> environmentPaths = SplitPathVariable(paths);
	for (const auto& envPath : environmentPaths)
	{
		const std::string fileName = subpathAndName.substr(subpathAndName.rfind("/") + 1);
		const std::string testDir = envPath + "/" + subpathAndName.substr(0, subpathAndName.size() - fileName.size());
		std::error_code ec;
		if (!std::filesystem::is_directory(testDir, ec))
			continue;
		const std::string completeFileName = testDir + fileName;
		if (!std::filesystem::is_regular_file(completeFileName, ec))
			continue;
		return completeFileName;
	}
	return "";
}
