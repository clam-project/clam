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

#include "AnalysisSynthesisExampleGUI.hxx"
#include <iostream>
#include <FL/Fl.H>

namespace CLAMGUI
{

void AnalysisSynthesisExampleGUI::Run(void)
{
	UserInterface userinterface;
	userinterface.mAnalysisSynthesisExample = this;
	Fl::get_system_colors();
	Fl::set_boxtype(FL_UP_BOX,FL_THIN_UP_BOX);
	Fl::set_boxtype(FL_DOWN_BOX,FL_THIN_DOWN_BOX);
	userinterface.mWindow->show();
	WaitMessageGUI::flThread = pthread_self();
	ProgressGUI::flThread = pthread_self();
	Fl::add_idle(WaitMessageGUI::Idle);
	Fl::add_idle(ProgressGUI::Idle);
	
	Fl::run();
}
	
Progress* AnalysisSynthesisExampleGUI::CreateProgress(const char* title,float from,float to) 
{
	return new ProgressGUI(title,from,to);
}

WaitMessage* AnalysisSynthesisExampleGUI::CreateWaitMessage(const char* title) 
{
	return new WaitMessageGUI(title);
}

void* AnalysisSynthesisExampleGUI::startfn_analyze(void* ptr)
{
	((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=1;
	((AnalysisSynthesisExampleGUI*)ptr)->Analyze();
	((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=2;
	return 0;
}

void* AnalysisSynthesisExampleGUI::startfn_synthesize(void* ptr)
{
	((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=1;
	((AnalysisSynthesisExampleGUI*)ptr)->Synthesize();
	((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=2;
	return 0;
}

void* AnalysisSynthesisExampleGUI::startfn_LoadAnalysis(void* ptr)
{
	
	((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=1;
	((AnalysisSynthesisExampleGUI*)ptr)->LoadAnalysis( ((AnalysisSynthesisExampleGUI*)ptr)->mFilename);
	((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=2;
	return 0;
}

void* AnalysisSynthesisExampleGUI::startfn_StoreAnalysis(void* ptr)
{
	
	((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=1;
	((AnalysisSynthesisExampleGUI*)ptr)->StoreAnalysis();
	((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=2;
	return 0;
}

void AnalysisSynthesisExampleGUI::AnalyzeThread()
{
	mThreadState = 0;
	pthread_create(&mThread,NULL,startfn_analyze,this);
	while (mThreadState==0) { Fl::wait(0.1); }
	while (mThreadState==1) { Fl::wait(0.1); }
	pthread_join(mThread,NULL);
}

void AnalysisSynthesisExampleGUI::SynthesizeThread()
{
	mThreadState = 0;
	pthread_create(&mThread,NULL,startfn_synthesize,this);
	while (mThreadState==0) { Fl::wait(0.1); }
	while (mThreadState==1) { Fl::wait(0.1); }
	pthread_join(mThread,NULL);
}



void AnalysisSynthesisExampleGUI::LoadAnalysisThread(const std::string& filename)
{
	mFilename = filename;
	mThreadState = 0;
	pthread_create(&mThread,NULL,startfn_LoadAnalysis,this);
	while (mThreadState==0) { Fl::wait(0.1); }
	while (mThreadState==1) { Fl::wait(0.1); }
	pthread_join(mThread,NULL);
}

void AnalysisSynthesisExampleGUI::StoreAnalysisThread(void)
{
	mThreadState = 0;
	pthread_create(&mThread,NULL,startfn_StoreAnalysis,this);
	while (mThreadState==0) { Fl::wait(0.1); }
	while (mThreadState==1) { Fl::wait(0.1); }
	pthread_join(mThread,NULL);
}


}
