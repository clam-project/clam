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
	/**
	* Constructs a DynamicType object that can hold @param nAttr attributes.
	* <B>This constructor must be only used from the concrete dyn. type constructor.</B>
	* This constructor creates a dynamic type that is a new prototype. That means
	* that has its own dynamic information (which attrs. are instanciated, etc.)
	* Furthermore, the new object is set owner of its memory.
	*/
	DynamicType(const int nAttr);

	/**
	* Copy constructor of a dynamic Type.
	* <B>This constructor must be only used from the concrete dyn. type constructor.</B> 
	* The created object will use the dynamic type description of anotyer dynamic Type.
	* @param prototype Another dynamic type from which the dynamic shape is taken.
	* @param shareData Tells whether the new object will share the 
	* same data of the prototype, or not.
	*/

	DynamicType(const DynamicType& prototype, const bool shareData, const bool deepCopy);
	DynamicType(const DynamicType& prototype);
	virtual ~DynamicType();
	
	virtual const char* GetClassName() const =0;
protected:
	/// \todo depracate: write a cerr text explaining the (simpler) alternative (using constructor)
	void DefaultInit() {
	
	};

	/**
	* implemented by the macros in the concrete class. Informs all attrs. to the typeDescTable.
	* used in UpdateData(). @see UpdateData()
	*/
	/// \todo update
	virtual void InformAll() {
			// lets calculates the offsets of the "Pre allocated mode"
		CLAM_DEBUG_ASSERT(typeDescTable,"static table don't exist. in DT::InformAll()");
		int adder=0;
		for (unsigned int i=0; i<numAttr; i++) {
			typeDescTable[i].offset = adder;
			adder += typeDescTable[i].size;
		}
		maxAttrSize = adder;

	};

	/// \todo depracate: write a cerr text explaining the (simpler) alternative (using constructor)
	void CopyInit(const DynamicType & dt) {
	};
// Inner classes declaration
	/// \todo document
	class DynamicInfo
	{
		//just allow instance creation from DynamicType
		friend DynamicType;
		DynamicInfo() : _numActiveAttr(0), _parentDT(0) {}
		
		void InitDynamicInfo( DynamicType *parent );
		
		// nested-nested class:
		class AttrDynamicInfo 
		{
			friend DynamicInfo;
			AttrDynamicInfo() : _added(false), _removed(false) {}
		public:
			void Add() { _added = true; }
			void Remove() { _removed = true; }
			bool Added() { return _added; }
			bool Removed() { return _removed; }
			int Offs() { return _offs; }

		private:
			bool _added;
			bool _removed;
			int _offs;
		};
	public:
		
		int NumActiveAttr() { return _numActiveAttr; }
	   	AttrDynamicInfo& Attr (int i ) {
			CLAM_DEBUG_ASSERT( _parentDT!=0, "at(int) without init" );
			return _dynInfoImpl[i];
		}
	private:
		DynamicType* _parentDT;
		int _numActiveAttr;
		AttrDynamicInfo* _dynInfoImpl; //C array
	}; // DynamicInfo

	/// \todo document
	class StaticInfo
	{
		// only DT can create StaticInfo
		friend DynamicType;
		
		StaticInfo() {			
		}
	public:
		void AddAttr() { NumAttr()++; }
		int &NumAttr() { 
			static int _numAttr = 0;
			return _numAttr; }
	private:
		int _numAttr;
	}; // StaticInfo

public:
	DynamicInfo& GetDynamicInfo() { 
		_dynInfo.InitDynamicInfo(this);
		return _dynInfo;
	}
	StaticInfo& GetStaticInfo() {
		static StaticInfo* p=0; //must be init?
		if (!p) {
			p = new StaticInfo; 
			InformAll();
		}
		return *p;
	}
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
	
private:
	// Types of the constructors and destructors that all registerd type must have.
	// A pointer to these functions is stored into the typeDescTable. (an array of TAttr) 
	// The definition of TAttr is following:
	/// \todo think about make this calls implicit using templates.
	typedef void* (*NewInplaceFn)(void* pos);
	typedef void* (*NewCopyInplaceFn)(void* pos,void* orig);
	typedef void (*DestructorInplaceFn)(void* pos);

