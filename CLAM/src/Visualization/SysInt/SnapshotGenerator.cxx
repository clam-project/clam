#include "SnapshotGenerator.hxx"
#include "FLTK/ArrayPresentation.hxx"

namespace CLAMGUI
{

void showSnapshotArray( const CLAM::DataArray& array )
{
	CLAMVM::WidgetTKWrapper& toolkit = CLAMVM::WidgetTKWrapper::GetWrapperFor( "FLTK" );
	
	ArrayPresentation pres( "Snapshot" );

	pres.UpdateData( array );
	pres.Show();

	do 
		{
			pres.Do();
			toolkit.Tick();
		} while ( !toolkit.IsClosing() );
}

void showSnapshotArray( CLAM::DataArray& array )
{
	CLAMVM::WidgetTKWrapper& toolkit = CLAMVM::WidgetTKWrapper::GetWrapperFor( "FLTK" );
	
	ArrayPresentation pres( "Snapshot" );

	pres.UpdateData( array );
	pres.Show();

	do 
		{
			pres.Do();
			toolkit.Tick();
		} while ( !toolkit.IsClosing() );

}

}
