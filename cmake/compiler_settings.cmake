function(set_common_library_compiler_settings targetname)
	if("${HOST_COMPILER}" STREQUAL "MSVC")
		target_compile_options(${targetname} PRIVATE /W4)
	else()
		# Unfortunately, we can't apply -pedantic here without some
		# significant changes to the codebase. When pedantic (ISO C)
		# compilation is enabled, passing non-const arrays into
		# functions which accept const arrays does not work. I don't
		# know why an array of non-const items is incompatible with
		# an argument accepting const items, but in any case, this
		# causes a lot of issues with functions which accept const
		# arrays as parameters, like matrix functions. The ideal
		# solution to this problem would be to represent matrices
		# and vectors as structs, and pass pointers to those, but
		# this would be a significant change to the codebase that
		# I don't want to make right now. Until that point, we have
		# to compile with -pedantic disabled. :c
		target_compile_options(${targetname} PRIVATE
			# This is very important, as without it the engine gets
			# confused as to where the filesytem global variables
			# (FI) actually live. I don't know why. This is a lesson
			# in not exposing global mutable variables across
			# library boundaries.
			-fvisibility=hidden

			-Wall
			-Wextra
		)

		target_link_options(${targetname} PRIVATE
			-Wl,--no-undefined
		)

		target_compile_definitions(${targetname} PRIVATE $<$<CONFIG:Debug>:_DEBUG=1>)
	endif()
endfunction()
