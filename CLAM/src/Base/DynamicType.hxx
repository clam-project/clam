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

#include "Component.hxx"
#include "DataTypes.hxx"

#include <new>
#include <vector>

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
	* Copy constructor of a dynamic Type.
	* <B>This constructor must be only used from the concrete dyn. type constructor.</B> 
	* The created object will use the dynamic type description of anotyer dynamic Type.
	* @param prototype Another dynamic type from which the dynamic info is taken.
	*/
	/// \todo remove?
	DynamicType(const DynamicType& prototype);

	virtual ~DynamicType();
	
	virtual const char* GetClassName() const =0;
protected:
	/// \depracated no more need of pseudo-constructors. Concrete DTs can define its constructors
	void DefaultInit();
	/// \depracated no more need of pseudo-constructors. Concrete DTs can define its constructors
	void CopyInit(const DynamicType & dt);
	
	/**
	* Used by macros
	*/
	/// \todo document why we need a void implementation
	virtual void InformAll() const {};

	
// Inner classes declaration
	/// \todo document
	class DynamicInfo
	{
		//just allow instance creation from DynamicType
		friend DynamicType;
		DynamicInfo() : _numInstantiatedAttr(0), _parentDT(0) {}
		
		void Init( DynamicType *parent );
		
		// nested-nested class:
		class AttrDynamicInfo 
		{
			friend DynamicInfo;
			AttrDynamicInfo() : _added(false), _removed(false) {}
		public:
			/// \todo put definitions after the class. Maybe after the nested class?
			void Add() { _added = true; }
			void Remove() { _removed = true; }
			bool Added() { return _added; }
			bool Removed() { return _removed; }
			int& Offs() { return _offs; }

		private:
			bool _added;
			bool _removed;
			int _offs;
		};
	public: // interface provided for subclasses of DynamicType
		
		AttrDynamicInfo& GetAttrInfo (int i ) {
			CLAM_DEBUG_ASSERT( _parentDT!=0, "at(int) without init" );
			CLAM_DEBUG_ASSERT( i>=0 && i<= _numAttr, "DT::DynamicInfo::GetAttrInfo bad index");

			return _dynInfoImpl[i];
		}
		int NumAttr() { return _numAttr; }
		int NumInstantiatedAttr() { return _numInstantiatedAttr; }
		int GetDataSize() { return _dataSize; }
		int GetAllocatedDataSize() { return _allocatedDataSize; }

	private:
		// private interface (accessed by DynamicType):
		void InitRefcount();
		void IncrementRefCount();
		void DecrementRefCount();
		int RefCount();
		
		// private attributes:
		DynamicType* _parentDT;
		int _numInstantiatedAttr;
		int _numAttr;
		int _dataSize;
		int _allocatedDataSize;
		AttrDynamicInfo* _dynInfoImpl; //C array
	}; // DynamicInfo

	/// \todo move typedefs inside AttrStaticInfo	
	typedef void* (*NewInplaceFn)(void* pos);
	typedef void* (*NewCopyInplaceFn)(void* pos,void* orig);
	typedef void (*DestructorInplaceFn)(void* pos);

	struct AttrStaticInfo
	{
		char *name;                   
		char *type;
		int size;
		int offset; // calculated from acomulated size
		NewInplaceFn newObj;
		NewCopyInplaceFn newObjCopy;
		DestructorInplaceFn destructObj;
		bool isComponent;
		bool isDynamicType;
		bool isPointer;
	};
	/// \todo document
	class StaticInfo
	{
		// only DT can create StaticInfo
		friend DynamicType;
		// well, a DT can access through a DynamicInfo
		friend DynamicInfo;

		StaticInfo()
		{}
	public:  
		/// pushes the attribute info enty into its container, and update _totalSize member variable
		void AddAttr( AttrStaticInfo& info );
		
		int NumAttr() const { return int( _attributes.size() ); }
		
		/// Gets the n-th class name of the hierarchie
		/// \todo check in which order
		const char* GetClassName(int n) const;
		
		void AddClassName( char* name);
		
		int CountClassNames() const { return int( _classNames.size() ); }
		
		int TotalAttrSize() { return _totalSize; }
		/// Returns 'static' info of attr number i. \see \c AttrStaticinfo
		/// Valid index-attribute value is asserted.
		/// \param iAttr ranges from 0 to NumAttr()-1
		/// \todo separate definition from declaration
		const AttrStaticInfo& GetAttrInfo( int iAttr ) const {
			CLAM_ASSERT( iAttr >= 0 && iAttr<NumAttr(), 
				"DT::StaticInfo::GetAttrInfo invalid attr index" );
			return _attributes[iAttr];
		}
		
		/// \todo should be private: (accessed through protected interface of DT) OR NOT!
		static void GetTypeInfo( const void* ptr, bool& isComponent, bool& isDynamicType  );
		static void GetTypeInfo( const Component* ptr, bool& isComponent, bool& isDynamicType  );
		static void GetTypeInfo( const DynamicType* ptr, bool& isComponent, bool& isDynamicType  );

	private:
		int _totalSize;
		std::vector<AttrStaticInfo> _attributes;
		std::vector<char*> _classNames;
	
	}; // StaticInfo

