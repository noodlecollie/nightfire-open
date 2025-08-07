set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(OPUS_BUILD_SHARED_LIBRARY OFF CACHE BOOL "" FORCE)
set(OPUS_CUSTOM_MODES ON CACHE BOOL "" FORCE)
set(OPUS_INSTALL_PKG_CONFIG_MODULE OFF CACHE BOOL "" FORCE)
set(OPUS_INSTALL_CMAKE_CONFIG_MODULE OFF CACHE BOOL "" FORCE)

# A slow speed warning message is printed if Opus is compiled without optimisations.
# We never want this, so just force -O3 here for GCC.
if(NOT MSVC)
	add_compile_options("-O3")
endif()

if(MSVC)
	# On MSVC there is some warning spam about "macro expansion producing 'defined' has undefined behavior".
	# I'm not sure where this comes from and haven't been able to work it out.
	# It seems to be produced by the use of the _CRT_INTERNAL_NONSTDC_NAMES macro.
	# The closest issue I've been able to find in the wild is https://stackoverflow.com/q/65402366
	# which seems to imply that CMake may be at fault, but I don't know how or why.
	# For now, we just disable this warning for Opus.
	add_compile_options("/wd5105")
else()
	# This is thrown on newer GCCs and I don't have the nerve to dig through
	# this third party code to work out why. We should update to a newer
	# version at some point.
	add_compile_options("-Wno-maybe-uninitialized")
endif()

set(OLD_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(opus EXCLUDE_FROM_ALL)
set(BUILD_SHARED_LIBS ${OLD_BUILD_SHARED_LIBS})
