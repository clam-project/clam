from SCons.Action import *
from buildtools import *

def handle_preinclude ( env ):
	env.Append(CCFLAGS='-include CLAM/%s'%env['preinclude'])
	return

def posix_lib_rules( name, version, headers, source_files, install_dirs, core_env) :
	lib_descriptor = core_env.File( 'clam_core.pc' )

	soname = 'libclam_core.so.%s'%version.split('.')[0]
	linker_name = 'libclam_core.so'
	core_env.Append(SHLINKFLAGS=['-Wl,-soname,%s'%soname ] )

	lib = core_env.SharedLibrary( 'clam_core', source_files, SHLIBSUFFIX='.so.%s'%version )

	soname_lib = core_env.SonameLink( soname, lib )
	linkername_lib = core_env.LinkerNameLink( linker_name, soname_lib)

	core_tgt = core_env.Alias( name, linkername_lib )

	install_headers = core_env.Install( install_dirs.inc+'/CLAM', headers )
	core_env.AddPostAction( install_headers, "chmod 644 $TARGET" )

	install_lib = core_env.Install( install_dirs.lib, linkername_lib )
	core_env.AddPostAction( install_lib, Action(make_lib_names, make_lib_names_message ) )

	install_descriptor = core_env.Install( install_dirs.lib+'/pkgconfig', lib_descriptor )

	core_install_tgt = core_env.Alias( 'install_core', [install_headers, install_lib, install_descriptor] )

	runtime_lib = core_env.Install( install_dirs.lib, soname_lib )

	core_env.Alias( 'install_core_runtime', soname_lib )

	static_lib = core_env.Library( 'clam_core', source_files )
	install_static = core_env.Install( install_dirs.lib, static_lib )

	dev_linkername =  core_env.LinkerNameLink( install_dirs.lib+'/'+linker_name, install_dirs.lib+'/'+soname) 
	core_env.Alias( 'install_core_dev', [install_headers,dev_linkername, install_descriptor, install_static] )

	return core_tgt, core_install_tgt
