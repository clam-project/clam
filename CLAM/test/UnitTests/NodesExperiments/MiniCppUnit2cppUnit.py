#! /usr/bin/python

import sys, re

if len(sys.argv) == 1 :
	print 'filename needed'
	sys.exit(1)
	
print 'processing file:', sys.argv[1]
fullText = ''

contains = lambda line, search : line.find(search)>=0

substitutions = [
	('ASSERT_IGUALS', 'CPPUNIT_ASSERT_EQUAL'),
	('#include\s*"MiniCppUnit.hxx"', '#include <cppunit/extensions/HelperMacros.h>'),
	('public GrupDeTests<\S+>','public CppUnit::TestFixture' ),
	(r'\tCAS_DE_TEST', 'CPPUNIT_TEST' )
	]

parsingState = 'DEFAULT'
def processLine(aLine) :
	global parsingState

	if parsingState == 'DEFAULT' :
		if contains(aLine, 'GRUP_DE_TESTS') :
			parsingState = 'TESTS_DECL'
	if parsingState == 'TESTS_DECL' :
		if contains(aLine, '{') :
			aLine = ''
		if contains(aLine, '}') :
			aLine = '\tCPPUNIT_TEST_SUITE_END();'
			
	changed = False
	for pattern, subst in substitutions :
		match = re.search( pattern, aLine)
		changed = changed or match
		if match:
			aLine = re.sub( pattern, subst, aLine )
	
	return changed, aLine

for line in file(sys.argv[1]) :
	changed, processedLine = processLine(line)
	if changed : 
		print '+++',
	else :
		print '···',
	print processedLine,
	fullText += processedLine

	
#matchFoo = re.search( 'GRUP_DE_TESTS\(\s*(\S+)\s*\)\n\s*\{*\}', fullText )
#matchFinalTestDeclaration = re.search( 'CPPUNIT_TEST\(\s*(\S*)\s*\);\s*\n\s*}', fullText ) 
#fullText = re.sub( 'CPPUNIT_TEST\(\s*(\S+)\s*\);\s*\n\s*}', 'CPPUNIT_TEST( ' + matchFinalTestDeclaration.group(1)+' );\n\t\tCPPUNIT_TEST_SUITE_END();\n\t}\n', fullText )
#print 'this is the full text',fullText
#print 'match foo:',matchFoo.group(1)