protected:


	/** Called from the virtual GetStaticInfo, which passes its static pointer
	 * to StaticInfo. And only calls this method when its pointer is not 
	 * initialized. This InformAll is called only once per class.
	 */
	void InitStaticInfo(StaticInfo* & pStaticInfo) const {
		pStaticInfo = new StaticInfo; 
		InformAll();
	}

public:
	DynamicInfo& GetDynamicInfo() { 
		_dynInfo.Init(this);
		return _dynInfo;
	}
	virtual StaticInfo& GetStaticInfo() const = 0;

	/**
	* Method used to resize the data space of the dynamic type, necessary when some
	* AddXxx() / RemoveXxx() (where Xxx is an attribute name) has been done.
	* This operation does not check it if has been some attributes changes.
	* In the case that the object is "not owner" of its memory this flag is changed
	* to "owner", and a new data table is created.
	*
	* \return whether some modification has ocurred or not.
	*/
	/// \todo update :-)
	bool UpdateData();
	



protected:
	/// \todo move to DynamicInfo
	void AddAttr_ (const unsigned i, const unsigned size);
	void RemoveAttr_ (const unsigned id);



public:
	enum {shrinkThreshold = 80}; // Bytes.  That constant means that when updating data, if the
	                             // used data disminish an amount superior that this threshold,
	                             // data will be reallocated (shrunk)
		

	// item of the dynamicTable, that holds the dynamic information of the dynamic type
	/// \todo move to DynInfo
	struct TDynInfo
	{
		int offs;  // attribute offset of the data table. Has a -1 value when
		           // the attr is not instantiated (have no entry at the data table).
		bool hasBeenAdded : 1;
		bool hasBeenRemoved : 1;
	};
	/// \todo why no call it clone? (make all components clonable?)
	virtual DynamicType& GetDynamicTypeCopy( const bool deep = false ) const =0;
	/// \todo we really need this now?
	virtual Component* ShallowCopy() const;
	virtual Component* DeepCopy() const;
	DynamicType& operator= (const DynamicType& source);


private:
	/// \todo move to StaticInfo
	inline unsigned    GetNumAttr() const { return numAttr; };
	/// \todo move to DynInfo
	inline unsigned    GetNumActiveAttr() const { return numActiveAttr; }
	inline char*       GetData() const { return data; }
	inline void        SetData(char* srcData) { data = srcData;}
	/// \todo remove
	inline TDynInfo*   GetDynamicTable() const { return dynamicTable; }
	/// \todo move to StaticInfo
	inline unsigned    GetDataSize() const { return dataSize; }
	inline bool        IsInstanciate() const { return (data != 0); }
		
protected:
	inline bool        ExistAttr(unsigned id) const;
	/**
	 * 
	 */
