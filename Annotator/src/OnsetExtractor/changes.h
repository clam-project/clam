#ifndef CHANGES
#define CHANGES

#include "params.h"
#include "defines.h"
#include "ocsound.h"
#include <math.h>
#include <iostream>

void selectpeaks(std::vector<double> &changesvect, int total_time); 
void applyderivative(std::vector<double> &onebandvector, int context);
void processmatrix(Tmatrix bands, double maxbandsval, std::vector<double>& output, int context); 
void applythreshold(std::vector<double> &vectchanges, double maxbandsval, int context);
void computechanges(int total_time, double *changesvect, Tmatrix &bands, double maxvalband, double timeres, char *filename, int context);

#endif
