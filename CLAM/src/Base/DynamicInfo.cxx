#include "DynamicInfo.hxx"

#include "DynamicType.hxx"
#include "Assert.hxx"

namespace CLAM
{
DynamicInfo::~DynamicInfo() {
	if (_dynInfoImpl && RefCount()==0) {
		delete [] _dynInfoImpl;
	}
}

void DynamicInfo::Init(DynamicType* p) {
	if (_parentDT) return;
		_parentDT = p;
		_cachedStaticInfo = &_parentDT->GetStaticInfo();
		_numAttr = _cachedStaticInfo->NumAttr();
		_dynInfoImpl = new AttrDynamicInfo[_numAttr+1]; // the last element doesn't describe
		                                                // an attribute, but the whole DT
		InitRefCount();
}

void DynamicInfo::InitRefCount() {
	_dynInfoImpl[NumAttr()].SetOffs( 1 ); // this references AttrDynamicInfo array. So, one reference
}

void DynamicInfo::IncrementRefCount() {
	_dynInfoImpl[NumAttr()].SetOffs( RefCount()+1 );
}

void DynamicInfo::DecrementRefCount() {
	_dynInfoImpl[NumAttr()].SetOffs( RefCount()-1 );
}

int DynamicInfo::RefCount() const {
	return _dynInfoImpl[NumAttr()].GetOffs();
}

bool DynamicInfo::ExistAttr( int i ) {
	if ( !_parentDT->HasData() ) return false;
	
	const bool attrHasData = GetAttrInfo(i).HasData();
	const bool attrAdded = GetAttrInfo(i).Added();
	const bool attrRemoved = GetAttrInfo(i).Removed();

	return (attrHasData && !attrAdded && !attrRemoved); 
}

DynamicInfo::AttrDynamicInfo& DynamicInfo::GetAttrInfo (int i ) {
	CLAM_DEBUG_ASSERT( _parentDT!=0, "at(int) without init" );
	CLAM_DEBUG_ASSERT( i>=0 && i<= _numAttr, "DT::DynamicInfo::GetAttrInfo bad index");

	return _dynInfoImpl[i];
}

bool DynamicInfo::AttrDynamicInfo::Added() { 
	return _added; 
}

bool DynamicInfo::AttrDynamicInfo::Removed() { 
	return _removed; 
}

void DynamicInfo::AttrDynamicInfo::SetAdded() { 
	_added = true; 
}

void DynamicInfo::AttrDynamicInfo::SetRemoved() { 
	_removed = true; 
}

void DynamicInfo::AttrDynamicInfo::UnsetAdded() { 
	_added = false;
}

void DynamicInfo::AttrDynamicInfo::UnsetRemoved() { 
	_removed = false; 
}

bool DynamicInfo::AnyRemoved() {
	return _dynInfoImpl[NumAttr()].Removed();
}

bool DynamicInfo::AnyAdded() {
	return _dynInfoImpl[NumAttr()].Added();
}

void DynamicInfo::UnsetAnyRemoved() {
	_dynInfoImpl[NumAttr()].UnsetRemoved();
}

void DynamicInfo::UnsetAnyAdded() {
	_dynInfoImpl[NumAttr()].UnsetAdded();
}

void DynamicInfo::TryToUnsetAnyRemoved() {
	// search for a Removed attribute. If not found
	// unset AnyRemoved
	for (int i=0; i<NumAttr(); i++) {
		if ( _dynInfoImpl[i].Removed() ) 
			return;
	}
	UnsetAnyRemoved();
}

void DynamicInfo::TryToUnsetAnyAdded() {
	// search for an Added attribute. In not found,
	// unset AnyAdded
	for (int i=0; i<NumAttr(); i++) {
		if ( _dynInfoImpl[i].Added() )
			return;
	}
	UnsetAnyAdded();
}

void DynamicInfo::SetAnyRemoved() {
	_dynInfoImpl[NumAttr()].SetRemoved();
}

void DynamicInfo::SetAnyAdded() {
	_dynInfoImpl[NumAttr()].SetAdded();
}

void DynamicInfo::CreateASeparatedDynInfoImpl() {
	DecrementRefCount();
	AttrDynamicInfo *old = _dynInfoImpl;
	_dynInfoImpl = new AttrDynamicInfo[NumAttr()+1];
	for (int i=0; i<NumAttr(); i++) 
		_dynInfoImpl[i] = old[i];
	InitRefCount();
}


void DynamicInfo::AddAttr( int idAttr, int attrSize ) {
	if ( GetAttrInfo(idAttr).Added() ) 
		return;

	if ( RefCount() > 1)  // since its dynamic info is shared, we need to separate it
		CreateASeparatedDynInfoImpl();

	if ( GetAttrInfo(idAttr).Removed() ) {
		GetAttrInfo(idAttr).UnsetRemoved();
		TryToUnsetAnyRemoved();
	} else {
		GetAttrInfo(idAttr).SetAdded();
		SetAnyAdded();
	}
	
	++_numInstantiatedAttr;
	_dataSize += attrSize;

	_parentDT->CheckInvariantIfExtraChecksIsSet();
}


void DynamicInfo::RemoveAttr( int idAttr, int attrSize ) {
	if ( GetAttrInfo(idAttr).Removed() )
		return;

	if ( RefCount() > 1)  // since its dynamic info is shared, we need to separate it
		CreateASeparatedDynInfoImpl();

	if ( GetAttrInfo(idAttr).Added() ) {
		GetAttrInfo(idAttr).UnsetAdded();
		TryToUnsetAnyAdded();
	} else {
		GetAttrInfo(idAttr).SetRemoved();
		SetAnyRemoved();
	}
	--_numInstantiatedAttr;
	_dataSize -= attrSize;

	_parentDT->CheckInvariantIfExtraChecksIsSet();
}

DynamicInfo& DynamicInfo::operator= (const DynamicInfo& arg) {
	_parentDT = arg._parentDT;
	_numInstantiatedAttr = arg._numInstantiatedAttr;
	_numAttr = arg._numAttr;
	_dataSize = arg._dataSize;
	_allocatedDataSize = arg._allocatedDataSize;
	_dynInfoImpl = arg._dynInfoImpl; //C array
	
	IncrementRefCount();
	
	return *this;
}


} // namespace CLAM
