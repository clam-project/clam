/*
 * Author: fabien gouyon 
 * http://www.iua.upf.es/~fgouyon
 * Description:
 *
 * Syntax: C++
 *
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
#include "RD_AutoCorrelationTD.hxx"
#include "CLAM_Math.hxx"
#include <numeric>
#include <iostream>

namespace CLAM
{

	namespace RhythmDescription
	{

		void AutoCorrelationTDConfig::DefaultInit()
		{
			AddAll();
			// All Attributes are added 
			UpdateData();
			//default values
			SetUpperLimit(10);
			SetAutomaticIntegTime(true);
			SetIntegrationTime(10);
		}


		AutoCorrelationTD::AutoCorrelationTD() 
		{
		}


		AutoCorrelationTD::~AutoCorrelationTD()
		{
		}

		const char* AutoCorrelationTD::GetClassName() const
		{
			return "AutoCorrelationTD";
		}

		const ProcessingConfig& AutoCorrelationTD::GetConfig() const
		{
			return mConfig;
		}

		// Configure the Processing Object according to the Config object
		bool AutoCorrelationTD::ConcreteConfigure(const ProcessingConfig& c)
		{
			CopyAsConcreteConfig( mConfig, c );
			
			return true;
		}

		bool  AutoCorrelationTD::Do(void)
		{
			CLAM_ASSERT( false, "AutoCorrelationTD::Do(): Supervised mode not implemented" );

			return false;
		}

		bool AutoCorrelationTD::Do(Array<TData>& sequence, Array<TData>& acf)
		{
			int ul = mConfig.GetUpperLimit();
			int it = mConfig.GetIntegrationTime();
			int maxIt = sequence.Size()-ul;

			if (it>=maxIt) 
				it = maxIt;

			if (mConfig.GetAutomaticIntegTime()) 
			{
				it = maxIt;
			}
			
			if(maxIt<=1) 
			{
				// the sequence is too short, ACF upper limit and integration time are being changed
				it = 2; 
				ul = sequence.Size()-it;
				// final estimate may be unreliable.
			}

			Array<TData> aux1, aux2;
			TData* ptr = sequence.GetPtr();

			//build aux1
			aux1.SetPtr(ptr,it);

			TData normAux1=0.0;

			for(int i=0;i<aux1.Size();i++) 
				normAux1 += aux1[i]*aux1[i];

			normAux1 = sqrt(normAux1);

			//acf computation

			TData tmpCoef;
			TData normAux2;

			for(int m=0;m<ul;m++) 
			{
				tmpCoef = 0.0;
			
				//build aux2
				aux2.SetPtr(ptr,it);
				normAux2=0.0;
				
				for(int i=0;i<aux2.Size();i++) 
					normAux2 += aux2[i]*aux2[i];
				
				normAux2 = sqrt(normAux2);
				
				for(int k=0;k<aux2.Size();k++) 
					tmpCoef += aux1[k]*aux2[k];
				
				tmpCoef /= normAux1*normAux2;
				
				acf.AddElem(tmpCoef);
				
				ptr++;
			}
			/*  --MATLAB code--
			    for featInd=1:size(A,2)
			    for m=1:upperLimit
			    aux1 = []; aux2 = []; aux = [];
			    aux1 = A(1:integTime+1,featInd);     
			    aux2 = A(m:m+integTime,featInd);
			    aux = aux1'*aux2/(norm(aux1)*norm(aux2));
			    atc(featInd,m) = aux;
			    end;
			    end;
			*/

			return true;
		}

	}

} // namespace CLAM

