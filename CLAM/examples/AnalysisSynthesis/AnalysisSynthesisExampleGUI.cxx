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


#include "UserInterface.hxx"
#include <FL/fl_file_chooser.H>
#include "Fl_Progress.hxx"
#include "Fl_WaitMessage.hxx"
#include "AnalysisSynthesisExampleBase.hxx"
#include <pthread.h>
#include "FLTKConfigurator.hxx"
#include "CLAMViews.hxx"
#include "CLAMPresentations.hxx"
#include "DebugSnapshots.hxx"
#include "AudioSnapshot.hxx"

/******* TRANSFORMATION *******/
#include "SMSFreqShift.hxx"

#include <iostream>

using namespace CLAM;
using namespace CLAMGUI;
using namespace std;

#ifndef WIN32
#include <unistd.h>
#define Sleep(var) usleep(var*1000)
#endif

#ifdef WIN32
#include <windows.h>
#endif

class ProgressGUI:public Progress
{
public:
	Fl_Progress* mFlProgress;
	static Fl_Progress* requested;
	static Fl_Progress* delRequested;
	static const char* requestedTitle;
	static pthread_t flThread;
	static float requestedFrom;	
	static float requestedTo;	
	
	void Update(float val)
	{
		mFlProgress->mValue = val;
	}

	ProgressGUI(const char* title,float from,float to)
		:Progress(title,from,to)
	{
		if (flThread == pthread_self())
		{

			mFlProgress = new Fl_Progress;
			mFlProgress->mLabel->label(mTitle);
			mFlProgress->mSlider->range(mFrom,mTo);
			mFlProgress->mWindow->show();
		}else{
			requested = 0;

			requestedFrom = mFrom;
			requestedTo = mTo;
			requestedTitle = mTitle;
			while (requested==0)
			{
				Sleep(1);
			}

			mFlProgress = requested;
		}
	}

	~ProgressGUI()
	{
		if (flThread == pthread_self())
		{

			delete mFlProgress;
		}else{
			delRequested = mFlProgress;
			while (delRequested)
			{
				Sleep(1);
			}
		}
	}	

	static void Idle(void* ptr)
	{
		std::cout << "Idle #1" << std::endl;
		if (requestedTitle)
		{

			const char* tmp = requestedTitle;
			float tmpFrom = requestedFrom;
			float tmpTo = requestedTo;
			requestedTitle = 0;
			Fl_Progress * o= new Fl_Progress;
			o->mLabel->label(tmp);
			o->mSlider->range(tmpFrom,tmpTo);
			o->mWindow->show();
			requested = o;
		}
		if (delRequested)
		{

			Fl_Progress* tmp = delRequested;
			delRequested = 0;
			delete tmp;
		}
	}
};

Fl_Progress* ProgressGUI::requested = 0;
Fl_Progress* ProgressGUI::delRequested = 0;
const char* ProgressGUI::requestedTitle = 0;
float ProgressGUI::requestedFrom;
float ProgressGUI::requestedTo;
pthread_t ProgressGUI::flThread;

class WaitMessageGUI:public WaitMessage
{
public:
	Fl_WaitMessage* mFlWaitMessage;
	static Fl_WaitMessage* requested;
	static Fl_WaitMessage* delRequested;
	static const char* requestedTitle;
	static pthread_t flThread;
	
	
	WaitMessageGUI(const char* title)
	:WaitMessage(title)
	{
		if (flThread == pthread_self())
		{
	
			mFlWaitMessage = new Fl_WaitMessage;
			mFlWaitMessage->mLabel->label(title);
			mFlWaitMessage->mWindow->show();
		}else{
			requested = 0;
	
			requestedTitle = title;
			while (requested==0)
			{
				Sleep(1);
			}
	
			mFlWaitMessage = requested;
		}
	}

	~WaitMessageGUI()
	{
		if (flThread == pthread_self())
		{

			delete mFlWaitMessage;
		}else{
			delRequested = mFlWaitMessage;
			while (delRequested)
			{
				Sleep(10);
			}
		}
	}	

	static void Idle(void* ptr)
	{
		std::cout << "Idle #2" << std::endl;
		if (requestedTitle)
		{

			const char* tmp = requestedTitle;
			requestedTitle = 0;
			Fl_WaitMessage * o= new Fl_WaitMessage;
			o->mLabel->label(tmp);
			o->mWindow->show();
			requested = o;
		}
		if (delRequested)
		{

			Fl_WaitMessage* tmp = delRequested;
			delRequested = 0;
			delete tmp;
		}
	}

};

