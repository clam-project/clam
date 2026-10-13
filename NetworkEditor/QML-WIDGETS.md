# QML widgets and Prototyper

Use `import CLAM.Widgets 1.0` to compose a Prototyper interface in QML.
The components are Qt Quick items and controls. The audio monitors reuse
CLAM's monitor processings, so the network format stays `.clamnetwork`.
Qt Creator, QML language tools and QML preview tools can discover the module
through its generated `qmldir` and `.qmltypes` metadata.

## Build and run

Enable `CLAM_BUILD_GUI_APPS` and build normally; the QML module and plugin are
included. Install Qt6 Declarative development tools and the Qt Quick,
Controls, Layouts, Window, Templates and QtQml WorkerScript runtime modules
(see the root README for Ubuntu package names).

```sh
cmake -S . -B build -DCLAM_BUILD_GUI_APPS=ON
cmake --build build -j4
cd NetworkEditor
../build/NetworkEditor/Prototyper example-data/FilePlayer.clamnetwork
../build/NetworkEditor/Prototyper example-data/tonalAnalysis-file.clamnetwork --start-paused
```

With no interface argument, Prototyper looks beside the network for a
same-named `.qml`, then `.ui`, then uses its built-in playback interface.
Pass an interface path to select one explicitly. A missing or invalid
explicit interface, or a binding error, fails with diagnostics. QML errors
also include the source location on stderr. Legacy `.ui` files remain
supported; the loader constructs the CLAM widgets without needing a
Designer plugin in the system Qt directory.

`--start-paused` leaves the network stopped until Play is pressed. The
normal startup starts audio after the interface binds successfully.
`-o` requires a network file and runs without a GUI. `-b pipewire`, `-b jack`
and `-b portaudio` choose the backend; repeated `-b` options are tried in
order. Without `-b`, the available backends are tried in that order.

## Authoring with QML tools

Point your tool's QML import path at the build's `qml` directory. On Qt6
installations that ship the `qml` runner:

```sh
# From the repository root; use your distribution's Qt6 qml executable.
QML_IMPORT_PATH="$PWD/build/qml" qml NetworkEditor/example-data/FilePlayer.qml
```

For Qt Creator, add `<absolute-build-directory>/qml` to the project's QML
import paths. The plugin and all libraries must be built first. For an
installed module, use `<prefix>/<libdir>/qt6/qml` as the import path.
Prototyper discovers this path relative to its installed executable.
The installation includes the QML sources and type metadata for editors.

Standalone tools display unbound components and preview the layout. Live
CLAM port/control binding happens when Prototyper loads the interface.
Unbound monitors show a preview label. No audio backend is needed to edit
or preview a layout. Qt Quick Controls provide their usual styling and
accessibility; you can replace backgrounds, handles, delegates and colors
in your own components.

The bundled examples guard the `prototyper` context object with `typeof`,
so they work in both Prototyper and independent preview tools. Set a QML
window's `visible` property to false for Prototyper; it shows the window
after binding. An `Item` root is also supported: Prototyper hosts it in a
window sized to the root's width/height or implicit size.

## Binding an interface

```qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CLAM.Widgets 1.0

ApplicationWindow {
    width: 600; height: 360
    visible: typeof prototyper === "undefined"
    ColumnLayout {
        anchors.fill: parent
        ControlSlider {
            clamInControl: "AudioMixer.Gain 0"
            from: 0; to: 1; value: 1
            Layout.fillWidth: true
        }
        Oscilloscope {
            clamOutPort: "AudioMixer.Output Audio"
            lineColor: "#65d598"
            Layout.fillWidth: true; Layout.fillHeight: true
        }
        PlaybackControls {
            controller: typeof prototyper !== "undefined" ? prototyper : null
        }
    }
}
```

Use the actual processing and connector names, including spaces:
`clamOutPort: "Processing.Output Port"`, `clamInControl: "Processing.Input Control"`
and `clamOutControl: "Processing.Output Control"`. A monitor's data type
must match the selected port. The network stops before UI destruction,
which detaches control observers before their processings are removed.
Declare these bindings in the interface loaded at startup; replacing
bindings or dynamically adding bound components after loading is not supported.

Monitors also accept legacy `objectName: "OutPort__Processing__Output___Port"`.
Prefer explicit properties for new interfaces. QML controls use their actual
value range; set `from`, `to` and `value` in control units. The legacy `.ui`
binder's implicit 0–100 mapping does not apply to QML. A checked boolean
sends true (or 1 for a float input); an unchecked boolean sends false/0.

`ControlBinding` lets any QML control send data or observe a numeric/boolean
output. Input types supported are CLAM float, bool, int and string; output
observers support float, bool and int. Set `value` for a reactive input
binding, or call `send(value)` for an event-driven control. Initial values
are sent when binding succeeds. `observedValue` changes on the GUI thread.
For example:

```qml
ControlBinding { id: gain; clamInControl: "AudioMixer.Gain 0" }
Button { text: "Mute"; onClicked: gain.send(0) }
ControlPrinter { clamOutControl: "Player.Current Time Position"; decimals: 2 }
```

