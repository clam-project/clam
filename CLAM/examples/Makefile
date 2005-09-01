CC=g++
CFLAGS=-O2 -Wall -include CLAM/preinclude.hxx `pkg-config --cflags clam_core clam_processing clam_audioio clam_vmfl clam_vmqt`
LDFLAGS=`pkg-config --libs clam_core clam_processing clam_audioio clam_vmfl clam_vmqt`

.PHONY: all clean

PROGRAMS = FFT_example \
	AudioFileReadingExample \
	AudioFileWritingExample \
	DescriptorComputationExample \
	FDFilterExample \
	FileInfoExample \
	FilePlaybackExample \
	LPCAnalysisExample \
	MIDIIOExample \
	MIDIOutExample \
	NetworkPersistenceExample \
	NetworkUsageExample \
	POCompositeExample \
	PersistenceThroughDT \
	SDIFAndSegment \
	SpectrumExample \
	StereoToMono


all: $(PROGRAMS)

clean:
	rm -rf *.o
	rm -rf $(PROGRAMS)


FFT_example : FFT_example.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

# Broken - doesn't compile
AudioApplicationExample : AudioApplicationExample.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

AudioFileReadingExample : AudioFileReading_example.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

AudioFileWritingExample : AudioFileWriting_example.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

# Broken - doesn't compile
AudioIOExample: AudioIOExample.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

# Broken - doesn't link
ConfiguratorsExample: Configurators_example.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

DescriptorComputationExample : DescriptorComputation_example.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

FDFilterExample : FDFilterExample.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

FileInfoExample : FileInfo_example.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

FilePlaybackExample : FilePlayback_example.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

# Broken - The example is jury-rigged to print the following message:
# "THIS EXAMPLE NEEDS FIXING"
FundamentalDetect : FundamentalDetect.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

LPCAnalysisExample : LPCAnalysis_example.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

MIDIIOExample : MIDIIOExample.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

MIDIOutExample : MIDIOut_example.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

# Broken - Doesn't compile
MIDISynthesizerExample : MIDI_Synthesizer_example.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

# Broken - Hangs
MIDI2XmlExample : Midi2XmlExample.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

NetworkPersistenceExample : NetworkPersistence_example.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

NetworkUsageExample: NetworkUsage_example.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

POCompositeExample : POCompositeExample.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

PersistenceThroughDT : PersistenceThroughDTs_example.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

ProcessingControls : ProcessingObject_controls_example.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

SDIFAndSegment : SDIF_And_Segment_example.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

SpectrumExample : Spectrum_example.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

StereoToMono : StereoToMonoFile.cxx
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)


