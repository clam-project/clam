#include "stack.c"
#include "list.c"
#include <stdio.h>

main()
{
	const char* i;
	stack* s = stack_new();
	
	stack_push(s,"aaa");
	stack_push(s,"bbb");
	stack_push(s,"ccc");
	stack_push(s,"ddd");
	
	while ((i=stack_top(s)))
	{
		fprintf(stderr,"  from %s\n",i);
		stack_pop(s);
	}

	stack_push(s,"aaa");
	stack_push(s,"bbb");
	stack_push(s,"ccc");
	stack_push(s,"ddd");
	stack_pop(s);
	stack_pop(s);
	stack_push(s,"eee");

	while ((i=stack_top(s)))
	{
		fprintf(stderr,"  from %s\n",i);
		stack_pop(s);
	}
}
