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
#include "AudioBrowser.hxx"
#include "SpectrumDisplay.hxx" 
#include <FL/fl_file_chooser.H>
#include "FLTKConfigurator.hxx"
#include "Fl_Browsable_Playable_Audio.hxx"
#include "AudioPlayer.hxx"

using CLAMVM::Fl_Browsable_Playable_Audio;
using CLAMVM::AudioBrowser;
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
	mVC.CloseAll();
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
		mVC.CloseAll();

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
	char* str = fl_file_chooser("Select analysis data file","{*.xml|*.sdif}","");
	if (str)
	{
		//mAnalysisDataText->value(str);
		std::string inputXMLFileName(str);
		mAnalysisSynthesisExample->LoadAnalysis(inputXMLFileName);
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
	mAnalysisSynthesisExample->Analyze();
	if (mAnalysisSynthesisExample->mHaveAnalysis)
	{
		mDisplayInSpec->activate();
		mSynthesize->activate();
		mMelodySM->activate();
		mStoreAnalysisData->activate();

		mCounter->activate();
		mCounter->range( 0, mAnalysisSynthesisExample->mSegment.GetnFrames() );
		mCounter->step( 1 );
		mCounter->lstep( mAnalysisSynthesisExample->mSegment.GetnFrames()/10 );

		mVC.mPaintSignal.Emit( true );
		ChangeFrame();

		mDisplayInSpec->activate();

		if(mAnalysisSynthesisExample->mHaveTransformationScore)
			mDoTransformation->activate();
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
	delete mWindow;
}

void UserInterface::StoreAnalysisData(void)
{
	mAnalysisSynthesisExample->StoreAnalysis();
}


void UserInterface::DisplayInputSpectrum(void)
{
	mVC.Display ( mVC.eSpectrumIn, mAnalysisSynthesisExample->mSegment.GetFrame( /*(int)*/ mCounter->value() ).GetSpectrum() );

// 	if (mInputSpectrum == NULL)
// 		mInputSpectrum = Attach( "Input Spectrum" , &mAnalysisSynthesisExample->mSegment.GetFrame()[ (int) mCounter->value() ].GetSpectrum(), 0 );
// 	else {
// 		List<Frame>& localFrames = mAnalysisSynthesisExample->mSegment.GetFramesArray();
// 		Frame& frame = localFrames[ mCounter->value() ];
// 		Spectrum& spectrum = frame.GetSpectrum();

// 		mInputSpectrumView.BindTo( spectrum );
// 		mInputSpectrumView.Publish();
// 	}
}

void UserInterface::DisplayInputSound(void)
{
	mVC.Display ( mVC.eAudioIn, mAnalysisSynthesisExample->mAudioIn );

//	if ( mAudioInputDisplay == NULL )
//		mAudioInputDisplay = AttachInputSound( "Audio Input" , &mAnalysisSynthesisExample->mAudioIn );
}

void UserInterface::DisplayOutputSound(void)
{
	mVC.Display ( mVC.eAudioOut, mAnalysisSynthesisExample->mAudioOut );

// 	if ( mAudioOutputDisplay == NULL )
// 		mAudioOutputDisplay = AttachOutputSound( "Audio Output" , &mAnalysisSynthesisExample->mAudioOut );
}

void UserInterface::DisplayOutputSpectrum(void)
{
	mVC.Display ( mVC.eSpectrumOut, mAnalysisSynthesisExample->mSegment.GetFrame( /*(int)*/ mCounter->value() ).GetOutSpec() );

// 	if ( mOutputSpectrum == NULL )
// 		mOutputSpectrum = Attach( "Output Spectrum" , &mAnalysisSynthesisExample->mSegment.GetFramesArray()[ (int) mCounter->value() ].GetSpectrum(), 1 );
}

void UserInterface::DisplayOutputSoundResidual(void)
{
	mVC.Display ( mVC.eAudioResidual, mAnalysisSynthesisExample->mAudioOutRes );

// 	if ( mAudioOutputResidualDisplay == NULL )
// 		mAudioOutputResidualDisplay = AttachSynthSineSound( "Residual" , &mAnalysisSynthesisExample->mAudioOutRes );
}

void UserInterface::DisplayOutputSoundSinusoidal(void)
{
	mVC.Display ( mVC.eAudioSinusoidal, mAnalysisSynthesisExample->mAudioOutSin );
// 	if ( mAudioOutputSinusoidalDisplay == NULL )
// 		mAudioOutputSinusoidalDisplay = AttachSynthResidualSound( "Sinusoidal" , &mAnalysisSynthesisExample->mAudioOutSin );
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

/*
void UserInterface::Detach(Fl_Window *w)
{
	
	mSmartTile->remove( w );
	mSmartTile->redraw();
	w->hide();
	
	if (w==mAudioInputDisplay) 
		mAudioInputDisplay = NULL;
	else if (w==mAudioOutputDisplay)
		mAudioOutputDisplay = NULL;
	else if (w==mAudioOutputResidualDisplay)
		mAudioOutputResidualDisplay = NULL;
	else if (w==mAudioOutputSinusoidalDisplay)
		mAudioOutputSinusoidalDisplay = NULL;
	else if (w==mInputSpectrum) 
		mInputSpectrum = NULL;
	else if (w==mOutputSpectrum)
		mOutputSpectrum = NULL;
	
//	mSmartTile->equalize();
}

Fl_Window* UserInterface::AttachInputSound(const char* title, CLAM::Audio* data )
{
	Fl_Browsable_Playable_Audio* localPresentation;

	mSoundView.BindTo( *data );

	//TODO: Calculate h() in a correct way
	localPresentation = new Fl_Browsable_Playable_Audio( 0, 0, mSmartTile->w(), mSmartTile->h()/(mSmartTile->children()+1), title);
	localPresentation->AttachTo( mSoundView );

	localPresentation->setAudioPlayer( new AudioPlayer( *data ) );

	//Link Signals with Slots
	mFrameSignal.Connect( *localPresentation->GetFrameSlot() );
	mPaintSignal.Connect( *localPresentation->GetPaintSlot() );
	localPresentation->GetSignal()->Connect( mSlot );

 	if (mAnalysisSynthesisExample->mHaveAnalysis)
 		localPresentation->SetPainting( true );
	
	// registering callback for notifying the ui the need of 'detaching'
	localPresentation->callback( (Fl_Callback*)_Detach, this );
	mSoundView.Publish();

	mSmartTile->add( localPresentation );
	localPresentation->Show();
	mSmartTile->redraw();

	return localPresentation;
}

Fl_Window* UserInterface::AttachOutputSound(const char* title, CLAM::Audio* data )
{
	AudioBrowser* localPresentation;

	mSynthesizedOutput.BindTo( *data );

	//TODO: Calculate h() in a correct way
	localPresentation = new AudioBrowser( 0, 0, mSmartTile->w(), mSmartTile->h()/(mSmartTile->children()+1), title);
	localPresentation->AttachTo( mSynthesizedOutput );

	//Link Signals with Slots
	mFrameSignal.Connect( *localPresentation->GetFrameSlot() );
	mPaintSignal.Connect( *localPresentation->GetPaintSlot() );
	localPresentation->GetSignal()->Connect( mSlot );

 	if (mAnalysisSynthesisExample->mHaveAnalysis)
 		localPresentation->setPainting( true );
	
	// registering callback for notifying the ui the need of 'detaching'
	localPresentation->callback( (Fl_Callback*)_Detach, this );
	mSynthesizedOutput.Publish();

	mSmartTile->add( localPresentation );
	localPresentation->Show();
	mSmartTile->redraw();

	return localPresentation;
}

Fl_Window* UserInterface::AttachSynthSineSound(const char* title, CLAM::Audio* data )
{
	AudioBrowser* localPresentation;

	mSynthResidualOut.BindTo( *data );

	//TODO: Calculate h() in a correct way
	localPresentation = new AudioBrowser( 0, 0, mSmartTile->w(), mSmartTile->h()/(mSmartTile->children()+1), title);
	localPresentation->AttachTo( mSynthSineOut );

	//Link Signals with Slots
	mFrameSignal.Connect( *localPresentation->GetFrameSlot() );
	mPaintSignal.Connect( *localPresentation->GetPaintSlot() );
	localPresentation->GetSignal()->Connect( mSlot );

 	if (mAnalysisSynthesisExample->mHaveAnalysis)
 		localPresentation->setPainting( true );
	
	// registering callback for notifying the ui the need of 'detaching'
	localPresentation->callback( (Fl_Callback*)_Detach, this );
	mSynthResidualOut.Publish();

	mSmartTile->add( localPresentation );
	localPresentation->Show();
	mSmartTile->redraw();

	return localPresentation;
}

Fl_Window* UserInterface::AttachSynthResidualSound(const char* title, CLAM::Audio* data )
{
	AudioBrowser* localPresentation;

	mSynthResidualOut.BindTo( *data );

	//TODO: Calculate h() in a correct way
	localPresentation = new AudioBrowser( 0, 0, mSmartTile->w(), mSmartTile->h()/(mSmartTile->children()+1), title);
	localPresentation->AttachTo( mSynthResidualOut );

	//Link Signals with Slots
	mFrameSignal.Connect( *localPresentation->GetFrameSlot() );
	mPaintSignal.Connect( *localPresentation->GetPaintSlot() );
	localPresentation->GetSignal()->Connect( mSlot );

 	if (mAnalysisSynthesisExample->mHaveAnalysis)
 		localPresentation->setPainting( true );
	
	// registering callback for notifying the ui the need of 'detaching'
	localPresentation->callback( (Fl_Callback*)_Detach, this );
	mSynthResidualOut.Publish();

	mSmartTile->add( localPresentation );
	localPresentation->Show();
	mSmartTile->redraw(); 

	return localPresentation;
}


Fl_Window* UserInterface::Attach(const char* title, CLAM::Spectrum* data, int type )
{
	//TODO: Calculate h() in a correct way
	Geometry g(0, 0, mSmartTile->w(), mSmartTile->h()/(mSmartTile->children()+1));


	LogMagSpectrumAdapter* selectedView;

	if( type == 0 )
		selectedView = &mInputSpectrumView;
	else
		selectedView = &mOutputSpectrumView;		


	SpectrumDisplay *localPresentation = 
		new SpectrumDisplay(g.GetX(), g.GetY(), g.GetW(), g.GetH(), title);

	// registering callback for notifying the ui the need of 'detaching'
	localPresentation->callback( (Fl_Callback*)_Detach, this );

	selectedView->BindTo( *data );
	localPresentation->AttachTo( *selectedView );
	selectedView->Publish();
	
	mSmartTile->add( localPresentation );
	localPresentation->Show();
	mSmartTile->redraw();
	
	return localPresentation;
}

void UserInterface::Init()
{
//	mAudioInputDisplay=NULL; 
//	mAudioOutputDisplay=NULL; 
//	mAudioOutputResidualDisplay=NULL; 
//	mAudioOutputSinusoidalDisplay=NULL;
//	mInputSpectrum=NULL;
//	mOutputSpectrum=NULL;
}
*/
void UserInterface::ChangeFrame()
{
	DisplayInputSound();
	DisplayInputSpectrum();

	int nframe = (int) mCounter->value();

	List<Frame>& localFrames = mAnalysisSynthesisExample->mSegment.GetFramesArray();
	
	TData nextcursorpos = localFrames[ nframe ].GetCenterTime();
	
	//Notify SigSlotted class to change
	mVC.mFrameSignal.Emit( nextcursorpos );

	//Change Spectrum Displayer
	// InputSpectrum Displayer is yet changed
		
//	mOutputSpectrumView.BindTo( mAnalysisSynthesisExample->mSegment.GetFramesArray()[ nframe ].GetSpectrum() );
//	mOutputSpectrumView.Publish();
//		mSmartTile->equalize();

	Fl::redraw();
}

void UserInterface::ChangeTimeTag( double tag )
{
	//Change mCounter
	TTime time(tag);
	TIndex nframe = mAnalysisSynthesisExample->mSegment.FindFrame( time );

	mCounter->value( (int) nframe );

	//Notify other GLPorts
	TData nextcursorpos = mAnalysisSynthesisExample->mSegment.GetFramesArray()[ nframe ].GetCenterTime();
	mVC.mFrameSignal.Emit( nextcursorpos );

	//Change Spectrum Displayer
	mVC.Display ( mVC.eSpectrumIn, mAnalysisSynthesisExample->mSegment.GetFrame( /*(int)*/ nframe ).GetSpectrum() );

//	mVC.mSpectrumAdapters[ mVC::eSpectrumIn ].BindTo( mAnalysisSynthesisExample->mSegment.GetFramesArray()[ nframe ].GetSpectrum() );
//	mVC.mSpectrumAdapters[ mVC::eSpectrumIn ].Publish();

//		mSmartTile->equalize();
//	mOutputSpectrumView.BindTo( mAnalysisSynthesisExample->mSegment.GetFramesArray()[ nframe ].GetSpectrum() );
//	mOutputSpectrumView.Publish();
//		mSmartTile->equalize();

	Fl::redraw();
}
