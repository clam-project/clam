#!/usr/bin/python

logfile = 'timesLog.txt'

import os, sys, commands, time

settingsToTest = ['FilePlayback', 'SMSTools']

def timeToExecute(cmd) :
	_,_,beforeUserTime,beforeSysTime,_ = os.times()
	os.system(cmd )
	_,_,afterUserTime,afterSysTime,_= os.times()
	return afterUserTime - beforeUserTime, afterSysTime - beforeSysTime

def appendTimesLog(testname, usrtime, systime) :
	date = time.strftime('%d/%m/%y\t(%A)')
	line = '%s\t%s\t%s\t%s\n' % (testname, usrtime, systime, date)
	file(logfile, 'a').write( line )

	
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
	usrtime, systime = timeToExecute('../srcdeps %s > %s ' % (config, result))
	print "User time: %s\nSys time: %s" % (usrtime, systime)
	appendTimesLog(test, usrtime, systime)

	toCompare = expected, result
	if commands.getoutput('diff -q %s %s' % toCompare) :
		os.system('xxdiff %s %s' % toCompare)
	else :
		print 'test %s ok' % test

