PROGRAM = AnalysisSynthesisGui
TOP=../..
SRCDEPS = $(TOP)/srcdeps/srcdeps

MAINSOURCES = \
	$(TOP)/examples/AnalysisSynthesis/mainAnaSynGUI.cxx \
	$(TOP)/src/Tools/AudioIO/Linux/ALSAAudioDevice.cxx \
	$(TOP)/src/Tools/AudioFileIO/extended.cxx \


SEARCHINCL = \
-R$(TOP)/src -R$(TOP)/examples/AnalysisSynthesis -R$(TOP)/externals -R$(TOP)/../xercesc/include

PREINCL = -include $(TOP)/src/Defines/Unix/config.h

all: $(PROGRAM)

clean:
	rm -rf obj/ $(PROGRAM)

distclean: clean
	rm -rf dep/
	
depend: .FORCE
	@echo Generating SOURCES, OBJECTS, DEPENDS and INCLUDES definitions
	$(SRCDEPS) -r -f \
	$(PREINCL) $(SEARCHINCL) \
	$(MAINSOURCES) > Makefile.vars

.FORCE:

Makefile.vars: Makefile
	@echo Generating SOURCES, OBJECTS, DEPENDS and INCLUDES definitions
	@$(SRCDEPS) -r -f \
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
	@$(CXX) -lasound -L$(TOP)/../xercesc/lib/ -lfltk -lfftw -lrfftw -lxerces-c $(OBJECTS) -o $@

%.o:	
	@mkdir -p obj
	@echo == compiling $<
	@g++ -c -ftemplate-depth-99 $(PREINCL) $(INCLUDES) $< -o $@

%.d:
	@mkdir -p dep
	@echo Generating dependency information for $<
	@$(SRCDEPS) \
	$(PREINCL) $(SEARCHINCL) \
	$< > $@

.SUFFIXES:
