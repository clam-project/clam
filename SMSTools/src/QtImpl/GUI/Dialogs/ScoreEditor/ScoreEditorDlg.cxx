#include <algorithm>
#include <QLayout>
#include <QListWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include "SMSConfigurator.hxx"
#include "ScoreEditorDlg.hxx"

namespace QtSMS
{
	ScoreEditorDlg::ScoreEditorDlg(QWidget* parent)
		: QDialog(parent)
		, mIsShowed(false)
		, mScoreChanged(false)
	{
		setupUi(this);
		InitScoreEditor();
		// Connections previously declared in the .ui file; wired here so the
		// typed-pointer connect() form resolves to ScoreEditorDlg slots
		// instead of QDialog (uic targets the form's base class). The Qt3
		// Q3ListBox::highlighted(QString) maps to QListWidget::currentTextChanged,
		// and highlighted(int) maps to currentRowChanged.
		connect(mOkBtn,         &QPushButton::clicked, this, &ScoreEditorDlg::accept);
		connect(mCancelBtn,     &QPushButton::clicked, this, &ScoreEditorDlg::reject);
		connect(mAddToScoreBtn, &QPushButton::clicked, this, &ScoreEditorDlg::addHighlightedToScore);
		connect(mMoveUpBtn,     &QPushButton::clicked, this, &ScoreEditorDlg::moveHighlightedUp);
		connect(mMoveDownBtn,   &QPushButton::clicked, this, &ScoreEditorDlg::moveHighlightedDown);
		connect(mRemoveBtn,     &QPushButton::clicked, this, &ScoreEditorDlg::removeHighlightedFromScore);
		connect(mAvailableTransformationList, &QListWidget::currentTextChanged, this, &ScoreEditorDlg::onSelectAvailableTransformation);
		connect(mScoreTransformationList,     &QListWidget::currentRowChanged,  this, &ScoreEditorDlg::onSelectTransformationInScore);
	}

	ScoreEditorDlg::~ScoreEditorDlg()
	{
		mHelpWidgetTable.clear();
		mSMSConfiguratorList.clear();
	}

    bool ScoreEditorDlg::Apply()
	{
		if(!mScoreChanged || !mSMSConfiguratorList.size()) return false;

		// first we insert the TransformationChainIO's at the beginning and at the end
		mTransformationChainCfg.GetConfigurations().clear();
		mTransformationChainCfg.GetOnArray().Resize(0);
		mTransformationChainCfg.GetOnArray().SetSize(0);

		CLAM::SMSTransformationChaineeConfig cfg;
		cfg.SetConcreteClassName("SMSTransformationChainIO");
		cfg.AddConcreteConfig();

		mTransformationChainCfg.GetConfigurations().push_back(cfg);
		mTransformationChainCfg.GetConfigurations().push_back(cfg);
		mTransformationChainCfg.GetOnArray().Resize(2);
		mTransformationChainCfg.GetOnArray().SetSize(2);
		mTransformationChainCfg.GetOnArray()[0] = mTransformationChainCfg.GetOnArray()[1] = true;

		for(unsigned i=0; i < mSMSConfiguratorList.size(); i++)
		{
			CLAM::SMSTransformationChaineeConfig cfg;
			cfg.SetConcreteClassName(mScoreTransformationList->item(i)->text().toStdString());
			cfg.SetConcreteConfig(mSMSConfiguratorList[i]->GetConfig());

			CLAM::SMSTransformationChainConfig::iterator it = mTransformationChainCfg.ConfigList_begin();
			// we insert the new configuration just before the 'ending' SMSTransformationChainIO
			std::advance(it, mTransformationChainCfg.ConfigList_size()-1);
			mTransformationChainCfg.GetConfigurations().insert(it, cfg);
			mTransformationChainCfg.GetOnArray().AddElem(1);
		}
		return true;
	}

	void ScoreEditorDlg::SetTransformationScore(const CLAM::SMSTransformationChainConfig& cfg)
	{
		mScoreChanged=false;
		CLAM::SMSTransformationChainConfig::const_iterator it = cfg.ConfigList_begin_const();
		it++;
		for(unsigned i=1; i < cfg.ConfigList_size()-1; i++, it++)
		{
			mScoreTransformationList->addItem(it->GetConcreteClassName().c_str());
			QtSMS::SMSConfigurator* pCfg = QtSMS::SMSConfiguratorFactory::GetInstance().Create(it->GetConcreteClassName().c_str());
			pCfg->SetConfig(it->GetConcreteConfig());
			mSMSConfiguratorList.push_back(pCfg);
			QWidget* paramsWidget = mSMSConfiguratorList[i-1]->GetParametersWidget();
			paramsWidget->setParent(mTabWidget->widget(1));
			paramsWidget->move(0, 0);
			mTabPage1Layout->addWidget(paramsWidget);
			paramsWidget->hide();
			connect(mSMSConfiguratorList[i-1],SIGNAL(configurationChanged()),SLOT(scoreChanged()));
		}
		HideAll();
		mAvailableTransformationList->clearSelection();
		mScoreTransformationList->clearSelection();
	}

	const CLAM::SMSTransformationChainConfig& ScoreEditorDlg::GetTransformationChain() const
	{
		return mTransformationChainCfg;
	}

