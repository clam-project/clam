#ifndef CLAM_VM_PALETTE_HELPERS_HXX
#define CLAM_VM_PALETTE_HELPERS_HXX

#include <QWidget>
#include <QPalette>
#include <QColor>

// Replacements for the Qt3 QWidget::setPaletteBackgroundColor() and
// setPaletteForegroundColor() methods that were dropped in Qt4.
namespace CLAM
{
	namespace VM
	{
		inline void setBgColor(QWidget* w, const QColor& c)
		{
			if (!w) return;
			QPalette pal = w->palette();
			pal.setColor(w->backgroundRole(), c);
			w->setPalette(pal);
			w->setAutoFillBackground(true);
		}

		inline void setFgColor(QWidget* w, const QColor& c)
		{
			if (!w) return;
			QPalette pal = w->palette();
			pal.setColor(w->foregroundRole(), c);
			w->setPalette(pal);
		}
	}
}

#endif
