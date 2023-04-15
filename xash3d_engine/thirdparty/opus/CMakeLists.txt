set(OPUS_BUILD_SHARED_LIBRARY OFF CACHE BOOL "" FORCE)
set(OPUS_CUSTOM_MODES ON CACHE BOOL "" FORCE)
set(OPUS_INSTALL_PKG_CONFIG_MODULE OFF CACHE BOOL "" FORCE)
set(OPUS_INSTALL_CMAKE_CONFIG_MODULE OFF CACHE BOOL "" FORCE)

# A slow speed warning message is printed if Opus is compiled without optimisations.
# We never want this, so just force -O3 here for GCC.
if(NOT MSVC)
add_compile_options("-O3")
endif()

set(OLD_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(opus EXCLUDE_FROM_ALL)
set(BUILD_SHARED_LIBS ${OLD_BUILD_SHARED_LIBS})
