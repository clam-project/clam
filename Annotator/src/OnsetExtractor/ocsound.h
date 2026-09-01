#ifndef OCSOUND
#define OCSOUND

#include <iostream>
#include <string>
#include <fstream>

#define ORCFILE	"onset.orc"
#define SCOFILE "onset.sco"

const std::string kORC1="\nsr = 44100\nkr = 4410\nnchnls = 1\n\ninstr 1\n  idur = p3\n  ain  soundin  \"";
const std::string kORC2="\"\n        out      ain\nendin\n\ninstr 2\n  idur = p3\n;  asal rand     20000\n  asal oscil  18000, 1000, 102 ; amp, frec, numtabla\n       out      asal\nendin\n ";
const std::string kSCO1="t 0 60 ; tempo = 60 BPM constant for having time measured in secs.\n\nf 102 0 1024 10 1 ; generation of the table with the sinusoidal waveshape\n\ni 1 0 ";
const std::string kSCO2="; Original sound from the beginning to the end\n\n";

void generateorc(char* filename);
void generatesco(double *onsets, int ntime, double time_res);

#endif
