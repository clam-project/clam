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
#include <FL/fl_message.H>

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
	ApplyReadyToAnalyzeState();
	mAnalysisSynthesisExample->mExplorer.CloseAll();
}

void UserInterface::LoadConfiguration(void)
{
	char* str = fl_file_chooser("Select configuration file","*.xml","");
	
	if ( str )
	{
		std::string inputXMLFileName(str);

		mAnalysisSynthesisExample->LoadConfig(inputXMLFileName);
		if (! LoadSound() )
			return;	
		ApplyReadyToAnalyzeState();
	
		if (mAnalysisSynthesisExample->mHaveAnalysis &&	mAnalysisSynthesisExample->mHaveConfig)
			ApplyAnalysisAvailableState();

		mAnalysisSynthesisExample->mExplorer.CloseAll();

		mWindow->redraw();
	}		
}

bool UserInterface::LoadSound(void)
{
	mAnalysisSynthesisExample->LoadInputSound();
	if ( !mAnalysisSynthesisExample->mHaveAudioIn )
	{
		fl_alert( "Unable to open the Input sound file!");
		ApplyInitialState();
		return false;
	}

	mAnalysisSynthesisExample->mExplorer.NewInputAudio( mAnalysisSynthesisExample->mAudioIn );
	
	return true;
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
		ApplyTransformationReadyState();
		mWindow->redraw();
	}
}

void UserInterface::LoadAnalysisData(void)
{

	mAnalysisSynthesisExample->LoadAnalysis(  );
	mAnalysisSynthesisExample->mHaveAnalysis = true;
	// @todo: Check this is true...
	mAnalysisSynthesisExample->mHaveConfig = true;
	ApplyAnalysisAvailableState();
	mWindow->redraw();
}

void UserInterface::Analyze(void)
{
	mAnalysisSynthesisExample->Analyze();
	if (mAnalysisSynthesisExample->mHaveAnalysis)
	{

/*		mSynthesize->activate();
		mMelodySM->activate();
		mStoreAnalysisData->activate();
		mSinTracksDisplay->activate();
		mCounter->activate();
		mCounter->range( 0, mAnalysisSynthesisExample->mSegment.GetnFrames() );
		mCounter->step( 1 );
		mCounter->lstep( mAnalysisSynthesisExample->mSegment.GetnFrames()/10 );
*/
		ApplyAnalysisAvailableState();

		mAnalysisSynthesisExample->mExplorer.NewSegment( mAnalysisSynthesisExample->mSegment );
		mAnalysisSynthesisExample->mExplorer.NewFrame( mAnalysisSynthesisExample->mSegment.GetFramesArray()[0]);
		
	}
}

void UserInterface::Synthesize(void)
{
	mAnalysisSynthesisExample->Synthesize();
	if (mAnalysisSynthesisExample->mHaveAudioOut)
	{
		ApplySynthesisAvailableState();
	}
	mAnalysisSynthesisExample->mExplorer.NewSynthesizedAudio(mAnalysisSynthesisExample->mAudioOut);
	mAnalysisSynthesisExample->mExplorer.NewSynthesizedSinusoidal(mAnalysisSynthesisExample->mAudioOutSin);
	mAnalysisSynthesisExample->mExplorer.NewSynthesizedResidual( mAnalysisSynthesisExample->mAudioOutRes );
	mWindow->redraw();
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
	ApplyMelodyAvailableState();
}

void UserInterface::StoreMelody(void)
{
	mAnalysisSynthesisExample->StoreMelody();
}

