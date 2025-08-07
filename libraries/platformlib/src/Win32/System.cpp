#include "PlatformLib/System.h"
#include <process.h>
#include <direct.h>
#include <vector>
#include "whereami/whereami.h"

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

int PlatformLib_ExecV(const char* path, char* const* argv)
{
	return static_cast<int>(_execv(path, argv));
}

char* PlatformLib_GetCWD(char* buffer, size_t maxlen)
{
	return _getcwd(buffer, static_cast<int>(maxlen));
}

const char* PlatformLib_GetEnv(const char* envVarName)
{
	static std::vector<char> cachedValue;

	// Make a first call to work out how big the value is.
	char dummy;
	size_t requiredSize = 0;
	errno_t result = getenv_s(&requiredSize, &dummy, sizeof(dummy), envVarName);

	if ( (result != 0 && result != ERANGE) || requiredSize < 1 )
	{
		// An error that we don't handle, or the var was not found.
		return nullptr;
	}

	// 99.9% of the time the above call should return ERANGE.
	// If it returns 0, then somehow the value fitted into a single char,
	// which I guess means the string is just empty?
	// Either way, make the call again with the properly sized buffer,
	// so that we can return it.

	cachedValue.resize(requiredSize);
	result = getenv_s(&requiredSize, cachedValue.data(), cachedValue.size(), envVarName);

	return (result == 0 && requiredSize > 0) ? cachedValue.data() : nullptr;
}

int PlatformLib_GetExecutablePath(char* out, int capacity, int* dirname_length)
{
	return wai_getExecutablePath(out, capacity, dirname_length);
}

int PlatformLib_GetModulePath(char* out, int capacity, int* dirname_length)
{
	return wai_getModulePath(out, capacity, dirname_length);
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
