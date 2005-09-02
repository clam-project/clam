CC=g++
CFLAGS=-O2 -Wall -include CLAM/preinclude.hxx `pkg-config --cflags clam_core clam_processing clam_audioio clam_vmfl clam_vmqt`
LDFLAGS=`pkg-config --libs clam_core clam_processing clam_audioio clam_vmfl clam_vmqt`

TARGET=MIDISynthExample

SOURCES = MIDI_Synthesizer_example.cxx

OBJECTS = $(SOURCES:.cxx=.o)

.PHONY: all clean

all: MIDISynthExample

clean :
	rm -rf *.o
	rm $(TARGET)

%.o : %.cxx
	$(CC) -c -o $@ $(CFLAGS) $<

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LDFLAGS)
