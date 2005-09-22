#!/usr/bin/python
import os, re, sys

isHeaderRE = re.compile( r"[^\.#].+\.[hH][xp]?[xp]?\Z" )
macro = re.compile( r"CLAM_DLL_EXPORT" )

def remove_macro( filename ) :
	newFile = []
	isDirty = False
	in_stream = file( filename, 'r' )
	for line in in_stream :
		match = macro.search(line)
		if match is not None :
			newFile.append( macro.sub( '', line ) )
			isDirty = True
		else :
			newFile.append( line )

	in_stream.close()

	if not isDirty : return

	print >> sys.stdout, "\tFixing", filename, "..."

	out_stream = file( filename, 'w' )
	out_stream.write( "".join( newFile ) )
	out_stream.close()
	

def is_header( filename ) :
	if isHeaderRE.search( filename ) is not None :
		return True
	return False

def headers( folder ) :
	for root, _, filelist in os.walk( folder ) :
		for filename in filelist :
			if is_header( filename ) :
				yield "%s/%s"%(root, filename )

def process_folder( folder_name ) :
	for fname in headers( folder_name ) :
		print >> sys.stdout, "Removing macros from", fname, "..."
		remove_macro( fname )

def main() :
	if len( sys.argv ) < 2 :
		print >> sys.stderr, "Insufficient parameters"
		sys.exit(-1)

	base_folder = sys.argv[1]

	process_folder( base_folder )

if __name__ == '__main__' :
	main()
