#!/usr/bin/python

logfile = 'timesLog.txt'

import os, sys, commands, time, shutil

settingsToTest = ['MIDIOut','FilePlayback', 'SMSTools']

if sys.platform in ['cygwin', 'win32'] :
	isWindows = True
	extension = '.dsp'
	diffcmd = 'vimdiff'
else :
	isWindows = False
	print 'in linux'
	extension = '.vars'
	diffcmd = 'xxdiff'
	
def timeToExecute(cmd) :
	_,_,beforeUserTime,beforeSysTime,_ = os.times()
	os.system(cmd )
	_,_,afterUserTime,afterSysTime,_= os.times()
	return afterUserTime - beforeUserTime, afterSysTime - beforeSysTime

def appendTimesLog(testname, usrtime, systime) :
	date = time.strftime('%d/%m/%y\t(%A)')
	line = '%s\t%s\t%s\t%s\n' % (testname, usrtime, systime, date)
	file(logfile, 'a').write( line )

def windowsRenameResult(test) :
	if not isWindows :
		return
	target = test + '_result' + extension
	shutil.copy(test+extension, target)
	
for test in settingsToTest :
	config = test + '.cfg'
	if not os.path.exists(config) :
		print 'missing %s' % config
		continue
	expected = test + '_expected' + extension
	result = test + '_result' + extension
	if not os.path.exists(expected) :
		print expected, 'created void'
		file(expected,'w')
	print 'testing', test
	usrtime, systime = timeToExecute('../srcdeps %s > %s ' % (config, result))
	windowsRenameResult(test)
	
	print "user time: %s\nsys time: %s" % (usrtime, systime)
	appendTimesLog(test, usrtime, systime)

	tocompare = expected, result
	if commands.getoutput('diff -q %s %s' % tocompare) :
		os.system( diffcmd+' %s %s' % tocompare)
	else :
		print 'test %s ok' % test

