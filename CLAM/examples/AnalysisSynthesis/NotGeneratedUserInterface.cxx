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
#include "AnalysisSynthesisExampleGUI.hxx"
#include <FL/fl_file_chooser.H>
#include <FL/Fl.H>
#include "FLTKConfigurator.hxx"
#include "AudioPlayer.hxx"

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
	mAnalysisSynthesisExample->mExplorer.CloseAll();
}

void UserInterface::LoadConfiguration(void)
{
	char* str = fl_file_chooser("Select configuration file","*.xml","");
	
	if ( str )
	{
		std::string inputXMLFileName(str);

		mAnalysisSynthesisExample->LoadConfig(inputXMLFileName);
		LoadSound();	
	
		if (mAnalysisSynthesisExample->mHaveAnalysis &&	mAnalysisSynthesisExample->mHaveConfig)
			mSynthesize->activate();
		Fl::redraw();
		mAnalysisSynthesisExample->mExplorer.CloseAll();
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
		}
		else
		{
			mAnalyze->deactivate();
			mAnalysisSynthesisExample->mHaveConfig=false;
		}
		mCounter->deactivate();
		mCounter->value( 0 );

		mSynthesize->deactivate();
	}
	
	mAnalysisSynthesisExample->mExplorer.NewInputAudio( mAnalysisSynthesisExample->mAudioIn );

	mWindow->redraw();
}

void UserInterface::StoreConfiguration(void)
{
	char* str = fl_file_chooser("Select configuration file","*.xml","");
	if ( str )
	{
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

		mSynthesize->activate();
		mMelodySM->activate();
		mStoreAnalysisData->activate();
		mSinTracksDisplay->activate();
		mCounter->activate();
		mCounter->range( 0, mAnalysisSynthesisExample->mSegment.GetnFrames() );
		mCounter->step( 1 );
		mCounter->lstep( mAnalysisSynthesisExample->mSegment.GetnFrames()/10 );


		if(mAnalysisSynthesisExample->mHaveTransformationScore)
			mDoTransformation->activate();

		mAnalysisSynthesisExample->mExplorer.NewSegment( mAnalysisSynthesisExample->mSegment );
		mAnalysisSynthesisExample->mExplorer.NewFrame( mAnalysisSynthesisExample->mSegment.GetFramesArray()[0]);
		
	}
}

void UserInterface::Synthesize(void)
{
	mAnalysisSynthesisExample->Synthesize();
	if (mAnalysisSynthesisExample->mHaveAudioOut)
	{
		mSynthesize->activate();
		Fl::redraw();
	}
	mAnalysisSynthesisExample->mExplorer.NewSynthesizedAudio(mAnalysisSynthesisExample->mAudioOut);
	mAnalysisSynthesisExample->mExplorer.NewSynthesizedSinusoidal(mAnalysisSynthesisExample->mAudioOutSin);
	mAnalysisSynthesisExample->mExplorer.NewSynthesizedResidual( mAnalysisSynthesisExample->mAudioOutRes );
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
	int nframe = (int) mCounter->value();

	mAnalysisSynthesisExample->mExplorer.NewFrame( mAnalysisSynthesisExample->mSegment.GetFramesArray()[nframe] );
}

void UserInterface::ChangeTimeTag( TTime tag )
{
}

void UserInterface::Init(  )
{
	mAnalysisSynthesisExample->SetCanvas( mSmartTile );
}

void UserInterface::DisplayInputSound()
{
	mAnalysisSynthesisExample->mExplorer.ShowInputAudio();
}

void UserInterface::DisplaySpectrumAndPeaks()
{
	mAnalysisSynthesisExample->mExplorer.ShowSpectrumAndPeaks();
}

void UserInterface::DisplaySinusoidalSpectrum()
{
	mAnalysisSynthesisExample->mExplorer.ShowSinusoidalSpectrum();
}

void UserInterface::DisplayResidualSpectrum()
{
	mAnalysisSynthesisExample->mExplorer.ShowResidualSpectrum();
}

void UserInterface::DisplaySynthesizedAudio()
{
	mAnalysisSynthesisExample->mExplorer.ShowSynthesizedAudio();
}

void UserInterface::DisplaySynthesizedSinusoidal()
{
	mAnalysisSynthesisExample->mExplorer.ShowSynthesizedSinusoidal();
}

void UserInterface::DisplaySynthesizedResidual()
{
	mAnalysisSynthesisExample->mExplorer.ShowSynthesizedResidual();
}

void UserInterface::DisplaySinusoidalTracks()
{
	mAnalysisSynthesisExample->mExplorer.ShowSinTracks();
}
