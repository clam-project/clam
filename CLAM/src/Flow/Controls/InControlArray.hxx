#ifndef _InControlArray_hxx_
#define _InControlArray_hxx_

#include "InControl.hxx"
//#include <vector>

namespace CLAM
{
/**
 * This class wrapps a vector (array) of InControls and gives its InControl automatic
 * names at construction
 */ 
class InControlArray
{
	Array<InControl*> mArray;
public:

	inline InControlArray(
		int size, 
		const std::string &name, 
		Processing* parent = 0);

	inline ~InControlArray();

	inline InControl       &operator[](int i)        { return *mArray[i]; }
	inline const InControl &operator[](int i) const  { return *mArray[i]; }
};


InControlArray::InControlArray(int size,
                               const std::string &name,
                               Processing *parent /*0 by default. See decl above*/)
{
	mArray.Resize(size);
	mArray.SetSize(size);
	for (int i=0; i<size; i++) {
		std::stringstream str;
		str << name << "_" << i;
		if (parent)
			mArray[i] = new InControl(str.str(),parent);
		else
			mArray[i] = new InControl(str.str());
	}
}

InControlArray::~InControlArray()
{
	int size = mArray.Size();
	for (int i=0; i<size; i++)
		delete mArray[i];
}

} //namespace CLAM

#endif
