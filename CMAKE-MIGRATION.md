# CMake migration

This repository now has a CMake build for the CLAM libraries and for the
NetworkEditor Qt application (with its reusable monitor-widget library). The
build targets C++17 and Qt6.

## Configure and build (libraries only)

```sh
cmake -S . -B build/cmake -DCLAM_BUILD_GUI_APPS=OFF
cmake --build build/cmake -j$(nproc)
```

The default build creates shared libraries:

- `clam_core`
- `clam_processing`
- `clam_audioio`

All targets compile with `CXX_STANDARD 17`. The previous C++03/C++11 syntax in
the public headers (notably dynamic exception specifications such as
`throw (IllegalValue)`) has been removed so the libraries build cleanly under
C++17.

## NetworkEditor and Qt targets

The Qt-dependent code is disabled by default. Enabling it builds both the
reusable monitor-widget library and the NetworkEditor application:

```sh
cmake -S . -B build/cmake-qt -DCLAM_BUILD_GUI_APPS=ON
cmake --build build/cmake-qt -j$(nproc)
```

Targets produced:

- `clam_qtmonitors` (library, public monitor/binder/widget classes)
- `NetworkEditor` (executable, controlled by `CLAM_NETWORKEDITOR_BUILD_APPS`)

`cmake/ClamQtCompat.cmake` generates forwarding headers so the legacy
`#include <QtGui/QWidget>`-style includes still resolve under Qt6, and maps
`QGLWidget` users to `QOpenGLWidget`. Removed-API call sites such as
`QWheelEvent::delta()` are rewritten to their Qt6 equivalents
(`angleDelta().y()`, etc.).

## SMSTools

`SMSTools/` builds against Qt6 only and is opt-in:

```sh
cmake -S . -B build/cmake-smstools -DCLAM_BUILD_SMSTOOLS=ON
cmake --build build/cmake-smstools -j$(nproc)
```

Targets produced:

- `SMSConsole` (console batch tool, no Qt)
- `SMSBatch` (directory-batch tool, no Qt)
- `SMSTools` (Qt GUI application)

The previous `SMSTools-qt4/` tree was the Qt3Support-era version; it has been
replaced by `SMSTools/`. The port replaces all `Q3*` widgets with their modern
equivalents, drops the Qt3 widget palette/font helpers in favour of `QPalette`,
and rewires the dialogs' signal/slot connections in C++ rather than in the
`.ui` files (Qt6 `uic` emits typed function pointers against the form's base
class, which does not see the subclass slots).

## Remaining work

The following applications still rely on SCons and have not been ported to
CMake:

- `Annotator/`
- `Voice2MIDI/`

Known source-level blockers that any future CMake/Qt6 port of these will need
to clear:

- Remove remaining Qt3Support usage (`Q3*` classes) in `Voice2MIDI/`.
- Review OpenGL widgets that still inherit from the compatibility-aliased
  `QGLWidget`; paint and context behavior under `QOpenGLWidget` are close but
  not identical, and a few widgets (e.g. `QFirstPerson`) may want a native
  `QOpenGLWidget` port.
