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

#include <FL/Fl.H>
#include "NotGeneratedUserInterface.hxx"
#include "GeometryKit.hxx"
#include "AnalysisSynthesisExampleGUI.hxx"
#include "SpectrumDisplay.hxx" 
#include <FL/fl_file_chooser.H>
#include <FL/Fl.H>
#include "FLTKConfigurator.hxx"
#include "Fl_Browsable_Playable_Audio.hxx"
#include "AudioPlayer.hxx"

using CLAMVM::Fl_Browsable_Playable_Audio;
using CLAMVM::SpectrumDisplay;
using CLAMVM::LogMagSpectrumAdapter;
using namespace CLAM;
using namespace CLAMGUI;

void UserInterface::EditConfiguration(void)
{
	CLAM::FLTKConfigurator * configurator = new CLAM::FLTKConfigurator;
	configurator->SetConfig(mAnalysisSynthesisExample->mGlobalConfig);
	configurator->SetApplyCallback(makeMemberFunctor0((*this), UserInterface, Update ));
	configurator->show();
}

void UserInterface::Update()
{
	mAnalysisSynthesisExample->SetHaveConfig(true);
 	mAnalysisSynthesisExample->InitConfigs();
	LoadSound();
	mAnalysisSynthesisExample->mVisualization.CloseAll();
	mAnalysisSynthesisExample->mVisualization.AnalysisToDo();
}

void UserInterface::LoadConfiguration(void)
{
	char* str = fl_file_chooser("Select configuration file","*.xml","");
	
	if ( str )
	{
		mConfigurationText->value(str);
		std::string inputXMLFileName(str);

		mAnalysisSynthesisExample->LoadConfig(inputXMLFileName);
		LoadSound();	
	
		if (mAnalysisSynthesisExample->mHaveAnalysis &&	mAnalysisSynthesisExample->mHaveConfig)
			mSynthesize->activate();
		Fl::redraw();
		mAnalysisSynthesisExample->mVisualization.CloseAll();
		mAnalysisSynthesisExample->mVisualization.AnalysisToDo();
		mSinTracksDisplay->deactivate();

	}		
}

void UserInterface::LoadSound(void)
{
	if (mAnalysisSynthesisExample->mHaveConfig)
	{	
		mAnalysisSynthesisExample->LoadInputSound();
		if (mAnalysisSynthesisExample->mHaveAudioIn)
		{
			mAnalyze->activate();
			mDisplayInSM->activate();
			mDisplayInSound->activate();
		}
		else
		{
			mAnalyze->deactivate();
			mDisplayInSM->deactivate();
			mDisplayInSound->deactivate();
			mAnalysisSynthesisExample->mHaveConfig=false;
		}
		mCounter->deactivate();
		mCounter->value( 0 );

		mSynthesize->deactivate();
		mOutputSM->deactivate();
	}
	Fl::redraw();
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
// 	char* str = fl_file_chooser("Select analysis data file","{*.xml|*.sdif}","");
// 	if (str)
// 	{
// 		//mAnalysisDataText->value(str);
// 		std::string inputXMLFileName(str);
// 		mAnalysisSynthesisExample->LoadAnalysis(inputXMLFileName);
// 		mStoreAnalysisData->deactivate();
// 		if (
// 			mAnalysisSynthesisExample->mHaveAnalysis &&
// 			mAnalysisSynthesisExample->mHaveConfig)
// 		{
// 			mSynthesize->activate();
// 		}
// 		Fl::redraw();
// 	}

	mAnalysisSynthesisExample->LoadAnalysis(  );
	mStoreAnalysisData->deactivate(  );
//	if (
	mAnalysisSynthesisExample->mHaveAnalysis = true;
// &&
	mAnalysisSynthesisExample->mHaveConfig = true;
// )
	{
		mSynthesize->activate(  );
	}
	Fl::redraw(  );
}

void UserInterface::Analyze(void)
{
	mAnalysisSynthesisExample->Analyze();
	if (mAnalysisSynthesisExample->mHaveAnalysis)
	{
		mAnalysisSynthesisExample->mVisualization.AnalysisDone();

		mDisplayInSpec->activate();
		mSynthesize->activate();
		mMelodySM->activate();
		mStoreAnalysisData->activate();
		mSinTracksDisplay->activate();
		mCounter->activate();
		mCounter->range( 0, mAnalysisSynthesisExample->mSegment.GetnFrames() );
		mCounter->step( 1 );
		mCounter->lstep( mAnalysisSynthesisExample->mSegment.GetnFrames()/10 );

		mDisplayInSpec->activate();

		if(mAnalysisSynthesisExample->mHaveTransformationScore)
			mDoTransformation->activate();

		//ChangeFrame();
		mAnalysisSynthesisExample->mVisualization.Display ( mAnalysisSynthesisExample->mVisualization.eAudioIn, 
								    mAnalysisSynthesisExample->mAudioIn );
		mAnalysisSynthesisExample->mVisualization.Display( mAnalysisSynthesisExample->mSegment );
		
		Fl::redraw();
	}
}

