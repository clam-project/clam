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

#include "AutocorrelationEvaluator.hxx"

#include <string>

namespace CLAM
{
	void AutocorrelationEvaluatorConfig::DefaultInit(void)
	{
		AddAll();
		UpdateData();
	}

	AutocorrelationEvaluator::AutocorrelationEvaluator() :
		mSize( 0 )
	{
		AutocorrelationEvaluatorConfig cfg;

		Configure( cfg );
	}

	AutocorrelationEvaluator::AutocorrelationEvaluator( const AutocorrelationEvaluatorConfig& cfg ) :
		mSize( 0 )
	{
		Configure( cfg );
	}

	AutocorrelationEvaluator::~AutocorrelationEvaluator()
	{}

	bool AutocorrelationEvaluator::ConcreteConfigure(const ProcessingConfig& cfg) throw(std::bad_cast)
	{
		mConfig = dynamic_cast< const AutocorrelationEvaluatorConfig& > (cfg);

		if( mConfig.HasSize() )
		{
			if( mConfig.GetSize() < 0 )
				throw( ErrProcessingObj( "Wrong (negative) Size in Autocorrelation Evaluator Configuration", this ) );
			mSize = mConfig.GetSize();
		}

		return true;
	}

	bool AutocorrelationEvaluator::ConcreteStart()
	{
		return true;
	}

	bool AutocorrelationEvaluator::ConcreteStop()
	{
		return false;
	}

	void AutocorrelationEvaluator::CheckTypes( const Audio& in, const Correlation& out) const
	{
		CLAM_BEGIN_CHECK

		if( out.GetSize() != mSize )
		{
			std::stringstream s;

			s << "Autocorrelation::Do: Wrong size in Autocorrelation Audio output\n"
			  << "	Expected: " << mSize << " , used " << out.GetSize();
			CLAM_ASSERT( 0, s.str().c_str() );
		}

		if( !in.HasBuffer() )
			CLAM_ASSERT( 0, "Autocorrelation Do: Float attribute required for Audio object. " );

		CLAM_END_CHECK
	}

	bool AutocorrelationEvaluator::Do(const Audio& in, Correlation &out)
	{
		int n, k ;
		TData *inbuffer, *outbuffer;

		if( !AbleToExecute() ) return true;
		
		CheckTypes( in, out );

		inbuffer = in.GetBuffer().GetPtr();

		for( k = 0; k < mSize; k++ )
		{
			for( n = 1 + k; n < in.GetSize(); n++ )
				out
		}

		return true;
	}
}