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

#include "OverlapAdd.hxx"


namespace CLAM
{

/* processing object method implementations */

OverlapAdd::OverlapAdd(void)
	: mInput( "Audio Input", this ),
	  mOutput( "Audio Output", this )
{
	Configure(OverlapAddConfig());
}

OverlapAdd::OverlapAdd(const OverlapAddConfig &c)
	: mInput( "Audio Input", this ),
	  mOutput( "Audio Output", this )
{
	Configure(c);
}

OverlapAdd::~OverlapAdd()
{
}


/* configure the processing object according to the config object */

bool OverlapAdd::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	int hopSize=mConfig.GetHopSize();
	int frameSize=mConfig.GetFrameSize();

	mInput.SetSize( frameSize );
	mInput.SetHop( frameSize );

	mOutput.SetSize( frameSize );
	mOutput.SetHop( hopSize );
	
	return true;
}

/* the supervised Do() function */

bool OverlapAdd::Do(void)
{
	bool result = Do( mInput.GetAudio(), mOutput.GetAudio() );
	mInput.Consume();
	mOutput.Produce();
	return result;
}

bool OverlapAdd::Do( const Audio &in, Audio & out)
{
	for( int i=0;i<out.GetSize()/2-1;i++)
	{
		out.GetBuffer()[i] += in.GetBuffer()[i];
	}
	
	for( int i=out.GetSize()/2;i<out.GetSize();i++)
	{
		out.GetBuffer()[i] = in.GetBuffer()[i];
	}
} 

} // namespace CLAM

