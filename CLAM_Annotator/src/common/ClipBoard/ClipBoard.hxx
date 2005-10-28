#ifndef CLIPBOARD_HXX
#define CLIPBOARD_HXX

#include "SongFiles.hxx"
#include <vector>

class ClipBoard : public std::vector<CLAM_Annotator::Song*>
{
public:
  static ClipBoard * getInstance();
protected:
  ClipBoard()
    {
    }
};

#endif

