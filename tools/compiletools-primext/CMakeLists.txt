# NFTODO: Evaluate whether this is worth it. It only applies to Windows anyway.
set(ENABLE_MAP_COMPILERS_AVX2 OFF)

# NFTODO: The compile tools needed these, apparently. Hopefully we can
# replace then with what we have instead.
# add_subdirectory("common/imagelib" imagelib)
# add_subdirectory("${CMAKE_SOURCE_DIR}/external/squish" squish)

add_subdirectory(common)
add_subdirectory(pxcsg)
add_subdirectory(pxbsp)
add_subdirectory(pxvis)
add_subdirectory(pxrad)
