#ifndef __MESSAGE__
#define __MESSAGE__

#include <string>
#include <QMessageBox>

namespace CLAM
{
	namespace VM
	{
		[[maybe_unused]] static void Message(QMessageBox::Icon icon,
							const std::string& caption,
							const std::string& message)
		{
			QMessageBox message_box(icon,
									caption.c_str(),
									message.c_str(),
									QMessageBox::Ok);
			message_box.exec();
		}
	}
}
#endif

