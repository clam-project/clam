/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __QT_PROCESSINGPRESENTATION_HXX__
#define __QT_PROCESSINGPRESENTATION_HXX__

#include <string>
#include <qwidget.h>
#include "ProcessingPresentation.hxx"
#include "Signalv1.hxx"
#include "Slotv1.hxx"

namespace CLAM
{
	class ProcessingConfig;
}

namespace CLAMVM
{
	class InPortAdapter;
	class OutPortAdapter;
	class InControlAdapter;
	class OutControlAdapter;
}

namespace NetworkGUI
{

class Qt_InPortPresentation; 
class Qt_OutPortPresentation; 
class Qt_InControlPresentation; 
class Qt_OutControlPresentation; 

class Qt_ProcessingPresentation : public QWidget, public ProcessingPresentation
{
public:
	Qt_ProcessingPresentation( std::string nameFromNetwork, QWidget *parent = 0, const char *name = 0);
	virtual ~Qt_ProcessingPresentation();
	virtual void Show();
	virtual void Hide();
	void EmitPositionOfChildren();
protected:
	virtual void SetObservedClassName(const std::string& name);

	// port methods
	virtual void SetInPort( CLAMVM::InPortAdapter* );
	virtual void SetOutPort( CLAMVM::OutPortAdapter* );	
	virtual void SetInPortClicked( Qt_InPortPresentation *);
	virtual void SetOutPortClicked( Qt_OutPortPresentation *);
	virtual void SetOutPortAfterClickInPort(const QPoint &);
	virtual void SetInPortAfterClickOutPort(const QPoint &);

	// control methods
	virtual void SetInControl( CLAMVM::InControlAdapter* );
	virtual void SetOutControl( CLAMVM::OutControlAdapter* );	
	virtual void SetInControlClicked( Qt_InControlPresentation *);
	virtual void SetOutControlClicked( Qt_OutControlPresentation *);
	virtual void SetOutControlAfterClickInControl(const QPoint &);
	virtual void SetInControlAfterClickOutControl(const QPoint &);


	void paintEvent( QPaintEvent * );
	void mousePressEvent( QMouseEvent * );
	void mouseReleaseEvent( QMouseEvent * );
	void mouseMoveEvent( QMouseEvent * );
	void keyPressEvent( QKeyEvent * );
	void UpdateOutPortsPosition();
	void UpdateOutControlsPosition();

// qt stuff
	bool        mDown;
	QPoint     mClickPos;

public: // signals
	SigSlot::Signalv1< Qt_InPortPresentation * > SignalAcquireInPortClicked;
	SigSlot::Signalv1< Qt_OutPortPresentation * > SignalAcquireOutPortClicked;
	SigSlot::Signalv1< Qt_InControlPresentation * > SignalAcquireInControlClicked;
	SigSlot::Signalv1< Qt_OutControlPresentation * > SignalAcquireOutControlClicked;

public: // slots
	// ports
	SigSlot::Slotv1< Qt_InPortPresentation * > SlotSetInPortClicked;
	SigSlot::Slotv1< Qt_OutPortPresentation * > SlotSetOutPortClicked;
	SigSlot::Slotv1< const QPoint & > SlotSetOutPortAfterClickInPort;
	SigSlot::Slotv1< const QPoint & > SlotSetInPortAfterClickOutPort;

	//controls
	SigSlot::Slotv1< Qt_InControlPresentation * > SlotSetInControlClicked;
	SigSlot::Slotv1< Qt_OutControlPresentation * > SlotSetOutControlClicked;
	SigSlot::Slotv1< const QPoint & > SlotSetOutControlAfterClickInControl;
	SigSlot::Slotv1< const QPoint & > SlotSetInControlAfterClickOutControl;

};


} // namespace NetworkGUI

#endif // __QT_PROCESSINGPRESENTATION_HXX__
