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
#include "SMSTools.hxx"
#include <FL/fl_file_chooser.H>
#include <FL/Fl.H>
#include "FLTKConfigurator.hxx"
#include "AudioPlayer.hxx"
#include <FL/fl_message.H>
#include <FL/fl_ask.H>

using namespace CLAM;
using namespace CLAMGUI;

void UserInterface::EditConfiguration(void)
{
	CLAM::FLTKConfigurator * configurator = new CLAM::FLTKConfigurator;
	configurator->SetConfig(mSMS->mGlobalConfig);
	configurator->SetApplyCallback(makeMemberFunctor0((*this), UserInterface, Update ));
	configurator->show();	
}

void UserInterface::Update()
{
	mSMS->SetHaveConfig(true);
 	mSMS->InitConfigs();
	// check if LoadSound operation could be carried
	if ( !LoadSound() )
		return;
	ApplyInitialState();
	ApplyReadyToAnalyzeState();
	mSMS->mExplorer.CloseAll();
	mCounter->value(0);
	mWindow->redraw();
}

void UserInterface::LoadConfiguration(void)
{
	char* str = fl_file_chooser("Select configuration file","*.xml","",1);
	
	if ( str )
	{
		std::string inputXMLFileName(str);

		mSMS->LoadConfig(inputXMLFileName);
		if (! LoadSound() )
			return;	
		ApplyReadyToAnalyzeState();
	
		if ( mSMS->mDataState.Query( DataState::HaveAnalysis )
		     &&	mSMS->mDataState.Query( DataState::HaveConfig ) )
			ApplyAnalysisAvailableState();

		mSMS->mExplorer.CloseAll();
		mCounter->value(0);
		mWindow->redraw();
	}		
}

bool UserInterface::LoadSelectedSound( )
{
	char* str = fl_file_chooser("Select audio file","*.wav","",1);
	if ( str )
	{
		mSMS->LoadInputSound( str );
		if ( !mSMS->mDataState.Query( DataState::HaveAudioIn ) )
		{
			fl_alert( "Unable to open the Input sound file!");
			ApplyInitialState();
			return false;
		}
		
		mSMS->mExplorer.NewInputAudio( mSMS->mOriginalSegment.GetAudio());
		
		return true;
	}
	return false;
}

bool UserInterface::LoadSound( )
{
	mSMS->LoadInputSound( );
	if ( !mSMS->mDataState.Query( DataState::HaveAudioIn ) )
	{
		fl_alert( "Unable to open the Input sound file!");
		ApplyInitialState();
		return false;
	}
	
	mSMS->mExplorer.NewInputAudio( mSMS->mOriginalSegment.GetAudio());
	
	return true;
}

void UserInterface::StoreConfiguration(void)
{
	char* str = fl_file_chooser("Select configuration file","*.xml","",1);
	if ( str )
	{
		std::string inputXMLFileName(str);
		mSMS->StoreConfig(inputXMLFileName);
	}		
}

void UserInterface::LoadTransformation(void)
{
	char* str = fl_file_chooser("Select configuration file","*.xml","",1);
	if ( str )
	{
		//mTransformationFileText->value(str);
		std::string inputXMLFileName(str);
		mSMS->LoadTransformationScore(inputXMLFileName);
		mSMS->mDataState.Reached( DataState::HaveTransformationScore );
		ApplyTransformationReadyState();
		mWindow->redraw();
	}
}

void UserInterface::LoadAnalysisData(void)
{

	if ( !mSMS->LoadAnalysis(  ) )
		return;
	mSMS->mDataState.Reached( DataState::HaveAnalysis );
	// @todo: Check this is true...
	mSMS->mDataState.Reached( DataState::HaveConfig );
	ApplyAnalysisAvailableState();
	DeactivateFrameDataMenuItems();
	mSMS->mExplorer.NewSegment( mSMS->mOriginalSegment );
	// @todo: determine what has to do the UserInterface for obtaining frame data when it is being loaded
	// so that one cannot rely on the fact that it is available in the segment object
	// mSMS->mExplorer.NewFrame( mSMS->mSegment.GetFramesArray()[0]);
	mFrameDataAvailable = false;
	mWindow->redraw();
}

