
#include <CLAM/Core/ProcessingData.hxx>
#include "TimeIndex.hxx"


namespace CLAM {

  void TimeIndex::DefaultInit()
  {
    AddPosition();
    AddWeight();
    UpdateData();
  }
}
