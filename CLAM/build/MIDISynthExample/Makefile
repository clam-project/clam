PROGRAM = MIDISynthExample
TOP=../..
SRCDEPS = $(TOP)/srcdeps/srcdeps

MAINSOURCES = \
	$(TOP)/examples/MIDI_Synthesizer_example.cxx \
	$(TOP)/src/Tools/MIDIIO/File/FileMIDIDevice.cxx \
	$(TOP)/src/Tools/AudioIO/Linux/ALSAAudioDevice.cxx

SEARCHINCL = \
-R$(TOP)/src -R$(TOP)/externals -R$(TOP)/../xercesc/include

PREINCL = -include $(TOP)/src/Defines/Unix/config.h

all: $(PROGRAM)

clean:
	rm -rf obj/ $(PROGRAM)

distclean: clean
	rm -rf dep/
	
depend: .FORCE
	@echo Generating SOURCES, OBJECTS, DEPENDS and INCLUDES definitions
	$(SRCDEPS) -r -D \
	$(PREINCL) $(SEARCHINCL) \
	$(MAINSOURCES) > Makefile.vars

.FORCE:

Makefile.vars: Makefile
	@echo Generating SOURCES, OBJECTS, DEPENDS and INCLUDES definitions
	@$(SRCDEPS) -r -D \
	$(PREINCL) $(SEARCHINCL) \
	$(MAINSOURCES) > Makefile.vars

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),depend)
include Makefile.vars
include $(DEPENDS)
endif
endif

$(PROGRAM): $(OBJECTS)
	@echo == linking $@
	@$(CXX) -lasound -L../xercesc/lib/ -lfltk -lfftw -lrfftw -lxerces-c $(OBJECTS) -o $@

%.o:	
	@mkdir -p obj
	@echo == compiling $<
	@g++ -c $(PREINCL) $(INCLUDES) $< -o $@

%.d:
	@mkdir -p dep
	@echo Generating dependency information for $<
	@$(SRCDEPS) \
	$(PREINCL) $(SEARCHINCL) \
	$< > $@

.SUFFIXES:
