/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __SPECTRUMCONVERSIONS__
#define __SPECTRUMCONVERSIONS__

#include "Array.hxx"
#include "Complex.hxx"
#include "Polar.hxx"
#include "BPF.hxx"

namespace CLAM
{
		void CLAM_DLL_EXPORT Complex2Polar( const Array<Complex>&, Array<Polar>& );
		
		void CLAM_DLL_EXPORT Polar2Complex( const Array<Polar>&, Array<Complex>& );

		void CLAM_DLL_EXPORT Complex2MagPhase( const Array<Complex>&, DataArray&, DataArray& );
		
		void CLAM_DLL_EXPORT Polar2MagPhase( const Array<Polar>&, DataArray&, DataArray& );

		void CLAM_DLL_EXPORT BPF2MagPhase( const BPF&, const BPF&, DataArray&, DataArray&, TSize, TData );

		void CLAM_DLL_EXPORT MagPhase2Polar( const DataArray&, const DataArray&, Array<Polar>& );

		void CLAM_DLL_EXPORT MagPhase2Complex( const DataArray&, const DataArray&, Array<Complex>& );

		void CLAM_DLL_EXPORT Log2LinearMagnitude( const DataArray&, DataArray& );
		
		void CLAM_DLL_EXPORT Linear2LogMagnitude( const DataArray&, DataArray& );

		void CLAM_DLL_EXPORT Complex2LogMagPhase(const Array<Complex> &src, DataArray &destMag, DataArray &destPhase);

		// Inplace conversion function overloads

		void CLAM_DLL_EXPORT Log2LinearMagnitude( DataArray& );
		
		void CLAM_DLL_EXPORT Linear2LogMagnitude( DataArray& );
}


#endif // SpectrumConversions.hxx
