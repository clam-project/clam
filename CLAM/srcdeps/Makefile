OBJECTS=objdepname.o dsp_parser.o parser.o config_parser.o strptr.o includepaths.o recursedir.o list.o tree.o strfuncs.o hash.o stack.o conditions.o main.o
DEPENDS=$(OBJECTS:.o=.d)
PROGRAM=srcdeps
CFLAGS=-Wall -g

all: $(PROGRAM)

clean:
	rm -f srcdeps $(OBJECTS) $(DEPENDS)

depend:
	rm -f $(DEPENDS)

ifneq ($(MAKECMDGOALS),depend)
ifneq ($(MAKECMDGOALS),clean)
include $(DEPENDS)
endif
endif

$(PROGRAM): $(OBJECTS)
	gcc $(OBJECTS) -o $(PROGRAM)
	
%.o: %.c
	gcc -c $(CFLAGS) $< -o $@

%.d: %.c
	gcc -M $< -o $@
	
	