bool UserInterface::FrameDataAvailable()
{
	return mFrameDataAvailable;
}

void UserInterface::Analyze(void)
{
	mSMS->Analyze();
	if ( mSMS->mDataState.Query( DataState::HaveAnalysis ) )
	{
		ApplyInitialState();
		ApplyReadyToAnalyzeState();
		ApplyAnalysisAvailableState();
		mFrameDataAvailable = true;
		mSMS->mExplorer.NewSegment( mSMS->mOriginalSegment );
		mSMS->mExplorer.NewFrame( mSMS->mOriginalSegment.GetFramesArray()[0],
													   FrameDataAvailable());
		mWindow->redraw();
	}
}

void UserInterface::Synthesize(void)
{
	mSMS->Synthesize();
	if ( mSMS->mDataState.Query( DataState::HaveAudioOut ) )
	{
		ApplySynthesisAvailableState();
	}
	mSMS->mExplorer.NewSynthesizedAudio(mSMS->mAudioOut);
	mSMS->mExplorer.NewSynthesizedSinusoidal(mSMS->mAudioOutSin);
	mSMS->mExplorer.NewSynthesizedResidual( mSMS->mAudioOutRes );
	mWindow->redraw();
}

void UserInterface::Exit(void)
{
	//delete mWindow;
	mWindow->hide();
}

void UserInterface::StoreAnalysisData(void)
{
	mSMS->StoreAnalysis();
}

void UserInterface::StoreTransformationData(void)
{
	mSMS->StoreTransformation();
}

void UserInterface::StoreOutputSound(void)
{
	char* str = fl_file_chooser("Select audio file","*.wav","",1);
	if ( str )
	{
		mSMS->StoreOutputSound( str );
	}
}

void UserInterface::StoreOutputSoundResidual(void)
{
	char* str = fl_file_chooser("Select audio file","*.wav","",1);
	if ( str )
	{
		mSMS->StoreOutputSoundResidual( str );
	}
}

void UserInterface::StoreOutputSoundSinusoidal(void)
{
	char* str = fl_file_chooser("Select audio file","*.wav","",1);
	if ( str )
	{
		mSMS->StoreOutputSoundSinusoidal( str );
	}
}

void UserInterface::AnalyzeMelody(void)
{
	mSMS->AnalyzeMelody();
	ApplyMelodyAvailableState();
}

void UserInterface::StoreMelody(void)
{
	char* str = fl_file_chooser("Select melody file","*.xml","",1);
	if ( str )
	{
		mSMS->StoreMelody( str );
	}
}

void UserInterface::Transform(void)
{

	mSMS->Transform();
	ApplyTransformationPerformedState();
	mSMS->mExplorer.CloseAll();
	mCounter->value(0);
	mSMS->mExplorer.NewSegment( mSMS->mTransformedSegment );
	mSMS->mExplorer.NewFrame( mSMS->mTransformedSegment.GetFramesArray()[0],
				  FrameDataAvailable() );
	mWindow->redraw();

}

void UserInterface::UndoTransform()
{
	mSMS->mDataState.Abandoned( DataState::HaveTransformation );
	ApplyInitialState();
	ApplyReadyToAnalyzeState();
	ApplyAnalysisAvailableState();
	mFrameDataAvailable = true;
	
	mSMS->mExplorer.NewSegment( mSMS->mOriginalSegment );
	mSMS->mExplorer.NewFrame( mSMS->mOriginalSegment.GetFramesArray()[0],
													   FrameDataAvailable());

	mWindow->redraw();
}

void UserInterface::ChangeFrame()
{
	int nframe = (int) mCounter->value();

	if ( mFrameDataAvailable )
	{
		if( mSMS->mDataState.Query( DataState::HaveTransformation ) )
			mSMS->mExplorer.NewFrame( mSMS->mTransformedSegment.GetFramesArray()[nframe],
													   FrameDataAvailable() );
		else
			mSMS->mExplorer.NewFrame( mSMS->mOriginalSegment.GetFramesArray()[nframe],
													   FrameDataAvailable() );
	}
}

void UserInterface::ChangeTimeTag( TTime tag )
{
}

