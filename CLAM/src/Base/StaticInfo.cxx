#include "StaticInfo.hxx"
#include "Assert.hxx"

namespace CLAM
{

const char* StaticInfo::GetClassName(int i) const { 
	CLAM_ASSERT( i>=0 && i<NumAttr(), "DT::StaticInfo::GetClassName bad index" );
	return _classNames[i];
}

void StaticInfo::AddClassName( char* name) {
	_classNames.push_back( name );
}

void StaticInfo::AddAttr( StaticInfo::AttrStaticInfo& info ) { 
	info.offset = _totalSize = info.size + _attributes.back().offset;
	_attributes.push_back( info );
}

void StaticInfo::GetTypeInfo( const void* ptr, bool& isComponent, bool& isDynamicType  )
{
	isComponent = false;
	isDynamicType = false;
}

void StaticInfo::GetTypeInfo( const Component* ptr, bool& isComponent, bool& isDynamicType )
{
	isComponent = true;
	isDynamicType = false;
}

void StaticInfo::GetTypeInfo(const DynamicType* ptr, bool& isComponent, bool& isDynamicType )
{
	isComponent = true;
	isDynamicType = true;
}



} // namespace CLAM