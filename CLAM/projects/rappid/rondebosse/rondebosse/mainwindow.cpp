#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './mainwindow.ui'
**
** Created: Wed Oct 16 11:24:59 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "mainwindow.h"

#include <qvariant.h>
#include <kled.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlcdnumber.h>
#include <qprogressbar.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qslider.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

static const char* const image0_data[] = { 
"16 16 7 1",
". c None",
"# c #000000",
"b c #0000ff",
"e c #00c0c0",
"d c #808080",
"a c #c0c0c0",
"c c #ffffff",
"......##........",
"......#a##......",
"..##..##bb##....",
"..#a###c##bb##..",
"..##bb##cc##ad#.",
"..#c##bb##cc###.",
"..#ccc##ad#acc#.",
"###ccccc###acc#.",
"#e##cccccc#acc#.",
"##ee##cccc#acc#.",
"#c##a#accc#aac#.",
"#acc##accc###a#.",
"##aac##aac#..##.",
"..##a#.##a#.....",
"....##...##.....",
"................"};

static const char* const image1_data[] = { 
"20 20 4 1",
"# c #000000",
". c #c0c0c0",
"a c #ffff00",
"b c #ffffff",
"....................",
"....................",
"....................",
"...##############...",
"...#a#bbbbbbbb#.#...",
"...#a#bbbbbbbb###...",
"...#a#bbbbbbbb#a#...",
"...#a#bbbbbbbb#a#...",
"...#a#bbbbbbbb#a#...",
"...#a#bbbbbbbb#a#...",
"...#aa########aa#...",
"...#aaaaaaaaaaaa#...",
"...#aa#########a#...",
"...#aa######bb#a#...",
"...#aa######bb#a#...",
"...#aa######bb#a#...",
"....#############...",
"....................",
"....................",
"...................."};

static const char* const image2_data[] = { 
"20 20 5 1",
"# c #000000",
"c c #808000",
". c #c0c0c0",
"a c #ffff00",
"b c #ffffff",
"....................",
"....................",
"....................",
"............###.....",
"...........#...#.#..",
"................##..",
"....###........###..",
"...#aba#######......",
"...#babababab#......",
"...#ababababa#......",
"...#baba###########.",
"...#aba#ccccccccc#..",
"...#ba#ccccccccc#...",
"...#a#ccccccccc#....",
"...##ccccccccc#.....",
"...###########......",
"....................",
"....................",
"....................",
"...................."};

static const char* const image3_data[] = { 
"20 20 3 1",
"# c #000000",
". c #c0c0c0",
"a c #ffffff",
"....................",
"....................",
"....................",
"....................",
"....................",
"......#.............",
"......###...........",
"......#####.........",
"......#######.......",
"......#########.....",
"......#######.aa....",
"......#####.aa......",
"......###.aa........",
"......#.aa..........",
".......a............",
"....................",
"....................",
"....................",
"....................",
"...................."};

static const char* const image4_data[] = { 
"20 20 3 1",
"# c #000000",
". c #c0c0c0",
"a c #ffffff",
"....................",
"....................",
"....................",
"....................",
"....................",
"....................",
"......########a.....",
"......########a.....",
"......########a.....",
"......########a.....",
"......########a.....",
"......########a.....",
"......########a.....",
"......########a.....",
"......aaaaaaaaa.....",
"....................",
"....................",
"....................",
"....................",
"...................."};

static const char* const image5_data[] = { 
"20 20 4 1",
"b c #000080",
"a c #008080",
"# c #00ffff",
". c #c0c0c0",
"....................",
"....................",
"....................",
".......#####........",
"......##aaaaa.......",
".....##aabbaab......",
".....#aab..#ab......",
".....abb...#ab......",
"..........#aab......",
".........#aab.......",
"........#aab........",
"........#ab.........",
"........abb.........",
"....................",
"........##a.........",
"........#ab.........",
"........abb.........",
"....................",
"....................",
"...................."};


