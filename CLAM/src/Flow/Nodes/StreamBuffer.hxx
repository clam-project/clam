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

#ifndef _STREAM_BUFFER_H
#define _STREAM_BUFFER_H

#include "StreamRegionContainer.hxx"
#include "Assert.hxx"

namespace CLAM {

	template<class T>
	class Array;

	/** Buffer class for streams of data objects.
	 * 
	 * This class implements the CLAM data stream abstraction, which
	 * consist of a flow of data tokens with several "clients" which
	 * read or modify the data flow through StreamRegion objects.
	 * 
	 * The utility of this class is two-fold:
	 *   - It provides very eficient stream position book-keeping for
	 *     the stream clients. They only need to perform Activate() and
	 *     Leave() operations on the region.
	 *   - It provides (not so eficient) run time checking in debug
	 *     mode, which eases the manual implementation of client
	 *     execution sequences
	 * This is a generic stream class, which provides the higher level
	 * stream capabilities. Its second template argument must be a stream
	 * implementation class with lower level buffering capabilities.
	 * @param D Data type.
	 * @param B Stream Implementation. Existent implementations:
	 *              - CircularStreamImpl
	 */
	template<class D, class B>
	class StreamBuffer {

		StreamRegionContainer mRegions;

		B mBuffer;

	public:

		StreamBuffer(unsigned int max_window_size = 0);

		StreamBuffer(const StreamBuffer&);

		/** @name Region creation methods.
		 * These methods are the StreamBuffer interface to
		 * Register the stream writer, new readers, delays, etc.
		 */
		//@{
		WriteStreamRegion   *NewWriter (unsigned int hop,
		                                unsigned int length);

		ReadStreamRegion    *NewReader (unsigned int hop,
		                                unsigned int length,
		                                SourceStreamRegion* source = 0);

		DelayStreamRegion   *NewDelay  (unsigned int hop,
		                                unsigned int length,
		                                SourceStreamRegion* source);

		InplaceStreamRegion *NewInplace(unsigned int hop, 
		                                unsigned int read_length,
		                                unsigned int write_length,
		                                SourceStreamRegion* source=0,
		                                unsigned int write_delay=0);
		//@}

		void SetPrototype(const D& proto) {};

		/** Configuration method.
		 * This method must be called after all the regions using the stream
		 * have been created. It will allocate all the necesary internal buffer
		 * resources (The lower level buffer class will actually do this).
		 */
		void Configure(unsigned int max_window_size=0);

		/** Region activation method.
		 * These methods provide the interface to access the data
		 * inside a region. They must always be followed by a
		 * Region Advance method.
		 */
		template<class REGION>
		D* GetAndActivate(REGION* r)
		{
			CLAM_DEBUG_ASSERT(mRegions.Contains(r),
			                  "StreamBuffer::GetAndActivate(): "
			                  "Invalid region argument ");
			CLAM_DEBUG_ASSERT(r->FulfilsInvariant(),
			                  "StreamBuffer::GetAndActivate(): "
			                  "Region inconsistent before activation.");
			r->Activate();
			CLAM_DEBUG_ASSERT(r->FulfilsInvariant(),
			                  "StreamBuffer::GetAndActivate(): "
			                  "Region inconsistent after activation.");
			return mBuffer.GetData(r);
		}

		template<class REGION>
		void GetAndActivate(REGION* r, Array<D> &a)
		{
			CLAM_DEBUG_ASSERT(mRegions.Contains(r),
			                  "StreamBuffer::GetAndActivate(): "
			                  "Invalid region argument ");
			CLAM_DEBUG_ASSERT(r->FulfilsInvariant(),
			                  "StreamBuffer::GetAndActivate(): "
			                  "Region inconsistent before activation.");
			r->Activate();
			CLAM_DEBUG_ASSERT(r->FulfilsInvariant(),
			                  "StreamBuffer::GetAndActivate(): "
			                  "Region inconsistent after activation.");
			mBuffer.GetData(r,a);
		}

