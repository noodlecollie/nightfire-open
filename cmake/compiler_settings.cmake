function(set_common_library_compiler_settings targetname)
	if(NOT HOST_COMPILER STREQUAL "MSVC")
		target_compile_options(${targetname} PRIVATE
			-fvisibility=hidden
		)

		target_link_options(${targetname} PRIVATE
			-Wl,--no-undefined
		)
	endif()
endfunction()
