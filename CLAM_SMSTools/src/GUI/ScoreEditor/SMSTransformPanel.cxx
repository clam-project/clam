#include "SMSTransformPanel.hxx"
#include "SMSTransformationConfig.hxx"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Select_Browser.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include "Factory.hxx"
#include "SMS_Configurator.hxx"
#include <iostream>

#define FL_DEFINED_UP_BOX FL_UP_BOX
#define FL_DEFINED_DOWN_BOX FL_DOWN_BOX

inline void SMSScoreEditor::cb_mMoveTransUpInScoreButton_i(Fl_Button*, void*) {
	MoveHighlightedUp();
}
void SMSScoreEditor::cb_mMoveTransUpInScoreButton(Fl_Button* o, void* v) {
	((SMSScoreEditor*)(o->parent()->parent()->user_data()))->cb_mMoveTransUpInScoreButton_i(o,v);
}

inline void SMSScoreEditor::cb_mMoveTransDownInScoreButton_i(Fl_Button*, void*) {
	MoveHighlightedDown();
}
void SMSScoreEditor::cb_mMoveTransDownInScoreButton(Fl_Button* o, void* v) {
	((SMSScoreEditor*)(o->parent()->parent()->user_data()))->cb_mMoveTransDownInScoreButton_i(o,v);
}

inline void SMSScoreEditor::cb_mRemoveTransFromScoreButton_i(Fl_Button*, void*) {
	RemoveHighlightedFromScore();
}
void SMSScoreEditor::cb_mRemoveTransFromScoreButton(Fl_Button* o, void* v) {
	((SMSScoreEditor*)(o->parent()->parent()->user_data()))->cb_mRemoveTransFromScoreButton_i(o,v);
}

inline void SMSScoreEditor::cb_mAddTransformToScoreButton_i(Fl_Button*, void*) {
	AddHighlightedToScore();
}
void SMSScoreEditor::cb_mAddTransformToScoreButton(Fl_Button* o, void* v) {
	((SMSScoreEditor*)(o->parent()->parent()->user_data()))->cb_mAddTransformToScoreButton_i(o,v);
}

inline void SMSScoreEditor::cb_mDiscardChangesButton_i(Fl_Button*, void*) {
	DiscardChangesAndClose();
}
void SMSScoreEditor::cb_mDiscardChangesButton(Fl_Button* o, void* v) {
	((SMSScoreEditor*)(o->parent()->user_data()))->cb_mDiscardChangesButton_i(o,v);
}

inline void SMSScoreEditor::cb_mApplyChangesButton_i(Fl_Button*, void*) {
	ApplyChangesAndClose();
}

void SMSScoreEditor::cb_mApplyChangesToCurrentCfg(Fl_Button* o, void* v) {
	((SMSScoreEditor*)v)->cb_mApplyChangesToCurrentCfg_i(o,v);
}

inline void SMSScoreEditor::cb_mApplyChangesToCurrentCfg_i(Fl_Button*, void*) {
	ApplyChangesToCurrentCfg();
}


void SMSScoreEditor::cb_mApplyChangesButton(Fl_Button* o, void* v) {
	((SMSScoreEditor*)(o->parent()->user_data()))->cb_mApplyChangesButton_i(o,v);
}

inline void SMSScoreEditor::cb_mRepositoryBrowser_i(Fl_Select_Browser* b, void*) 
{
	if ( b->value() > b->size() || b->value() < 1 )
	     return;
	mScoreContentsBox->deselect( mHighlightedConfig );
	mHighlightedConfig = 0;
	RemoveCurrentConfiguratorFromTabs();
	ActivateConfigurator( b->value(), mRepositoryBox );
	ShowActiveConfiguratorHelp();
	mTransTabs->redraw();
}

void SMSScoreEditor::cb_mRepositoryBrowser(Fl_Select_Browser* o, void* v) 
{
	((SMSScoreEditor*)v)->cb_mRepositoryBrowser_i(o,v);
}

