#! /usr/bin/python

# update level: 0-Keep, 1-Update, 2-CleanCheckout
# when the sandbox is not present always clean checkout
updateLevelForCLAM = 0
updateLevelForExamples = 0
updateLevelForTestData = 0

# When false keeps already compiled objects
doCleanMake = False
# When false does nothing
doAutoconf = False

enableSendMail = False
publicAddress = 'clam-devel@iua.upf.es'
privateAddress = 'parumi@iua.upf.es'
subject = 'nightly tests report'
executionTime = 15 
#TODO: this will be used only when it's not set in the environment
CVSROOT = ':ext:parumi@mtg150.upf.es:/mnt/cvsroot'
configurations = ['release'] #['debug', 'release'] 

import commands
import os
import string
import sys

CLAM_SANDBOXES = os.path.abspath( os.path.dirname(sys.argv[0])+'/../..' ) + '/'
print 'CLAM_SANDBOXES=',CLAM_SANDBOXES

MODULE_TAG = 'development-branch'
SANDBOX_NAME = 'clean-'+MODULE_TAG

BUILDPATH = CLAM_SANDBOXES + '%s/build/' % (SANDBOX_NAME)
SALTO_DATA_FOLDER = CLAM_SANDBOXES + 'SaltoDataFolder/'

unitTestsPath = BUILDPATH+'Tests/UnitTests/'
functionalTestsPath = BUILDPATH+'Tests/FunctionalTests/'
spvTestsPath = BUILDPATH + 'Tests/SupervisedTests/'
nonPortedTestsPath = BUILDPATH + 'Tests/NonPortedTests/'

sandboxes = [ # Module, Sandbox, Tag, Update level
	( 'CLAM', SANDBOX_NAME, MODULE_TAG, updateLevelForCLAM),
	( 'CLAM_SMSTools', 'CLAM_SMSTools', '', updateLevelForExamples ),
	( 'CLAM_Salto', 'CLAM_Salto', '', updateLevelForExamples ),
	( 'CLAM_SpectralDelay', 'CLAM_SpectralDelay', '', updateLevelForExamples ),
	( 'CLAM_NetworkEditor', 'CLAM_NetworkEditor', '', updateLevelForExamples ),
	( 'CLAM_Voice2MIDI', 'CLAM_Voice2MIDI', '', updateLevelForExamples ),
	( 'CLAM-TestData', 'CLAM-TestData', '', updateLevelForTestData )
]
# update level: 0-Keep, 1-Update, 2-CleanCheckout
# when the sandbox is not present always clean checkout

automaticTests = [
	( 'UnitTests', unitTestsPath ),
	( 'FunctionalTests', functionalTestsPath )
]

externalApplications = [
	( 'SpectralDelay', CLAM_SANDBOXES+'CLAM_SpectralDelay/build/'),
	( 'SpectralDelay-Offline', CLAM_SANDBOXES+'CLAM_SpectralDelay/build/Offline/'),
	( 'SpectralDelay-MultiBandProxyTest', CLAM_SANDBOXES+'CLAM_SpectralDelay/build/MultiBandProxyTest/'),
	( 'SpectralDelay-DelayPoolTest', CLAM_SANDBOXES+'CLAM_SpectralDelay/build/DelayPoolTest/'),
	( 'NetworkEditor', CLAM_SANDBOXES+'CLAM_NetworkEditor/build/' ),
	( 'Voice2MIDI', CLAM_SANDBOXES+'CLAM_Voice2MIDI/build/' ),
	( 'SMSTools', CLAM_SANDBOXES+'CLAM_SMSTools/build/Tools/' ),
	( 'SMSBatch', CLAM_SANDBOXES+'CLAM_SMSTools/build/Batch/' ),
	( 'SMSConsole', CLAM_SANDBOXES+'CLAM_SMSTools/build/Console/' ),
	( 'Salto', CLAM_SANDBOXES+'CLAM_Salto/build/' )
]

supervisedTests = [
	('SpectralPeaksPresentation', spvTestsPath+'SpectralPeaksPresentation/' ), 
	('SpectrumPresentation', spvTestsPath+'SpectrumPresentation/' ),
	('AudioPresentation', spvTestsPath+'AudioPresentation/' ),
	('FunFreqPresentationTest', spvTestsPath+'FundFreqPresentation/' ),
	('AudioIOTest', spvTestsPath+'AudioIO/' ),
	('MIDIIOTest', spvTestsPath+'MIDIIO/' ),
	('Fl_EnvelopeTest', spvTestsPath+'Fl_Envelope/' ),
	('Test_Multiplot', spvTestsPath+'Plotsv2/Test_Multiplot/' ),
	('Test_SinlglePlot', spvTestsPath+'Plotsv2/Test_SinglePlot/' ),
	('Test_SpecificPlots', spvTestsPath+'Plotsv2/Test_Multiplot/' )
]