void UserInterface::Synthesize(void)
{
	mAnalysisSynthesisExample->Synthesize();
	if (mAnalysisSynthesisExample->mHaveAudioOut)
	{
		mSynthesize->activate();
		mOutputSM->activate();
		Fl::redraw();
	}
}

void UserInterface::Exit(void)
{
	//delete mWindow;
	mWindow->hide();
}

void UserInterface::StoreAnalysisData(void)
{
	mAnalysisSynthesisExample->StoreAnalysis();
}


void UserInterface::DisplayInputSpectrum(void)
{
	mAnalysisSynthesisExample->mVisualization.Display ( mAnalysisSynthesisExample->mVisualization.eSpectrumIn, mAnalysisSynthesisExample->mSegment.GetFrame( /*(int)*/ mCounter->value() ).GetSpectrum() );
}

void UserInterface::DisplayInputSound(void)
{
	mAnalysisSynthesisExample->mVisualization.Display ( mAnalysisSynthesisExample->mVisualization.eAudioIn, mAnalysisSynthesisExample->mAudioIn );
}

void UserInterface::DisplaySinusoidalTracks()
{
	mAnalysisSynthesisExample->mVisualization.Display( mAnalysisSynthesisExample->mSegment );
}

void UserInterface::DisplayOutputSound(void)
{
	mAnalysisSynthesisExample->mVisualization.Display ( mAnalysisSynthesisExample->mVisualization.eAudioOut, mAnalysisSynthesisExample->mAudioOut );
}

void UserInterface::DisplayOutputSpectrum(void)
{
	mAnalysisSynthesisExample->mVisualization.Display ( mAnalysisSynthesisExample->mVisualization.eSpectrumOut, mAnalysisSynthesisExample->mSegment.GetFrame( /*(int)*/ mCounter->value() ).GetOutSpec() );
}

void UserInterface::DisplayOutputSoundResidual(void)
{
	mAnalysisSynthesisExample->mVisualization.Display ( mAnalysisSynthesisExample->mVisualization.eAudioResidual, mAnalysisSynthesisExample->mAudioOutRes );
}

void UserInterface::DisplayOutputSoundSinusoidal(void)
{
	mAnalysisSynthesisExample->mVisualization.Display ( mAnalysisSynthesisExample->mVisualization.eAudioSinusoidal, mAnalysisSynthesisExample->mAudioOutSin );
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

void UserInterface::ChangeFrame()
{
//	DisplayInputSound();
//	DisplayInputSpectrum();

	int nframe = (int) mCounter->value();

	List<Frame>& localFrames = mAnalysisSynthesisExample->mSegment.GetFramesArray();

	TData nextcursorpos = localFrames[ nframe ].GetCenterTime() * mAnalysisSynthesisExample->mSegment.GetSamplingRate();
	
	//Notify SigSlotted class to change
	mAnalysisSynthesisExample->mVisualization.mFrameSignal.Emit( nextcursorpos );
	mAnalysisSynthesisExample->mVisualization.Display ( mAnalysisSynthesisExample->mVisualization.eAudioIn );
	mAnalysisSynthesisExample->mVisualization.Display ( mAnalysisSynthesisExample->mVisualization.eSpectrumIn, 
							    mAnalysisSynthesisExample->mSegment.GetFrame( /*(int)*/ nframe ).GetSpectrum() );

//	Fl::redraw();
}

void UserInterface::ChangeTimeTag( TTime tag )
{
	//Change mCounter
	TTime time( tag / mAnalysisSynthesisExample->mSegment.GetSamplingRate() );
	TIndex nframe = mAnalysisSynthesisExample->mSegment.FindFrame( time );

	mCounter->value( (int) nframe );

	//Notify other GLPorts
	TData nextcursorpos = mAnalysisSynthesisExample->mSegment.GetFramesArray()[ nframe ].GetCenterTime();
	nextcursorpos *= mAnalysisSynthesisExample->mSegment.GetSamplingRate();
	mAnalysisSynthesisExample->mVisualization.mFrameSignal.Emit( nextcursorpos );

	//Change Spectrum Displayer
	mAnalysisSynthesisExample->mVisualization.Display ( mAnalysisSynthesisExample->mVisualization.eAudioIn );
	mAnalysisSynthesisExample->mVisualization.Display ( mAnalysisSynthesisExample->mVisualization.eSpectrumIn, mAnalysisSynthesisExample->mSegment.GetFrame( /*(int)*/ nframe ).GetSpectrum() );

//	Fl::redraw();
}

void UserInterface::Init(  )
{
	mAnalysisSynthesisExample->mVisualization.mSlot.Wrap( this, &UserInterface::ChangeTimeTag );
	mAnalysisSynthesisExample->SetCanvas( mSmartTile );
}