protected:
	/// \todo do we really need to store this type info? we allready have typed visitors 
	virtual void InformAttr_ (unsigned id, char* name, unsigned size, char* type, const bool isPtr,
	                       const NewInplaceFn, const NewCopyInplaceFn, const DestructorInplaceFn);
		
	inline void InformTypedAttr_(unsigned id, char* name, unsigned size, char *type, const bool isPtr,
	                          const NewInplaceFn, const NewCopyInplaceFn, const DestructorInplaceFn, const Component* ptr);

	inline void InformTypedAttr_(unsigned id, char* name, unsigned size, char *type, const bool isPtr,
	                          const NewInplaceFn, const NewCopyInplaceFn, const DestructorInplaceFn, const DynamicType* ptr);

	inline void InformTypedAttr_(unsigned id, char* name, unsigned size, char *type, const bool isPtr,
	                          const NewInplaceFn, const NewCopyInplaceFn, const DestructorInplaceFn, const void* ptr);


	void AddAttr_ (const unsigned i, const unsigned size);
	void RemoveAttr_ (const unsigned id);



public:
	enum {shrinkThreshold = 80}; // Bytes.  That constant means that when updating data, if the
	                             // used data disminish an amount superior that this threshold,
	                             // data will be reallocated (shrunk)
	// item of the typeDescTable, that is static created only once in the concrete class constructor
	struct TAttr
	{
		char *id;                   
		char *type;
		int size;
		int offset;
		NewInplaceFn newObj;
		NewCopyInplaceFn newObjCopy;
		DestructorInplaceFn destructObj;

//		bool isInformed : 1;   Deprecated!! Now the concrete constr. calls InformAll() chain.method.
		bool isComponent : 1;
		bool isStorable : 1;
		bool isDynamicType : 1;
		bool isPointer : 1;
	};

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
	virtual DynamicType& GetDynamicTypeCopy(const bool shareData = false, const bool deep = false) const =0;
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
	/// \todo remove
	inline TAttr*      GetTypeDescTable() const { return typeDescTable; }
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
	TAttr           *typeDescTable;
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
	virtual void StoreDynAttributes(CLAM::Storage & s);
	virtual void LoadDynAttributes(CLAM::Storage & s);
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

inline void DynamicType::DynamicInfo::InitDynamicInfo(DynamicType* p) {
	if (_parentDT) return;
		int n = _parentDT->GetStaticInfo().NumAttr();
		_dynInfoImpl = new AttrDynamicInfo[n];
		_parentDT = p;
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



inline void DynamicType::InformTypedAttr_(unsigned val, char*name, unsigned size, char *type, const bool isPtr,
                                       const NewInplaceFn fnew, const NewCopyInplaceFn fcopy, const DestructorInplaceFn fdestr, const void* ptr)
{
	InformAttr_(val, name, size, type, isPtr, fnew, fcopy, fdestr);
	typeDescTable[val].isComponent = false;
	typeDescTable[val].isDynamicType = false;
	typeDescTable[val].isStorable = false;
}

inline void DynamicType::InformTypedAttr_(unsigned val, char*name, unsigned size, char *type, const bool isPtr,
                                       const NewInplaceFn fnew, const NewCopyInplaceFn fcopy, const DestructorInplaceFn fdestr, const Component* ptr)
{
	InformAttr_(val, name, size, type, isPtr, fnew, fcopy, fdestr);
	typeDescTable[val].isComponent = true;
	typeDescTable[val].isDynamicType = false;
	typeDescTable[val].isStorable = false;
}

inline void DynamicType::InformTypedAttr_(unsigned val, char*name, unsigned size, char *type, const bool isPtr,
                                       const NewInplaceFn fnew, const NewCopyInplaceFn fcopy, const DestructorInplaceFn fdestr, const DynamicType* ptr)
{
	InformAttr_(val, name, size, type, isPtr, fnew, fcopy, fdestr);
	typeDescTable[val].isComponent = true;
	typeDescTable[val].isDynamicType = true;
	typeDescTable[val].isStorable = false;
}


}; //namespace CLAM

#endif // !defined _DynamicType_

