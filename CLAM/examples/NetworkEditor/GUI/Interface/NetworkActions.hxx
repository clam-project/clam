
#ifndef __NetworkActions_hxx__
#define __NetworkActions_hxx__

#include <qframe.h> 
#include "Signalv1.hxx"
#include "Slotv1.hxx"
#include "ActionButton.hxx"
#include <qbuttongroup.h>
#include <qpushbutton.h>

namespace NetworkGUI
{

class NetworkButton : public ActionButton
{
public:
	NetworkButton(  bool condition = true, QWidget *parent = 0, const char *name = 0 );
protected:
	bool mCondition;
	void mousePressEvent ( QMouseEvent * );
};


class NetworkActions : public QFrame
{
public:
	NetworkActions(  QWidget *parent = 0, const char *name = 0 );
	virtual ~NetworkActions();

private:
	void OnButtonPressed( bool );	

	QButtonGroup mActions;
	NetworkButton mState;
protected:
	void        mousePressEvent( QMouseEvent *);
	void        mouseReleaseEvent( QMouseEvent *);
	void        mouseMoveEvent( QMouseEvent *);
	bool        mDown;
	QPoint     mClickPos;

public: //signals
	SigSlot::Signalv1 < bool > ChangeNetworkState;
	//slots
	SigSlot::Slotv1 < bool > ButtonPressed;

};

} // namespace NetworkGUI

#endif  //  __NetworkActions_hxx__
