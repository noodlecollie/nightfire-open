function(set_common_library_compiler_settings targetname)
	if(HOST_COMPILER STREQUAL "MSVC")
		add_compile_options(/W4 /WX)
		add_link_options(/WX)
	else()
		target_compile_options(${targetname} PRIVATE
			-fvisibility=hidden
			# NFTODO: Apply these properly
			# -Wall
			# -Wextra
			# -pedantic
			# -Werror
		)

		target_link_options(${targetname} PRIVATE
			-Wl,--no-undefined
			-Wl,--fatal-warnings
		)
	endif()
endfunction()
