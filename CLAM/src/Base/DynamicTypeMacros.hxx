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


#include "Assert.hxx"
#include "TypeInfoStd.hxx"
#include "StaticBool.hxx"

//////////////////////////////////////////////////////////////////////
// Reimplementation using chained template methods
// These macros expand functions in the concrete (derived) class of DynamicType.
//
// Related macros: 
// * DYNAMIC_TYPE
// * DYNAMIC_TYPE_USING_INTERFACE
// * DYN_ATTRIBUTE
// * DYN_CONTAINER_ATTRIBUTE
// * SUB_DYNAMIC_TYPE    (incorported on Febr 2003)
//////////////////////////////////////////////////////////////////////

#define SUB_DYNAMIC_TYPE( SUB_CLASS, SUPER_CLASS, NATTRS ) \
public: \
	typedef SUB_CLASS _Sub; \
	typedef SUPER_CLASS _Super; \
	enum { BaseAttributeNumber = _Super::InheritanceFollowNumber }; \
	enum { InheritanceFollowNumber = BaseAttributeNumber+NATTRS} ; \
	virtual const char* GetClassName() const { \
		return #SUB_CLASS; \
	}\
	CLAM::DynamicType& GetDynamicTypeCopy() const\
	{ \
		return *new _Sub(*this); \
	}\
	virtual CLAM::StaticInfo& GetStaticInfo() const { \
		static CLAM::StaticInfo* p=0; \
		if (!p) { \
			InitStaticInfo( p ); \
		} \
		return *p;\
	} \
	/** Visit all Dynamic Attributes */ \
	template <typename Visitor> \
	/* non virtual, because is template */ \
	void VisitAll (Visitor & visitor) { \
		_Super::VisitAll( visitor ); \
		VisitChainedAttr((AttributePosition< BaseAttributeNumber >*)NULL, visitor); \
	} \
	/** Remove all Dynamic Attributes */ \
	virtual void RemoveAll () { \
		_Super::RemoveAll(); \
		RemoveChainedAttr((AttributePosition< BaseAttributeNumber >*)NULL); \
	} \
	/** Add all Dynamic Attributes */ \
	virtual void AddAll () { \
		_Super::AddAll(); \
		AddChainedAttr((AttributePosition< BaseAttributeNumber >*)NULL); \
	} \
