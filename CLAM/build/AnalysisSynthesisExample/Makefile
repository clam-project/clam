PROGRAM = AnalysisSynthesisExampleGUI
TOP=../..
SRCDEPS = $(TOP)/srcdeps/srcdeps

all: $(PROGRAM)

clean:
	rm -rf obj/ $(PROGRAM)

distclean: clean
	rm -rf dep/

depend: .FORCE
	@echo Generating SOURCES, OBJECTS, DEPENDS and INCLUDES definitions
	$(SRCDEPS) settings.cfg > Makefile.vars

.FORCE:

Makefile.vars: Makefile $(SRCDEPS)
	@echo Generating SOURCES, OBJECTS, DEPENDS and INCLUDES definitions
	$(SRCDEPS) settings.cfg > Makefile.vars

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),depend)
include Makefile.vars
include $(DEPENDS)
endif
endif

$(PROGRAM): $(OBJECTS)
	@echo == linking $@
	@$(CXX) $(LIBRARY_PATHS) $(LIBRARIES) $(OBJECTS) -o $@

%.o:	
	@mkdir -p obj
	@echo == compiling $<
	@g++ -c $(PREINCL) -ftemplate-depth-99 $(DEFINES) $(PRE_INCLUDES) $(INCLUDES) $< -o $@

%.d:
	@mkdir -p dep
	@echo Generating dependency information for $<
	@$(SRCDEPS) -d settings.cfg $< > $@

.SUFFIXES:
