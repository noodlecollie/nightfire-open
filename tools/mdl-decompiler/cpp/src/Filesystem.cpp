// From https://stackoverflow.com/a/29828907/2054335

#include "Filesystem.h"
#include <string>
#include <sys/stat.h>  // stat
#include <errno.h>  // errno, ENOENT, EEXIST
#include <limits.h>

#if defined(_WIN32)
#include <direct.h>  // _mkdir
#else
#include <unistd.h>
#endif

static size_t FindFinalPathSeparator(const std::string& path)
{
	size_t pos = path.find_last_of('/');

#if defined(_WIN32)
	if ( pos == std::string::npos )
	{
		pos = path.find_last_of('\\');
	}
#endif

	return pos;
}

static int MkDir(const std::string& path)
{
#if defined(_WIN32)
	return _mkdir(path.c_str());
#else
	constexpr mode_t MODE = 0755;
	return mkdir(path.c_str(), MODE);
#endif
}

static std::string SanitisePath(std::string path)
{
	while ( *path.rbegin() == '\\' )
	{
		path.pop_back();
	}

	return path;
}

bool DirectoryExists(const std::string& path)
{
	const std::string sanitisedPath = SanitisePath(path);

#if defined(_WIN32)
	struct _stat info;

	if ( _stat(sanitisedPath.c_str(), &info) != 0 )
	{
		return false;
	}

	return (info.st_mode & _S_IFDIR) != 0;
#else
	struct stat info;

	if ( stat(sanitisedPath.c_str(), &info) != 0 )
	{
		return false;
	}

	return (info.st_mode & S_IFDIR) != 0;
#endif
}

bool FileExists(const std::string& path)
{
	const std::string sanitisedPath = SanitisePath(path);

#if defined(_WIN32)
	struct _stat info;

	if ( _stat(sanitisedPath.c_str(), &info) != 0 )
	{
		return false;
	}

	return (info.st_mode & _S_IFREG) != 0;
#else
	struct stat info;

	if ( stat(sanitisedPath.c_str(), &info) != 0 )
	{
		return false;
	}

	return (info.st_mode & S_IFREG) != 0;
#endif
}

bool MakeDirectoryRecursive(const std::string& path)
{
	int ret = MkDir(path);

	if ( ret == 0 )
	{
		return true;
	}

	switch ( errno )
	{
		case ENOENT:
		{
			// parent didn't exist, try to create it
			{
				size_t pos = FindFinalPathSeparator(path);

				if ( pos == std::string::npos )
				{
					return false;
				}

				if ( !MakeDirectoryRecursive(path.substr(0, pos)) )
				{
					return false;
				}
			}

			// now, try to create again
			return MkDir(path) == 0;
		}

		case EEXIST:
		{
			return DirectoryExists(path);
		}

		default:
		{
			return false;
		}
	}
}

std::string GetCurrentDirectory()
{
#if defined(_WIN32)
	char cwd[_MAX_PATH];
	cwd[0] = '\0';

	if ( !_getcwd(cwd, sizeof(cwd)) )
	{
		cwd[0] = '\0';
	}

	return std::string(cwd);
#else
	char cwd[PATH_MAX];
	cwd[0] = '\0';

	if ( !getcwd(cwd, sizeof(cwd)) )
	{
		cwd[0] = '\0';
	}

	return std::string(cwd);
#endif
}
