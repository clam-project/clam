#!/usr/bin/python

import os, sys, commands

settingsToTest = ['FilePlayback', 'SMSTools']

for test in settingsToTest :
	config = test + '.cfg'
	if not os.path.exists(config) :
		print 'missing %s' % config
		continue
	expected = test + '_expected.vars'
	result = test + '_result.vars'
	if not os.path.exists(expected) :
		print expected, 'created void'
		file(expected,'w')
	print 'testing', test
	[_,_,beforeUserTime,beforeSysTime,_] = os.times()
	os.system('../srcdeps %s > %s ' % (config, result) )
	[_,_,afterUserTime,afterSysTime,_]= os.times()
	print "User time:", afterUserTime - beforeUserTime
	print "Sys time:", afterSysTime - beforeSysTime
	toCompare = expected, result
	if commands.getoutput('diff -q %s %s' % toCompare) :
		os.system('xxdiff %s %s' % toCompare)
	else :
		print 'test %s ok' % test