public:
	inline void        SetPreAllocateAllAttributes() { bPreAllocateAllAttributes=true; }



	// Developing tools:
	void Debug();
	void FullfilsInvariant() const;

protected:
/// \todo move almost all attributes	
	unsigned        numActiveAttr;
	char            *data;
	TDynInfo        *dynamicTable;
	unsigned        dataSize;
	unsigned		numAttr;    // the total number of dyn. attrs.
	unsigned		maxAttrSize;	// the total dyn. attrs. size
	unsigned        allocatedDataSize;

	inline int      DynTableRefCounter();
	inline void     InitDynTableRefCounter();
	inline int      DecrementDynTableRefCounter();
	inline int      IncrementDynTableRefCounter();
private:
	DynamicInfo     _dynInfo;

	inline bool   AttrHasData(unsigned i) const { return (dynamicTable[i].offs > -1); };
	inline void   RemoveAllMem();
	inline void*  GetPtrToData_(const unsigned id) const;
	inline void*  GetDataAsPtr_(const unsigned id) const;
	inline void   SetDataAsPtr_(const unsigned id, void* p);
	
	/** support method for UpdateData(). @see UpdateData() 
	 *  SHRINK MODE: now we'll reuse the allocated data table deleting the gaps.
	 *  two traversals: the first one is for moving the existing attributes:
	 *  the second one for allocating the new attributes
	 */
	void UpdateDataByShrinking();

	/** support method for UpdateData(). @see UpdateData() 
	 *  STANDARD MODE: a new reallocation of data table is done.
	 *  and all existing attributes copies (copy constructor)
	 */
	void UpdateDataByStandardMode();
	
	/** support method for UpdateData(). @see UpdateData() 
	 *  Going to Pre Allocated Mode: the last reallocation is done, and the fixed offs are used.
	 */
	void UpdateDataGoingToPreAllocatedMode();
	
	/** support method for UpdateData(). @see UpdateData() Fixed offs (taken from
	 * typeDescTable are used.
	 */
	void UpdateDataInPreAllocatedMode();

	void SelfCopyPrototype(const DynamicType &orig);
	void SelfSharedCopy(const DynamicType &orig);
	void SelfShallowCopy(const DynamicType &orig);
	void SelfDeepCopy(const DynamicType &orig);
	bool bPreAllocateAllAttributes;

public:
	/// \depracated Not longer useful. Users of DTs can write its normal C++ constructors
	void MandatoryInit();

	virtual void StoreOn(CLAM::Storage & s) {
		this->StoreDynAttributes(s);
	}
	virtual void LoadFrom(CLAM::Storage & s) {
		this->LoadDynAttributes(s);
	}
	template <unsigned int NAttrib> 
	class AttributePositionBase { 
	public:
		static const int value;
	};
	
protected:
	virtual void StoreDynAttributes(CLAM::Storage & s) = 0;
	virtual void LoadDynAttributes(CLAM::Storage & s) = 0;
	template <typename AttribType>
	void StoreAttribute(StaticTrue* asLeave, CLAM::Storage &s ,AttribType & object, char* name) {
#ifdef CLAM_USE_XML
		CLAM::XMLAdapter<AttribType> adapter(object, name, true);
		s.Store (&adapter);
#endif//CLAM_USE_XML
	}
	template <typename AttribType>
	void StoreAttribute(StaticFalse* asLeave, CLAM::Storage &s ,AttribType & object, char* name) {
#ifdef CLAM_USE_XML
		CLAM::XMLComponentAdapter adapter(object, name, true);
		s.Store (&adapter);
#endif//CLAM_USE_XML
	} 
	template <typename AttribType>
	void StoreIterableAttribute(CLAM::Storage &s ,AttribType & object, char* name, char* elemName) {
#ifdef CLAM_USE_XML
		CLAM::XMLIterableAdapter<AttribType> adapter(object, elemName, name, true);
		s.Store (&adapter);
#endif//CLAM_USE_XML
	} 

	template <typename AttribType>
	bool LoadAttribute(StaticTrue* asLeave, CLAM::Storage &s ,AttribType & object, char* name) {
#ifdef CLAM_USE_XML
		CLAM::XMLAdapter<AttribType> adapter(object, name, true);
		return s.Load (&adapter);	
#else 
		return false;
#endif//CLAM_USE_XML
	}
	template <typename AttribType>
	bool LoadAttribute(StaticFalse* asLeave, CLAM::Storage &s ,AttribType & object, char* name) {
#ifdef CLAM_USE_XML
		CLAM::XMLComponentAdapter adapter(object, name, true);
		return s.Load (&adapter);	
#else 
		return false;
#endif//CLAM_USE_XML
	} 
	template <typename AttribType>
	bool LoadIterableAttribute(CLAM::Storage &s ,AttribType & object, char* name, char* elemName) {
#ifdef CLAM_USE_XML
		CLAM::XMLIterableAdapter<AttribType> adapter(object, elemName, name, true);
		return s.Load (&adapter);
#else 
		return false;
#endif//CLAM_USE_XML
	} 
};


