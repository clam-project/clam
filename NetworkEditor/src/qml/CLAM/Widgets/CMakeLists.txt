# A reusable module for Prototyper, qmlscene, Qt Creator and QML preview tools.
qt_add_library(clam_qmlwidgets SHARED)
qt_add_qml_module(clam_qmlwidgets
    URI CLAM.Widgets
    VERSION 1.0
    RESOURCE_PREFIX /qt/qml
    DEPENDENCIES QtQuick
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml/CLAM/Widgets"
    DESIGNER_SUPPORTED
    SOURCES Monitor.hxx Monitor.cxx ControlBinding.hxx ControlBinding.cxx QmlBindings.hxx QmlBindings.cxx
    QML_FILES
        Oscilloscope.qml BufferOscilloscope.qml Vumeter.qml SpectrumView.qml
        PeakView.qml VectorView.qml HistogramView.qml LPModelView.qml
        MelSpectrumView.qml MelCepstrumView.qml Spectrogram.qml Tonnetz.qml
        KeySpace.qml ChordRanking.qml PolarChromaPeaks.qml Tunner.qml SegmentationView.qml
        ControlSlider.qml ControlDial.qml ControlSpinBox.qml BoolControlSender.qml
        ControlPrinter.qml BoolControlDisplay.qml ControlSurface.qml MIDIPiano.qml
        ProgressControl.qml QFileLineEdit.qml PlaybackControls.qml QFirstPerson.qml
)
add_library(CLAM::qmlwidgets ALIAS clam_qmlwidgets)
clam_set_legacy_cxx(clam_qmlwidgets)
set_target_properties(clam_qmlwidgets PROPERTIES EXPORT_NAME qmlwidgets)
target_include_directories(clam_qmlwidgets PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CLAM/qmlwidgets>"
)
target_link_libraries(clam_qmlwidgets PUBLIC CLAM::qtmonitors Qt6::Quick Qt6::Qml)
install(TARGETS clam_qmlwidgets EXPORT CLAMQmlWidgetsTargets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
# Use a relocatable path belonging to this install prefix, not Qt's system directory.
set(_qml_install_dir "${CMAKE_INSTALL_LIBDIR}/qt6/qml/CLAM/Widgets")
set_target_properties(clam_qmlwidgetsplugin PROPERTIES INSTALL_RPATH "$ORIGIN/../../../..")
install(TARGETS clam_qmlwidgetsplugin LIBRARY DESTINATION "${_qml_install_dir}")
install(FILES
    "${CMAKE_BINARY_DIR}/qml/CLAM/Widgets/qmldir"
    "${CMAKE_BINARY_DIR}/qml/CLAM/Widgets/clam_qmlwidgets.qmltypes"
    DESTINATION "${_qml_install_dir}"
)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/" DESTINATION "${_qml_install_dir}"
    FILES_MATCHING PATTERN "*.qml"
)
install(FILES Monitor.hxx ControlBinding.hxx QmlBindings.hxx
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/CLAM/qmlwidgets"
)
install(EXPORT CLAMQmlWidgetsTargets NAMESPACE CLAM:: DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CLAM")
