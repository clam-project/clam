
#ifndef _DynamicInfo_hxx_
#define _DynamicInfo_hxx_

namespace CLAM
{

// forward declaration
class DynamicType;
class StaticInfo;


// Inner classes declaration
/**
 * Helper class for DynamicType. It holds the dynamic information of the
 * DT. General information for data management and instantiation information
 * per attribute.
 * It support DT inheritance.
 */

class DynamicInfo
{
	//just allow instance creation from DynamicType
	friend DynamicType;

	/// DynamicInfo nested class: in manages the dynamic attribute info
	class AttrDynamicInfo 
	{
		friend DynamicInfo;
	public:
		AttrDynamicInfo() : 
			_added(false), 
			_removed(false), 
			_offs(-1) 
		{}
		
		void SetAdded();
		void SetRemoved();
		void UnsetAdded();
		void UnsetRemoved();
		bool Added();
		bool Removed();
		/// Get the offset in the DT data chunk table, where the attr mem sits.
		int GetOffs();
		void SetOffs( int o );
		/// Returns true if DT has data chunk and the attribute offset is >0
		bool HasData();

	private:
		bool _added;
		bool _removed;
		int _offs;
	};
public:
	// Public interface:
	int NumAttr() const;
	int NumInstantiatedAttr() const;
	AttrDynamicInfo& GetAttrInfo (int idAttr );
	bool AnyRemoved();
	bool AnyAdded();
	int RefCount() const;

private:
	// interface for the friendly DynamicType :
	//   constructor/destructor:
	DynamicInfo();
	DynamicInfo( const DynamicInfo& );

	~DynamicInfo();

	void Init( DynamicType *parent );
	int GetDataSize() const;
	void SetDataSize( int s );
	int GetAllocatedDataSize() const;
	void SetAllocatedDataSize( int s );
	bool ExistAttr( int idAttr );
	
	void IncrementRefCount();
	void DecrementRefCount();
	void InitRefCount();	

	void AddAttr( int idAttr, int attrSize );
	void RemoveAttr( int idAttr, int attrSize );

	
	void TryToUnsetAnyRemoved();
	void TryToUnsetAnyAdded();
	void UnsetAnyRemoved();
	void UnsetAnyAdded();
	void SetAnyRemoved();
	void SetAnyAdded();
	void CreateASeparatedDynInfoImpl();

	StaticInfo& GetCachedStaticInfo();

	DynamicInfo& operator= (const DynamicInfo& arg);

// Attributes:
	DynamicType* _parentDT;
	int _numInstantiatedAttr;
	int _numAttr;
	int _dataSize;
	int _allocatedDataSize;
	AttrDynamicInfo* _dynInfoImpl; //C array
	StaticInfo* _cachedStaticInfo;
}; // DynamicInfo



// inline member definitions:

inline int DynamicInfo::NumAttr() const{ return _numAttr; }
inline int DynamicInfo::NumInstantiatedAttr() const { return _numInstantiatedAttr; }

inline int DynamicInfo::AttrDynamicInfo::GetOffs() { return _offs; }
inline void DynamicInfo::AttrDynamicInfo::SetOffs( int o ) { _offs=o; }
inline bool DynamicInfo::AttrDynamicInfo::HasData() { return _offs>=0; }

inline int DynamicInfo::GetDataSize() const { return _dataSize; }
inline void DynamicInfo::SetDataSize( int s ) { _dataSize = s; }
inline int DynamicInfo::GetAllocatedDataSize() const { return _allocatedDataSize; }
inline void DynamicInfo::SetAllocatedDataSize( int s ) { _allocatedDataSize=s; }
inline StaticInfo& DynamicInfo::GetCachedStaticInfo() { return *_cachedStaticInfo; }

} // namespace CLAM

#endif
