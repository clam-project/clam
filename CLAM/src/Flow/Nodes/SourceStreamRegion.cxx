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

#include "SourceStreamRegion.hxx"
#include "ReadStreamRegion.hxx"
#include "Assert.hxx"

namespace CLAM {

	SourceStreamRegion::reader_iterator SourceStreamRegion::readers_begin()
	{
		return mReaders.begin();
	}

	SourceStreamRegion::reader_iterator SourceStreamRegion::readers_end()
	{
		return mReaders.end();
	}

	SourceStreamRegion::reader_const_iterator SourceStreamRegion::readers_begin() const
	{
		return mReaders.begin();
	}

	SourceStreamRegion::reader_const_iterator SourceStreamRegion::readers_end()   const
	{
		return mReaders.end();
	}

	SourceStreamRegion::SourceStreamRegion(unsigned int hop,
	                                       unsigned int length)
		: StreamRegion(hop,length)
	{}

	bool SourceStreamRegion::IsSourceOf(const ReadStreamRegion* reader) const
	{
		reader_const_iterator it;
		for (it=readers_begin(); it!=readers_end(); it++)
			if (reader == *it)
				return true;
		return false;
	}

	void SourceStreamRegion::AddReader(ReadStreamRegion *reader)
	{
		CLAM_ASSERT(!IsSourceOf(reader),
					"SourceStreamRegion::AddReader(): reader already added");
		mReaders.push_back(reader);
	}


	bool SourceStreamRegion::FulfilsInvariant() const
	{
		reader_const_iterator rit;
		for (rit=readers_begin();
			 rit != readers_end();
			 rit++)
			if (!Preceeds(*rit))
				return false;

		return true;
	}

	void SourceStreamRegion::InitReaders(unsigned int offset) 
	{
		reader_iterator it;
		for (it=readers_begin(); it!=readers_end(); it++)
			(*it)->Init(offset);
	}
	
	void SourceStreamRegion::Init(unsigned int offset)
	{
		unsigned int realOffset = Chop( FindLargestReadRegionLength()*0.5f ) + offset;
		unsigned int hopsInHalfWindow = realOffset / mHop;

		for(unsigned int i=0; i<hopsInHalfWindow; i++)
		{
			Activate();
			//Would have to make sure that zeros are here
			LeaveAndAdvance();
		}
		mOffset= realOffset;
		InitReaders(realOffset);
	}

	unsigned int SourceStreamRegion::FindLargestReadRegionLength()
	{
		unsigned int len=0;

		reader_const_iterator rit;

		for (rit=readers_begin();  rit != readers_end();  rit++)
				if ((*rit)->MaxLength() >len) len=(*rit)->MaxLength();
		
		return len;
	}

}