/* 
 *  Constructs a MainWindow which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
MainWindow::MainWindow( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    QPixmap image0( ( const char** ) image0_data );
    QPixmap image1( ( const char** ) image1_data );
    QPixmap image2( ( const char** ) image2_data );
    QPixmap image3( ( const char** ) image3_data );
    QPixmap image4( ( const char** ) image4_data );
    QPixmap image5( ( const char** ) image5_data );
    if ( !name )
	setName( "MainWindow" );
    resize( 592, 326 ); 
    setMinimumSize( QSize( 592, 326 ) );
    setMaximumSize( QSize( 592, 326 ) );
    setCaption( i18n( "Ronde Bosse" ) );
    setIcon( image0 );

    Frame3 = new QFrame( this, "Frame3" );
    Frame3->setGeometry( QRect( 3, 298, 586, 25 ) ); 
    Frame3->setFrameShape( QFrame::StyledPanel );
    Frame3->setFrameShadow( QFrame::Sunken );

    TextLabel1 = new QLabel( Frame3, "TextLabel1" );
    TextLabel1->setGeometry( QRect( 5, 4, 40, 16 ) ); 
    TextLabel1->setText( i18n( "Status:" ) );

    StatusLabel = new QLabel( Frame3, "StatusLabel" );
    StatusLabel->setGeometry( QRect( 47, 4, 325, 16 ) ); 
    StatusLabel->setText( i18n( "Stoped..." ) );

    Frame13 = new QFrame( this, "Frame13" );
    Frame13->setGeometry( QRect( 0, 0, 592, 36 ) ); 
    Frame13->setFrameShape( QFrame::StyledPanel );
    Frame13->setFrameShadow( QFrame::Raised );

    btnFileSave = new QPushButton( Frame13, "btnFileSave" );
    btnFileSave->setGeometry( QRect( 43, 4, 28, 28 ) ); 
    QPalette pal;
    QColorGroup cg;
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 227, 227, 227) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    btnFileSave->setPalette( pal );
    btnFileSave->setText( QString::null );
    btnFileSave->setPixmap( image1 );
    QToolTip::add( btnFileSave, i18n( "Save configuration file..." ) );

    Frame23 = new QFrame( Frame13, "Frame23" );
    Frame23->setGeometry( QRect( 77, 5, 9, 27 ) ); 
    Frame23->setFrameShape( QFrame::VLine );
    Frame23->setFrameShadow( QFrame::Raised );

    btnExit = new QPushButton( Frame13, "btnExit" );
    btnExit->setGeometry( QRect( 538, 4, 46, 28 ) ); 
    btnExit->setText( i18n( "E&xit" ) );
    QToolTip::add( btnExit, QString::null );

    btnFileOpen = new QPushButton( Frame13, "btnFileOpen" );
    btnFileOpen->setGeometry( QRect( 8, 4, 28, 28 ) ); 
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 227, 227, 227) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    btnFileOpen->setPalette( pal );
    btnFileOpen->setText( QString::null );
    btnFileOpen->setPixmap( image2 );
    QToolTip::add( btnFileOpen, i18n( "Open configuration file..." ) );

    btnStart = new QPushButton( Frame13, "btnStart" );
    btnStart->setGeometry( QRect( 92, 4, 28, 28 ) ); 
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 227, 227, 227) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    btnStart->setPalette( pal );
    btnStart->setText( QString::null );
    btnStart->setPixmap( image3 );
    QToolTip::add( btnStart, i18n( "Start" ) );

    Frame23_3 = new QFrame( Frame13, "Frame23_3" );
    Frame23_3->setGeometry( QRect( 525, 5, 9, 27 ) ); 
    Frame23_3->setFrameShape( QFrame::VLine );
    Frame23_3->setFrameShadow( QFrame::Raised );

    btnStop = new QPushButton( Frame13, "btnStop" );
    btnStop->setGeometry( QRect( 128, 4, 28, 28 ) ); 
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 227, 227, 227) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    btnStop->setPalette( pal );
    btnStop->setText( QString::null );
    btnStop->setPixmap( image4 );
    QToolTip::add( btnStop, i18n( "Stop" ) );

    btnHelp = new QPushButton( Frame13, "btnHelp" );
    btnHelp->setGeometry( QRect( 493, 4, 28, 28 ) ); 
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 227, 227, 227) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    btnHelp->setPalette( pal );
    btnHelp->setText( QString::null );
    btnHelp->setPixmap( image5 );
    QToolTip::add( btnHelp, i18n( "Help" ) );

    Frame23_2 = new QFrame( Frame13, "Frame23_2" );
    Frame23_2->setGeometry( QRect( 163, 5, 9, 27 ) ); 
    Frame23_2->setFrameShape( QFrame::VLine );
    Frame23_2->setFrameShadow( QFrame::Raised );

    TabWidget2 = new QTabWidget( this, "TabWidget2" );
    TabWidget2->setGeometry( QRect( 3, 41, 586, 253 ) ); 
    QFont TabWidget2_font(  TabWidget2->font() );
    TabWidget2->setFont( TabWidget2_font ); 

    tab = new QWidget( TabWidget2, "tab" );

    Frame7 = new QFrame( tab, "Frame7" );
    Frame7->setGeometry( QRect( 6, 34, 200, 185 ) ); 
    Frame7->setFrameShape( QFrame::StyledPanel );
    Frame7->setFrameShadow( QFrame::Raised );

    GroupBox1_4 = new QGroupBox( Frame7, "GroupBox1_4" );
    GroupBox1_4->setGeometry( QRect( 7, 136, 186, 41 ) ); 
    QFont GroupBox1_4_font(  GroupBox1_4->font() );
    GroupBox1_4_font.setPointSize( 7 );
    GroupBox1_4->setFont( GroupBox1_4_font ); 
    GroupBox1_4->setFrameShape( QGroupBox::StyledPanel );
    GroupBox1_4->setTitle( i18n( "Threshold" ) );

    LCDLeft4 = new QLCDNumber( GroupBox1_4, "LCDLeft4" );
    LCDLeft4->setEnabled( TRUE );
    LCDLeft4->setGeometry( QRect( 143, 11, 39, 26 ) ); 
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 241, 241, 241) );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    LCDLeft4->setPalette( pal );
    LCDLeft4->setFrameShape( QLCDNumber::StyledPanel );
    LCDLeft4->setFrameShadow( QLCDNumber::Sunken );
    LCDLeft4->setLineWidth( 1 );
    LCDLeft4->setSmallDecimalPoint( FALSE );
    LCDLeft4->setNumDigits( 3 );
    LCDLeft4->setSegmentStyle( QLCDNumber::Flat );
    LCDLeft4->setProperty( "intValue", 50 );

    sldLeft4 = new QSlider( GroupBox1_4, "sldLeft4" );
    sldLeft4->setGeometry( QRect( 6, 17, 131, 16 ) ); 
    sldLeft4->setMaxValue( 99 );
    sldLeft4->setPageStep( 1 );
    sldLeft4->setValue( 50 );
    sldLeft4->setOrientation( QSlider::Horizontal );

    GroupBox1_3 = new QGroupBox( Frame7, "GroupBox1_3" );
    GroupBox1_3->setGeometry( QRect( 7, 92, 186, 41 ) ); 
    QFont GroupBox1_3_font(  GroupBox1_3->font() );
    GroupBox1_3_font.setPointSize( 7 );
    GroupBox1_3->setFont( GroupBox1_3_font ); 
    GroupBox1_3->setFrameShape( QGroupBox::StyledPanel );
    GroupBox1_3->setTitle( i18n( "Level" ) );

    LCDLeft3 = new QLCDNumber( GroupBox1_3, "LCDLeft3" );
    LCDLeft3->setEnabled( TRUE );
    LCDLeft3->setGeometry( QRect( 143, 11, 39, 26 ) ); 
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 241, 241, 241) );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    LCDLeft3->setPalette( pal );
    LCDLeft3->setFrameShape( QLCDNumber::StyledPanel );
    LCDLeft3->setFrameShadow( QLCDNumber::Sunken );
    LCDLeft3->setLineWidth( 1 );
    LCDLeft3->setSmallDecimalPoint( FALSE );
    LCDLeft3->setNumDigits( 3 );
    LCDLeft3->setSegmentStyle( QLCDNumber::Flat );
    LCDLeft3->setProperty( "intValue", 99 );

    sldLeft3 = new QSlider( GroupBox1_3, "sldLeft3" );
    sldLeft3->setGeometry( QRect( 6, 17, 131, 16 ) ); 
    sldLeft3->setMaxValue( 99 );
    sldLeft3->setPageStep( 1 );
    sldLeft3->setValue( 99 );
    sldLeft3->setOrientation( QSlider::Horizontal );

    GroupBox1_2 = new QGroupBox( Frame7, "GroupBox1_2" );
    GroupBox1_2->setGeometry( QRect( 7, 48, 186, 41 ) ); 
    QFont GroupBox1_2_font(  GroupBox1_2->font() );
    GroupBox1_2_font.setPointSize( 7 );
    GroupBox1_2->setFont( GroupBox1_2_font ); 
    GroupBox1_2->setFrameShape( QGroupBox::StyledPanel );
    GroupBox1_2->setTitle( i18n( "Envelope length" ) );

    sldLeft2 = new QSlider( GroupBox1_2, "sldLeft2" );
    sldLeft2->setGeometry( QRect( 6, 17, 131, 16 ) ); 
    sldLeft2->setMaxValue( 99 );
    sldLeft2->setPageStep( 1 );
    sldLeft2->setValue( 50 );
    sldLeft2->setOrientation( QSlider::Horizontal );

    LCDLeft2 = new QLCDNumber( GroupBox1_2, "LCDLeft2" );
    LCDLeft2->setEnabled( TRUE );
    LCDLeft2->setGeometry( QRect( 143, 11, 39, 26 ) ); 
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 241, 241, 241) );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    LCDLeft2->setPalette( pal );
    LCDLeft2->setFrameShape( QLCDNumber::StyledPanel );
    LCDLeft2->setFrameShadow( QLCDNumber::Sunken );
    LCDLeft2->setLineWidth( 1 );
    LCDLeft2->setSmallDecimalPoint( FALSE );
    LCDLeft2->setNumDigits( 3 );
    LCDLeft2->setSegmentStyle( QLCDNumber::Flat );
    LCDLeft2->setProperty( "intValue", 50 );

    GroupBox1 = new QGroupBox( Frame7, "GroupBox1" );
    GroupBox1->setGeometry( QRect( 7, 3, 186, 41 ) ); 
    QFont GroupBox1_font(  GroupBox1->font() );
    GroupBox1_font.setPointSize( 7 );
    GroupBox1->setFont( GroupBox1_font ); 
    GroupBox1->setFrameShape( QGroupBox::StyledPanel );
    GroupBox1->setTitle( i18n( "Interpolation period" ) );

    sldLeft1 = new QSlider( GroupBox1, "sldLeft1" );
    sldLeft1->setGeometry( QRect( 6, 17, 131, 16 ) ); 
    sldLeft1->setMaxValue( 99 );
    sldLeft1->setPageStep( 1 );
    sldLeft1->setValue( 50 );
    sldLeft1->setOrientation( QSlider::Horizontal );

    LCDLeft1 = new QLCDNumber( GroupBox1, "LCDLeft1" );
    LCDLeft1->setEnabled( TRUE );
    LCDLeft1->setGeometry( QRect( 143, 11, 39, 26 ) ); 
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 241, 241, 241) );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    LCDLeft1->setPalette( pal );
    LCDLeft1->setFrameShape( QLCDNumber::StyledPanel );
    LCDLeft1->setFrameShadow( QLCDNumber::Sunken );
    LCDLeft1->setLineWidth( 1 );
    LCDLeft1->setSmallDecimalPoint( FALSE );
    LCDLeft1->setNumDigits( 3 );
    LCDLeft1->setSegmentStyle( QLCDNumber::Flat );
    LCDLeft1->setProperty( "intValue", 50 );

    Frame7_2 = new QFrame( tab, "Frame7_2" );
    Frame7_2->setGeometry( QRect( 375, 34, 200, 185 ) ); 
    Frame7_2->setFrameShape( QFrame::StyledPanel );
    Frame7_2->setFrameShadow( QFrame::Raised );

    GroupBox1_4_2 = new QGroupBox( Frame7_2, "GroupBox1_4_2" );
    GroupBox1_4_2->setGeometry( QRect( 7, 136, 186, 41 ) ); 
    QFont GroupBox1_4_2_font(  GroupBox1_4_2->font() );
    GroupBox1_4_2_font.setPointSize( 7 );
    GroupBox1_4_2->setFont( GroupBox1_4_2_font ); 
    GroupBox1_4_2->setFrameShape( QGroupBox::StyledPanel );
    GroupBox1_4_2->setTitle( i18n( "Threshold" ) );

    sldRight4 = new QSlider( GroupBox1_4_2, "sldRight4" );
    sldRight4->setGeometry( QRect( 6, 17, 131, 16 ) ); 
    sldRight4->setMaxValue( 99 );
    sldRight4->setPageStep( 1 );
    sldRight4->setValue( 50 );
    sldRight4->setOrientation( QSlider::Horizontal );

    LCDRight4 = new QLCDNumber( GroupBox1_4_2, "LCDRight4" );
    LCDRight4->setEnabled( TRUE );
    LCDRight4->setGeometry( QRect( 143, 11, 39, 26 ) ); 
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 241, 241, 241) );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    LCDRight4->setPalette( pal );
    LCDRight4->setFrameShape( QLCDNumber::StyledPanel );
    LCDRight4->setFrameShadow( QLCDNumber::Sunken );
    LCDRight4->setLineWidth( 1 );
    LCDRight4->setSmallDecimalPoint( FALSE );
    LCDRight4->setNumDigits( 3 );
    LCDRight4->setSegmentStyle( QLCDNumber::Flat );
    LCDRight4->setProperty( "intValue", 50 );

    GroupBox1_3_2 = new QGroupBox( Frame7_2, "GroupBox1_3_2" );
    GroupBox1_3_2->setGeometry( QRect( 7, 92, 186, 41 ) ); 
    QFont GroupBox1_3_2_font(  GroupBox1_3_2->font() );
    GroupBox1_3_2_font.setPointSize( 7 );
    GroupBox1_3_2->setFont( GroupBox1_3_2_font ); 
    GroupBox1_3_2->setFrameShape( QGroupBox::StyledPanel );
    GroupBox1_3_2->setTitle( i18n( "Level" ) );

    sldRight3 = new QSlider( GroupBox1_3_2, "sldRight3" );
    sldRight3->setGeometry( QRect( 6, 17, 131, 16 ) ); 
    sldRight3->setMaxValue( 99 );
    sldRight3->setPageStep( 1 );
    sldRight3->setValue( 99 );
    sldRight3->setOrientation( QSlider::Horizontal );

    LCDRight3 = new QLCDNumber( GroupBox1_3_2, "LCDRight3" );
    LCDRight3->setEnabled( TRUE );
    LCDRight3->setGeometry( QRect( 143, 11, 39, 26 ) ); 
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 241, 241, 241) );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    LCDRight3->setPalette( pal );
    LCDRight3->setFrameShape( QLCDNumber::StyledPanel );
    LCDRight3->setFrameShadow( QLCDNumber::Sunken );
    LCDRight3->setLineWidth( 1 );
    LCDRight3->setSmallDecimalPoint( FALSE );
    LCDRight3->setNumDigits( 3 );
    LCDRight3->setSegmentStyle( QLCDNumber::Flat );
    LCDRight3->setProperty( "intValue", 99 );

    GroupBox1_2_2 = new QGroupBox( Frame7_2, "GroupBox1_2_2" );
    GroupBox1_2_2->setGeometry( QRect( 7, 48, 186, 41 ) ); 
    QFont GroupBox1_2_2_font(  GroupBox1_2_2->font() );
    GroupBox1_2_2_font.setPointSize( 7 );
    GroupBox1_2_2->setFont( GroupBox1_2_2_font ); 
    GroupBox1_2_2->setFrameShape( QGroupBox::StyledPanel );
    GroupBox1_2_2->setTitle( i18n( "Envelope length" ) );

    sldRight2 = new QSlider( GroupBox1_2_2, "sldRight2" );
    sldRight2->setGeometry( QRect( 6, 17, 131, 16 ) ); 
    sldRight2->setMaxValue( 99 );
    sldRight2->setPageStep( 1 );
    sldRight2->setValue( 50 );
    sldRight2->setOrientation( QSlider::Horizontal );

    LCDRight2 = new QLCDNumber( GroupBox1_2_2, "LCDRight2" );
    LCDRight2->setEnabled( TRUE );
    LCDRight2->setGeometry( QRect( 143, 11, 39, 26 ) ); 
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 241, 241, 241) );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    LCDRight2->setPalette( pal );
    LCDRight2->setFrameShape( QLCDNumber::StyledPanel );
    LCDRight2->setFrameShadow( QLCDNumber::Sunken );
    LCDRight2->setLineWidth( 1 );
    LCDRight2->setSmallDecimalPoint( FALSE );
    LCDRight2->setNumDigits( 3 );
    LCDRight2->setSegmentStyle( QLCDNumber::Flat );
    LCDRight2->setProperty( "intValue", 50 );

    GroupBox1_5 = new QGroupBox( Frame7_2, "GroupBox1_5" );
    GroupBox1_5->setGeometry( QRect( 7, 3, 186, 41 ) ); 
    QFont GroupBox1_5_font(  GroupBox1_5->font() );
    GroupBox1_5_font.setPointSize( 7 );
    GroupBox1_5->setFont( GroupBox1_5_font ); 
    GroupBox1_5->setFrameShape( QGroupBox::StyledPanel );
    GroupBox1_5->setTitle( i18n( "Interpolation period" ) );

    LCDRight1 = new QLCDNumber( GroupBox1_5, "LCDRight1" );
    LCDRight1->setEnabled( TRUE );
    LCDRight1->setGeometry( QRect( 143, 11, 39, 26 ) ); 
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 241, 241, 241) );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    LCDRight1->setPalette( pal );
    LCDRight1->setFrameShape( QLCDNumber::StyledPanel );
    LCDRight1->setFrameShadow( QLCDNumber::Sunken );
    LCDRight1->setLineWidth( 1 );
    LCDRight1->setSmallDecimalPoint( FALSE );
    LCDRight1->setNumDigits( 3 );
    LCDRight1->setSegmentStyle( QLCDNumber::Flat );
    LCDRight1->setProperty( "intValue", 50 );

    sldRight1 = new QSlider( GroupBox1_5, "sldRight1" );
    sldRight1->setGeometry( QRect( 6, 17, 131, 16 ) ); 
    sldRight1->setMaxValue( 99 );
    sldRight1->setPageStep( 1 );
    sldRight1->setValue( 50 );
    sldRight1->setOrientation( QSlider::Horizontal );

    Frame8_2 = new QFrame( tab, "Frame8_2" );
    Frame8_2->setGeometry( QRect( 375, 7, 200, 27 ) ); 
    Frame8_2->setFrameShape( QFrame::StyledPanel );
    Frame8_2->setFrameShadow( QFrame::Raised );

    TextLabel1_2_2 = new QLabel( Frame8_2, "TextLabel1_2_2" );
    TextLabel1_2_2->setGeometry( QRect( 8, 5, 83, 16 ) ); 
    TextLabel1_2_2->setText( i18n( "Right channel" ) );

    ProgressBarRight = new QProgressBar( Frame8_2, "ProgressBarRight" );
    ProgressBarRight->setGeometry( QRect( 91, 8, 106, 12 ) ); 
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 241, 241, 241) );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 255, 0, 0) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 255, 0, 0) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 255, 0, 0) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    ProgressBarRight->setPalette( pal );
    ProgressBarRight->setProgress( 50 );
    ProgressBarRight->setCenterIndicator( FALSE );

    GroupBox18 = new QGroupBox( tab, "GroupBox18" );
    GroupBox18->setGeometry( QRect( 216, 2, 148, 35 ) ); 
    QFont GroupBox18_font(  GroupBox18->font() );
    GroupBox18_font.setPointSize( 7 );
    GroupBox18->setFont( GroupBox18_font ); 
    GroupBox18->setFrameShape( QGroupBox::StyledPanel );
    GroupBox18->setTitle( i18n( "Control level" ) );

    sldControl = new QSlider( GroupBox18, "sldControl" );
    sldControl->setGeometry( QRect( 9, 14, 131, 16 ) ); 
    sldControl->setMinValue( 0 );
    sldControl->setMaxValue( 100 );
    sldControl->setPageStep( 1 );
    sldControl->setValue( 50 );
    sldControl->setOrientation( QSlider::Horizontal );

    Frame8 = new QFrame( tab, "Frame8" );
    Frame8->setGeometry( QRect( 6, 7, 200, 27 ) ); 
    Frame8->setFrameShape( QFrame::StyledPanel );
    Frame8->setFrameShadow( QFrame::Raised );

    ProgressBarLeft = new QProgressBar( Frame8, "ProgressBarLeft" );
    ProgressBarLeft->setGeometry( QRect( 90, 8, 106, 12 ) ); 
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 241, 241, 241) );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 255, 0, 0) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 255, 0, 0) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 255, 0, 0) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    ProgressBarLeft->setPalette( pal );
    ProgressBarLeft->setProgress( 50 );
    ProgressBarLeft->setCenterIndicator( FALSE );

    TextLabel1_2 = new QLabel( Frame8, "TextLabel1_2" );
    TextLabel1_2->setGeometry( QRect( 8, 5, 76, 16 ) ); 
    TextLabel1_2->setText( i18n( "Left channel" ) );

    Frame6 = new QFrame( tab, "Frame6" );
    Frame6->setGeometry( QRect( 216, 47, 148, 172 ) ); 
    Frame6->setFrameShape( QFrame::StyledPanel );
    Frame6->setFrameShadow( QFrame::Raised );

    sldConfig = new QSlider( Frame6, "sldConfig" );
    sldConfig->setGeometry( QRect( 118, 8, 16, 105 ) ); 
    sldConfig->setPageStep( 1 );
    sldConfig->setValue( 50 );
    sldConfig->setOrientation( QSlider::Vertical );

    TextLabel1_3 = new QLabel( Frame6, "TextLabel1_3" );
    TextLabel1_3->setGeometry( QRect( 31, 3, 66, 16 ) ); 
    QFont TextLabel1_3_font(  TextLabel1_3->font() );
    TextLabel1_3_font.setPointSize( 7 );
    TextLabel1_3->setFont( TextLabel1_3_font ); 
    TextLabel1_3->setText( i18n( "Configuration" ) );

    LedConfig = new KLed( Frame6, "LedConfig" );
    LedConfig->setGeometry( QRect( 9, 5, 16, 16 ) ); 
    LedConfig->setState( KLed::Off );
    LedConfig->setShape( KLed::Circular );
    LedConfig->setLook( KLed::Sunken );
    LedConfig->setColor( QColor( 255, 0, 0 ) );
    LedConfig->setDarkFactor( 900 );

    LCDConfig = new QLCDNumber( Frame6, "LCDConfig" );
    LCDConfig->setEnabled( TRUE );
    LCDConfig->setGeometry( QRect( 9, 23, 95, 87 ) ); 
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 241, 241, 241) );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 85, 170, 0) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 228, 228, 228) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 114, 114, 114) );
    cg.setColor( QColorGroup::Mid, QColor( 152, 152, 152) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, black );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    LCDConfig->setPalette( pal );
    LCDConfig->setFrameShape( QLCDNumber::StyledPanel );
    LCDConfig->setFrameShadow( QLCDNumber::Sunken );
    LCDConfig->setLineWidth( 1 );
    LCDConfig->setSmallDecimalPoint( FALSE );
    LCDConfig->setNumDigits( 2 );
    LCDConfig->setSegmentStyle( QLCDNumber::Flat );
    LCDConfig->setProperty( "intValue", 50 );

    btnStore = new QPushButton( Frame6, "btnStore" );
    btnStore->setGeometry( QRect( 6, 138, 62, 27 ) ); 
    QFont btnStore_font(  btnStore->font() );
    btnStore->setFont( btnStore_font ); 
    btnStore->setText( i18n( "&Store" ) );

    btnRecall = new QPushButton( Frame6, "btnRecall" );
    btnRecall->setGeometry( QRect( 80, 138, 62, 27 ) ); 
    QFont btnRecall_font(  btnRecall->font() );
    btnRecall->setFont( btnRecall_font ); 
    btnRecall->setText( i18n( "&Recall" ) );

    Frame26 = new QFrame( Frame6, "Frame26" );
    Frame26->setGeometry( QRect( 7, 117, 134, 16 ) ); 
    Frame26->setFrameShape( QFrame::HLine );
    Frame26->setFrameShadow( QFrame::Raised );
    TabWidget2->insertTab( tab, i18n( " Real time " ) );

    tab_2 = new QWidget( TabWidget2, "tab_2" );

    ButtonGroup1 = new QButtonGroup( tab_2, "ButtonGroup1" );
    ButtonGroup1->setGeometry( QRect( 6, 7, 144, 112 ) ); 
    ButtonGroup1->setTitle( i18n( "INPUT: Left channel" ) );

    rbRealTimeLeft = new QRadioButton( ButtonGroup1, "rbRealTimeLeft" );
    rbRealTimeLeft->setGeometry( QRect( 12, 20, 90, 24 ) ); 
    rbRealTimeLeft->setText( i18n( "Real time" ) );
    rbRealTimeLeft->setChecked( TRUE );

    rbWaveFileLeft = new QRadioButton( ButtonGroup1, "rbWaveFileLeft" );
    rbWaveFileLeft->setGeometry( QRect( 13, 43, 90, 30 ) ); 
    rbWaveFileLeft->setFocusPolicy( QRadioButton::TabFocus );
    rbWaveFileLeft->setText( i18n( "Wave file" ) );

    rbSynthLeft = new QRadioButton( ButtonGroup1, "rbSynthLeft" );
    rbSynthLeft->setGeometry( QRect( 13, 72, 90, 32 ) ); 
    rbSynthLeft->setFocusPolicy( QRadioButton::TabFocus );
    rbSynthLeft->setText( i18n( "Synthesized" ) );

    btnFileOpenLeft = new QPushButton( ButtonGroup1, "btnFileOpenLeft" );
    btnFileOpenLeft->setGeometry( QRect( 104, 44, 28, 28 ) ); 
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 227, 227, 227) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    btnFileOpenLeft->setPalette( pal );
    btnFileOpenLeft->setText( QString::null );
    btnFileOpenLeft->setPixmap( image2 );
    QToolTip::add( btnFileOpenLeft, i18n( "Open input wave file..." ) );

    ButtonGroup1_2 = new QButtonGroup( tab_2, "ButtonGroup1_2" );
    ButtonGroup1_2->setGeometry( QRect( 159, 7, 144, 112 ) ); 
    ButtonGroup1_2->setTitle( i18n( "INPUT: Right channel" ) );

    rbSynthRight = new QRadioButton( ButtonGroup1_2, "rbSynthRight" );
    rbSynthRight->setGeometry( QRect( 13, 72, 90, 32 ) ); 
    rbSynthRight->setText( i18n( "Synthesized" ) );
    ButtonGroup1_2->insert( rbSynthRight, 2 );

    rbWaveFileRight = new QRadioButton( ButtonGroup1_2, "rbWaveFileRight" );
    rbWaveFileRight->setGeometry( QRect( 13, 43, 90, 30 ) ); 
    rbWaveFileRight->setText( i18n( "Wave file" ) );
    ButtonGroup1_2->insert( rbWaveFileRight, 1 );

    rbRealTimeRight = new QRadioButton( ButtonGroup1_2, "rbRealTimeRight" );
    rbRealTimeRight->setGeometry( QRect( 12, 20, 90, 24 ) ); 
    rbRealTimeRight->setText( i18n( "Real time" ) );
    rbRealTimeRight->setChecked( TRUE );

    btnFileOpenRight = new QPushButton( ButtonGroup1_2, "btnFileOpenRight" );
    btnFileOpenRight->setGeometry( QRect( 104, 44, 28, 28 ) ); 
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 227, 227, 227) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    btnFileOpenRight->setPalette( pal );
    btnFileOpenRight->setText( QString::null );
    btnFileOpenRight->setPixmap( image2 );
    ButtonGroup1_2->insert( btnFileOpenRight, 3 );
    QToolTip::add( btnFileOpenRight, i18n( "Open input wave file..." ) );

    ButtonGroup1_2_2 = new QButtonGroup( tab_2, "ButtonGroup1_2_2" );
    ButtonGroup1_2_2->setGeometry( QRect( 313, 7, 144, 112 ) ); 
    ButtonGroup1_2_2->setTitle( i18n( "OUTPUT" ) );

    chbWaveFile = new QCheckBox( ButtonGroup1_2_2, "chbWaveFile" );
    chbWaveFile->setGeometry( QRect( 13, 48, 78, 19 ) ); 
    chbWaveFile->setText( i18n( "Wave file" ) );

    chbRealTime = new QCheckBox( ButtonGroup1_2_2, "chbRealTime" );
    chbRealTime->setGeometry( QRect( 13, 18, 78, 19 ) ); 
    chbRealTime->setText( i18n( "Real time" ) );
    chbRealTime->setChecked( TRUE );

    btnFileSaveOutput = new QPushButton( ButtonGroup1_2_2, "btnFileSaveOutput" );
    btnFileSaveOutput->setGeometry( QRect( 104, 44, 28, 28 ) ); 
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 227, 227, 227) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 200, 200, 200) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 230, 230, 230) );
    cg.setColor( QColorGroup::Dark, QColor( 100, 100, 100) );
    cg.setColor( QColorGroup::Mid, QColor( 133, 133, 133) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    btnFileSaveOutput->setPalette( pal );
    btnFileSaveOutput->setText( QString::null );
    btnFileSaveOutput->setPixmap( image2 );
    ButtonGroup1_2_2->insert( btnFileSaveOutput, 3 );
    QToolTip::add( btnFileSaveOutput, i18n( "Open output wave file..." ) );
    TabWidget2->insertTab( tab_2, i18n( " Static       " ) );

    // signals and slots connections
    connect( btnStart, SIGNAL( clicked() ), this, SLOT( OnbtnStartClick() ) );
    connect( btnStop, SIGNAL( clicked() ), this, SLOT( OnbtnStopClick() ) );
    connect( sldLeft1, SIGNAL( valueChanged(int) ), this, SLOT( OnsldLeft1Move() ) );
    connect( sldLeft2, SIGNAL( valueChanged(int) ), this, SLOT( OnsldLeft2Move() ) );
    connect( sldLeft3, SIGNAL( valueChanged(int) ), this, SLOT( OnsldLeft3Move() ) );
    connect( sldLeft4, SIGNAL( valueChanged(int) ), this, SLOT( OnsldLeft4Move() ) );
    connect( btnFileSave, SIGNAL( clicked() ), this, SLOT( OnbtnFileSaveClick() ) );
    connect( sldControl, SIGNAL( valueChanged(int) ), this, SLOT( OnsldControlMove() ) );
    connect( sldConfig, SIGNAL( valueChanged(int) ), this, SLOT( OnsldConfigMove() ) );
    connect( btnExit, SIGNAL( clicked() ), this, SLOT( OnbtnExitClick() ) );
    connect( btnFileOpen, SIGNAL( clicked() ), this, SLOT( OnbtnFileOpenClick() ) );
    connect( btnStore, SIGNAL( clicked() ), this, SLOT( OnbtnStoreClick() ) );
    connect( btnRecall, SIGNAL( clicked() ), this, SLOT( OnbtnRecallClick() ) );
    connect( rbRealTimeLeft, SIGNAL( clicked() ), this, SLOT( OnrbRealTimeLeftClick() ) );
    connect( rbRealTimeRight, SIGNAL( clicked() ), this, SLOT( OnrbRealTimeRightClick() ) );
    connect( rbWaveFileLeft, SIGNAL( clicked() ), this, SLOT( OnrbWaveFileLeftClick() ) );
    connect( rbWaveFileRight, SIGNAL( clicked() ), this, SLOT( OnrbWaveFileRightClick() ) );
    connect( rbSynthLeft, SIGNAL( clicked() ), this, SLOT( OnrbSynthLeftClick() ) );
    connect( rbSynthRight, SIGNAL( clicked() ), this, SLOT( OnrbSynthRightClick() ) );
    connect( btnFileOpenLeft, SIGNAL( clicked() ), this, SLOT( OnbtnFileOpenLeftClick() ) );
    connect( btnFileOpenRight, SIGNAL( clicked() ), this, SLOT( OnbtnFileOpenRightClick() ) );
    connect( btnHelp, SIGNAL( clicked() ), this, SLOT( OnbtnHelpClick() ) );
    connect( chbRealTime, SIGNAL( stateChanged(int) ), this, SLOT( OnchbRealTimeClick() ) );
    connect( chbWaveFile, SIGNAL( stateChanged(int) ), this, SLOT( OnchbWaveFileClick() ) );
    connect( sldRight1, SIGNAL( valueChanged(int) ), this, SLOT( OnsldRight1Move() ) );
    connect( sldRight2, SIGNAL( valueChanged(int) ), this, SLOT( OnsldRight2Move() ) );
    connect( sldRight3, SIGNAL( valueChanged(int) ), this, SLOT( OnsldRight3Move() ) );
    connect( sldRight4, SIGNAL( valueChanged(int) ), this, SLOT( OnsldRight4Move() ) );
    connect( btnFileSaveOutput, SIGNAL( clicked() ), this, SLOT( OnbtnFileSaveOutputClick() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
MainWindow::~MainWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

void MainWindow::OnbtnExitClick()
{
    qWarning( "MainWindow::OnbtnExitClick(): Not implemented yet!" );
}

void MainWindow::OnbtnFileOpenClick()
{
    qWarning( "MainWindow::OnbtnFileOpenClick(): Not implemented yet!" );
}

void MainWindow::OnbtnFileOpenLeftClick()
{
    qWarning( "MainWindow::OnbtnFileOpenLeftClick(): Not implemented yet!" );
}

void MainWindow::OnbtnFileOpenRightClick()
{
    qWarning( "MainWindow::OnbtnFileOpenRightClick(): Not implemented yet!" );
}

void MainWindow::OnbtnFileSaveClick()
{
    qWarning( "MainWindow::OnbtnFileSaveClick(): Not implemented yet!" );
}

void MainWindow::OnbtnFileSaveOutputClick()
{
    qWarning( "MainWindow::OnbtnFileSaveOutputClick(): Not implemented yet!" );
}

void MainWindow::OnbtnHelpClick()
{
    qWarning( "MainWindow::OnbtnHelpClick(): Not implemented yet!" );
}

void MainWindow::OnbtnRecallClick()
{
    qWarning( "MainWindow::OnbtnRecallClick(): Not implemented yet!" );
}

void MainWindow::OnbtnStartClick()
{
    qWarning( "MainWindow::OnbtnStartClick(): Not implemented yet!" );
}

void MainWindow::OnbtnStopClick()
{
    qWarning( "MainWindow::OnbtnStopClick(): Not implemented yet!" );
}

void MainWindow::OnbtnStoreClick()
{
    qWarning( "MainWindow::OnbtnStoreClick(): Not implemented yet!" );
}

void MainWindow::OnchbRealTimeClick()
{
    qWarning( "MainWindow::OnchbRealTimeClick(): Not implemented yet!" );
}

void MainWindow::OnchbWaveFileClick()
{
    qWarning( "MainWindow::OnchbWaveFileClick(): Not implemented yet!" );
}

void MainWindow::OnrbRealTimeLeftClick()
{
    qWarning( "MainWindow::OnrbRealTimeLeftClick(): Not implemented yet!" );
}

void MainWindow::OnrbRealTimeRightClick()
{
    qWarning( "MainWindow::OnrbRealTimeRightClick(): Not implemented yet!" );
}

void MainWindow::OnrbSynthLeftClick()
{
    qWarning( "MainWindow::OnrbSynthLeftClick(): Not implemented yet!" );
}

void MainWindow::OnrbSynthRightClick()
{
    qWarning( "MainWindow::OnrbSynthRightClick(): Not implemented yet!" );
}

void MainWindow::OnrbWaveFileLeftClick()
{
    qWarning( "MainWindow::OnrbWaveFileLeftClick(): Not implemented yet!" );
}

void MainWindow::OnrbWaveFileRightClick()
{
    qWarning( "MainWindow::OnrbWaveFileRightClick(): Not implemented yet!" );
}

void MainWindow::OnsldConfigMove()
{
    qWarning( "MainWindow::OnsldConfigMove(): Not implemented yet!" );
}

void MainWindow::OnsldControlMove()
{
    qWarning( "MainWindow::OnsldControlMove(): Not implemented yet!" );
}

void MainWindow::OnsldLeft1Move()
{
    qWarning( "MainWindow::OnsldLeft1Move(): Not implemented yet!" );
}

void MainWindow::OnsldLeft2Move()
{
    qWarning( "MainWindow::OnsldLeft2Move(): Not implemented yet!" );
}

void MainWindow::OnsldLeft3Move()
{
    qWarning( "MainWindow::OnsldLeft3Move(): Not implemented yet!" );
}

void MainWindow::OnsldLeft4Move()
{
    qWarning( "MainWindow::OnsldLeft4Move(): Not implemented yet!" );
}

void MainWindow::OnsldRight1Move()
{
    qWarning( "MainWindow::OnsldRight1Move(): Not implemented yet!" );
}

void MainWindow::OnsldRight2Move()
{
    qWarning( "MainWindow::OnsldRight2Move(): Not implemented yet!" );
}

void MainWindow::OnsldRight3Move()
{
    qWarning( "MainWindow::OnsldRight3Move(): Not implemented yet!" );
}

void MainWindow::OnsldRight4Move()
{
    qWarning( "MainWindow::OnsldRight4Move(): Not implemented yet!" );
}

#include "mainwindow.moc"
