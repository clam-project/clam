
#include "FactoryToolBox.hxx"
#include "ProcessingLabel.hxx"
#include <iostream>

namespace NetworkGUI
{

FactoryToolBox::FactoryToolBox(  QWidget * parent , const char * name )
	: QFrame( parent, name ),
	  mDown(false),
	  mNumProcessings(0)
{
	setFrameStyle( QFrame::ToolBarPanel | QFrame::Raised );
	setLineWidth( 1 );
	setPalette( QPalette( QColor( 100, 100, 200 )));
	AddProcessingLabel( "Oscillator" );
	AddProcessingLabel( "Mixer" );
}

FactoryToolBox::~FactoryToolBox()
{	
	std::vector< ProcessingLabel* >::iterator it;
	for (it=mProcessingLabels.begin(); it!=mProcessingLabels.end(); it++)
		delete *it;
}

void FactoryToolBox::AddProcessingLabel( const std::string & name)
{
	ProcessingLabel * proc = new ProcessingLabel( name , this);
	resize (140, 10+ (mProcessingLabels.size()+1)*30 );
	proc->move(5, 5+ mProcessingLabels.size()*30 );
	proc->show();
	mProcessingLabels.push_back(proc);
}


void FactoryToolBox::mousePressEvent( QMouseEvent * m )
{

	if (m->button() == RightButton )
	{
		mDown = true;
		mClickPos = m->pos();
		grabMouse();
	}
}

void FactoryToolBox::mouseReleaseEvent( QMouseEvent *)
{	
	releaseMouse();
	mDown = false;
}

void FactoryToolBox::mouseMoveEvent( QMouseEvent * m )
{	
	if(mDown)
	{
		QPoint difference(QWidget::mapFromGlobal(m->globalPos()));
		difference = mapToParent(difference) - mClickPos;
		move(difference);
		QWidget * parent = parentWidget();
		parent->repaint();
	}
}

void FactoryToolBox::IncrementNumProcessings()
{
	mNumProcessings++;
}

int FactoryToolBox::GetNumProcessings()
{
	return mNumProcessings;
}


} // namespace NetworkGUI