inline void SMSScoreEditor::cb_mScoreBrowser_i(Fl_Select_Browser* b, void*) 
{
	if ( b->value() > b->size() || b->value() < 1 )
	     return;
	if ( mHighlightedConfig == b->value() )
		return;
	mHighlightedConfig = b->value();
	RemoveCurrentConfiguratorFromTabs();
	ActivateConfigurator( mHighlightedConfig, mScoreContentsBox);
	ShowActiveConfiguratorHelp();
	ShowActiveConfiguratorEditWidget();

	mScoreContentsBox->select( mHighlightedConfig );
	mTransTabs->redraw();

}

void SMSScoreEditor::cb_mScoreBrowser(Fl_Select_Browser* o, void* v) 
{
	((SMSScoreEditor*)v)->cb_mScoreBrowser_i(o,v);
}



SMSScoreEditor::SMSScoreEditor() 
	: mUserChangedSomething( false ),  mpCurrentConfigurator( NULL ), mHighlightedConfig( 0 )
{
	Fl_Window* w;

	{
		Fl_Window* o = mMainWindow = new Fl_Window(890, 475, "SMS - Transformation Score Edition");
		w = o;
		o->box(FL_FLAT_BOX);
		o->user_data((void*)(this));
	
		{ 
			Fl_Group* o = mChainTopologyDock = new Fl_Group(5, 20, 455, 420,"Score");
			o->labelfont(FL_HELVETICA);
			o->labelsize(14);
			o->align(FL_ALIGN_TOP);
			o->box(FL_ENGRAVED_BOX);
			{ 
				Fl_Select_Browser* o = 
					mScoreContentsBox = 
					new Fl_Select_Browser(10, 40, 220, 365, "Transformations in the Score");
        o->labelsize(12);
        o->textsize(12);
				o->callback( (Fl_Callback*)cb_mScoreBrowser, this );
        o->align(FL_ALIGN_TOP);
			}
			{ 
				Fl_Select_Browser* o = 
					mRepositoryBox = 
					new Fl_Select_Browser(235, 40, 220, 365, "Available SMS Transformations");
	      o->labelsize(12);
  	    o->textsize(12);
				o->callback( (Fl_Callback*)cb_mRepositoryBrowser, this );
	      o->align(FL_ALIGN_TOP);
			}
			{ 
				Fl_Button* o = 
					mMoveTransUpInScoreButton = 
					new Fl_Button(10, 410, 65, 25, "Move &up");
        o->tooltip("Move selected transformation up in the score");
        o->box(FL_DEFINED_UP_BOX);
        o->down_box(FL_DEFINED_DOWN_BOX);
        o->shortcut(0x80075);
        o->callback((Fl_Callback*)cb_mMoveTransUpInScoreButton);
	      o->labelsize(12);
			}
			{
				Fl_Button* o = 
					mMoveTransDownInScoreButton = 
					new Fl_Button(80, 410, 85, 25, "Move &down");
        o->tooltip("Move selected transformation down in the score");
        o->box(FL_DEFINED_UP_BOX);
        o->down_box(FL_DEFINED_DOWN_BOX);
        o->shortcut(0x80064);
        o->callback((Fl_Callback*)cb_mMoveTransDownInScoreButton);
	      o->labelsize(12);
			}
			{ Fl_Button* o = 
					mRemoveTransFromScoreButton = 
					new Fl_Button(170, 410, 60, 25, "&Remove");
        o->tooltip("Remove selected transformation from score");
        o->box(FL_DEFINED_UP_BOX);
        o->down_box(FL_DEFINED_DOWN_BOX);
        o->shortcut(0x80072);
        o->callback((Fl_Callback*)cb_mRemoveTransFromScoreButton);
	      o->labelsize(12);
			}
			{
				Fl_Button* o = 
					mAddTransformToScoreButton = 
					new Fl_Button(235, 410, 220, 25, "Add &transformation to score");
        o->tooltip("Adds selected transformation to score");
        o->box(FL_DEFINED_UP_BOX);
        o->down_box(FL_DEFINED_DOWN_BOX);
        o->shortcut(0x80074);
        o->callback((Fl_Callback*)cb_mAddTransformToScoreButton);
	      o->labelsize(12);
			}
			o->end();
		} // Score group
		
		{ 
			Fl_Button* o = 
				mApplyChangesToScoreButton = 
				new Fl_Button(5, 445, 225, 25, "&Apply Changes to Score");
			o->box(FL_DEFINED_UP_BOX);
			o->down_box(FL_DEFINED_DOWN_BOX);
			o->shortcut(0x80061);
			o->callback( (Fl_Callback*)cb_mApplyChangesButton );
      o->labelsize(12);
		}
		{ 
			Fl_Button* o = 
				mDiscardChangesButton = 
				new Fl_Button(235, 445, 225, 25, "&Discard Changes to Score");
			o->box(FL_DEFINED_UP_BOX);
			o->down_box(FL_DEFINED_DOWN_BOX);
			o->shortcut(0x80064);
			o->callback((Fl_Callback*)cb_mDiscardChangesButton);
		}

		{
			Fl_Group* o = 
				mTransParmDock = 
				new Fl_Group(465, 20, 420, 420, "Transformation");
			o->labelfont(FL_HELVETICA);
			o->labelsize(14);
			o->align(FL_ALIGN_TOP);
			o->box(FL_ENGRAVED_BOX);
		
			{ 
				Fl_Tabs* o = mTransTabs = new Fl_Tabs(470, 25, 410, 410);
        o->box(FL_DEFINED_UP_BOX);
        o->labelsize(12);

				{
					Fl_Group* o = 
						mHelpWidgetContainer = 
						new Fl_Group( mTransTabs->x()+5, mTransTabs->y()+30, mTransTabs->w()-10, mTransTabs->h()-35 );
					o->label( "Description" );
					o->labelsize( 12 );
					o->box( FL_FLAT_BOX );
					o->end();
				}
				{
					Fl_Group* o = 
						mConfigWidgetContainer = 
						new Fl_Group( mTransTabs->x()+5, mTransTabs->y()+30, mTransTabs->w()-10, mTransTabs->h()-35 );
					o->label( "Parameters" );
					o->labelsize( 12 );
					o->box( FL_FLAT_BOX );
					o->end();
					o->deactivate();
				}

        o->end();
			} // tabs
			o->end();
		} // Transformation Group

		{ 
			Fl_Button* o = 
				mApplyChangesToCurrentCfg = 
				new Fl_Button(580, 445, 150, 25, "Apply configuration");
			o->tooltip("Applies configuration to the selected transformation");
			o->box(FL_DEFINED_UP_BOX);
			o->down_box(FL_DEFINED_DOWN_BOX);
			o->callback( (Fl_Callback*) cb_mApplyChangesToCurrentCfg, this );
			o->deactivate();
      o->labelsize(12);
		}
		{ 
			Fl_Button* o = 
				mDiscardConfigButton = 
				new Fl_Button(735, 445, 150, 25, "Discard configuration");
      o->box(FL_DEFINED_UP_BOX);
      o->down_box(FL_DEFINED_DOWN_BOX);
      o->deactivate();
      o->labelsize(12);
		}
	
		o->set_modal();
		o->end();
	}

	{
		mNoConfigWidgetAvailable = new Fl_Box( 0, 0, 100, 100 );
		mNoConfigWidgetAvailable->label( "Non-editable configuration" );
		mNoConfigWidgetAvailable->align( FL_ALIGN_INSIDE );
	}

	SetTransformationScore.Wrap( this, &SMSScoreEditor::OnSetTransformationScore );

	ResetChainConfig();
	mPreviousChainConfig = mChainConfig;
}