notPortedTests = [
	( 'Array', nonPortedTestsPath+'Array/'),
        ( 'Array2', nonPortedTestsPath+'Array2/'),
        ( 'Assert', nonPortedTestsPath+'Assert/'),
        ( 'BPF', nonPortedTestsPath+'BPF/'),
        ( 'EnvelopeExtractor', nonPortedTestsPath+'EnvelopeExtractor/'),
        ( 'Error', nonPortedTestsPath+'Error/'),
        ( 'FDFilterGen', nonPortedTestsPath+'FDFilterGen/'),
        ( 'List', nonPortedTestsPath+'List/'),
        ( 'Segment', nonPortedTestsPath+'Segment/'),
        ( 'Signalv1', nonPortedTestsPath+'Signalv1/'),
        ( 'Spectrum', nonPortedTestsPath+'Spectrum/'),
        ( 'SpectrumAdder', nonPortedTestsPath+'SpectrumAdder/'),
        ( 'SpectrumAdder2', nonPortedTestsPath+'SpectrumAdder2/'),
        ( 'SpectrumProduct', nonPortedTestsPath+'SpectrumProduct/'),
        ( 'TabFunct', nonPortedTestsPath+'TabFunct/'),
        ( 'Threading', nonPortedTestsPath+'Threading/'),
        ( 'WaveGenerator', nonPortedTestsPath+'WaveGenerator/'),
        ( 'Windowing', nonPortedTestsPath+'Windowing/')
]


testsToRun = []
# insert sub-lists to the main list: 
#    this makes debugging easier
testsToRun[-1:-1] = externalApplications 
testsToRun[-1:-1] = supervisedTests
testsToRun[-1:-1] = notPortedTests
testsToRun[-1:-1] = automaticTests 

sender = '"automatic tests script" <parumi@iua.upf.es>'

# global vars. ugly, yes.
foundCompilationErrors = False 
foundTestsFailures = False
foundExecutionErrors = False


def sendmail(fromaddr, toaddrs, subject, body) :
	import smtplib

	# Add the From: and To: headers at the start!
	msg = "From: %s\r\nTo: %s\r\nSubject: %s\r\n" % (fromaddr, toaddrs, subject) + body
	if not enableSendMail :
		print msg
		return
	
	file(CLAM_SANDBOXES + 'testslog.txt','w').write(msg)
	
	server = smtplib.SMTP('iua-mail.upf.es')
	server.set_debuglevel(1)
	server.sendmail(fromaddr, toaddrs, msg)
	server.quit()

def checkPaths() :
	global totalDetails, totalSummary, testsToRun
	if not os.access(CLAM_SANDBOXES, os.F_OK) :
		err = "Sorry can't access CLAM_SANDBOXES path : " + CLAM_SANDBOXES
		sendError(err)
		sys.exit(1)
	dirsExpected = setfilesExpected = ''
	i=0
	for name, path in testsToRun :
		if not os.access(path, os.F_OK) :
			dirsExpected += '\t- %s\n'%path
			testsToRun[i] = ('','')
		else :
			setfile = path+'settings.cfg'
			if not os.access(setfile, os.F_OK) :
				setfilesExpected +='\t- %s\n'%setfile
				testsToRun[i] = ('','')
		i += 1
	if dirsExpected!='' or setfilesExpected!='' :
		msg = 'dirs not found:\n\n%s\n files not found\n%s\n'
		totalSummary = msg % (dirsExpected,setfilesExpected)
		print totalSummary
	

def parseCompilationWarnings(compilationOut) :
	nwarnings = compilationOut.count('warning')
	warnings = ''
	if nwarnings > 0 :
		warnings = ' - found %d warnings !'% (nwarnings)
		print warnings
	return warnings

def parseTestsFailures( testsOut ) :
	state='TESTS_INFO'
	summary = 'tests results: '
	details = ''
	for line in testsOut.split('\n') :
		if state == 'TESTS_INFO' :
			if line.find('!!!FAILURES!!!') >=0 :
				details = line + '\n'
				summary +=  line
				state = 'FAILURES'
			if line.find('OK (') == 0 :
				details = ''
				summary += line
		elif state == 'FAILURES' :
			details +=  line + '\n'
		else :
			assert(false)
	return summary, details

