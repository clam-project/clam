#! /usr/bin/python

import sys, re

if len(sys.argv) == 1 :
	print 'filename needed'
	sys.exit(1)
	
print 'processing file:', sys.argv[1]

contains = lambda line, search : line.find(search)>=0

substitutions = [
	('ASSERT_IGUALS', 'CPPUNIT_ASSERT_EQUAL'),
	('#include\s*"MiniCppUnit.hxx"', '#include <cppunit/extensions/HelperMacros.h>'),
	('public GrupDeTests<\S+>','public CppUnit::TestFixture' ),
	('\tCAS_DE_TEST', 'CPPUNIT_TEST' ),
	('GRUP_DE_TESTS','CPPUNIT_TEST_SUITE'),
	('FALLA', 'CPPUNIT_FAIL' )
	]

parsingState = 'DEFAULT'


def checkAsserts(aLine):
	assertPattern = 'ASSERT\((.*)' 
	matchAssert = re.search(assertPattern, aLine)	
	if matchAssert:
		cppunitAssertPattern = 'CPPUNIT_ASSERT('+matchAssert.group(1)
		aLine = re.sub( assertPattern, cppunitAssertPattern, aLine )
	return aLine
	
def processLine(aLine) :
	global parsingState

	if parsingState == 'DEFAULT' :
		if contains(aLine, 'GRUP_DE_TESTS') :
			matchGrup = re.search('GRUP_DE_TESTS\(\s*(\S+)\s*\)', aLine )
			aLine = re.sub( 'GRUP_DE_TESTS\(\s*(\S+)\s*\)', 'CPPUNIT_TEST_SUITE( '+matchGrup.group(1) + ' );', aLine )
			parsingState = 'TESTS_DECL'
		if contains(aLine, 'int main'):
			parsingState = 'MAIN'
			aLine = ''
			
	if parsingState == 'TESTS_DECL' :
		if contains(aLine, '{') :
			aLine = '\n'
		if contains(aLine, '}') :
			aLine = '\n\tCPPUNIT_TEST_SUITE_END();\n'
			parsingState = 'DEFAULT'

	if parsingState == 'MAIN' :		
		if contains(aLine,'}') :
			parsingState = 'DEFAULT'
		aLine = ''
	
	changed = False
	for pattern, subst in substitutions :
		match = re.search( pattern, aLine)
		changed = changed or match
		if match:
			aLine = re.sub( pattern, subst, aLine )

	aLine = checkAsserts( aLine )
	
	return changed, aLine

for line in file(sys.argv[1]) :
	changed, processedLine = processLine(line)
	if changed : 
		print '+++',
	else :
		print '···',
	print processedLine,

