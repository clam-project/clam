/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "DynamicType.hxx"

#include <iostream>  // used in Debug and (depracated) MandatoryInit
#ifdef CLAM_USE_XML
#	include <fstream>  // idem
#endif
#include <list>	   // needed in UpdateData 

#ifdef CLAM_USE_XML
#	include "XMLStaticAdapter.hxx"
#	include "XMLStorage.hxx"
#endif//CLAM_USE_XML

#include "ErrDynamicType.hxx"

// IMPORTANT: this next flag will slow all the dynamic types mechanisms
// So it can only be set (defined) when testing or bug-hunting with
// dynamic types.

// #define CLAM_EXTRA_CHECKS_ON_DT

// If necessary, this flag should be defined in the test project/makefile

namespace CLAM {

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


DynamicType::DynamicType() : _data(0), _bPreAllocateAllAttributes(false)
{}

/// \todo fix deepCopy appereances
DynamicType::DynamicType(const DynamicType& prototype)
{
	bool deepCopy = true; // cludge \todo !

	if (prototype._data)
	{
		if (deepCopy)
			; //SelfDeepCopy(prototype);
		else
			SelfShallowCopy(prototype);
	}
	else 
		SelfCopyPrototype(prototype);
}
DynamicType::~DynamicType(){
	RemoveAllMem();
}

void DynamicType::RemoveAllMem()
{
	// we avoid calling GetStaticInfo() which is virtual.

	if ( !DynamicInfoIsInit() ) // nothing to remove. 
		// Morover calling GetDynamicInit would generate a virtual call.
		return;
	
	// CheckInvariantIfExtraChecksIsSet(); 
	// Non sure: it can get called from the destr. caused by an exception and rise another one
	// note: FullfilsInvariant doesn't call virtual GetStaticInfo.

	if (_data) {
		const int numAttr = GetDynamicInfo().NumAttr();

		for (int i=0; i<numAttr; i++) {
			const bool attrHasData = GetDynamicInfo().GetAttrInfo(i).HasData();
			if ( attrHasData ){
				const DestructorInplaceFn dest = GetDynamicInfo().GetCachedStaticInfo().GetAttrInfo(i).destructObj;
				// tricky: we access static info through
				const int attrOffs = GetDynamicInfo().GetAttrInfo(i).GetOffs();
				dest ( _data+attrOffs );
			}
		}
		delete [] _data;
		_data = 0;
	}
	GetDynamicInfo().DecrementRefCount();
}



void DynamicType::MandatoryInit() {
	std::cerr << "MandatoryInit is not Not longer useful. Users of DTs can write its normal C++ constructors";	
}

void DynamicType::DefaultInit() {
	std::cerr << "DynamicType::DefaultInit depracated: \n"
		"No more need of pseudo-constructors. Concrete DTs can define its constructors";
}

void DynamicType::CopyInit(const DynamicType & dt) {
		std::cerr << "DynamicType::DefaultInit depracated: \n"
		"No more need of pseudo-constructors. Concrete DTs can define its constructors";
}

//////////////////////////////////////////////////////////////////////////////////////////////////7
// return whether some update has been made.
bool DynamicType::UpdateData()
{

	CheckInvariantIfExtraChecksIsSet();

	// if no AddXXX or RemoveXXX has been done then the update is not necessary
	if( !GetDynamicInfo().AnyAdded() && !GetDynamicInfo().AnyRemoved() )
		return false;

	
	const int maxAttrSize = GetStaticInfo().TotalAttrSize();
	int dataSize = GetDynamicInfo().GetDataSize();
	const int allocatedDataSize = GetDynamicInfo().GetAllocatedDataSize();

	CLAM_DEBUG_ASSERT(maxAttrSize >= allocatedDataSize && maxAttrSize >= dataSize, 
		"DT::UpdateData() data sizes error");

	if (_bPreAllocateAllAttributes) {
		GetDynamicInfo().SetDataSize( maxAttrSize );
		dataSize = maxAttrSize;
	}
	
	if (dataSize <= allocatedDataSize && allocatedDataSize-dataSize > shrinkThreshold)  
		// this "shrinkThreshold" constant  decides when to 
		// reallocate (and shrink or _compact_) memory
	{
		UpdateDataByShrinking();
		return true;
	} else if (dataSize==maxAttrSize && allocatedDataSize < maxAttrSize) 
		// it's the first that dataSize reach the maximum. (probably by the use of bPreAllocatedAllAttr flag.
		// now the offsets will be taken from the static table 
	{
		UpdateDataGoingToPreAllocatedMode();
		return true;

	} else if (dataSize==maxAttrSize && dataSize<=allocatedDataSize) 
	{
		// in this PreAllocatedMode the attr. offsets are fixed by the static table.
		UpdateDataInPreAllocatedMode();
		return true;
	}
	
	// else: memory has increasead or the amount decreased is bigger than the threshold
	// so do it in the STANDARD MODE (reallocate and compact memory)
	UpdateDataByStandardMode();
	
	CheckInvariantIfExtraChecksIsSet();
	
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
/** Updata support functions */

/** SHRINK MODE: now we'll reuse the allocated data table.
 * two traversals: the first one is for moving the existing attributes:
 */
void DynamicType::UpdateDataByShrinking()
{
	const int numAttr = GetDynamicInfo().NumAttr();
	
	std::list< std::pair<int,int> > attrList(numAttr);  
	std::list< std::pair<int,int> >::iterator it;

	{
		int i=0;
		for (it=attrList.begin(); it!=attrList.end(); it++)
		{
			(*it).first = GetDynamicInfo().GetAttrInfo(i).GetOffs();
			(*it).second = i++;
		}
	}
	attrList.sort();

	int offs=0;
	

	for (it=attrList.begin(); it!=attrList.end(); it++)
	{
		const int j = (*it).second; // ordered attribute index
		
		const NewCopyInplaceFn   newc  = GetStaticInfo().GetAttrInfo(j).newObjCopy;
		const DestructorInplaceFn dest = GetStaticInfo().GetAttrInfo(j).destructObj;
		const NewInplaceFn fnew = GetStaticInfo().GetAttrInfo(j).newObj;
		
		const bool attrHasData = GetDynamicInfo().GetAttrInfo(j).HasData();
		const bool attrRemoved = GetDynamicInfo().GetAttrInfo(j).Removed();
		const int attrOffs = GetDynamicInfo().GetAttrInfo(j).GetOffs();
		const int attrSize = GetStaticInfo().GetAttrInfo(j).size;

		if ( attrHasData && !attrRemoved) {
			if ( attrOffs != offs ) { // only move data if necessary
				/// \todo It can be optimized in the case that the intermediate copy is not needed.
				char* aux = new char[attrSize];
				newc( aux,_data+attrOffs );
				dest( _data+attrOffs );
				newc( _data+offs,aux );
				dest( aux );
				delete [] aux;
				GetDynamicInfo().GetAttrInfo(j).SetOffs( offs );
			}
			offs += attrSize;
		} else if (attrHasData && attrRemoved) {
			dest( _data+attrOffs );
			GetDynamicInfo().GetAttrInfo(j).SetOffs( -1 );
			GetDynamicInfo().GetAttrInfo(j).UnsetRemoved();
		}
	} 
	// now it's time for the new (added) attributes
	for (int i=0; i<numAttr; i++) {
		const bool attrAdded = GetDynamicInfo().GetAttrInfo(i).Added();
		const int attrSize = GetStaticInfo().GetAttrInfo(i).size;

		if ( attrAdded ) {
			const NewInplaceFn fnew = GetStaticInfo().GetAttrInfo(i).newObj;
			fnew( _data+offs );
			GetDynamicInfo().GetAttrInfo(i).SetOffs( offs );
			offs += attrSize;
			GetDynamicInfo().GetAttrInfo(i).UnsetAdded();
		}
	}

	GetDynamicInfo().UnsetAnyRemoved();
	GetDynamicInfo().UnsetAnyAdded();
}

//  STANDARD MODE (reallocate and compact memory)
void DynamicType::UpdateDataByStandardMode ()
{
	const int dataSize = GetDynamicInfo().GetDataSize();
	char* olddata = _data;
	_data = new char[dataSize];

	int offs=0;
	const int numAttr = GetDynamicInfo().NumAttr();

	for (int i=0; i<numAttr; i++) {
		const DestructorInplaceFn dest = GetStaticInfo().GetAttrInfo(i).destructObj;
		const NewCopyInplaceFn newc = GetStaticInfo().GetAttrInfo(i).newObjCopy;
		const NewInplaceFn fnew=GetStaticInfo().GetAttrInfo(i).newObj;
		
		const bool attrHasData = GetDynamicInfo().GetAttrInfo(i).HasData();
		const bool attrRemoved = GetDynamicInfo().GetAttrInfo(i).Removed();
		const bool attrAdded = GetDynamicInfo().GetAttrInfo(i).Added();
		const int attrOffs = GetDynamicInfo().GetAttrInfo(i).GetOffs();
		const int attrSize = GetStaticInfo().GetAttrInfo(i).size;

		if ( attrHasData ) {
			if ( attrRemoved ) {
				
				dest (olddata+attrOffs);
				GetDynamicInfo().GetAttrInfo(i).SetOffs( -1 );
				GetDynamicInfo().GetAttrInfo(i).UnsetRemoved();
			} else {
				newc( _data+offs,olddata+attrOffs );
				dest( olddata+attrOffs );
				GetDynamicInfo().GetAttrInfo(i).SetOffs( offs );
				offs += attrSize;
			}
		} else { // !attrHasData
			if ( attrAdded ) {
				fnew( _data+offs );
				GetDynamicInfo().GetAttrInfo(i).UnsetAdded();
				GetDynamicInfo().GetAttrInfo(i).SetOffs( offs );
				offs += attrSize;
			}
		}
	} // for
		
	delete [] olddata;
	GetDynamicInfo().SetAllocatedDataSize( dataSize );
	GetDynamicInfo().UnsetAnyAdded();
	GetDynamicInfo().UnsetAnyRemoved();
}

// the last reallocation to the DT live
void DynamicType::UpdateDataGoingToPreAllocatedMode()
{	
	const int maxAttrSize = GetStaticInfo().TotalAttrSize();
	char* olddata = _data;
	_data = new char[maxAttrSize];
	const int numAttr = GetDynamicInfo().NumAttr();
	const int dataSize = GetDynamicInfo().GetDataSize();

	// from now one we'll use these pre-fixed offsets.
	// the copy of attributes:
	for (int i=0; i<numAttr; i++)
	{
		const DestructorInplaceFn dest = GetStaticInfo().GetAttrInfo(i).destructObj;
		const NewCopyInplaceFn newc = GetStaticInfo().GetAttrInfo(i).newObjCopy;
		const NewInplaceFn fnew=GetStaticInfo().GetAttrInfo(i).newObj;

		const int newAttrOffs = GetStaticInfo().GetAttrInfo(i).offset;
		const int attrOffs = GetDynamicInfo().GetAttrInfo(i).GetOffs();
		const bool attrRemoved = GetDynamicInfo().GetAttrInfo(i).Removed();
		const bool attrAdded = GetDynamicInfo().GetAttrInfo(i).Added();
		const bool attrHasData = GetDynamicInfo().GetAttrInfo(i).HasData();

		if ( attrHasData ) {
			if ( attrRemoved ) {
				dest( olddata+attrOffs) ;
				GetDynamicInfo().GetAttrInfo(i).UnsetRemoved();
				GetDynamicInfo().GetAttrInfo(i).SetOffs(-1);
			} else {
				
				newc( _data+newAttrOffs, olddata+attrOffs );
				dest( olddata+attrOffs );
				GetDynamicInfo().GetAttrInfo(i).SetOffs( newAttrOffs );
			}
		}
		else { // !attrHasData
			if ( attrAdded ) {
				fnew( _data+newAttrOffs );
				GetDynamicInfo().GetAttrInfo(i).UnsetAdded();
				GetDynamicInfo().GetAttrInfo(i).SetOffs( newAttrOffs );
			}
		}
	} // for each attribute.

	delete [] olddata;
	GetDynamicInfo().SetAllocatedDataSize( dataSize );
	GetDynamicInfo().UnsetAnyAdded();
	GetDynamicInfo().UnsetAnyRemoved();
	
}

void DynamicType::UpdateDataInPreAllocatedMode()
{
	// now, no reallocation.
	// we'll use these pre-fixed offsets. 
	// we need no attributes-copies. only creations and destructions:
	const int numAttr = GetDynamicInfo().NumAttr();

	for ( int i=0; i<numAttr; i++) {
		const DestructorInplaceFn dest = GetStaticInfo().GetAttrInfo(i).destructObj;
		const NewInplaceFn fnew = GetStaticInfo().GetAttrInfo(i).newObj;
		const int offs = GetStaticInfo().GetAttrInfo(i).offset;
		const bool attrHasData = GetDynamicInfo().GetAttrInfo(i).HasData();
		const bool attrRemoved = GetDynamicInfo().GetAttrInfo(i).Removed();
		const bool attrAdded = GetDynamicInfo().GetAttrInfo(i).Added();

		if ( attrHasData ) {
			if ( attrRemoved ) {
				dest( _data+offs );
				GetDynamicInfo().GetAttrInfo(i).UnsetRemoved();
				GetDynamicInfo().GetAttrInfo(i).SetOffs( -1 );
			}
			// else leave the attribute in peace.
		} else { // !AttrHasData(i)
			if ( attrAdded ) {
				fnew( _data+offs );
				GetDynamicInfo().GetAttrInfo(i).UnsetAdded();
				GetDynamicInfo().GetAttrInfo(i).SetOffs( offs );
			}
		}
	} // for each attribute.

	GetDynamicInfo().UnsetAnyAdded();
	GetDynamicInfo().UnsetAnyRemoved();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// these methods are important to be inline. TODO: check if the compiler does it automatically
// or should be better move them to the .hxx

/*
int DynamicType::DynTableRefCounter()
{
	return dynamicTable[numAttr].offs;
}
void DynamicType::InitDynTableRefCounter()
{
	dynamicTable[numAttr].offs = 1;       //at least the object that has created the table points to it.
}





int DynamicType::DecrementDynTableRefCounter()
{
	return --dynamicTable[numAttr].offs;
}
int DynamicType::IncrementDynTableRefCounter()
{
	return ++dynamicTable[numAttr].offs;
}
*/

//////////////////////////////////////////////////////////////////////
// Component interface implementation
//////////////////////////////////////////////////////////////////////

Component* DynamicType::ShallowCopy() const
{
	DynamicType* selfCopy = &(GetDynamicTypeCopy(false));

	return selfCopy;
}


Component* DynamicType::DeepCopy() const
{	
	DynamicType* selfCopy = &(GetDynamicTypeCopy(true));

	return selfCopy;
};


void DynamicType::SelfCopyPrototype(const DynamicType &prototype)
{
	RemoveAllMem(); // deletes all mem in data and calls de destructor of every object.
	
	// the _dynInfo._dynInfoImpl is not copied, but is referenced (reference counter incremented)
	_dynInfo = prototype._dynInfo;
	_bPreAllocateAllAttributes = prototype._bPreAllocateAllAttributes;
}

bool DynamicType::ExistAttr(unsigned id) const
{
	return GetDynamicInfo().ExistAttr(id);
}

void DynamicType::SelfShallowCopy(const DynamicType &prototype)
{
	// that counter is in the table, so it is shared by prototype and this
	// this check is not really necessary, but it can be usefull to catch bugs of the user of this class
	CLAM_ASSERT( prototype.GetDynamicInfo().AnyAdded() ||prototype.GetDynamicInfo().AnyRemoved(),
		"DT::SelfShallowCopy(arg) copy argument is not updated (has added or removed attrs) "
		"call UpdateData before copy" );

	SelfCopyPrototype(prototype);

	const int allocatedDataSize = GetDynamicInfo().GetAllocatedDataSize();
	const int numAttr = GetDynamicInfo().NumAttr();

	_data = new char[allocatedDataSize];

	for ( int i = 0; i < numAttr; i++ ) {
		if (!ExistAttr(i)) continue;
		void* pos = GetPtrToData_(i);
		const NewCopyInplaceFn fcopy = GetStaticInfo().GetAttrInfo(i).newObjCopy;
		fcopy(pos, prototype.GetPtrToData_(i));
	}
}

/*
void DynamicType::SelfDeepCopy(const DynamicType &prototype)
{
	// that counter is in the table, so it is shared by prototype and this
	// this check is not really necessary, but it can be usefull to catch bugs of the user of this class
	if (prototype.dynamicTable[prototype.numAttr].hasBeenAdded || 
		prototype.dynamicTable[prototype.numAttr].hasBeenRemoved)
		throw ErrDynamicType("in SelfDeepCopy(). A copy is not allowed when the origin object is not updated.\
		call Update() before making the copy.");

	SelfCopyPrototype(prototype);

	data = new char[allocatedDataSize];

	// Copies (deepCopy) all the objects pointed by this dynamic type that derives from
	// Component. Copies this object and then link the copy of this, with the
	// children copies.
	Component** copyChildren = new Component * [prototype.numAttr];   // could be done without a table if space efficency is needed
	unsigned int i;
	for (i = 0; i < numAttr; i++)
	{
		if (prototype.ExistAttr(i) && GetStaticInfo().GetAttrInfo(i).isComponent && GetStaticInfo().GetAttrInfo(i).isPointer)
			copyChildren[i] = static_cast<Component*>(prototype.GetDataAsPtr_(i))->DeepCopy();
		else
			copyChildren[i] = 0;
	}

	for (i = 0; i < numAttr; i++)
	{
		if (!ExistAttr(i)) continue;
		void* pos = GetPtrToData_(i);
		if(copyChildren[i])
			SetDataAsPtr_(i, copyChildren[i]);
		else
		{
			//now a nested object must be replaced. It maight be a pointer not registered as it.
			//the nested object will be copied from the nested object at "this"
			const NewCopyInplaceFn fcopy = GetStaticInfo().GetAttrInfo(i).newObjCopy;
			fcopy(pos, prototype.GetPtrToData_(i));
		}
	}
	delete [] copyChildren;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////

DynamicType& DynamicType::operator=(const DynamicType& source)
{
//	SelfDeepCopy(source);
	SelfShallowCopy(source);

	CheckInvariantIfExtraChecksIsSet();

	return *this;
}

/////////////////////////////////////////////////////////////////////////////////////77
// Developing aids methods: FullfilsInvariant and Debug

void DynamicType::FullfilsInvariant() const
{
	DynamicInfo &dInfo = GetDynamicInfo(); //shortcut
	StaticInfo &sInfo = dInfo.GetCachedStaticInfo(); // avoiding direct GetStaticInfo which is virtual
	
	const char* className = sInfo.GetClassName(0);
	const int dataSize = dInfo.GetDataSize();
	const int allocatedDataSize = dInfo.GetAllocatedDataSize();
	const int numAttr = dInfo.NumAttr();

	int auxAllocatedSize=0;
	bool someAdded = false, someRemoved = false;
	int incData=0, decData=0;
	bool *usedblock = new bool[allocatedDataSize];
		
	for (int j=0; j<allocatedDataSize; j++) usedblock[j] = false;

	for (int i=0; i<numAttr; i++)
	{
		
		const bool attrAdded = dInfo.GetAttrInfo(i).Added();
		const bool attrRemoved = dInfo.GetAttrInfo(i).Removed();
		const int attrOffs = dInfo.GetAttrInfo(i).GetOffs();
		const bool attrHasData = dInfo.GetAttrInfo(i).HasData();
		const int attrSize = sInfo.GetAttrInfo(i).size;
	
		// check state consistency.
		if ( attrAdded && attrRemoved ) 
			throw ErrDynamicType("in FullfilsInvariant: an attribute has both Added & Removed flags set. Class: ", GetClassName() );

		if ( attrAdded ) someAdded = true;
		if ( attrRemoved ) someRemoved = true;

		if ( attrOffs < -1 ) 
			throw ErrDynamicType("in FullfilsInvariant: a dynamic offset < -1");
		if( ! attrHasData && attrRemoved )  
			throw ErrDynamicType(" in FullfilsInvariant: an attribute has"
				"no data (offs==-1) but do has the hasBeenRemoved flag set. Class: ", className );
		
		if( attrHasData && attrAdded )  
			throw ErrDynamicType(" in FullfilsInvariant: an attribute has"
				"data (offs>0) but do has the hasBeenAdded flag set. Class: ", className );
		// data size calculation
		if ( attrOffs >= 0) 
		{
			auxAllocatedSize += attrSize;
			for (int j = attrOffs; j < attrOffs + attrSize; j++)
				if (usedblock[j]) throw ErrDynamicType("in FullfilsInvariant: overlaped area in data table");
				else usedblock[j]=true;
		}
		if ( attrHasData ) 
		{
			if ( attrRemoved ) decData += attrSize;
			if (!_data) throw ErrDynamicType("in FullfilsInvariant: An attr. has data but data==0");
		}
		else 
			if ( attrAdded ) incData += attrSize;
		
		else if ( attrOffs != -1) 
			throw ErrDynamicType(" in FullfilsInvariant: attribute not informed with dynamic offset <> -1");

	}
	if (!_bPreAllocateAllAttributes) {
		if (auxAllocatedSize+incData-decData != dataSize) 
			throw ErrDynamicType("in FullfilsInvariant: dataSize attribute is not consistent. Class: ", className );
		if (auxAllocatedSize + incData - decData != dataSize)
			throw ErrDynamicType("in FullfilsInvariant: the dataSize is not well calculated. Class: ", className );
	}
	if (auxAllocatedSize > allocatedDataSize) 
		throw ErrDynamicType("in FullfilsInvariant: allocatedDataSize attribute is not consistent. Class: ", className );
	if ( dInfo.AnyAdded() != someAdded ) 
		throw ErrDynamicType("in FullfilsInvariant: global 'hasBeenAdded' flag inconsistent. Class: ", className );
	if ( dInfo.AnyRemoved() != someRemoved ) 
		throw ErrDynamicType("in FullfilsInvariant: global 'hasBeenRemoved' flag inconsistent. Class: ", className );
	
	delete[] usedblock;
}


void DynamicType::Debug()
{
	const int dataSize = GetDynamicInfo().GetDataSize();
	const int allocatedDataSize = GetDynamicInfo().GetAllocatedDataSize();
	const int maxAttrSize = GetStaticInfo().TotalAttrSize();
	const int numAttr = GetDynamicInfo().NumAttr();

	std::cout <<std::endl<<"Class Name: "<< GetClassName() << " at: " << this <<std::endl << \
		"[#attr.], dyn_offs,statc_offs,name,type,{comp,dynType,ptr,strble},exist,size,Ptr"\
		<< std::endl << "------------------------------------------------------------------------------"<<std::endl;
	std::cout << "{ size, allocatedSize, maxAttrsSize } = { " << dataSize << " , " << allocatedDataSize << " , "
			<< maxAttrSize << " }\n";
	for (int i=0; i<numAttr; i++)
	{
		const bool attrAdded = GetDynamicInfo().GetAttrInfo(i).Added();
		const bool attrRemoved = GetDynamicInfo().GetAttrInfo(i).Removed();
		const int attrOffs = GetDynamicInfo().GetAttrInfo(i).GetOffs();
		const bool attrHasData = GetDynamicInfo().GetAttrInfo(i).HasData();
		const AttrStaticInfo &attr = GetStaticInfo().GetAttrInfo(i);
		
		std::cout << std::endl;
		if ( attrAdded ) std::cout << " A";
		else std::cout << " -";
		if ( attrRemoved ) std::cout << "R";
		else std::cout << "-";
		

		std::cout << " [" <<i<<"] ";

		std::cout << attrOffs << " , "<< attr.offset <<" , "<< attr.name <<" , "<< attr.type <<" , {"\
			<< attr.isComponent <<","<< attr.isDynamicType <<","\
			<< "} , "<<ExistAttr(i)<<" , "<<attr.size\
			<<" , ";
		if(ExistAttr(i)) 
			std::cout << GetPtrToData_(i);
				
	}
	std::cout<<std::endl;

#	ifdef CLAM_USE_XML
		XMLStorage storage ("XML-Class_Debug");
		std::fstream fileout("Debug.xml", std::ios::out);
		storage.Dump(*this, GetClassName(), fileout);
#	endif//CLAM_USE_XML
}

void DynamicType::StoreOn(CLAM::Storage & s) {
	this->StoreDynAttributes(s);
}

void DynamicType::LoadFrom(CLAM::Storage & s) {
	this->LoadDynAttributes(s);
}


}; //namespace CLAM