def parseExecutionErrors( executionOut ) :
	print 'executed non test\n', executionOut
	if executionOut.find('# ASSERTION FAILED #')>=0 :
		return 'execution: assertion failed!', executionOut
	if executionOut.find('aborted')>=0:
		return 'execution: aborted!', executionOut
	#TODO violacio de segment
	if executionOut.find('# WARNING #')>=0 :
		return 'execution OK - althought warning-assert(s)', executionOut
	return 'execution OK', ''


def isTest(path) :
	return path.find('UnitTests/')>=0 or path.find('FunctionalTests/')>=0 or path.find('build/Tests/')>= 0  

#----------------------------------------------------------------
def getStatusOutput(cmd) :	
	"returns wheather cmd exits correctly and the output"
	print 'executing ',cmd
	stat, output = commands.getstatusoutput(cmd)		
	if stat != 0 : 
		print output
	else :
		print cmd,' OK'
	return (stat == 0), output

def executeMandatory(cmd) :
	stat, output = getStatusOutput(cmd)
	if not stat : 
		sendError(output)
		sys.exit(1)
#----------------------------------------------------------------

def formatSummary(name, configuration, result) :
	nameConfig = '%s (%s) ' % (name, configuration)
	secondRow = 50
	npoints = 0
	if len(nameConfig) < secondRow :
		npoints += secondRow - len(nameConfig)
	points = '.'
	for i in range(npoints): 
		points +='.'
	return nameConfig + points + result+'\n'

def compileAndRun(name, path) :
	global foundCompilationErrors, foundExecutionErrors, foundTestsFailures, configurations
	if name == '' :
		print 'found removed test (invalid dir or settings file)'
		return '',''
	os.chdir(path)
	# compilation phase
	summary = details = s = d = ''
	for configuration in configurations :
		if doCleanMake :
			getStatusOutput('make clean')
			getStatusOutput('make depend')
		makecmd = 'make CONFIG=%s' % (configuration)
		ok, output = getStatusOutput( makecmd )
		foundCompilationErrors = foundCompilationErrors or not ok
		if not ok :
			s = 'COMPILATION ERRORS'
		else :
			s = 'compilation OK'
		s += parseCompilationWarnings( output )
		summary += formatSummary(name, configuration, s)
		detailsFormat = '\n\n%s\n-----------------------------\n%s\n'
		if not ok :
			details += detailsFormat % (name, output)
		print 'summary: ',summary

		if not ok : 
			continue
			
		# execution phase
		execcmd = './'+name
		if isTest(path) :
			print 'isTest yes\nrunning tests'
			ok, output = getStatusOutput( execcmd )
			foundTestsFailues = foundTestsFailures or not ok
			s, d = parseTestsFailures( output )
		else :
			print 'isTest no\nexecuting application for a while'
			ok, output = runInBackgroundForAWhile(path, execcmd, executionTime)
			s, d = parseExecutionErrors( output )
			foundExecutionErrors =  foundExecutionErrors or not ok or s.find('OK')==-1
		
		summary += formatSummary(name, configuration, s)
		if d != '' :
			details += detailsFormat % (name, d)
	return summary, details


mailTemplate = '''
(This message has been automatically generated)

Status of CLAM on tag: %s 

TODO:
  - behaviour: send public mail when a)something fails, or 
    b)everything ok, but last time something failed.
    

-------  
SUMMARY
-------
%s

---------------------------------------
DETAILS (only if finds errors/failures)
---------------------------------------
%s

-----------------------------------------------------------
// Powered by Python //
'''

import time, string, signal
def runInBackgroundForAWhile(path, command, sleeptime=10) :
	os.chdir(path)
	out, err = '/tmp/removeme.out', '/tmp/removeme.err'
	#file(out, 'w')
	#file(err, 'w')
	fullcmd = '%s > %s 2> %s &' % (command, out, err)
	print fullcmd
	print 'result ',os.system( fullcmd )
	time.sleep( sleeptime )
	withoutSlash = command[command.find('/')+1 : ]

	status, dummy = commands.getstatusoutput('killall '+ withoutSlash)
	
	print 'kill status ', status, dummy
	result = string.join( file('/tmp/removeme.out').readlines() )
	result += string.join( file('/tmp/removeme.err').readlines() )
	#os.remove(out)
	#os.remove(err)
	return True, result #TODO status


