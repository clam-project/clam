// SPDX-License-Identifier: GPL-2.0-or-later
#include "QmlBindings.hxx"
#include "ControlBinding.hxx"
#include "Monitor.hxx"

bool bindQmlInterface(QObject *root, CLAM::Network &network, QStringList &errors) {
    if (!root)
        return false;
    if (auto *monitor = qobject_cast<ClamQuickMonitor *>(root))
        monitor->bind(network, errors);
    if (auto *control = qobject_cast<ClamControlBinding *>(root))
        control->bind(network, errors);
    for (QObject *child : root->children())
        bindQmlInterface(child, network, errors);
    return errors.isEmpty();
}
