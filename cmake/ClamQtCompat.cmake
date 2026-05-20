include_guard(GLOBAL)

function(clam_qt_write_forward_header root old_module header new_include)
	file(MAKE_DIRECTORY "${root}/${old_module}")
	file(WRITE "${root}/${old_module}/${header}"
		"#pragma once\n"
		"#include <${new_include}>\n"
	)
endfunction()

function(clam_generate_qt_compat_headers out_include_dir)
	set(_root "${CMAKE_CURRENT_BINARY_DIR}/generated/qt_compat/include")

	set(_qtgui_to_widgets
		QAbstractButton
		QAbstractSlider
		QApplication
		QCheckBox
		QColorDialog
		QColormap
		QComboBox
		QCompleter
		QDial
		QDialog
		QDialogButtonBox
		QDockWidget
		QDoubleSpinBox
		QFileDialog
		QFontComboBox
		QFrame
		QGraphicsItem
		QGraphicsProxyWidget
		QGraphicsRectItem
		QGraphicsScene
		QGraphicsSceneContextMenuEvent
		QGraphicsSceneMouseEvent
		QGraphicsView
		QGridLayout
		QHBoxLayout
		QHeaderView
		QInputDialog
		QLabel
		QLineEdit
		QMenu
		QMessageBox
		QPlainTextEdit
		QPushButton
		QScrollArea
		QSlider
		QSpinBox
		QStatusBar
		QStyleOption
		QTextEdit
		QToolBar
		QToolButton
		QToolTip
		QTreeWidget
		QVBoxLayout
		QWhatsThis
		QWidget
	)
	foreach(_header IN LISTS _qtgui_to_widgets)
		clam_qt_write_forward_header("${_root}" QtGui "${_header}" "QtWidgets/${_header}")
	endforeach()

	foreach(_header QPrintDialog QPrinter)
		clam_qt_write_forward_header("${_root}" QtGui "${_header}" "QtPrintSupport/${_header}")
	endforeach()

	foreach(_header QAction QActionGroup)
		clam_qt_write_forward_header("${_root}" QtGui "${_header}" "${_header}")
	endforeach()

	file(MAKE_DIRECTORY "${_root}/QtOpenGL")
	file(WRITE "${_root}/QtOpenGL/QGLWidget"
		"#pragma once\n"
		"#include <QOpenGLWidget>\n"
		"using QGLWidget = QOpenGLWidget;\n"
	)

	file(MAKE_DIRECTORY "${_root}/QtSvg")
	file(WRITE "${_root}/QtSvg/QSvgWidget"
		"#pragma once\n"
		"#include <QtSvgWidgets/QSvgWidget>\n"
	)

	file(MAKE_DIRECTORY "${_root}/QtDesigner")
	file(WRITE "${_root}/QtDesigner/QDesignerExportWidget"
		"#pragma once\n"
		"#ifndef QDESIGNER_WIDGET_EXPORT\n"
		"#define QDESIGNER_WIDGET_EXPORT\n"
		"#endif\n"
	)

	set(${out_include_dir} "${_root}" PARENT_SCOPE)
endfunction()
