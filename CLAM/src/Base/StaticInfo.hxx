
#ifndef _StaticInfo_hxx_
#define _StaticInfo_hxx_

#include <vector>
#include "Assert.hxx"

namespace CLAM
{

// forward declarations
class DynamicType;
class DynamicInfo;
class Component;



/// Helper class for DynamicType. This class that holds the static info.
/// Thus is shared among all the instance of the same concrete DT class.
class StaticInfo
{
	// only DT can create StaticInfo
	friend DynamicType;
	// well, a DT can access through a DynamicInfo
	friend DynamicInfo;

	StaticInfo() : _totalSize(0) {}

	class AttrStaticInfo
	{
		friend DynamicType; // because DT imports its typedefs

		typedef void* (*NewInplaceFn)(void* pos);
		typedef void* (*NewCopyInplaceFn)(void* pos,void* orig);
		typedef void (*DestructorInplaceFn)(void* pos);

	public:	
		char *name;                   
		char *type;
		int size;
		int offset; // calculated from acomulated size
		NewInplaceFn newObj;
		NewCopyInplaceFn newObjCopy;
		DestructorInplaceFn destructObj;
		bool isComponent;
		bool isDynamicType;
	};

public:  
	/// pushes the attribute info enty into its container, and update _totalSize member variable
	void AddAttr( AttrStaticInfo& info );
	
	int NumAttr() const;
	
	/// Gets the n-th class name of the hierarchie
	/// \todo check in which order
	const char* GetClassName(int n) const;
	
	void AddClassName( char* name);
	
	int CountClassNames() const;
	
	int TotalAttrSize();
	/// Returns 'static' info of attr number i. \see \c AttrStaticinfo
	/// Valid index-attribute value is asserted.
	/// \param iAttr ranges from 0 to NumAttr()-1
	/// \todo separate definition from declaration
	const AttrStaticInfo& GetAttrInfo( int iAttr ) const;
	
	/// \todo should be private: (accessed through protected interface of DT) OR NOT!
	static void DeduceTypeInfo( const void* ptr, bool& isComponent, bool& isDynamicType  );
	static void DeduceTypeInfo( const Component* ptr, bool& isComponent, bool& isDynamicType  );
	static void DeduceTypeInfo( const DynamicType* ptr, bool& isComponent, bool& isDynamicType  );

private:
	int _totalSize;
	std::vector<AttrStaticInfo> _attributes;
	std::vector<char*> _classNames;

}; // StaticInfo

// inline method definitions: 

inline int StaticInfo::NumAttr() const { 
	return int( _attributes.size() ); 
}

inline int StaticInfo::CountClassNames() const { 
	return int( _classNames.size() ); 
}

inline int StaticInfo::TotalAttrSize() { 
	return _totalSize; 
}

inline const StaticInfo::AttrStaticInfo& StaticInfo::GetAttrInfo( int iAttr ) const {
	CLAM_ASSERT( iAttr >= 0 && iAttr<NumAttr(), 
		"DT::StaticInfo::GetAttrInfo invalid attr index" );
	return _attributes[iAttr];
}

} // namespace CLAM

#endif