void UserInterface::Init(  )
{
	mSMS->SetCanvas( mSmartTile );
	ApplyInitialState();
	mFrameDataAvailable = false;
}

void UserInterface::DisplayInputSound()
{
	mSMS->mExplorer.ShowInputAudio();
	mWindow->redraw();
}

void UserInterface::DisplaySpectrumAndPeaks()
{
	mSMS->mExplorer.ShowSpectrumAndPeaks();
	mWindow->redraw();

}

void UserInterface::DisplaySinusoidalSpectrum()
{
	mSMS->mExplorer.ShowSinusoidalSpectrum();
	mWindow->redraw();
}

void UserInterface::DisplayResidualSpectrum()
{
	mSMS->mExplorer.ShowResidualSpectrum();
	mWindow->redraw();

}

void UserInterface::DisplaySynthesizedAudio()
{
	mSMS->mExplorer.ShowSynthesizedAudio();
	mWindow->redraw();

}

void UserInterface::DisplaySynthesizedSinusoidal()
{
	mSMS->mExplorer.ShowSynthesizedSinusoidal();
	mWindow->redraw();

}

void UserInterface::DisplaySynthesizedResidual()
{
	mSMS->mExplorer.ShowSynthesizedResidual();
	mWindow->redraw();

}

void UserInterface::DisplaySinusoidalTracks()
{
	mSMS->mExplorer.ShowSinTracks();
	mWindow->redraw();

}

void UserInterface::ApplyInitialState()
{
	mSMS->mExplorer.CloseAll();
	mCounter->value(0);
	mSMSAnalysisButton->deactivate();
	mSegmentButton->deactivate();
	mFirstSegmentButton->deactivate();
	mSMSTransformationLoad->deactivate();
	mSMSTransformationButton->deactivate();
	mSecondSegmentButton->deactivate();
	mSMSSynthesisButton->deactivate();
	mAudioInButton->deactivate();
	mStoreMelodyButton->deactivate();
	mStoreAnalysis->deactivate();
}

void UserInterface::ApplyReadyToAnalyzeState()
{
	mSMSAnalysisButton->activate();
	mAudioInButton->activate();
	mStoreAnalysis->deactivate();
	mSMSSynthesisButton->deactivate();
	mSMSTransformationLoad->deactivate();
	mSMSTransformationButton->deactivate();
	mWindow->redraw();
}

void UserInterface::ApplyAnalysisAvailableState()
{
	ApplyReadyToAnalyzeState();
	mCounter->activate();
	mCounter->range( 0, mSMS->mOriginalSegment.GetnFrames() );
	mCounter->step( 1 );
	mCounter->lstep( mSMS->mOriginalSegment.GetnFrames()/10 );

	mStoreAnalysis->activate();
	mExtractMelody->activate();
	mSMSSynthesisButton->activate();
	
	mFirstSegmentButton->activate();

	mSMSTransformationLoad->activate();
	if ( mSMS->mDataState.Query( DataState::HaveTransformationScore ) )
		mSMSTransformationButton->activate();

	mWindow->redraw();

}

void UserInterface::DeactivateFrameDataMenuItems()
{
	mFirstSegmentButton->deactivate();
	mWindow->redraw();
}

void UserInterface::ApplyMelodyAvailableState()
{
	mStoreMelodyButton->activate();
	mWindow->redraw();
}

void UserInterface::ApplyTransformationReadyState()
{
	mSMSTransformationLoad->activate();
	if ( mSMS->mDataState.Query( DataState::HaveAnalysis ) )
		mSMSTransformationButton->activate();
	mWindow->redraw();
}

void UserInterface::ApplySynthesisAvailableState()
{
	mStoreSMSSynthSound->activate();
	mStoreSMSSynthSinusoidal->activate();
	mStoreSMSSynthResidual->activate();

	mShowSynthesizedAudio->activate();
	mShowSynthesizedSinusoidal->activate();
	mShowSynthesizedResidual->activate();
	mWindow->redraw();
}

void UserInterface::ApplyTransformationPerformedState()
{
	mStoreTransformation->activate();
	mSMSTransformationUndo->activate();
	mWindow->redraw();
}
