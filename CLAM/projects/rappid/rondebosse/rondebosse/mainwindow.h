/****************************************************************************
** Form interface generated from reading ui file './mainwindow.ui'
**
** Created: Mon Oct 14 16:51:53 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class KLed;
class QButtonGroup;
class QCheckBox;
class QFrame;
class QGroupBox;
class QLCDNumber;
class QLabel;
class QProgressBar;
class QPushButton;
class QRadioButton;
class QSlider;
class QTabWidget;
class QWidget;

class MainWindow : public QDialog
{ 
    Q_OBJECT

public:
    MainWindow( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~MainWindow();

    QFrame* Frame3;
    QLabel* TextLabel1;
    QLabel* StatusLabel;
    QFrame* Frame13;
    QPushButton* btnFileSave;
    QFrame* Frame23;
    QPushButton* btnExit;
    QPushButton* btnFileOpen;
    QPushButton* btnStart;
    QFrame* Frame23_3;
    QPushButton* btnStop;
    QPushButton* btnHelp;
    QFrame* Frame23_2;
    QTabWidget* TabWidget2;
    QWidget* tab;
    QFrame* Frame7;
    QGroupBox* GroupBox1_4;
    QLCDNumber* LCDLeft4;
    QSlider* sldLeft4;
    QGroupBox* GroupBox1_3;
    QLCDNumber* LCDLeft3;
    QSlider* sldLeft3;
    QGroupBox* GroupBox1_2;
    QSlider* sldLeft2;
    QLCDNumber* LCDLeft2;
    QGroupBox* GroupBox1;
    QSlider* sldLeft1;
    QLCDNumber* LCDLeft1;
    QFrame* Frame7_2;
    QGroupBox* GroupBox1_4_2;
    QSlider* sldRight4;
    QLCDNumber* LCDRight4;
    QGroupBox* GroupBox1_3_2;
    QSlider* sldRight3;
    QLCDNumber* LCDRight3;
    QGroupBox* GroupBox1_2_2;
    QSlider* sldRight2;
    QLCDNumber* LCDRight2;
    QGroupBox* GroupBox1_5;
    QLCDNumber* LCDRight1;
    QSlider* sldRight1;
    QFrame* Frame8_2;
    QLabel* TextLabel1_2_2;
    QProgressBar* ProgressBarRight;
    QGroupBox* GroupBox18;
    QSlider* sldControl;
    QFrame* Frame8;
    QProgressBar* ProgressBarLeft;
    QLabel* TextLabel1_2;
    QFrame* Frame6;
    QSlider* sldConfig;
    QLabel* TextLabel1_3;
    KLed* LedConfig;
    QLCDNumber* LCDConfig;
    QPushButton* btnStore;
    QPushButton* btnRecall;
    QFrame* Frame26;
    QWidget* tab_2;
    QButtonGroup* ButtonGroup1;
    QRadioButton* rbRealTimeLeft;
    QRadioButton* rbWaveFileLeft;
    QRadioButton* rbSynthLeft;
    QPushButton* btnFileOpenLeft;
    QButtonGroup* ButtonGroup1_2;
    QRadioButton* rbSynthRight;
    QRadioButton* rbWaveFileRight;
    QRadioButton* rbRealTimeRight;
    QPushButton* btnFileOpenRight;
    QButtonGroup* ButtonGroup1_2_2;
    QCheckBox* chbWaveFile;
    QCheckBox* chbRealTime;
    QPushButton* btnFileSaveOutput;


public slots:
    virtual void OnbtnExitClick();
    virtual void OnbtnFileOpenClick();
    virtual void OnbtnFileOpenLeftClick();
    virtual void OnbtnFileOpenRightClick();
    virtual void OnbtnFileSaveClick();
    virtual void OnbtnFileSaveOutputClick();
    virtual void OnbtnHelpClick();
    virtual void OnbtnRecallClick();
    virtual void OnbtnStartClick();
    virtual void OnbtnStopClick();
    virtual void OnbtnStoreClick();
    virtual void OnchbRealTimeClick();
    virtual void OnchbWaveFileClick();
    virtual void OnrbRealTimeLeftClick();
    virtual void OnrbRealTimeRightClick();
    virtual void OnrbSynthLeftClick();
    virtual void OnrbSynthRightClick();
    virtual void OnrbWaveFileLeftClick();
    virtual void OnrbWaveFileRightClick();
    virtual void OnsldConfigMove();
    virtual void OnsldControlMove();
    virtual void OnsldLeft1Move();
    virtual void OnsldLeft2Move();
    virtual void OnsldLeft3Move();
    virtual void OnsldLeft4Move();
    virtual void OnsldRight1Move();
    virtual void OnsldRight2Move();
    virtual void OnsldRight3Move();
    virtual void OnsldRight4Move();

protected:
};

#endif // MAINWINDOW_H