Alternatively, `clamProcessing` on a scalar control addresses an existing
`OutControlSender`, `ControlSource` (first channel), or `BoolControlSender`
(first output) processing. Set the component's range and initial value to
match its configuration.

For processing dialogs and file selection:

```qml
ControlBinding { id: reader; clamProcessing: "AudioFileMemoryLoader" }
Button { text: "Open audio…"; onClicked: reader.chooseAudioFile() }
Button { text: "Configure…"; onClicked: reader.configure() }
```

`chooseAudioFile()` supports `MonoAudioFileReader` and
`AudioFileMemoryLoader`. File selection preserves the previous playback
state when reconfiguration succeeds. `configure()` opens the existing CLAM
configuration dialog. These actions require a running Prototyper (preview
tools display the buttons but have no network to configure).

## Component catalog and migration

The monitors are `Oscilloscope`, `BufferOscilloscope`, `Vumeter`,
`SpectrumView`, `PeakView`, `VectorView`, `HistogramView`, `LPModelView`,
`MelSpectrumView`, `MelCepstrumView`, `Spectrogram`, `Tonnetz`, `KeySpace`,
`ChordRanking`, `PolarChromaPeaks`, `Tunner` (legacy spelling retained) and
`SegmentationView`. Each has `clamOutPort`, `lineColor`, `backgroundColor`,
`refreshInterval` (milliseconds, default 50) and read-only `sampleCount`.
Vector plots auto-scale. Other linear plots use source bounds when present,
or `lowerBound`/`upperBound` (defaults −1/+1). Spectrum plots use a dB scale;
peak plots show frequency and dB magnitude. `SegmentationView.timeSpan`
sets the visible history in seconds (default 10). `Spectrogram` displays a
256-frame history of vector values; connect a vector port, as in the old
widget. Tonnetz displays the fifth/third lattice; KeySpace shows major/minor
key strengths; ChordRanking lists the eight strongest labels.

| Legacy widget/use | QML component | Binding / behavior |
| --- | --- | --- |
| Monitors, including `CLAM::VM::*` | Same short type name | `clamOutPort`; native Quick painting |
| Slider, `QPixmapRange` | `ControlSlider` | `clamInControl` or `clamProcessing`; `from`, `to`, `value` |
| `QSynthKnob`, dial | `ControlDial` | Same bindings and range as slider |
| Numeric sender / spin box | `ControlSpinBox` | `decimals`, integer `from`/`to`/`value`; sends `controlValue = value / 10^decimals` |
| `BoolControlSenderWidget` | `BoolControlSender` | `clamInControl` or `clamProcessing`; `checked` |
| `ControlPrinterWidget` | `ControlPrinter` | `clamOutControl`; `decimals` |
| `BoolControlDisplay` | `BoolControlDisplay` | `clamOutControl`; active/inactive colors |
| `ControlSurfaceWidget` | `ControlSurface` | `clamXControl` and `clamYControl`, or existing `ControlSurface` via `clamProcessing`; X/Y ranges |
| `MIDIPianoWidget` | `MIDIPiano` | Existing `ControlPiano` via `clamProcessing`; MIDI `firstNote` (default 60), `velocity` |
| `ProgressControlWidget` | `ProgressControl` | Player via `clamProcessing`, or explicit `clamInControl`/`clamOutControl`; values 0–1 |
| `QFileLineEdit` | `QFileLineEdit` | String `clamInControl`; sends on editing finished |
| Play/Pause/Stop buttons | `PlaybackControls` | `controller: prototyper` |
| `QPixmapButton`, regular buttons | Qt Quick Controls `Button` | Call a `ControlBinding` action or send a value |
| `QFirstPerson` | `QFirstPerson` | Canvas perspective view; `clamXControl`, `clamYControl`, `clamOrientationControl` |

`QFirstPerson` retains arrow-key navigation, Alt+left/right strafing,
Shift for larger steps, PageUp/PageDown elevation, and W/A/S/D emitter
movement. Its `viewX`, `viewY`, `orientation`, `elevation`, `sourceX` and
`sourceY` properties can be bound to other QML controls. Its floor and emitter
are rendered with Canvas rather than the original lit OpenGL scene.

The QML rendering and styling are new. Designer-specific palette, pixmap
skin, LED and OpenGL properties do not have a one-to-one translation;
customize the QML component instead. The existing QWidget versions still
serve NetworkEditor's embedded panels and legacy `.ui` files.

## Validation

With `BUILD_TESTING` (on by default), `networkeditor_qml_widgets` exercises every
component, audio/vector frame delivery, empty frames, input/output controls,
incompatible bindings, observer cleanup, the legacy UI loader, Item roots,
both bundled QML layouts and destruction order without audio hardware.
Prototyper CLI startup/help and backend failure have separate CTest checks.

```sh
cmake --build build --target clam_qmlwidgets_qmllint
ctest --test-dir build --output-on-failure
```

The module uses Qt's [QML module build helpers](https://doc.qt.io/qt-6/qt-add-qml-module.html)
and [QQuickPaintedItem](https://doc.qt.io/qt-6/qquickpainteditem.html).
