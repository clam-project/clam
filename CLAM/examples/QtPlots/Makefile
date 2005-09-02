CC=g++
CFLAGS=-O2 -Wall -include CLAM/preinclude.hxx `pkg-config --cflags clam_core clam_processing clam_audioio clam_vmfl clam_vmqt`
LDFLAGS=`pkg-config --libs clam_core clam_processing clam_audioio clam_vmfl clam_vmqt`

.PHONY: all clean

TARGETS = BPFEditor \
	DPAudio \
	DPAudioStereo \
	DPFundamental \
	DPMultiPlot \
	DPSpectrumAndPeaks \
	MultiBPFViewer \
	ListPlot \
	QtAudioPlot \
	QtSinTracksPlot \
	QtSpectrogram \
	QtSpectrumPlot

all: $(TARGETS)
	cp $(TARGETS) ../

clean :
	rm -rf *.o
	rm -rf $(TARGETS)
	for target in $(TARGETS); do rm -rf ../$$target; done

UTILS_SOURCES= utils/analysis_utils.cxx \
	utils/audio_file_utils.cxx \
	utils/wave_utils.cxx

UTILS_OBJECTS=$(UTILS_SOURCES:.cxx=.o)

UTILS_LIB=libutils.a

%.o: %.cxx
	$(CC) -c -o $@ $< $(CFLAGS)

$(UTILS_LIB) : $(UTILS_OBJECTS)
	ar cru $@ $(UTILS_OBJECTS)

BPFEditor: BPFEditorExample/BPFEditorExample.cxx $(UTILS_LIB)
	$(CC) -o $@ $(CFLAGS) -I./utils $< $(LDFLAGS) -L. -lutils


DPAudio: DirectPlotsExamples/DPAudioExample.cxx $(UTILS_LIB)
	$(CC) -o $@ $(CFLAGS) -I./utils $< $(LDFLAGS) -L. -lutils

DPAudioStereo: DirectPlotsExamples/DPAudioStereoExample.cxx $(UTILS_LIB)
	$(CC) -o $@ $(CFLAGS) -I./utils $< $(LDFLAGS) -L. -lutils

DPFundamental: DirectPlotsExamples/DPFundamentalExample.cxx $(UTILS_LIB)
	$(CC) -o $@ $(CFLAGS) -I./utils $< $(LDFLAGS) -L. -lutils

DPMultiPlot: DirectPlotsExamples/DPMultiPlotExample.cxx $(UTILS_LIB)
	$(CC) -o $@ $(CFLAGS) -I./utils $< $(LDFLAGS) -L. -lutils

DPSpectrumAndPeaks: DirectPlotsExamples/DPSpectrumAndPeaksExample.cxx $(UTILS_LIB)
	$(CC) -o $@ $(CFLAGS) -I./utils $< $(LDFLAGS) -L. -lutils

MultiBPFViewer: DirectPlotsExamples/MultiBPFViewerExample.cxx $(UTILS_LIB)
	$(CC) -o $@ $(CFLAGS) -I./utils $< $(LDFLAGS) -L. -lutils

ListPlot: ListPlotExample/ListPlotExample.cxx $(UTILS_LIB)
	$(CC) -o $@ $(CFLAGS) -I./utils $< $(LDFLAGS) -L. -lutils

QtAudioPlot: QtPlotsExamples/QtAudioPlotExample.cxx $(UTILS_LIB)
	$(CC) -o $@ $(CFLAGS) -I./utils $< $(LDFLAGS) -L. -lutils

QtSinTracksPlot: QtPlotsExamples/QtSinTracksPlotExample.cxx $(UTILS_LIB)
	$(CC) -o $@ $(CFLAGS) -I./utils $< $(LDFLAGS) -L. -lutils

QtSpectrogram: QtPlotsExamples/QtSpectrogramExample.cxx $(UTILS_LIB)
	$(CC) -o $@ $(CFLAGS) -I./utils $< $(LDFLAGS) -L. -lutils

QtSpectrumPlot: QtPlotsExamples/QtSpectrumPlotExample.cxx $(UTILS_LIB)
	$(CC) -o $@ $(CFLAGS) -I./utils $< $(LDFLAGS) -L. -lutils

