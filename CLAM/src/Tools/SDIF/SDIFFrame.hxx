#ifndef __SDIFFrame__
#define __SDIFFrame__

#include "SDIFHeader.hxx"
#include <list>

namespace SDIF
{
	class Matrix;

	/** A special kind of frame at the beginning of each SDIF file
	*/
	class OpeningsFrame {
	public:
		FrameHeader mHeader;
		TInt32 mSpecVersion;
		TInt32 mStandardTypesVersion;
	public:	
		/** Create the frame */
		OpeningsFrame():mHeader("SDIF")
		{
			mHeader.mSize = 8;
			mSpecVersion = 0; mStandardTypesVersion = 0;
		}
	};

	/** SDIF data is stored in frames, that contain number of matrices.
	*       An SDIFFrame can be aware of it's successors and precessors in time 
	* (both in the whole list of frames and in the streams)
	*/
	class Frame {
	friend class File;
	friend class Collection;
	public:
		typedef std::list<Matrix*>::const_iterator iterator;
	private:
		DataFrameHeader mHeader;
		std::list<Matrix*> mMatrixList;
	public:
		/** Create a new Frame
		* @param type Identifier specifying the frame type
		* @param time Timestamp of the frame
		* @param id Stream ID
		*/
		Frame(const Type& type = Type::sDefault,
			TFloat64 time =0.,TInt32 streamId = 0);
		~Frame();

		/** Add a SDIFMatrix to the frame
		* @param pMatrix pointer to the SDIFMatrix to add to the frame
		*/
		void Add(Matrix* pMatrix);
		
		iterator Begin(void) const { return mMatrixList.begin(); }
		iterator End(void) const { return mMatrixList.end(); }
		
		TFloat64 Time(void) { return mHeader.mTime; }
		Type	Type(void) {return mHeader.mType;}
		
	};
}

#endif