protected: \
	/** Add all Dynamic Attributes */ \
	virtual void InformAll() const { \
		_Super::InformAll(); \
		GetStaticInfo().AddClassName( #SUB_CLASS ); \
		InformChainedAttr((AttributePosition< BaseAttributeNumber >*)NULL); \
	} \
	/** Store all Dynamic Attributes */ \
	virtual void StoreDynAttributes(CLAM::Storage & s) { \
		_Super::StoreDynAttributes( s ); \
		StoreChainedAttr((AttributePosition< BaseAttributeNumber >*)NULL,s); \
	} \
	/** Load all Dynamic Attributes */ \
	virtual void LoadDynAttributes(CLAM::Storage & s) { \
		_Super::LoadDynAttributes( s ); \
		AddAll(); \
		UpdateData(); \
		LoadChainedAttr((AttributePosition< BaseAttributeNumber >*)NULL,s); \
		UpdateData(); \
	} \
private: \
	template <unsigned int NAttrib> \
		class AttributePosition : public CLAM::DynamicType::AttributePositionBase<NAttrib> { \
		public: \
			typedef StaticBool<!(NAttrib>=NATTRS)> InboundsCheck; \
	}; \
	/** Instantiated whenever a Attribute number is out of range. 
	 * Gives a compilation error message.
	 */ \
	template <unsigned int NAttrib> \
	void CheckAttribute (StaticFalse*inRange,AttributePosition<NAttrib>*a) const { \
		AttributePosition<(NAttrib)-1>* previous; \
		previous->CompilationError_AttributePositionOutOfBounds(); \
	}\
	/** 
	 * Instantiated whenever a Attribute number is left.
	 * Gives a compilation error message.
	 */ \
	template <unsigned int NAttrib> \
	void CheckAttribute (StaticTrue*inRange,AttributePosition<NAttrib>*a) const { \
		a->CompilationError_AttributeNotDefined(); \
	}\
	/** Undefined link for the Visit method chain (Visit)*/  \
	/* \
	template <unsigned int NAttrib, typename Visitor> \
	void VisitChainedAttr (AttributePosition<NAttrib>*a, Visitor & visitor) { \
		CheckAttribute ((AttributePosition<NAttrib>::InboundsCheck*)NULL, \
		                (AttributePosition<NAttrib>*)NULL); \
	}*/\
	/** Undefined link for the Remove method chain (Remove) */ \
	template <unsigned int NAttrib> \
	void RemoveChainedAttr (AttributePosition<NAttrib>*a) { \
		CheckAttribute ((AttributePosition<NAttrib>::InboundsCheck*)NULL, \
		                (AttributePosition<NAttrib>*)NULL); \
	}\
	/** Undefined link for the Add method chain (Add) */ \
	template <unsigned int NAttrib> \
	void AddChainedAttr (AttributePosition<NAttrib>*a) { \
		CheckAttribute ((AttributePosition<NAttrib>::InboundsCheck*)NULL, \
		                (AttributePosition<NAttrib>*)NULL); \
	}\
	/** Undefined link for the Inform method chain (Inform) */ \
	template <unsigned int NAttrib> \
	void InformChainedAttr (AttributePosition<NAttrib>*a) const { \
		CheckAttribute ((AttributePosition<NAttrib>::InboundsCheck*)NULL, \
		                (AttributePosition<NAttrib>*)NULL); \
	}\
	/** Undefined link on the method chain (Store) */ \
	template <unsigned int NAttrib> \
	void StoreChainedAttr (AttributePosition<NAttrib>*a,CLAM::Storage & s) { \
		CheckAttribute ((AttributePosition<NAttrib>::InboundsCheck*)NULL, \
		                (AttributePosition<NAttrib>*)NULL); \
	}\
	/** Undefined link on the method chain (Load) */ \
	template <unsigned int NAttrib> \
	void LoadChainedAttr (AttributePosition<NAttrib>*a,CLAM::Storage & s) { \
		CheckAttribute ((AttributePosition<NAttrib>::InboundsCheck*)NULL, \
		                (AttributePosition<NAttrib>*)NULL); \
	}\
private: \
	/** Method chain terminator */ \
	template <typename Visitor> \
	void VisitChainedAttr (AttributePosition< InheritanceFollowNumber >*, Visitor & visitor) { \
	} \
	/** Method chain terminator */ \
	void RemoveChainedAttr (AttributePosition< InheritanceFollowNumber>*) { \
	} \
	/** Method chain terminator */ \
	void AddChainedAttr (AttributePosition< InheritanceFollowNumber>*) { \
	} \
	/** Method chain terminator */ \
	void InformChainedAttr (AttributePosition< InheritanceFollowNumber>*) const { \
	} \
	/** Method chain terminator */ \
	void StoreChainedAttr (AttributePosition< InheritanceFollowNumber>*pos, CLAM::Storage &s) { \
	} \
	/** Method chain terminator */ \
	void LoadChainedAttr (AttributePosition< InheritanceFollowNumber>*pos, CLAM::Storage &s) { \
	} \


#define DYNAMIC_TYPE(CLASS_NAME, NATTRS)\
public: \
	SUB_DYNAMIC_TYPE( CLASS_NAME, CLAM::DynamicType, NATTRS ); \




///// ----------------->>> old 
#define DYNAMIC_TYPE_USING_INTERFACE(CLASS_NAME, N, INTERFACE_NAME) \
public: \
	__COMMON_DYNAMIC_TYPE(CLASS_NAME,N); \


#define __COMMON_DYN_ATTRIBUTE(ID_ATTR, ACCESS, TYPE,NAME) \
private: \
	static void* _new_##NAME(void* p) { \
		return static_cast<void*> (new(p) TYPE());\
	}\
	static void* _new_##NAME(void* pos, void* orig) { \
		TYPE* typed = static_cast< TYPE*>(orig);\
		return static_cast<void*>( new(pos) TYPE(*typed) );\
	}\
	static void _destructor_##NAME(void* p) { \
		typedef TYPE __Ty;\
		static_cast<__Ty*>(p)->~__Ty();\
	}\
	\
	/* \todo remove if we can do the same with templates (VC6 can be a problem) */ \
	struct {} CLAM_compile_time_error_Duplicated_Attribute_Index_##ID_ATTR;\
	\