//////////////////////////////////////////////////////////////////
// STATIC MEMBERS DEFINITION

template <unsigned int NAttrib> const int DynamicType::AttributePositionBase<NAttrib>::value = NAttrib;

//////////////////////////////////////////////////////////////////
// IMPLEMENTATION OF INLINE FUNCTIONS

inline void DynamicType::DynamicInfo::Init(DynamicType* p) {
	if (_parentDT) return;
		_parentDT = p;
		_numAttr = _parentDT->GetStaticInfo().NumAttr();
		_dynInfoImpl = new AttrDynamicInfo[_numAttr+1]; // the last element doesn't describe
		                                                // an attribute, but the whole DT
}

inline void DynamicType::DynamicInfo::InitRefcount() {
}

inline void DynamicType::DynamicInfo::IncrementRefCount() {
	_dynInfoImpl[NumAttr()].Offs()++;

}
inline void DynamicType::DynamicInfo::DecrementRefCount() {

}
inline int DynamicType::DynamicInfo::RefCount() {

}


inline const char* DynamicType::StaticInfo::GetClassName(int i) const { 
	CLAM_ASSERT( i>=0 && i<NumAttr(), "DT::StaticInfo::GetClassName bad index" );
	return _classNames[i];
}

inline void DynamicType::StaticInfo::AddClassName( char* name) {
	_classNames.push_back( name );
}

inline void DynamicType::StaticInfo::AddAttr( AttrStaticInfo& info ) { 
	info.offset = _totalSize = info.size + _attributes.back().offset;
	_attributes.push_back( info );
}


inline bool DynamicType::ExistAttr(unsigned id) const 
{ 

	if (!data) return false;

	TDynInfo &inf = dynamicTable[id];
	return (inf.offs != -1 && !inf.hasBeenAdded && !inf.hasBeenRemoved); 
}

inline void* DynamicType::GetDataAsPtr_(const unsigned id) const
{
	return *(void**)&data[dynamicTable[id].offs];
}

inline void* DynamicType::GetPtrToData_(const unsigned id) const
{
	return (void*)&data[dynamicTable[id].offs];
}

inline void DynamicType::SetDataAsPtr_(const unsigned id, void* p)
{
	*(void**)&data[dynamicTable[id].offs] = p;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////



inline void DynamicType::StaticInfo::GetTypeInfo( const void* ptr, bool& isComponent, bool& isDynamicType  )
{
	isComponent = false;
	isDynamicType = false;
}

inline void DynamicType::StaticInfo::GetTypeInfo( const Component* ptr, bool& isComponent, bool& isDynamicType )
{
	isComponent = true;
	isDynamicType = false;
}

inline void DynamicType::StaticInfo::GetTypeInfo(const DynamicType* ptr, bool& isComponent, bool& isDynamicType )
{
	isComponent = true;
	isDynamicType = true;
}


}; //namespace CLAM

#endif // !defined _DynamicType_

