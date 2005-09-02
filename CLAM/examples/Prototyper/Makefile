CC=g++
CFLAGS=-O2 -Wall -include CLAM/preinclude.hxx `pkg-config --cflags clam_core clam_processing clam_audioio clam_vmfl clam_vmqt`
LDFLAGS=`pkg-config --libs clam_core clam_processing clam_audioio clam_vmfl clam_vmqt`

TARGET=JACKPrototyper

SOURCES=JACKPrototypeLoader.cxx 
OBJECTS=$(SOURCES:.cxx=.o)

.PHONY: all clean

all: $(TARGET)

clean :
	rm -rf *.o
	rm -rf $(TARGET)
	rm -rf ../$(TARGET)

%.o : %.cxx
	$(CC) -c -o $@ $(CFLAGS) $<

$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)
