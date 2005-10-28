#ifndef CLIPBOARDWIDGET_HXX
#define CLIPBOARDWIDGET_HXX

#include <qwidget.h>
#include <qwidgetplugin.h>

class QLabel;
class ClipBoard;

class ClipBoardWidget : public QWidget
{
  Q_OBJECT
public:
  ClipBoardWidget( QWidget * parent = 0, const char * name = 0 ) : QWidget(parent,name)
  {
    init();
  }
  ClipBoard * getInstance();
private:
  void init();
  QLabel * mLabel;
};

#endif
