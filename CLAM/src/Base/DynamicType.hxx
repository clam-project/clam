/* DynamicType.hxx: interface for the DynamicType class.
 * written by Pau Arumí - May 2001
 * new version (that stores every type) : 21-July-2001
 *
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

#ifndef _DynamicType_
#define _DynamicType_

#ifdef CLAM_USE_XML
	#include "XMLAdapter.hxx"
	#include "XMLIterableAdapter.hxx"
	#include "XMLComponentAdapter.hxx"
#endif//CLAM_USE_XML

#include "DynamicTypeMacros.hxx"  //this file is not included anywhere but here.
#include "DynamicInfo.hxx"
#include "StaticInfo.hxx"

#include "Component.hxx"
#include "DataTypes.hxx"

#include <new>


/**
  DynamicTypes had suffered a major refactoring in its internal structure
  in order to implement the new inheritance feature:
  Here it goes a list of changes and a brief description:

  - DT macros don't expand constructors. Hence they remain available to the 
  user. 
    -- pseudo macros (DefaultInit, CopyInit, MandatoryInit) depracated
  
  - New nested classes DynamicInfo and StaticInfo, which takes much of
  the responsability formerly taken by DT.

  - The dependency between these two classes has been carefully setted. In
  a way that InformAll can be called only once per class. And not in the 
  concrete constructor (as before)
    -- A pseudo code illustrating the dependencies:

	GetStaticInfo()                   <------------·
	  if static local p DON'T exist                |
	    Create _staticInfo // of type StaticInfo   |
		InformAll()  // chained method             |
	  endif                                        |
	  return *p                                    |
                                                   |
    GetDynamicInfo()                               |
	_dynamicInfo.Init()                            |
	  if _parentDT don't exist                     |
	    _numAttr=GetStaticInfo().NumAttr() --------·
		create array of (_numAttr) AttrDynInfo s
	  endif
	  return _dynamicInfo

	  /todo InformAll also calls GetStaticInfo (for adding attr)
        
	The first call to GetDynamicInfo will perform a susequent call  to 
	StaticInfo. The first call to StaticInfo will perform the unique 
	call to InformAll, which, using chained methods inform static info and
	calculate the number of attributes.
	Of course InformAll is done *once per concrete class*

  - DynamicType-base constructors no longer necessary, primarly because
  of last item: we don't need to know the number of attributes BEFORE calling
  InformAll.

  - Removed flag and support for share data.

  - Removed flag for deep/shallow copy
  
 
 
 */

namespace CLAM {

/**
 * The base class for dynamic types (DTs).
 * It allows to add and remove attributes at run time. Thus, giving instantiation
 * semantics at attributes, and optimize memory used, if it's the case of 
 * only a subset of attributes is instantiated.
 *
 * All the dynamic attributes interface is strongly typed. So the compiler
 * can garant the type consistency in every access to the dynamic attributes.
 * It also allows herarchic structures and implements de Component interface
 * so it can be stored all the tree (to XML format, for example)
 * and can be copied (swallow or deep copy). (see the methods: SwallowCopy, 
 * DeepCopy and StoreOn )
 * 
 * This class holds all the implementation of the memory management and is an abstract
 * class: is necessary to define a concrete dynamic type (subclass). 
 * A DT must be defined following 
 * a very specific set of rules; basically the attributes are registered using a
 * macros mechanism that expand a known interface for accessing attributes.
 * 
 * As these methods are expanded by macros, they can not be documented inside the
 * concrete dynamic type. Hence they will be documented here:
 * 
 * \see Component
 *
 */
class DynamicType : public Component
{
public:
	DynamicType();
	/**
	* The created object will use the attribute instantiation of the copied DT.
	* @param prototype Another dynamic type from which the dynamic info is taken.
	*/
	DynamicType(const DynamicType& prototype);

	virtual ~DynamicType();
	
	virtual const char* GetClassName() const =0;
	
	virtual StaticInfo& GetStaticInfo() const = 0;

	DynamicInfo& GetDynamicInfo() const;
	
	/**
	* Method used to resize the data space of the dynamic type, necessary when some
	* AddXxx() / RemoveXxx() (where Xxx is an attribute name) has been done.
	* This operation does not check it if has been some attributes changes.
	* In the case that the object is "not owner" of its memory this flag is changed
	* to "owner", and a new data table is created.
	*
	* \return whether some modification has ocurred or not.
	*/
	bool UpdateData();
	
	bool HasData() const { return _data != 0; }


public:
	enum {shrinkThreshold = 80}; // Bytes.  That constant means that when updating data, if the
	                             // used data disminish an amount superior that this threshold,
	                             // data will be reallocated (shrunk)
		
	/// \todo why no call it clone? (make all components clonable?)
	virtual DynamicType& GetDynamicTypeCopy( const bool deep = false ) const =0;
	/// \todo we really need this now?
	virtual Component* ShallowCopy() const;
	virtual Component* DeepCopy() const;
	DynamicType& operator= (const DynamicType& source);
	
