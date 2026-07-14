#ifndef DEFINES
#define DEFINES

#include <iostream>
#include <map>
#include <set>
#include <vector>



/*--------------------------------------------

 Ordering of integers in a hash_set

---------------------------------------------*/

struct ltint
{
  bool operator()(const int i1, const int i2) const
  {
    return i1<i2;
  }
};

/****
 NOT USED!
***/

struct ltdoub
{
  bool operator()(const double s1, const double s2) const
  {
    return s1>s2;
  }
};

/*----------------------------------------------

  Ordering of spectral patterns (probability ordering) NOT USED!!!!

----------------------------------------------*/

//struct ltsp
//{
//  bool operator()(const spectralpattern sp1, const spectralpattern sp2) const
//  {
//  	return sp1.prob>sp2.prob;
//  }
//};

typedef std::map<int, double, ltint> mapa2;
typedef std::map<int, double, ltint>::iterator itermapa2;

typedef std::vector<mapa2> peaksvector;
typedef std::vector<mapa2>::iterator iterpeaksvector;

typedef std::pair<int,double> notepair;
typedef std::vector<notepair> notesvector;

typedef std::set<int> hashset;
typedef std::vector< std::vector<double> > Tmatrix;


#endif
