CC=g++
CFLAGS=-O2 -Wall -include CLAM/preinclude.hxx `pkg-config --cflags clam_core clam_processing clam_audioio clam_vmfl clam_vmqt`
LDFLAGS=`pkg-config --libs clam_core clam_processing clam_audioio clam_vmfl clam_vmqt`

TARGET=ControlArrayExample

SOURCES= main.cxx \
	MyProcessingWithControlArray.cxx \
	MyProcessingWithSimpleControls.cxx

OBJECTS=$(SOURCES:.cxx=.o)

.PHONY: all clean

all: ControlArrayExample

clean :
	rm -rf *.o
	rm -rf ControlArrayExample

%.o : %.cxx
	$(CC) -c -o $@ $(CFLAGS) $<

ControlArrayExample : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)
