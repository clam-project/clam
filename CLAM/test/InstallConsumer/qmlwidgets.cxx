// SPDX-License-Identifier: GPL-2.0-or-later
#include <QGuiApplication>
#include <QQmlComponent>
#include <QQmlEngine>
#include <Monitor.hxx>
#include <iostream>
#include <memory>

int main(int argc, char ** argv)
{
    QGuiApplication app(argc, argv);
    ClamQuickMonitor monitor; // Verify the installed headers and exported target too.
    QQmlEngine engine;
    engine.addImportPath("qrc:/qt/qml");
    QQmlComponent component(&engine);
    component.setData("import QtQuick\nimport CLAM.Widgets 1.0\nOscilloscope {}", QUrl());
    while (component.isLoading()) app.processEvents();
    std::unique_ptr<QObject> item(component.create());
    if (!item) {
        std::cerr << component.errorString().toStdString();
        return 1;
    }
    std::cout << "Installed CLAM.Widgets module loaded\n";
    return 0;
}