ACCESS: \
	inline TYPE& Get##NAME() const {\
		CheckBeforeDynamicAttributeAccessor( BaseAttributeNumber+ID_ATTR ); \
		void *p=_data + GetAttrOffs( BaseAttributeNumber+ID_ATTR );\
		return *static_cast<TYPE*>(p); \
	}\
	\
	/*  already exist an object of the type in that position (that will be deleted)*/\
	inline void Set##NAME(TYPE const & arg) {\
		CheckBeforeDynamicAttributeAccessor( BaseAttributeNumber+ID_ATTR ); \
		void* orig = (void*)(&arg); \
		char* pos = _data + GetAttrOffs( BaseAttributeNumber+ID_ATTR ); \
		_destructor_##NAME(pos); \
		_new_##NAME(pos, orig); \
	} \
	inline void Add##NAME() { \
		_AddAttr(  BaseAttributeNumber+ID_ATTR , sizeof(TYPE) ); \
	} \
	template <typename Visitor> \
	inline void Visit##NAME(Visitor & visitor) { \
		if (Has##NAME()) \
			visitor.Accept(#NAME,Get##NAME()); \
	}\
	inline void Remove##NAME() { \
		_RemoveAttr(  BaseAttributeNumber+ID_ATTR , sizeof(TYPE) ); \
	}\
	inline bool Has##NAME() const { \
		return ExistAttr( BaseAttributeNumber+ID_ATTR ); \
	} \
private: \
	template <typename Visitor> \
	void VisitChainedAttr(AttributePosition<BaseAttributeNumber+ID_ATTR>*, Visitor & visitor) { \
		Visit##NAME(visitor); \
		VisitChainedAttr((AttributePosition<(BaseAttributeNumber+ID_ATTR)+1>*)NULL, visitor); \
	} \
	void RemoveChainedAttr(AttributePosition<BaseAttributeNumber+ID_ATTR>*) { \
		Remove##NAME(); \
		RemoveChainedAttr((AttributePosition<(BaseAttributeNumber+ID_ATTR)+1>*)NULL); \
	} \
	void AddChainedAttr(AttributePosition<BaseAttributeNumber+ID_ATTR>*) { \
		Add##NAME(); \
		AddChainedAttr((AttributePosition<(BaseAttributeNumber+ID_ATTR)+1>*)NULL); \
	} \
	void InformChainedAttr(AttributePosition<BaseAttributeNumber+ID_ATTR>*) const { \
		AttrStaticInfo attr; \
		CLAM::StaticInfo::DeduceTypeInfo((TYPE*)NULL, attr.isComponent, attr.isDynamicType); \
		attr.name= #NAME; \
		attr.type= #TYPE; \
		attr.size= sizeof(TYPE); \
		attr.newObj= _new_##NAME; \
		attr.newObjCopy= _new_##NAME; \
		attr.destructObj= _destructor_##NAME; \
		GetStaticInfo().AddAttr( attr ); \
		InformChainedAttr((AttributePosition<(BaseAttributeNumber+ID_ATTR)+1>*)NULL); \
	} \
	void StoreChainedAttr(AttributePosition<BaseAttributeNumber+ID_ATTR>*, CLAM::Storage & s) { \
		Store##NAME(s); \
		StoreChainedAttr((AttributePosition<(BaseAttributeNumber+ID_ATTR)+1>*)NULL,s); \
	} \
	void LoadChainedAttr(AttributePosition<BaseAttributeNumber+ID_ATTR>*, CLAM::Storage & s) { \
		Load##NAME(s); \
		LoadChainedAttr((AttributePosition<(BaseAttributeNumber+ID_ATTR)+1>*)NULL,s); \
	} \


#define DYN_ATTRIBUTE(ID_ATTR, ACCESS, TYPE, NAME) \
	__COMMON_DYN_ATTRIBUTE(ID_ATTR, ACCESS, TYPE, NAME) \
protected: \
	void Store##NAME(CLAM::Storage & s) { \
		if (Has##NAME()) { \
			StoreAttribute((CLAM::TypeInfo<TYPE >::StorableAsLeaf*)NULL, s, Get##NAME(), #NAME); \
		} \
	} \
	bool Load##NAME(CLAM::Storage & s) { \
		TYPE obj; \
		if (!LoadAttribute((CLAM::TypeInfo<TYPE >::StorableAsLeaf*)NULL, s, obj, #NAME)) { \
			Remove##NAME(); \
			return false; \
		} \
		Set##NAME(obj); \
		return true; \
	} \
ACCESS: \


#define DYN_CONTAINER_ATTRIBUTE( ID_ATTR, ACCESS, TYPE, NAME, ENAME) \
	__COMMON_DYN_ATTRIBUTE(ID_ATTR, ACCESS, TYPE, NAME) \
protected: \
	void Store##NAME(CLAM::Storage & s) { \
		if (Has##NAME()) { \
			StoreIterableAttribute(s, Get##NAME(), #NAME, #ENAME); \
		} \
	} \
	bool Load##NAME(CLAM::Storage & s) { \
		Add##NAME(); \
		UpdateData(); \
		if (! LoadIterableAttribute(s, Get##NAME(), #NAME, #ENAME)) { \
			Remove##NAME(); \
			return false; \
		} \
		return true; \
	} \
ACCESS: \



