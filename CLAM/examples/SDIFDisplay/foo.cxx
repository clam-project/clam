#include <stdio.h>
#include <list>

main()
{
	list<int> a;
	list<int>::iterator it;

	a.push_back(0);	 
	a.push_back(1);	 
	a.push_back(2);	 
	a.push_back(3);

	it = a.end();
	it--;
	printf("%d\n",*it);
}	