Fl_WaitMessage* WaitMessageGUI::requested = 0;
Fl_WaitMessage* WaitMessageGUI::delRequested = 0;
const char* WaitMessageGUI::requestedTitle = 0;
pthread_t WaitMessageGUI::flThread;

class AnalysisSynthesisExampleGUI:public AnalysisSynthesisExampleBase
{
public:
	friend class UserInterface;
	void Run(void)
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
	
	CLAM::Progress* CreateProgress(const char* title,float from,float to) 
	{
		return new ProgressGUI(title,from,to);
	}

	CLAM::WaitMessage* CreateWaitMessage(const char* title) 
	{
		return new WaitMessageGUI(title);
	}

	int mThreadState;

	static void* startfn_analyze(void* ptr)
	{
		((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=1;
		((AnalysisSynthesisExampleGUI*)ptr)->Analyze();
		((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=2;
		return 0;
	}

	static void* startfn_synthesize(void* ptr)
	{
		((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=1;
		((AnalysisSynthesisExampleGUI*)ptr)->Synthesize();
		((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=2;
		return 0;
	}

	static void* startfn_LoadAnalysis(void* ptr)
	{

		((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=1;
		((AnalysisSynthesisExampleGUI*)ptr)->LoadAnalysis(
			((AnalysisSynthesisExampleGUI*)ptr)->mFilename);
		((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=2;
		return 0;
	}

	static void* startfn_StoreAnalysis(void* ptr)
	{

		((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=1;
		((AnalysisSynthesisExampleGUI*)ptr)->StoreAnalysis();
		((AnalysisSynthesisExampleGUI*)ptr)->mThreadState=2;
		return 0;
	}
	
	pthread_t mThread;
	
	void AnalyzeThread(void)
	{
		mThreadState = 0;
		pthread_create(&mThread,NULL,startfn_analyze,this);
		while (mThreadState==0) { Fl::wait(0.1); }
		while (mThreadState==1) { Fl::wait(0.1); }
		pthread_join(mThread,NULL);
	}

	void SynthesizeThread(void)
	{
		mThreadState = 0;
		pthread_create(&mThread,NULL,startfn_synthesize,this);
		while (mThreadState==0) { Fl::wait(0.1); }
		while (mThreadState==1) { Fl::wait(0.1); }
		pthread_join(mThread,NULL);
	}

	std::string mFilename;

	void LoadAnalysisThread(const std::string& filename)
	{
		mFilename = filename;
		mThreadState = 0;
		pthread_create(&mThread,NULL,startfn_LoadAnalysis,this);
		while (mThreadState==0) { Fl::wait(0.1); }
		while (mThreadState==1) { Fl::wait(0.1); }
		pthread_join(mThread,NULL);
	}

	void StoreAnalysisThread(void)
	{
		mThreadState = 0;
		pthread_create(&mThread,NULL,startfn_StoreAnalysis,this);
		while (mThreadState==0) { Fl::wait(0.1); }
		while (mThreadState==1) { Fl::wait(0.1); }
		pthread_join(mThread,NULL);
	}

};

void UserInterface::EditConfiguration(void)
{
	CLAM::FLTKConfigurator * configurator = new CLAM::FLTKConfigurator;
	configurator->SetConfig(mAnalysisSynthesisExample->mGlobalConfig);
	configurator->show();
	Fl::run();
}

void UserInterface::LoadConfiguration(void)
{
	
	char* str = fl_file_chooser("Select configuration file","*.xml","");
	
	if ( str )
	{
		mConfigurationText->value(str);
		
		std::string inputXMLFileName(str);
		
		mAnalysisSynthesisExample->LoadConfig(inputXMLFileName);
		
		if (mAnalysisSynthesisExample->mHaveConfig)
		{	
			mAnalysisSynthesisExample->LoadInputSound();
			if (mAnalysisSynthesisExample->mHaveAudioIn)
			{
				mAnalyze->activate();
				mDisplayInSM->activate();
				mDisplayInSound->activate();
				mPlayInputSound->activate();
			}
			else 
				mAnalysisSynthesisExample->mHaveConfig=false;
			for(int i=0;i<4;i++){
				if(mAttachedPresentations[i]!=NULL){
					if( mAttachedPresentations[i]->GetWindow()->shown() ) 
						mAttachedPresentations[i]->GetWindow()->hide();
				}
			}
		}
		
		if (mAnalysisSynthesisExample->mHaveAnalysis &&	mAnalysisSynthesisExample->mHaveConfig)
			mSynthesize->activate();
		Fl::redraw();
	}		
}

void UserInterface::StoreConfiguration(void)
{
	char* str = fl_file_chooser("Select configuration file","*.xml","");
	if ( str )
	{
		mConfigurationText->value(str);
		std::string inputXMLFileName(str);
		mAnalysisSynthesisExample->StoreConfig(inputXMLFileName);
	}		
}

void UserInterface::LoadTransformation(void)
{
	char* str = fl_file_chooser("Select configuration file","*.xml","");
	if ( str )
	{
		//mTransformationFileText->value(str);
		std::string inputXMLFileName(str);
		mAnalysisSynthesisExample->LoadTransformationScore(inputXMLFileName);
		mAnalysisSynthesisExample->mHaveTransformationScore=true;
		if (mAnalysisSynthesisExample->mHaveAnalysis)
			mDoTransformation->activate();
		Fl::redraw();
	}
}
void UserInterface::LoadAnalysisData(void)
{
	char* str = fl_file_chooser("Select analysis data file","{*.xml|*.sdif}","");
	if (str)
	{
		//mAnalysisDataText->value(str);
		std::string inputXMLFileName(str);
		mAnalysisSynthesisExample->LoadAnalysisThread(inputXMLFileName);
		mStoreAnalysisData->deactivate();
		if (
			mAnalysisSynthesisExample->mHaveAnalysis &&
			mAnalysisSynthesisExample->mHaveConfig)
		{
			mSynthesize->activate();
		}
		Fl::redraw();
	}
}

void UserInterface::Analyze(void)
{
	mAnalysisSynthesisExample->AnalyzeThread();
	if (mAnalysisSynthesisExample->mHaveAnalysis)
	{
		mDisplayInSpec->activate();
		mSynthesize->activate();
		mMelodySM->activate();
		mStoreAnalysisData->activate();
		if(mAnalysisSynthesisExample->mHaveTransformationScore)
			mDoTransformation->activate();
		Fl::redraw();
	}
}

void UserInterface::Synthesize(void)
{
	mAnalysisSynthesisExample->SynthesizeThread();
	if (mAnalysisSynthesisExample->mHaveAudioOut)
	{
		mSynthesize->activate();
		mOutputSM->activate();
		mPlayOutputs->activate();
		Fl::redraw();
	}
}

void UserInterface::Exit(void)
{
	delete mWindow;
}

void UserInterface::StoreAnalysisData(void)
{
	mAnalysisSynthesisExample->StoreAnalysisThread();
}

void UserInterface::DisplayInputSound(void)
{
	if(mAttachedPresentations[0]==NULL){
		Geometry g(0, 0, 890, 490);
		mAttachedViews[0] = new ProcDataView<Audio>;
		mAttachedPresentations[0] = new ProcDataPresentation<Audio>(g, "Input Audio");
		
		mAttachedViews[0]->BindTo( &mAnalysisSynthesisExample->mAudioIn );
		mAttachedPresentations[0]->LinkWithView( mAttachedViews[0] );
		
		Attach( mAttachedPresentations[0]->GetWindow() );
		mAttachedPresentations[0]->Show();
		mAttachedViews[0]->Refresh();
	}
 	else{
		//MRJ: Don't forget to always refresh associated views!
		mAttachedViews[0]->Refresh();
		if( mAttachedPresentations[0]->GetWindow()->shown() ) {
			mAttachedPresentations[0]->GetWindow()->hide();
		}
		else {
			mAttachedPresentations[0]->GetWindow()->show();
		}
 	}
	Fl::redraw();
}

void UserInterface::DisplayInputSpectrum(void)
{
	Geometry g(0, 0, 890, 490);
	ProcDataView<Spectrum> *view = new ProcDataView<Spectrum>;
	ProcDataPresentation<Spectrum> *presentation = 
		new ProcDataPresentation<Spectrum>(g,"Input Audio Spectrum");

	view->BindTo( &mAnalysisSynthesisExample->mSegment.GetFramesArray()[1].GetSpectrum() );
	presentation->LinkWithView( view );

	Attach(presentation->GetWindow());

	presentation->Show();
	view->Refresh();

	Fl::redraw();
}

void UserInterface::DisplayOutputSound(void)
{
	if(mAttachedPresentations[1]==NULL){
		Geometry g(0, 0, 890, 490);
		mAttachedViews[1] = new ProcDataView<Audio>;
		mAttachedPresentations[1] = new ProcDataPresentation<Audio>(g, "Output Audio");
		
		mAttachedViews[1]->BindTo( &mAnalysisSynthesisExample->mAudioIn );
		mAttachedPresentations[1]->LinkWithView( mAttachedViews[1] );
		
		Attach( mAttachedPresentations[1]->GetWindow() );
		mAttachedPresentations[1]->Show();
		mAttachedViews[1]->Refresh();
	}
	else{
		mAttachedViews[1]->Refresh();
		if( mAttachedPresentations[1]->GetWindow()->shown() ) {
			mAttachedPresentations[1]->GetWindow()->hide();
		}
		else {
			mAttachedPresentations[1]->GetWindow()->show();
		}
	}
	Fl::redraw();
}

void UserInterface::DisplayOutputSoundResidual(void)
{
	if(mAttachedPresentations[2]==NULL){
		Geometry g(0, 0, 890, 490);
		mAttachedViews[2] = new ProcDataView<Audio>;
		mAttachedPresentations[2] = new ProcDataPresentation<Audio>(g, "Output Residual");
		
		mAttachedViews[2]->BindTo( &mAnalysisSynthesisExample->mAudioIn );
		mAttachedPresentations[2]->LinkWithView( mAttachedViews[2] );
		
		Attach( mAttachedPresentations[2]->GetWindow() );
		mAttachedPresentations[2]->Show();
		mAttachedViews[2]->Refresh();
	}
	else{
		mAttachedViews[2]->Refresh();
		if( mAttachedPresentations[2]->GetWindow()->shown() ) {
			mAttachedPresentations[2]->GetWindow()->hide();
		}
		else {
			mAttachedPresentations[2]->GetWindow()->show();
		}
	}
	Fl::redraw();
}

void UserInterface::DisplayOutputSoundSinusoidal(void)
{
	if(mAttachedPresentations[3]==NULL){
		Geometry g(0, 0, 890, 490);
		mAttachedViews[3] = new ProcDataView<Audio>;
		mAttachedPresentations[3] = new ProcDataPresentation<Audio>(g, "Output Sinusoidal");
		
		mAttachedViews[3]->BindTo( &mAnalysisSynthesisExample->mAudioIn );
		mAttachedPresentations[3]->LinkWithView( mAttachedViews[3] );
		
		Attach( mAttachedPresentations[3]->GetWindow() );
		mAttachedPresentations[3]->Show();
		mAttachedViews[3]->Refresh();
	}
	else{
		mAttachedViews[3]->Refresh();
		if( mAttachedPresentations[3]->GetWindow()->shown() ) {
			mAttachedPresentations[3]->GetWindow()->hide();
		}
		else {
			mAttachedPresentations[3]->GetWindow()->show();
		}
	}
	Fl::redraw();
}

void UserInterface::StoreOutputSound(void)
{
	mAnalysisSynthesisExample->StoreOutputSound();
}

void UserInterface::StoreOutputSoundResidual(void)
{
	mAnalysisSynthesisExample->StoreOutputSoundResidual();
}

void UserInterface::StoreOutputSoundSinusoidal(void)
{
	mAnalysisSynthesisExample->StoreOutputSoundSinusoidal();
}

void UserInterface::AnalyzeMelody(void)
{
	mAnalysisSynthesisExample->AnalyzeMelody();
	mMelodyStore->activate();
}

void UserInterface::StoreMelody(void)
{
	mAnalysisSynthesisExample->StoreMelody();
}

void UserInterface::Transform(void)
{
	mAnalysisSynthesisExample->Transform();
}

void UserInterface::PlayInputSound(void)
{

	mAnalysisSynthesisExample->PlayInputSound();
}

void UserInterface::PlayOutputSound(void)
{

	mAnalysisSynthesisExample->PlayOutputSound();
}

void UserInterface::PlaySinusoidal(void)
{

	mAnalysisSynthesisExample->PlaySinusoidal();
}

void UserInterface::PlayResidual(void)
{

	mAnalysisSynthesisExample->PlayResidual();
}

void UserInterface::Attach(Fl_Window* canvas)
{
	canvas->resizable();
	mSmartTile->add(canvas);
	mSmartTile->equalize();
	canvas->show();
}

int main(void)
{
	try{
		AnalysisSynthesisExampleGUI example;
		/******* TRANSFORMATION *******/
		SMSFreqShift myTrans;

		example.SetTransformation(&myTrans);
		/******* TRANSFORMATION *******/

		example.Run();
	}
	catch(Err error)
	{
		error.Print();
		std::cerr << "Abnormal Program Termination" << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl; 
	}
	
	std::clog << "Finished successfully!"<<std::endl;
	return 0;
}
