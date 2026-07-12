#!/usr/bin/python

# Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
#                         UNIVERSITAT POMPEU FABRA
#
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


import sys
import re

def Lab2Plot(labfile, output, attributeName, filter, childScope, labelAttribute):
	print('<?xml version="1.0" encoding="UTF-8" standalone="no" ?>', file=output)
	print('<DescriptorsPool>', file=output)
	print('\t<ScopePool name="Song" size="1">', file=output)
	tokens = list()
	labelFilter = re.compile(filter)
	for line in labfile:
		segment = line.split(" ",3)
		if len(segment)!=3:
			print("Found a line with %s tokens, 3 expected"%len(segment), file=sys.stderr)
			continue
		segment[2]=segment[2].rstrip().lstrip()
		if not labelFilter.match(segment[2]): continue
		tokens.append(segment)

	sampleRate=44100

	print('\t\t<AttributePool name="%s" size="%s">'%(attributeName, len(tokens)), end=' ', file=output)
	for segment in tokens :
		print(float(segment[0])*sampleRate, float(segment[1])*sampleRate, end=' ', file=output)
	print('</AttributePool>', file=output)
	print('\t</ScopePool>', file=output)

	if childScope!=None:
		print('\t<ScopePool name="%s" size="%s">' % (childScope, len(tokens)), file=output)
		print('\t\t<AttributePool name="%s">' % (labelAttribute), file=output)

		for segment in tokens :
			print('\t\t\t<Enumerated>%s</Enumerated>' % ( segment[2] ), file=output)
		print('\t\t</AttributePool>', file=output)
		print('\t</ScopePool>', file=output)
	print('</DescriptorsPool>', file=output)
	return dict.fromkeys([ segment[2] for segment in tokens ]).keys() # unique labels


if __name__ == "__main__":
	# Default parameters
	filter = '.*'
	childScope = None
	labelAttribute = None

	if len(sys.argv) == 3:
		(labfilename,attributeName) = sys.argv[1:3]
	elif len(sys.argv) == 4:
		(labfilename,attributeName,filter) = sys.argv[1:4]
	elif len(sys.argv) == 5:
		(labfilename,attributeName,childScope,labelAttribute) = sys.argv[1:5]
	elif len(sys.argv) == 6:
		(labfilename,attributeName,filter,childScope,labelAttribute) = sys.argv[1:6]
	else:
		print("Lab2Pools v1.0: Wavesurfer Lab file to CLAM Annotator pools converter", file=sys.stderr)
		print(file=sys.stderr)
		print("Usage: " + sys.argv[0] + " <labFile> <attributeName> [<labelFilter>] [<childScope> <labelAttribute>]", file=sys.stderr)
		print(file=sys.stderr)
		print(" <labFile>        An annotation file from wave surfer", file=sys.stderr)
		print(" <attributeName>  Attribute name for the segmentation", file=sys.stderr)
		print(" <labelFilter>    A regular expresion to filter segments by label (default: all segments)", file=sys.stderr)
		print(" <childScope>     If defined the child scope that should contain the label attribute (by default it is discarded)", file=sys.stderr)
		print(" <labelAttribute> The attribute that holds the label", file=sys.stderr)
		sys.exit()

	print(Lab2Plot( 
			file(labfilename), 
			file(labfilename+".pool","w"), 
			attributeName, 
			filter, 
			childScope, labelAttribute))


