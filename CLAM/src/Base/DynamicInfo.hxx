
#ifndef _DynamicInfo_hxx_
#define _DynamicInfo_hxx_

namespace CLAM
{

// forward declaration
class DynamicType;
class StaticInfo;


// Inner classes declaration
/// \todo document
class DynamicInfo
{
	//just allow instance creation from DynamicType
	friend DynamicType;

	// nested class:
	class AttrDynamicInfo 
	{
		friend DynamicInfo;
	public:
		AttrDynamicInfo() : _added(false), _removed(false), _offs(-1) {}

		/// \todo put definitions after the class. Maybe after the nested class?
		void SetAdded();
		void SetRemoved();
		void UnsetAdded();
		void UnsetRemoved();
		bool Added();
		bool Removed();
		int GetOffs() { return _offs; }
		void SetOffs( int o ) { _offs=o; }
		bool HasData() { return _offs>=0; }

	private:
		bool _added;
		bool _removed;
		int _offs;
	};
public:
	// Public interface:
	int NumAttr() const{ return _numAttr; }
	int NumInstantiatedAttr() const { return _numInstantiatedAttr; }
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
	int GetDataSize() const { return _dataSize; }
	void SetDataSize( int s ) { _dataSize = s; }
	int GetAllocatedDataSize() const { return _allocatedDataSize; }
	void SetAllocatedDataSize( int s ) { _allocatedDataSize=s; }
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

	StaticInfo& GetCachedStaticInfo() { return *_cachedStaticInfo; }

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

} // namespace CLAM

#endif
