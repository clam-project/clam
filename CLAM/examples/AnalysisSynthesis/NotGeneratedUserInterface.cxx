#include "UserInterface.hxx"
#include "AnalysisSynthesisExampleGUI.hxx"
#include <FL/fl_file_chooser.H>
#include <FL/Fl.H>
#include "FLTKConfigurator.hxx"
#include "DebugSnapshots.hxx"
#include "AudioSnapshot.hxx"

using namespace CLAM;
using namespace CLAMGUI;

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