void SMSScoreEditor::OnSetTransformationScore( const CLAM::SMSTransformationChainConfig& cfg ) 
{
	mChainConfig = cfg;
}

SMSScoreEditor::~SMSScoreEditor()
{
	if ( mNoConfigWidgetAvailable->parent() == NULL )
		delete mNoConfigWidgetAvailable;
}

void SMSScoreEditor::Show( ) 
{
	ShowScoreOnBrowser();
	ShowFactoryProductsOnBrowser();
	mMainWindow->show();
}

void SMSScoreEditor::Hide( ) 
{
	mUserChangedSomething = false;
	mHighlightedConfig = 0;
	RemoveCurrentConfiguratorFromTabs();
	mMainWindow->hide();
}

void SMSScoreEditor::ShowActiveConfiguratorHelp()
{
	CLAM_ASSERT( mpCurrentConfigurator->GetHelpWidget()!=NULL, "Configurator did not provide help widget" );

	mHelpWidgetContainer->add( mpCurrentConfigurator->GetHelpWidget() );
	mpCurrentConfigurator->GetHelpWidget()->resize( mHelpWidgetContainer->x(), mHelpWidgetContainer->y()+5,
							mHelpWidgetContainer->w(), mHelpWidgetContainer->h()-5 );
	mpCurrentConfigurator->GetHelpWidget()->show();
	//mTransTabs->value( mpCurrentConfigurator->GetHelpWidget() );
	mHelpWidgetContainer->activate();
	mTransTabs->value( mHelpWidgetContainer );
}