void UserInterface::Transform(void)
{
	mAnalysisSynthesisExample->Transform();
	ApplyTransformationPerformedState();
	mAnalysisSynthesisExample->mExplorer.CloseAll();
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
	ApplyInitialState();
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

void UserInterface::ApplyInitialState()
{
	mAnalysisSynthesisExample->mExplorer.CloseAll();
	mFileMenuItem->activate();
	mConfigurationOpsMenuItem->activate();
	mEditCfgMenuItem->deactivate();
	mLoadCfgMenuItem->activate();
	mEditCfgMenuItem->deactivate();
	mStoreCfgMenuItem->deactivate();
	mAnalysisFileOpsMenuItem->activate();
	mLoadAnalysisMenuItem->activate();
	mStoreAnalysisMenuItem->deactivate();
	mMelodyFileOpsMenuItem->activate();
	mStoreMelodyMenuItem->deactivate();
	mSMSTransFileOpsMenuItem->activate();
	mLoadSMSTransScoreMenuItem->deactivate();
	mSMSSynthesisFileOpsMenuItem->activate();
	mStoreSMSSynthSoundMenuItem->deactivate();
	mStoreSMSSynthSinusoidalMenuItem->deactivate();
	mStoreSMSSynthResidualMenuItem->deactivate();
	mSMSAnalysisMenuItem->activate();
	mDoSMSAnalysisMenuItem->deactivate();
	mMelodyExtractionMenuItem->deactivate();
	mSMSTransformationMenuItem->activate();
	mDoSMSTransMenuItem->deactivate();
	mSMSSynthesisMenuItem->activate();
	mDoSMSSynthesisMenuItem->deactivate();
	mViewMenuItem->activate();
	mShowOriginalAudioMenuItem->deactivate();
	mShowAnalysisResultsMenuItem->deactivate();
	mShowSinTracksMenuItem->deactivate();
	mViewFrameDataMenuItem->deactivate();
	mShowSinusoidalSpectrumMenuItem->deactivate();
	mShowSpectrumAndPeaksMenuItem->deactivate();
	mShowResidualSpectrumMenuItem->deactivate();
	mViewSynthesisResultsMenuItem->deactivate();
	mShowSynthesizedAudioMenuItem->deactivate();
	mShowSynthesizedSinusoidalMenuItem->deactivate();
	mShowSynthesizedResidualMenuItem->deactivate();
	mAppExitMenuItem->activate();
	mHelpMenuItem->activate();
	mCounter->deactivate();
}

void UserInterface::ApplyReadyToAnalyzeState()
{
	mEditCfgMenuItem->activate();
	mStoreCfgMenuItem->activate();
	mDoSMSAnalysisMenuItem->activate();
	mShowOriginalAudioMenuItem->activate();
	mStoreAnalysisMenuItem->deactivate();
	mDoSMSSynthesisMenuItem->deactivate();
}

void UserInterface::ApplyAnalysisAvailableState()
{
	ApplyReadyToAnalyzeState();
	mCounter->activate();
	mCounter->range( 0, mAnalysisSynthesisExample->mSegment.GetnFrames() );
	mCounter->step( 1 );
	mCounter->lstep( mAnalysisSynthesisExample->mSegment.GetnFrames()/10 );

	mStoreAnalysisMenuItem->activate();
	mLoadSMSTransScoreMenuItem->activate();
	mMelodyExtractionMenuItem->activate();
	mDoSMSSynthesisMenuItem->activate();
	
	mShowAnalysisResultsMenuItem->activate();
	mShowSinTracksMenuItem->activate();
	mViewFrameDataMenuItem->activate();
	mShowSpectrumAndPeaksMenuItem->activate();
	mShowResidualSpectrumMenuItem->activate();

	mWindow->redraw();

}

void UserInterface::ApplyMelodyAvailableState()
{
	mStoreMelodyMenuItem->activate();
}

void UserInterface::ApplyTransformationReadyState()
{
	mDoSMSTransMenuItem->activate();
}

void UserInterface::ApplySynthesisAvailableState()
{
	mStoreSMSSynthSoundMenuItem->activate();
	mStoreSMSSynthSinusoidalMenuItem->activate();
	mStoreSMSSynthResidualMenuItem->activate();

	mViewSynthesisResultsMenuItem->activate();
	mShowSynthesizedAudioMenuItem->activate();
	mShowSynthesizedSinusoidalMenuItem->activate();
	mShowSynthesizedResidualMenuItem->activate();
}

void UserInterface::ApplyTransformationPerformedState()
{
}
