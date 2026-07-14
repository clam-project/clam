#include <QDesignerContainerExtension>
#include <QtUiPlugin/customwidget.h>
#include <QtPlugin>

class CLAMWidgets : public QObject,
	public QDesignerCustomWidgetCollectionInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomCollectionInterface")
	Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
public:
	CLAMWidgets(QObject* parent=0);
	virtual ~CLAMWidgets();
	QList<QDesignerCustomWidgetInterface*> customWidgets() const;

private:
	QList<QDesignerCustomWidgetInterface*> _widgetPlugins;
};


