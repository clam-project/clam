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
#include "CLAM_Math.hxx"

#ifndef _STREAM_REGION_H
#define _STREAM_REGION_H


namespace CLAM {

	class RegionVisitor;

	class StreamRegion {
	protected:
		unsigned int mHop;
		unsigned int mPos;
		unsigned int mLen;
		unsigned int mEnd;
		//XA
		unsigned int mOffset;
	public:
		StreamRegion(unsigned int hop,
		             unsigned int length);

		unsigned int Hop() const {return mHop;}
		unsigned int Pos() const {return mPos;}
		unsigned int End() const {return mEnd;}
		unsigned int Len() const {return mEnd-mPos;}
		unsigned int RealLength() const {return mLen;}
		
		//XA:
		unsigned int Center() const {return Chop((mPos+mEnd)*0.5)-mOffset;} 
		unsigned int MaxLength() const {return mLen;}

		void Activate();
		void LeaveAndAdvance();

		//XA
		void Leave();

		//XA
		void SetCenter(unsigned int center){
			mPos=center-mLen*0.5+mOffset;
		}
		//XA
		void CenterToZero() {SetCenter(0);}
		void Init(unsigned int offset) {
			mOffset=offset;
			CenterToZero();}

		bool Preceeds(const StreamRegion*) const;
		bool Follows (const StreamRegion*) const;

		virtual bool FulfilsInvariant() const;
		virtual bool Accept(RegionVisitor& v) const = 0;
	};

	class ReadStreamRegion;
	class SourceStreamRegion;
	class WriteStreamRegion;
	class DelayStreamRegion;
	class InplaceStreamRegion;

	class RegionVisitor {
	public:
		virtual bool Visit(const ReadStreamRegion&)    = 0;
		virtual bool Visit(const SourceStreamRegion&)  = 0;
		virtual bool Visit(const WriteStreamRegion&)   = 0;
		virtual bool Visit(const DelayStreamRegion&)   = 0;
		virtual bool Visit(const InplaceStreamRegion&) = 0;
	};

	class RegionCheck : public RegionVisitor
	{
	public:
		virtual bool Visit(const ReadStreamRegion&);
		virtual bool Visit(const SourceStreamRegion&);
		virtual bool Visit(const WriteStreamRegion&);
		virtual bool Visit(const DelayStreamRegion&);
		virtual bool Visit(const InplaceStreamRegion&);
	};



};


#endif