void SMSScoreEditor::ShowActiveConfiguratorEditWidget()
{
	Fl_Widget* configWidget = mpCurrentConfigurator->GetParametersWidget();
	mApplyChangesToCurrentCfg->activate();	

	if ( !configWidget ) // if no configuration widget then show the fallback		
	{
		configWidget = mNoConfigWidgetAvailable;
		mApplyChangesToCurrentCfg->deactivate();	
	}

	mConfigWidgetContainer->add( configWidget );
	configWidget->resize( mConfigWidgetContainer->x(), mConfigWidgetContainer->y()+5,
			      mConfigWidgetContainer->w(), mConfigWidgetContainer->h()-5 );

	mConfigWidgetContainer->activate();
	configWidget->show();
	mTransTabs->value( mConfigWidgetContainer );

}



void SMSScoreEditor::ActivateConfigurator( int index, Fl_Select_Browser* browser  )
{

	RemoveCurrentConfiguratorFromTabs();

	
	mpCurrentConfigurator = (CLAMVM::SMSConfigurator*)browser->data( index );

}

void SMSScoreEditor::ShowFactoryProductsOnBrowser()
{
	mRepositoryBox->clear();
	std::list< std::string > availableTransformationsList;

	CLAMVM::SMSConfiguratorFactory::GetInstance().GetRegisteredNames( availableTransformationsList );

	std::list< std::string >::const_iterator i = availableTransformationsList.begin();

 	for ( ; i != availableTransformationsList.end(); i++ )
	{
		mRepositoryBox->add( i->c_str() );
		mRepositoryBox->data( mRepositoryBox->size(), 
				      CLAMVM::SMSConfiguratorFactory::GetInstance().Create( i->c_str() ) );
	}

}

void SMSScoreEditor::ShowScoreOnBrowser( )
{
	mScoreContentsBox->clear();
	CLAM::SMSTransformationChainConfig::const_iterator i = mChainConfig.ConfigList_begin_const();

	i++;
	for ( int k = 1; k < mChainConfig.ConfigList_size()-1; k++ )
	{
		if ( mChainConfig.GetOnArray()[k] )
		{
			mScoreContentsBox->add( i->GetConcreteClassName().c_str() );
		}
		else
		{
			std::string italicToken = "@i";
			italicToken += i->GetConcreteClassName().c_str();
			mScoreContentsBox->add( italicToken.c_str() );
		}
		CLAMVM::SMSConfigurator* pCfg = CLAMVM::SMSConfiguratorFactory::GetInstance().Create( i->GetConcreteClassName().c_str() );
		pCfg->SetConfig( i->GetConcreteConfig() );
		mScoreContentsBox->data( mScoreContentsBox->size(), pCfg );
		i++;		
	}

}

void SMSScoreEditor::ResetChainConfig()
{
	mChainConfig.GetConfigurations().clear();
	mChainConfig.GetOnArray().Resize(0);
	mChainConfig.GetOnArray().SetSize(0);

	CLAM::ProcessingChaineeConfig cfg;
	cfg.SetConcreteClassName( "SMSTransformationChainIO" );
	cfg.AddConcreteConfig();

	mChainConfig.GetConfigurations().push_back(  cfg  );
	mChainConfig.GetConfigurations().push_back(  cfg );
	mChainConfig.GetOnArray().Resize( 2 );
	mChainConfig.GetOnArray().SetSize( 2 );
	mChainConfig.GetOnArray()[0] = mChainConfig.GetOnArray()[1] = true;

}