	/// \depracated Not longer useful. Users of DTs can write its normal C++ constructors
	void MandatoryInit();

	virtual void StoreOn(CLAM::Storage & s);
	virtual void LoadFrom(CLAM::Storage & s);

	void SetPreAllocateAllAttributes() { _preAllocateAllAttributes=true; }

	/// Developing/testing method
	void Debug();
	/// Developing/testing method
	void FullfilsInvariant() const;
	
	/** This inline method contains actual code only if preprocessor flag: 
	 * CLAM_EXTRA_CHECKS_ON_DT is set.
	 * This method is called at the end of some other class methods, for consistency
	 * checks. It needs to be public because DynamicInfo alse calls it. 
	 */
	void CheckInvariantIfExtraChecksIsSet();

protected:
// Inferface for the macro expanded code:


	/// \depracated no more need of pseudo-constructors. Concrete DTs can define its constructors
	void DefaultInit();
	/// \depracated no more need of pseudo-constructors. Concrete DTs can define its constructors
	void CopyInit(const DynamicType & dt);
	
	// Import of types to be accessible from sub-classes
	typedef StaticInfo::AttrStaticInfo AttrStaticInfo;
	typedef StaticInfo::AttrStaticInfo::NewInplaceFn NewInplaceFn;
	typedef StaticInfo::AttrStaticInfo::NewCopyInplaceFn NewCopyInplaceFn;
	typedef StaticInfo::AttrStaticInfo::DestructorInplaceFn DestructorInplaceFn;
	
	/** Called from the virtual GetStaticInfo, which passes its static pointer
	 * to StaticInfo. And only calls this method when its pointer is not 
	 * initialized. This InformAll is called only once per class.
	 */
	void InitStaticInfo(StaticInfo* & pStaticInfo) const;


	bool ExistAttr(unsigned id) const;
	int NumAttr() const;
	/// Used in macro expanded AddXxx(). Hence, this _protected_ name.
	void _AddAttr( int idAttr, int attrSize ) {	GetDynamicInfo().AddAttr( idAttr, attrSize ); }
	/// Used in macro expanded RemoveXxx(). Hence, this _protected_ name.
	void _RemoveAttr( int idAttr, int attrSize ) { GetDynamicInfo().RemoveAttr( idAttr, attrSize ); }

	int GetAttrOffs( int idAttr ) const;
	
	template <typename AttribType>
	void StoreAttribute(StaticTrue* asLeave, CLAM::Storage &s ,AttribType & object, char* name);
	template <typename AttribType>
	void StoreAttribute(StaticFalse* asLeave, CLAM::Storage &s ,AttribType & object, char* name);
	template <typename AttribType>
	void StoreIterableAttribute(CLAM::Storage &s ,AttribType & object, char* name, char* elemName);
	template <typename AttribType>
	bool LoadAttribute(StaticTrue* asLeave, CLAM::Storage &s ,AttribType & object, char* name);
	template <typename AttribType>
	bool LoadAttribute(StaticFalse* asLeave, CLAM::Storage &s ,AttribType & object, char* name);
	template <typename AttribType>
	bool LoadIterableAttribute(CLAM::Storage &s ,AttribType & object, char* name, char* elemName);

	/// helper nested-class used in macro expanded code.
	template <unsigned int NAttrib> 
	class AttributePositionBase 
	{ 
	public:
		static const int value;
	};
	void CheckBeforeDynamicAttributeAccessor(int idAttr) const; 
	//Long name! so minimizing chances of name collision
	
	enum {InheritanceFollowNumber=0};
	virtual void AddAll() {}
	virtual void RemoveAll() {}
	virtual void InformAll() const { GetStaticInfo().AddClassName( "CLAM::DynamicType" ); };

	template <typename Visitor>	void VisitAll(Visitor & ) {}
	virtual void StoreDynAttributes( Storage& ) {}
	virtual void LoadDynAttributes( Storage& ) {}

	
private:
	bool DynamicInfoIsInit() const { return _dynInfo._parentDT != 0; }
	virtual void RemoveAllMem();
	void* GetPtrToData_(const int id) const;
	void* GetDataAsPtr_(const int id) const;
	void SetDataAsPtr_(const int id, void* p);
	
