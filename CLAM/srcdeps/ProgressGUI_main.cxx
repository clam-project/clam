#include "ProgressGUI.hxx"

#include <stdio.h>
#include <stdlib.h>

int main( int argc, char **argv)
{
	if (argc!=3)
	{
		fprintf(stderr,"Usage srcdeps SETTINGSFILE DSPFILE\n");
		exit(-1);
	}

	ProgressGUIAdapter winmain(argc,argv); // third ctr. argument is bool moreDetail=true
		
	return ProgressGUIAdapter::runEventsLoop();	
}
