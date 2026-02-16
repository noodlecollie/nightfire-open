function(construct_content_path in_path out_var)
	if(NOT "${in_path}" STREQUAL "")
		cmake_path(IS_RELATIVE in_path content_path_is_relative)

		if(${content_path_is_relative})
			set(full_path "${CMAKE_SOURCE_DIR}/${in_path}")
		else()
			set(full_path "${in_path}")
		endif()

		if(NOT EXISTS "${full_path}")
			message(FATAL_ERROR "Could not find content path: ${full_path}")
		endif()

		set(${out_var} "${full_path}" PARENT_SCOPE)
	endif()
endfunction()

# Adapted from https://stackoverflow.com/a/75065206/2054335
macro(install_dependencies_from_directory target_name dir_path)
	set(options "")
	set(single_value_args TARGET)
	set(multi_value_args DIRECTORIES)

	cmake_parse_arguments(INSTALLDEPS "${options}" "${single_value_args}" "${multi_value_args}" ${ARGN})

	install(TARGETS ${INSTALLDEPS_TARGET}
		COMPONENT ${INSTALLDEPS_TARGET}
		RUNTIME_DEPENDENCIES
		# Avoid installing system libs
		PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-" "ld-linux-" "libc\\.so" "libm\\.so"
		POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
		DIRECTORIES
		${dir_path}
		ARCHIVE DESTINATION ${INSTALL_ROOT}
		LIBRARY DESTINATION ${INSTALL_ROOT}
		RUNTIME DESTINATION ${INSTALL_ROOT}
	)
endmacro()
