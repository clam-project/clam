#! /usr/bin/python
# -*- coding: iso-8859-15 -*-
import sys, re

if len(sys.argv) != 2 :
	print 'filename needed'
	sys.exit(1)
	
print 'processing file:', sys.argv[1]

def contains( line, pattern ):
	return re.search( pattern, line )

def processLine(aLine) :
	changed = False
	attribute = '_(\S)(\S*)'
	matchAttribute = re.search( attribute, aLine )
	include = contains( aLine, '_hxx' )
	if matchAttribute and not include:
		changed = True
		newAttribute = 'm%s%s' % ( matchAttribute.group(1).upper(), matchAttribute.group(2) )
		aLine = re.sub( attribute, newAttribute, aLine )
		
	method = '([a-z])(\S+)\((.*)\)'
	matchMethod = re.search( method, aLine )
	if matchMethod :
		changed = True
		print 'method', matchMethod.group(1), matchMethod.group(2) 
		if matchMethod.group(2).find('\.'):
				print 'call'
		newMethod = '%s%s(%s)' % (matchMethod.group(1).upper(),matchMethod.group(2), matchMethod.group(3) )
		aLine = re.sub( method, newMethod, aLine )
	return changed, aLine



newfile = file('__'+sys.argv[1], 'w')
originalFile = file(sys.argv[1]).readlines()

for line in originalFile :
	changed, processedLine = processLine(line)
	if changed : 
		print '+++',
	else :
		print '···',
	print processedLine,
	newfile.write(processedLine)
	