void SMSScoreEditor::DiscardChangesAndClose() 
{
	if ( !mUserChangedSomething )
	{
		Hide();
		return;
	}
	int answer = fl_ask( "Are you sure to discard score changes?" );

	
	if ( answer )
	{
		mChainConfig = mPreviousChainConfig;
		Hide();
	}

}

void SMSScoreEditor::ApplyChangesAndClose()
{
	if ( !mUserChangedSomething )
	{
		fl_message( "The score has not been changed" );
		Hide();
		return;
	}

	int answer = fl_ask( "Are you sure to apply score changes?" );
	
	if ( answer ) 
	{
		mPreviousChainConfig = mChainConfig;
		TransformationChainChanged.Emit( mChainConfig );
		Hide();
	}
}

void SMSScoreEditor::AddHighlightedToScore( )
{
	if ( mRepositoryBox->size() == 0 )
		return;

	int insertedTransformation = mRepositoryBox->value();

	mScoreContentsBox->add( mRepositoryBox->text( insertedTransformation ) ); 
	std::string key = mRepositoryBox->text( insertedTransformation );
	CLAMVM::SMSConfigurator* pCfg = CLAMVM::SMSConfiguratorFactory::GetInstance().Create( key );
	mScoreContentsBox->data( mScoreContentsBox->size(), pCfg );


	CLAM::ProcessingChaineeConfig cfg;
	cfg.SetConcreteClassName( mRepositoryBox->text( insertedTransformation ) );
	cfg.AddConcreteConfig();
	pCfg->SetConfig( cfg.GetConcreteConfig() );

	CLAM::SMSTransformationChainConfig::iterator i = mChainConfig.ConfigList_begin();
	std::advance( i, mChainConfig.ConfigList_size()-1 );
	mChainConfig.GetConfigurations().insert( i, cfg );
	//mChainConfig.GetConfigurations().push_back( cfg );
	mChainConfig.GetOnArray().AddElem( 1 );
	mHighlightedConfig = mScoreContentsBox->size();

	
	ActivateConfigurator( mHighlightedConfig, mScoreContentsBox );
	ShowActiveConfiguratorHelp();
	ShowActiveConfiguratorEditWidget();


	mScoreContentsBox->select( mHighlightedConfig );
	mUserChangedSomething = true;

}

void SMSScoreEditor::RemoveCurrentConfiguratorFromTabs()
{
	if ( !mpCurrentConfigurator )
		return;

	mpCurrentConfigurator->GetHelpWidget()->hide();
	mHelpWidgetContainer->remove(*(mpCurrentConfigurator->GetHelpWidget() ) );
	
	if ( mpCurrentConfigurator->GetParametersWidget() )
	{
		mApplyChangesToCurrentCfg->deactivate();
		mpCurrentConfigurator->GetParametersWidget()->hide();
		mConfigWidgetContainer->remove(*(mpCurrentConfigurator->GetParametersWidget() ) );		
	}
	else
	{
		if ( mNoConfigWidgetAvailable->visible() )
			mNoConfigWidgetAvailable->hide();
		if ( mNoConfigWidgetAvailable->parent() == mConfigWidgetContainer )
			mConfigWidgetContainer->remove(*(mNoConfigWidgetAvailable) );		

	}
	
	mHelpWidgetContainer->deactivate();
	mConfigWidgetContainer->deactivate();
	mTransTabs->redraw();

	mpCurrentConfigurator = NULL;
}

void SMSScoreEditor::RemoveHighlightedFromScore( )
{
	int removedTransformation = mScoreContentsBox->value();
	CLAMVM::SMSConfigurator* cfg = (CLAMVM::SMSConfigurator*)mScoreContentsBox->data( removedTransformation );
	if ( cfg == mpCurrentConfigurator )
		RemoveCurrentConfiguratorFromTabs();
	delete cfg;
	mScoreContentsBox->data( removedTransformation, NULL );
	mScoreContentsBox->remove(  removedTransformation );

	CLAM::SMSTransformationChainConfig::iterator it = mChainConfig.ConfigList_begin();

	std::advance( it, removedTransformation );

	mChainConfig.GetConfigurations().erase( it );//mChainConfig.GetConfigurations().begin()+removedTransformation );
	mChainConfig.GetOnArray().DeleteElem( removedTransformation );
	mUserChangedSomething = true;
	mHighlightedConfig = 0;
}

