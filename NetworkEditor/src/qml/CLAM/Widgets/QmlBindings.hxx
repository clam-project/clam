// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef CLAM_QML_BINDINGS_HXX
#define CLAM_QML_BINDINGS_HXX
#include <QStringList>
class QObject;
namespace CLAM {
class Network;
}
bool bindQmlInterface(QObject *root, CLAM::Network &network, QStringList &errors);
#endif
