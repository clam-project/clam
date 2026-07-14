#ifndef PEAKSATT
#define PEAKSATT

#include "defines.h"
#include <iostream>

class peaksatt {
	public: 

	  peaksatt();
	  peaksatt(int t_ini, int t_end, mapa2 spectrum_peaks); 
	  peaksatt(const peaksatt& patt);
	
	  mapa2 specpeaks;	// CREO QUE DEBERIA SER UN PUNTERO!!
	  int tini;
	  int tend;
};

std::ostream &operator<<(std::ostream &output, peaksatt patt);

typedef std::vector<peaksatt> peaksattvector;
typedef std::vector<peaksatt>::iterator iterpeaksattvector;

#endif