def sendError(usermsg='') :
	import traceback
	tb = string.join( traceback.format_exception(sys.exc_info()[0], sys.exc_info()[1], sys.exc_info()[2]) )
	subject = 'unexpected error on runTests.py script'
	errormsg = usermsg + 'Python interpreter said:\n' + tb
	print errormsg
	if privateAddress != '' :
		sendmail( sender, privateAddress, subject, errormsg )

def checkoutSandbox (module, sandbox, tag) :
	tagOption = ''
	if tag!='' : tagOption = '-r '+tag
	os.chdir(CLAM_SANDBOXES)
	executeMandatory('cvs checkout %s -d %s %s' % (tagOption, sandbox, module) )




def updateSandboxes() :
	global sandboxes
	for module, sandbox, tag, level in sandboxes :
		if not os.access(sandbox, os.F_OK) :
			checkoutSandbox(module, sandbox, tag)
		elif level == 2 :
			print 'The sandbox %s already exists, deleting it'%(sandbox)
			getStatusOutput('rm -rf '+sandbox )
			checkoutSandbox(module, sandbox, tag)
		elif level == 1 :
			print 'The sandbox %s already exists, updating it'%(sandbox)
			os.chdir(sandbox)
			ok, output = getStatusOutput( 'cvs update -dP' )
			if output.find('\nC ')>=0 :
				print 'CVS CONFLICT !!', output
				# TODO: Inform about them
		elif level == 0 :
			print 'The sandbox %s already exists, keeping it'%(sandbox)

def deployClamBuildSystem() :
	# BuildSrcDeps
	os.chdir(BUILDPATH+'srcdeps/')
	executeMandatory('make')

	# ConfigureClam
	os.chdir(BUILDPATH)
	if doAutoconf:
		executeMandatory('autoconf')
		executeMandatory('./configure')

	# Setting the clam location
	global sandboxes
	os.chdir(CLAM_SANDBOXES)
	for module, sandbox, tag, level in sandboxes :
		clamlocationfile = sandbox + '/build/clam-location.cfg'
		if not os.access(clamlocationfile, os.F_OK) :
			continue
		executeMandatory('echo \'CLAM_PATH = %s%s\' > %s' %(CLAM_SANDBOXES, SANDBOX_NAME, clamlocationfile))


	
#-------------------------------------------------------------------------------------  
#  Aplication Logic
#
totalSummary = totalDetails = ['']
def runTests() :
	global totalSummary, totalDetails		
	subj = [subject]
	report = []
	if 'CVSROOT' not in os.environ :
		print 'warning: CVSROOT not found in environ'
		os.environ['CVSROOT'] = CVSROOT

	os.environ['CVS_RSH'] = getStatusOutput('which ssh')[1]
	os.chdir(CLAM_SANDBOXES)
	#sanity check
	if SANDBOX_NAME in ['devel','CLAM'] : 
		sendError( 'ups, trying to remove devel sandbox !!' )
		sys.exit(-1)

	updateSandboxes()
	checkPaths();
	deployClamBuildSystem()

	# LinkSaltoDataFolder
#	os.chdir(CLAM_SANDBOXES + SANDBOX_NAME+ '/build/Examples/Salto')
#	executeMandatory('ln -s ' + SALTO_DATA_FOLDER)

	# compile and run/tests entries
	for name, path in testsToRun :
		print '\n\nname\t\t %s \npath \t\t%s \n' % (name, path)
		summary, details  = compileAndRun(name, path)
		totalSummary.append(summary)
		totalDetails.append(details)

		print "".join(totalSummary)
		report.append( (name, summary, details) )


	mailBody = mailTemplate  % ( MODULE_TAG, "".join(totalSummary), "".join(totalDetails) )
	if foundCompilationErrors : 
		subj.append(' - compilation err!')
	if foundTestsFailures :
		subj.append(' - tests failures!')
	if foundExecutionErrors :
		subj.append(' - execution errs!')

	if foundCompilationErrors or foundTestsFailures or foundExecutionErrors :
		sendReportTo = publicAddress
	else :
		sendReportTo = privateAddress
	if sendReportTo != '' :
		sendmail( sender, sendReportTo, subj, mailBody )
	else :
		print 'nowbody to send report'
		print 'subject: ', "".join(subj)
		print mailBody

#--------------------------------------------------------------
#
#  If called from command-line, parse arguments and take actions
#
if __name__ == '__main__':
	try :
		runTests()
	except KeyboardInterrupt :
		print 'interrupted by the user'
	except:
		sendError()

