/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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


#include "NaiveFlowControl.hxx"
#include "Processing.hxx"
#include <algorithm>
#include "OutPort.hxx"
#include "InPort.hxx"
#include "Network.hxx"

namespace CLAM
{

NaiveFlowControl::NaiveFlowControl()
{
}

double NaiveFlowControl::GetProcessingYPos(Processing & proc)
{
	CLAM_ASSERT(mNetwork, "The mNetwork pointer should not be 0");
	const std::string & name = mNetwork->GetProcessingName( proc );
	return mNetwork->getProcessingGeometry( name ).y;
}

void NaiveFlowControl::ProcessingAddedToNetwork( Processing & added )
{
	NetworkTopologyChanged();
	std::string processingType = added.GetClassName();

	if ( processingType == "AudioSource" || processingType == "AudioBufferSource" )
	{
		mSources.push_back( &added );
		return;
	}
	if (added.GetNInPorts()==0 && added.GetNOutPorts()==0) // port-less, for example ControlSource/Sink
	{
		mPortless.push_back( std::make_pair(GetProcessingYPos(added), &added) );
		mPortless.sort();
		return;
	}
	if (added.GetNInPorts()==0 && added.GetNOutPorts()!=0)
	{
		mGenerators.push_back( &added);
		return;
	}
	if ( processingType == "AudioSink" || processingType == "AudioBufferSink" )
	{
		mSinks.push_back( &added );
		return;
	}
	mNormalProcessings.push_back ( &added );
}

void NaiveFlowControl::ProcessingRemovedFromNetwork( Processing & removed )
{
	NetworkTopologyChanged();
	std::string processingType = removed.GetClassName();
	if ( processingType == "AudioSource" || processingType == "AudioBufferSource" )
	{
		auto _it = std::find(mSources.begin(), mSources.end(), &removed);
		if (_it != mSources.end()) mSources.erase(_it);
		return;
	}
	if (removed.GetNInPorts()==0 && removed.GetNOutPorts()==0) // port-less, for example ControlSource/Sink
	{
		for (ProcessingAndPosList::iterator i=mPortless.begin(); i!=mPortless.end(); i++)
		{
			if (i->second == &removed)
			{
				mPortless.erase(i);
				return;
			}
		}
		return;
	}
	if (removed.GetNInPorts()==0 && removed.GetNOutPorts()!=0)
	{
		auto _it = std::find(mGenerators.begin(), mGenerators.end(), &removed);
		if (_it != mGenerators.end()) mGenerators.erase(_it);
		return;
	}
	if ( processingType == "AudioSink" || processingType == "AudioBufferSink" )
	{
		auto _it = std::find(mSinks.begin(), mSinks.end(), &removed);
		if (_it != mSinks.end()) mSinks.erase(_it);
		return;
	}
	auto _it = std::find(mNormalProcessings.begin(), mNormalProcessings.end(), &removed);
	if (_it != mNormalProcessings.end()) mNormalProcessings.erase(_it);
}

void NaiveFlowControl::Do()
{
	// by now it have nothing of pulling
	// a naive approach: do sources, do normal processings, do sinks
	ProcessingList pendingSinks(mSinks);
	for (ProcessingAndPosList::iterator it=mPortless.begin(); it!=mPortless.end(); it++ )
	{
		Processing* proc = it->second;
		if (proc->CanConsumeAndProduce())
		{
			proc->Do();
		}
	}
	for (ProcessingList::iterator it=mSources.begin(); it!=mSources.end(); it++ )
	{
		Processing* proc = *it;
		if (proc->CanConsumeAndProduce())
		{
			//std::cerr << "Do: "<<proc->GetClassName() << std::endl;
			proc->Do();
		}
		else 
		{
			std::cerr << "Warning: some AudioSource was not able to consume incoming audio from the call-back.";
		}
	}
	while (true)
	{
		bool noProcessingRun = true;
		for (ProcessingList::iterator it=mNormalProcessings.begin(); it!=mNormalProcessings.end(); it++)
		{
			Processing* proc = *it;
			if (!proc->CanConsumeAndProduce() )
			{
				//std::cerr << "could NOT Do: "<<proc->GetClassName() << std::endl;
				continue;
			}
			//std::cerr << "Do: "<<proc->GetClassName() << std::endl;
			noProcessingRun = false;
			proc->Do();
		}
		for (ProcessingList::iterator it=pendingSinks.begin(); it!=pendingSinks.end(); )
		{
			Processing* proc = *it;
			if (!proc->CanConsumeAndProduce())
			{
				it++;
				continue;
			}
			//std::cerr << "Do: "<<proc->GetClassName() << std::endl;
			proc->Do();
			it = pendingSinks.erase(it);
			noProcessingRun = false;
		}
		if (noProcessingRun && !pendingSinks.empty())
		{
			for (ProcessingList::iterator it=mGenerators.begin(); it!=mGenerators.end(); it++)
			{
				Processing* proc = *it;
				if (!proc->CanConsumeAndProduce() )
				{
					// std::cerr << "could NOT Do: "<<proc->GetClassName() << std::endl;
					continue;
				}
				//std::cerr << "Do: "<<proc->GetClassName() << std::endl;
				noProcessingRun = false;
				proc->Do();
			}
		}
		if (noProcessingRun) break;
	}
	if (!pendingSinks.empty())
		std::cerr << "Warning: " << pendingSinks.size() << " sinks were not fed, so could not send audio to the callback." << std::endl;

	//std::cerr << "<<< Network.Do() is Done" << std::endl;

}


} // namespace CLAM


