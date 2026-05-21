include_guard(GLOBAL)

function(clam_parse_version out_var product changes_file)
	if(NOT EXISTS "${changes_file}")
		message(FATAL_ERROR "Cannot parse ${product} version: missing ${changes_file}")
	endif()
	file(STRINGS "${changes_file}" _clam_changes LIMIT_COUNT 1)
	set(_version "0.0.0")
	if(_clam_changes MATCHES "${product}[ \t]+([0-9]+\\.[0-9]+\\.[0-9]+)")
		set(_version "${CMAKE_MATCH_1}")
	endif()
	set(${out_var} "${_version}" PARENT_SCOPE)
endfunction()

function(clam_generate_version_sources out_source out_include_dir)
	set(options)
	set(oneValueArgs NAMESPACE BASENAME PRODUCT CHANGES_FILE)
	cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "" ${ARGN})

	if(NOT ARG_NAMESPACE OR NOT ARG_BASENAME OR NOT ARG_PRODUCT OR NOT ARG_CHANGES_FILE)
		message(FATAL_ERROR "clam_generate_version_sources requires NAMESPACE, BASENAME, PRODUCT and CHANGES_FILE")
	endif()

	clam_parse_version(_clam_version "${ARG_PRODUCT}" "${ARG_CHANGES_FILE}")
	set(_gen_root "${CMAKE_CURRENT_BINARY_DIR}/generated/${ARG_BASENAME}")
	set(_include_dir "${_gen_root}/include")
	file(MAKE_DIRECTORY "${_include_dir}/CLAM")

	set(_header "${_include_dir}/CLAM/${ARG_BASENAME}.hxx")
	set(_source "${_gen_root}/${ARG_BASENAME}.cxx")

	file(WRITE "${_header}"
		"namespace ${ARG_NAMESPACE} { const char * GetFullVersion(); }\n"
		"namespace ${ARG_NAMESPACE} { const char * GetVersion(); }\n"
	)
	file(WRITE "${_source}"
		"#include <CLAM/${ARG_BASENAME}.hxx>\n"
		"namespace ${ARG_NAMESPACE} { const char * GetFullVersion() { return \"${_clam_version}\"; } }\n"
		"namespace ${ARG_NAMESPACE} { const char * GetVersion() { return \"${_clam_version}\"; } }\n"
	)

	set(${out_source} "${_source}" PARENT_SCOPE)
	set(${out_include_dir} "${_include_dir}" PARENT_SCOPE)
endfunction()

function(clam_collect_module_files out_sources out_headers out_include_dirs)
	set(options)
	set(oneValueArgs BASE_DIR)
	set(multiValueArgs FOLDERS BLACKLIST)
	cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if(NOT ARG_BASE_DIR)
		message(FATAL_ERROR "clam_collect_module_files requires BASE_DIR")
	endif()

	set(_source_extensions ".c" ".C" ".cc" ".cpp" ".cxx" ".c++")
	set(_header_extensions ".h" ".H" ".hh" ".hpp" ".hxx")
	set(_sources)
	set(_headers)
	set(_include_dirs)

	foreach(_folder IN LISTS ARG_FOLDERS)
		file(GLOB _matches CONFIGURE_DEPENDS "${ARG_BASE_DIR}/${_folder}")
		foreach(_match IN LISTS _matches)
			if(IS_DIRECTORY "${_match}")
				list(APPEND _include_dirs "${_match}")
				file(GLOB _entries CONFIGURE_DEPENDS "${_match}/*")
			else()
				get_filename_component(_match_dir "${_match}" DIRECTORY)
				list(APPEND _include_dirs "${_match_dir}")
				set(_entries "${_match}")
			endif()

			foreach(_entry IN LISTS _entries)
				if(IS_DIRECTORY "${_entry}")
					continue()
				endif()

				get_filename_component(_name "${_entry}" NAME)
				set(_blacklisted FALSE)
				foreach(_regex IN LISTS ARG_BLACKLIST)
					if(_name MATCHES "${_regex}" OR _entry MATCHES "${_regex}")
						set(_blacklisted TRUE)
					endif()
				endforeach()
				if(_blacklisted)
					continue()
				endif()

				get_filename_component(_ext "${_entry}" EXT)
				if(_ext IN_LIST _source_extensions)
					list(APPEND _sources "${_entry}")
				elseif(_ext IN_LIST _header_extensions)
					list(APPEND _headers "${_entry}")
				endif()
			endforeach()
		endforeach()
	endforeach()

	list(REMOVE_DUPLICATES _sources)
	list(REMOVE_DUPLICATES _headers)
	list(REMOVE_DUPLICATES _include_dirs)

	set(${out_sources} "${_sources}" PARENT_SCOPE)
	set(${out_headers} "${_headers}" PARENT_SCOPE)
	set(${out_include_dirs} "${_include_dirs}" PARENT_SCOPE)
endfunction()

function(clam_generate_flat_include_tree out_include_dir)
	set(options)
	set(oneValueArgs)
	set(multiValueArgs HEADERS)
	cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	set(_include_dir "${CMAKE_CURRENT_BINARY_DIR}/generated/legacy_flat_include")
	file(MAKE_DIRECTORY "${_include_dir}/CLAM")

	foreach(_header IN LISTS ARG_HEADERS)
		get_filename_component(_name "${_header}" NAME)
		file(TO_CMAKE_PATH "${_header}" _header_path)
		file(WRITE "${_include_dir}/CLAM/${_name}"
			"#pragma once\n"
			"#include \"${_header_path}\"\n"
		)
	endforeach()

	set(${out_include_dir} "${_include_dir}" PARENT_SCOPE)
endfunction()

function(clam_set_legacy_cxx target)
	set_target_properties(${target} PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS YES
	)
	if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
		target_compile_options(${target} PRIVATE -Wall)
	endif()
endfunction()
