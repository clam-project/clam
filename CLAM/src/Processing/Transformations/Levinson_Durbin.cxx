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

#include "Levinson_Durbin.hxx"

namespace CLAM
{
	void Levinson_DurbinConfig::DefaultInit(void)
	{
		AddAll();
		UpdateData();
	}

	Levinson_Durbin::Levinson_Durbin() :
		mOrder( 0 )
	{
		Levinson_DurbinConfig cfg;

		Configure( cfg );
	}

	Levinson_Durbin::Levinson_Durbin( const Levinson_DurbinConfig& cfg ) :
		mOrder( 0 )	
	{
		Configure( cfg );
	}

	Levinson_Durbin::~Levinson_Durbin()
	{}

	bool Levinson_Durbin::ConcreteConfigure( const ProcessingConfig& cfg ) throw( std::bad_cast )
	{
		mConfig = dynamic_cast< const Levinson_DurbinConfig& > ( cfg );

		if( mConfig.HasOrder() )
		{
			if( mConfig.GetOrder() < 0 )
				throw( ErrProcessingObj( "Wrong (negative) Order Filter in Levinson_Durbin Configuration" ), this );
			mOrder = mConfig.GetOrder();
		}

		return true;
	}

	bool Levinson_Durbin::ConcreteStart()
	{
		return true;
	}

	bool Levinson_Durbin::ConcreteStop()
	{
		return true;
	}

	void Levinson_Durbin::CheckTypes( const Correlation& in, const DataArray& A, const DataArray& K, DataArray& E ) const
	{
		CLAM_BEGIN_CHECK

		if( A.Size() != mOrder )
		{
			std::stringstream s;

			s << "Levinson_Durbin::Do: Wrong size in array of filter coefficients\n"
			  << "	Expected: " << mOrder << "	, used " << A.Size();
			CLAM_ASSERT( 0, s.str().c_str() );
		}

		if( K.Size() != mOrder )
		{
			std::stringstream s;

			s << "Levinson_Durbin::Do: Wrong size in array of reflection coefficients\n"
			  << "	Expected: " << mOrder << "	, used " << K.Size();
			CLAM_ASSERT( 0, s.str().c_str() );
		}

		if( E.Size() != mOrder )
		{
			std::stringstream s;

			s << "Levinson_Durbin::Do: Wrong size in array of the average mean squared error\n"
			  << "	Expected: " << mOrder << "	, used " << E.Size();
			CLAM_ASSERT( 0, s.str().c_str() );
		}

		if( !in.HasBuffer() )
			CLAM_ASSERT( 0, "Levinson_Durbin Do: Float attribute required for Correlation object. " );

		CLAM_END_CHECK

	}

	bool Levinson_Durbin::Do( const Correlation& in , DataArray& A, DataArray& K, DataArray& E )
	{
/*		int i = 0;
		A[ 0 ] = 1 ;
		E[ 0 ] = in[ 0 ];

		while( i < mOrder )
		{
			i++;
			CalculateK( K[ i ], A, in, E[ i - 1 ], i ) ;
			A[  ]
		}
*/		
		return true;
	}
	
}