		/** @name Region Advance methods.
		 * This methods notify the buffer that data processing in the
		 * region which was requested by a previous GetAndActivate
		 * call is done.
		 * These methods increment the region position, so that the
		 * next call to GetAndActivate on that region will take the
		 * next chunk of data.
		 */
		template<class REGION>
		void LeaveAndAdvance(REGION*r)
		{
			CLAM_DEBUG_ASSERT(mRegions.Contains(r),
							  "StreamBuffer::LeaveAndAdvance(): "
							  "Invalid region argument ");
			mBuffer.Leave(r);
			r->LeaveAndAdvance();
		}

	private:

		// This is private only because it returns a non-const
		// value. The non-template GetAndActivate methods above
		// exist only to force the return value constness when it
		// is needed.

	};

}

// Template method implementations:

#include "Array.hxx"

namespace CLAM {

	template<class D, class B>
	StreamBuffer<D,B>::StreamBuffer(unsigned int max_window_size)
		: mRegions(),
		  mBuffer(max_window_size,mRegions)
	{}

	template<class D, class B>
	StreamBuffer<D,B>::StreamBuffer(const StreamBuffer& buf)
		: mRegions(buf.mRegions),
		  mBuffer(buf.mBuffer.MaxSize(),mRegions)
	{}


	template<class D, class B>
	WriteStreamRegion *StreamBuffer<D,B>::NewWriter (unsigned int hop,
	                                                 unsigned int length)
	{
		CLAM_ASSERT(mRegions.Writer() == 0,
		            "StreamBuffer::NewWirter(): Writer already registered");
		WriteStreamRegion *writer = new WriteStreamRegion(hop,length);
		mRegions.SetWriter(writer);
		return writer;
	}

	template<class D, class B>
	ReadStreamRegion *StreamBuffer<D,B>::NewReader (unsigned int hop,
	                                                unsigned int length,
	                                                SourceStreamRegion* source)
	{
		CLAM_ASSERT(source?mRegions.Contains(source):true,
		            "StreamBuffer::NewReader(): Invalid source argument.");
		if (!source)
			source = mRegions.Writer();
 		ReadStreamRegion *reader = new ReadStreamRegion(hop,length,source);
		mRegions.AddReader(reader,source);
		return reader;
	}

	template<class D, class B>
	DelayStreamRegion *StreamBuffer<D,B>::NewDelay  (unsigned int hop,
	                                                 unsigned int length,
	                                                 SourceStreamRegion* source = 0)
	{
		CLAM_ASSERT(source?mRegions.Contains(source):true,
		            "StreamBuffer::NewDelay(): Invalid source argument.");
		if (!source)
			source = mRegions.Writer();
 		DelayStreamRegion *delay = new DelayStreamRegion(hop,length,source);
		mRegions.AddDelay(delay,source);
		return delay;
	}

	template<class D, class B>
	InplaceStreamRegion *StreamBuffer<D,B>::NewInplace(unsigned int hop, 
	                                                   unsigned int read_length,
	                                                   unsigned int write_length,
	                                                   SourceStreamRegion* source,
	                                                   unsigned int write_delay)
	{
		CLAM_ASSERT(source?mRegions.Contains(source):true,
		            "StreamBuffer::NewInplace(): Invalid source argument.");
		if (!source)
			source = mRegions.Writer();
 		InplaceStreamRegion *inplace = new InplaceStreamRegion(hop,length,source);
		mRegions.AddInplace(inplace,source);
		return inplace;
	}

	template<class D, class B>
	void StreamBuffer<D,B>::Configure(unsigned int max_window_size)
	{
		CLAM_ASSERT(mRegions.FulfilsInvariant(),
		            "StreamBuffer::Configure(): "
		            "Inconsisntent regions before configuration")

		mBuffer.Configure(max_window_size);

		CLAM_ASSERT(mBuffer.FulfilsInvariant(),
		            "StreamBuffer::Configure(): "
		            "Inconsistent buffer after configuration.");
	}

}
// Class speciallizations:

#include "AudioStreamBuffer.hxx"

#endif