	/** Helper method for UpdateData(). @see UpdateData() 
	 *  SHRINK MODE: now we'll reuse the allocated data table deleting the gaps.
	 *  two traversals: the first one is for moving the existing attributes:
	 *  the second one for allocating the new attributes
	 */
	void UpdateDataByShrinking();
	/** Helper method for UpdateData(). @see UpdateData() 
	 *  STANDARD MODE: a new reallocation of data table is done.
	 *  and all existing attributes copies (copy constructor)
	 */
	void UpdateDataByStandardMode();
	/** Helper method for UpdateData(). @see UpdateData() 
	 *  Going to Pre Allocated Mode: the last reallocation is done, and the fixed offs are used.
	 */
	void UpdateDataGoingToPreAllocatedMode();
	/** Helper method for UpdateData(). @see UpdateData() Fixed offs (taken from
	 * typeDescTable are used.
	 */
	void UpdateDataInPreAllocatedMode();

	
	void SelfShallowCopy(const DynamicType &orig);
//	void SelfDeepCopy(const DynamicType &orig);
	
// Atributes:
protected:
	///\todo make private
	char* _data;
private:
	bool _preAllocateAllAttributes;
	mutable DynamicInfo _dynInfo; // can't never be const because it's accessed via GetDynamicInfo
};




//////////////////////////////////////////////////////////////////
// Static members definitions

template <unsigned int NAttrib> const int DynamicType::AttributePositionBase<NAttrib>::value = NAttrib;

//////////////////////////////////////////////////////////////////
// Implementation of inline methods

inline DynamicInfo& DynamicType::GetDynamicInfo() const { 
	_dynInfo.Init( const_cast<DynamicType*>(this) );
	return _dynInfo;
}

inline void DynamicType::InitStaticInfo(StaticInfo* & pStaticInfo) const {
	pStaticInfo = new StaticInfo; 
	InformAll();
}

inline int DynamicType::NumAttr() const {
	return GetDynamicInfo().NumAttr();
}

inline int DynamicType::GetAttrOffs( int idAttr ) const {
	return GetDynamicInfo().GetAttrInfo(idAttr).GetOffs();
}

inline void* DynamicType::GetDataAsPtr_(const int idAttr ) const
{
	return *(void**)&_data[ GetAttrOffs(idAttr) ];
}

inline void* DynamicType::GetPtrToData_(const int idAttr) const
{
	return (void*)&_data[ GetAttrOffs(idAttr) ];
}

inline void DynamicType::SetDataAsPtr_(const int idAttr, void* p)
{
	*(void**)&_data[ GetAttrOffs(idAttr) ] = p;
}
inline void DynamicType::CheckBeforeDynamicAttributeAccessor(int idAttr) const
{
	CLAM_DEBUG_ASSERT( idAttr<NumAttr(),
		"There are more registered Attributes than the number "
        "defined in the DYNAMIC_TYPE macro.");
	CLAM_ASSERT( ExistAttr(idAttr),
		"You are trying to access a dynamic attribute "
		" that is not Added or not Updated.");
	CLAM_DEBUG_ASSERT( HasData(),
		"No data allocated for the accessed dynamic type attribute" );
}

inline void DynamicType::CheckInvariantIfExtraChecksIsSet()
{
#	ifdef CLAM_EXTRA_CHECKS_ON_DT
		FullfilsInvariant();
#	endif //CLAM_EXTRA_CHECKS_ON_DT
}

template <typename AttribType>
inline void DynamicType::StoreAttribute(StaticTrue* asLeave, CLAM::Storage &s ,AttribType & object, char* name) {
#	ifdef CLAM_USE_XML
		CLAM::XMLAdapter<AttribType> adapter(object, name, true);
		s.Store (&adapter);
#	endif//CLAM_USE_XML
}

template <typename AttribType>
inline void DynamicType::StoreAttribute(StaticFalse* asLeave, CLAM::Storage &s ,AttribType & object, char* name) {
#	ifdef CLAM_USE_XML
		CLAM::XMLComponentAdapter adapter(object, name, true);
		s.Store (&adapter);
#	endif//CLAM_USE_XML
} 

template <typename AttribType>
inline void DynamicType::StoreIterableAttribute(CLAM::Storage &s ,AttribType & object, char* name, char* elemName) {
#	ifdef CLAM_USE_XML
		CLAM::XMLIterableAdapter<AttribType> adapter(object, elemName, name, true);
		s.Store (&adapter);
#	endif//CLAM_USE_XML
} 

template <typename AttribType>
inline bool DynamicType::LoadAttribute(StaticTrue* asLeave, CLAM::Storage &s ,AttribType & object, char* name) {
#	ifdef CLAM_USE_XML
		CLAM::XMLAdapter<AttribType> adapter(object, name, true);
		return s.Load (&adapter);	
#	else 
		return false;
#	endif//CLAM_USE_XML
}

template <typename AttribType>
inline bool DynamicType::LoadAttribute(StaticFalse* asLeave, CLAM::Storage &s ,AttribType & object, char* name) {
#	ifdef CLAM_USE_XML
		CLAM::XMLComponentAdapter adapter(object, name, true);
		return s.Load (&adapter);	
#	else 
		return false;
#	endif//CLAM_USE_XML
} 

template <typename AttribType>
inline bool DynamicType::LoadIterableAttribute(CLAM::Storage &s ,AttribType & object, char* name, char* elemName) {
#	ifdef CLAM_USE_XML
		CLAM::XMLIterableAdapter<AttribType> adapter(object, elemName, name, true);
	return s.Load (&adapter);
#	else 
	return false;
#	endif//CLAM_USE_XML
} 


///////////////////////////////////////////////////////////////////////////////////////////////////////////


}; //namespace CLAM

#endif // !defined _DynamicType_

