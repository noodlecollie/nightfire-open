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