	void ScoreEditorDlg::onSelectAvailableTransformation(const QString& str)
	{
		if(!mIsShowed)
		{
			mAvailableTransformationList->clearSelection();
			mIsShowed=true;
			return;
		}
		HideAll();
		mScoreTransformationList->clearSelection();
		mTabWidget->setCurrentWidget(mTabWidget->widget(0));
		mHelpWidgetTable[str.toStdString()]->show();
	}

	void ScoreEditorDlg::onSelectTransformationInScore(int index)
	{
		HideAll();
		mAvailableTransformationList->clearSelection();
		mTabWidget->setCurrentWidget(mTabWidget->widget(1));
		mSMSConfiguratorList[index]->GetParametersWidget()->show();
		mHelpWidgetTable[mScoreTransformationList->item(index)->text().toStdString()]->show();
	}

	void ScoreEditorDlg::addHighlightedToScore()
	{
		QString str = mAvailableTransformationList->currentItem()->text();
		mScoreTransformationList->addItem(str);
		mSMSConfiguratorList.push_back(QtSMS::SMSConfiguratorFactory::GetInstance().Create(str.toStdString()));
		QWidget* paramsWidget = mSMSConfiguratorList.back()->GetParametersWidget();
		paramsWidget->setParent(mTabWidget->widget(1));
		paramsWidget->move(0, 0);
		mTabPage1Layout->addWidget(paramsWidget);
		mScoreTransformationList->setCurrentRow(int(mSMSConfiguratorList.size()) - 1);

		scoreChanged();
	}

	void ScoreEditorDlg::removeHighlightedFromScore()
	{
		if(!mScoreTransformationList->count()) return;
		int index = mScoreTransformationList->currentRow();
		if(mScoreTransformationList->count()==1)
		{
			mScoreTransformationList->clear();
		}
		else
		{
			delete mScoreTransformationList->takeItem(index);
		}
		HideAll();
		SMSConfiguratorList::iterator pos = std::find(mSMSConfiguratorList.begin(),
													  mSMSConfiguratorList.end(),
													  mSMSConfiguratorList[index]);
		mTabPage1Layout->removeWidget(mSMSConfiguratorList[index]->GetParametersWidget());
		delete mSMSConfiguratorList[index];
		mSMSConfiguratorList.erase(pos);
		if(mScoreTransformationList->count())
		{
			mScoreTransformationList->setCurrentRow(mScoreTransformationList->currentRow());
		}
		scoreChanged();
	}

	void ScoreEditorDlg::moveHighlightedUp()
	{
		int source = mScoreTransformationList->currentRow();
		if(source <= 0) return;
		SwapTransformations(source,source-1);
	}

	void ScoreEditorDlg::moveHighlightedDown()
	{
		int source = mScoreTransformationList->currentRow();
		if(source == mScoreTransformationList->count()-1) return;
		SwapTransformations(source,source+1);
	}

	void ScoreEditorDlg::scoreChanged()
	{
		if(!mScoreChanged) mScoreChanged=true;
	}

	void ScoreEditorDlg::GetAvailableTransformations()
	{
		mHelpWidgetTable.clear();
		mAvailableTransformationList->clear();
		
		std::list<std::string> availableTransformationsList;
		QtSMS::SMSConfiguratorFactory::GetInstance().GetRegisteredNames(availableTransformationsList);
		std::list<std::string>::const_iterator it = availableTransformationsList.begin();
		for(; it != availableTransformationsList.end(); it++)
		{
			mAvailableTransformationList->addItem(it->c_str());
			QWidget* helpWidget = QtSMS::SMSConfiguratorFactory::GetInstance().Create(it->c_str())->GetHelpWidget();
			mHelpWidgetTable[(*it)] = helpWidget;
			helpWidget->setParent(mTabWidget->widget(0));
			helpWidget->move(0, 0);
			mTabPage0Layout->addWidget(helpWidget);
			helpWidget->hide();
		}
	}

	void ScoreEditorDlg::SwapTransformations(int source, int dest)
	{
		QString sourceText = mScoreTransformationList->item(source)->text();
		QString destText = mScoreTransformationList->item(dest)->text();
		mScoreTransformationList->item(source)->setText(destText);
		mScoreTransformationList->item(dest)->setText(sourceText);
		SMSConfigurator* cfg = mSMSConfiguratorList[source];
		mSMSConfiguratorList[source]=mSMSConfiguratorList[dest];
		mSMSConfiguratorList[dest]=cfg;
		onSelectTransformationInScore(dest);
		scoreChanged();
	}

	void ScoreEditorDlg::HideHelpWidgets()
	{
		std::map<std::string,QWidget*>::const_iterator it = mHelpWidgetTable.begin();
		for(; it != mHelpWidgetTable.end(); it++)
		{
			it->second->hide();
		}
	}

	void ScoreEditorDlg::HideAll()
	{
		HideHelpWidgets();
		for(unsigned i=0; i < mSMSConfiguratorList.size(); i++)
		{
			mSMSConfiguratorList[i]->GetParametersWidget()->hide();
		}
	}

	void ScoreEditorDlg::InitScoreEditor()
	{
		mTabPage0Layout = new QVBoxLayout(mTabWidget->widget(0));
		mTabPage1Layout = new QVBoxLayout(mTabWidget->widget(1));
		GetAvailableTransformations();
	}
}

// END