void SMSScoreEditor::MoveHighlightedDown()
{
	int destination = mScoreContentsBox->value()+1;
	int source = mScoreContentsBox->value();

	if ( destination > mScoreContentsBox->size() )
		return;

	mScoreContentsBox->move( destination,  source );

	CLAM::ProcessingChaineeConfig tmpCfg;
	bool tmpFlag;

	CLAM::SMSTransformationChainConfig::iterator itSrc = mChainConfig.ConfigList_begin();
	std::advance( itSrc, source );
	CLAM::SMSTransformationChainConfig::iterator itDst = mChainConfig.ConfigList_begin();
	std::advance( itDst, destination );
	

	tmpCfg.SetConcreteClassName( itSrc->GetConcreteClassName() );
	tmpCfg.SetConcreteConfig( itSrc->GetConcreteConfig() );

	itSrc->SetConcreteClassName( itDst->GetConcreteClassName() );
	itSrc->SetConcreteConfig( itDst->GetConcreteConfig() );

	itDst->SetConcreteClassName( tmpCfg.GetConcreteClassName() );
	itDst->SetConcreteConfig( tmpCfg.GetConcreteConfig() );

	tmpFlag = mChainConfig.GetOnArray()[source];
	mChainConfig.GetOnArray()[source] = mChainConfig.GetOnArray()[destination];
	mChainConfig.GetOnArray()[destination] = tmpFlag;

	mScoreContentsBox->select( destination );
	mUserChangedSomething = true;	
	mHighlightedConfig++;
}

void SMSScoreEditor::MoveHighlightedUp() 
{
	int destination = mScoreContentsBox->value()-1;
	int source = mScoreContentsBox->value();

	if ( destination < 1 )
		return;

	mScoreContentsBox->move( destination, source );

	CLAM::ProcessingChaineeConfig tmpCfg;
	bool tmpFlag;

	CLAM::SMSTransformationChainConfig::iterator itSrc = mChainConfig.ConfigList_begin();
	std::advance( itSrc, source );
	CLAM::SMSTransformationChainConfig::iterator itDst = mChainConfig.ConfigList_begin();
	std::advance( itDst, destination );
	

	tmpCfg.SetConcreteClassName( itSrc->GetConcreteClassName() );
	tmpCfg.SetConcreteConfig( itSrc->GetConcreteConfig() );

	itSrc->SetConcreteClassName( itDst->GetConcreteClassName() );
	itSrc->SetConcreteConfig( itDst->GetConcreteConfig() );

	itDst->SetConcreteClassName( tmpCfg.GetConcreteClassName() );
	itDst->SetConcreteConfig( tmpCfg.GetConcreteConfig() );

	tmpFlag = mChainConfig.GetOnArray()[source];
	mChainConfig.GetOnArray()[source] = mChainConfig.GetOnArray()[destination];
	mChainConfig.GetOnArray()[destination] = tmpFlag;


	mScoreContentsBox->select( destination );
	mHighlightedConfig--;
	mUserChangedSomething = true;
}

void SMSScoreEditor::ApplyChangesToCurrentCfg()
{
	CLAM::SMSTransformationChainConfig::iterator it = mChainConfig.ConfigList_begin();
	CLAM_ASSERT( mHighlightedConfig >= 1, "mHighlightedConfig has an invalid value!" );
	CLAM_ASSERT( mHighlightedConfig <= mScoreContentsBox->size(), "mHighlightedConfig has an invalid value!" );
	CLAM_ASSERT( mHighlightedConfig < mChainConfig.ConfigList_size(), "mHighlightedConfig has an invalid value!" );
	CLAM_ASSERT( mpCurrentConfigurator!=NULL, "No configurator active!" );
	std::advance( it, mHighlightedConfig );
	
	it->SetConcreteConfig( mpCurrentConfigurator->GetConfig() );

	mScoreContentsBox->select( mHighlightedConfig );

	mUserChangedSomething = true;

	fl_message("Score has been changed");
}
