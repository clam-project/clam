#include <iostream>
#include <set>
#include <string>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>

#include "cppUnitHelper.hxx"
#include "Assert.hxx" // CLAM

// Usage: UnitTests [--skip <SuiteName>]...
// Suites named with --skip (e.g. CLAMTest::AudioFileTest) are left out of the
// run, which lets the build run the tests that do not need the external test
// data checkout.
int main(int argc, char ** argv){
	// this flag allows clam asserts to behave throwing an exception instead
	// of doing breakpoint. Thus making auto-testing of asserts possible.
	// Notice: while debugging tests (if breakpoints are wanted back) you might need to
	// modify this flag before the point to be debugged.
	CLAM::ErrAssertionFailed::breakpointInCLAMAssertEnabled = false;

	std::set<std::string> skipped;
	for (int i=1; i<argc; i++)
	{
		const std::string arg = argv[i];
		if (arg == "--skip" && i+1 < argc)
			skipped.insert(argv[++i]);
		else
		{
			std::cerr << "Usage: " << argv[0] << " [--skip <SuiteName>]..." << std::endl;
			return 2;
		}
	}

	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	CppUnit::Test * allTests = registry.makeTest();
	CppUnit::Test * theTest = allTests;
	if (!skipped.empty())
	{
		// Re-parent the wanted suites. allTests is deliberately never
		// deleted: it still holds pointers to the suites now owned by theTest.
		CppUnit::TestSuite * filtered = new CppUnit::TestSuite(allTests->getName());
		for (int i=0; i<allTests->getChildTestCount(); i++)
		{
			CppUnit::Test * child = allTests->getChildTestAt(i);
			if (skipped.count(child->getName()))
			{
				std::cout << "Skipping " << child->getName() << std::endl;
				continue;
			}
			filtered->addTest(child);
		}
		theTest = filtered;
	}
	CLAMTest::Helper::printTestNames( theTest );

	// We could just run() the suite. But using TestRunner we get
	// the exit code as well.
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( theTest ); // caution: it deletes the suite on termination.
	return !runner.run("");

}
