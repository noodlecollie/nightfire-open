function(set_common_library_compiler_settings targetname)
	if(HOST_PLATFORM STREQUAL "LINUX")
		target_compile_options(${targetname} PRIVATE
			-fvisibility=hidden
			$<$<STREQUAL:${LIB_MODE},"SHARED">: -fPIC >
		)

		target_link_options(${targetname} PRIVATE
			-Wl,--no-undefined
		)
	endif()
endfunction()
