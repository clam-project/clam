# CMake migration

This branch replaces the SCons builds of the CLAM libraries, NetworkEditor
and SMSTools with one CMake build (CMake 3.22 or later), moves the code to
C++17 and ports the GUI applications to Qt6. [`README.md`](README.md) has the
build instructions and options; this file records what changed and what is
left.

## Build system

- The top-level `CMakeLists.txt` builds the libraries (`CLAM/`) and,
  optionally, `NetworkEditor/`, `SMSTools/` and the unit tests (`CLAM/test/`).
- `cmake/ClamSources.cmake` collects the sources folder by folder with
  blacklists, as the SCons scripts did, and generates the
  `CLAMVersion`/`NetworkEditorVersion`/`SMSToolsVersion` sources from the
  `CHANGES` files. Generated files are only rewritten when their content
  changes, so reconfiguring does not trigger a rebuild.
- The libraries are always shared, with the SCons SONAME scheme
  (`libclam_core.so.1.4`). Their processings register themselves in factories
  from static initializers, which a static link would drop.
- `CLAM_WITH_*` options replace the SCons `with_*` options. Each one either
  finds its dependency or stops with an error, and adds the same `USE_*`
  definitions as before; public definitions reach consumers through the
  exported targets and the pkg-config files.
- Debug builds define `_DEBUG`, which turns on `CLAM_DEBUG_ASSERT`, as the
  SCons debug builds did.
- `BUILD_TESTING` (from CTest, on by default) builds the CppUnit unit tests
  and the threading test and registers them with CTest.
- Linux and macOS build in CI; the Windows build (MSVC, vcpkg) has a CI job
  that is disabled.

## Install and packaging

`cmake --install` installs what the SCons `install` targets did: libraries,
headers (`include/CLAM`, `include/CLAM/qtmonitors`), applications, example
data, desktop and MIME files, icons, man pages and translations. It also
installs:

- a CMake package (`lib/cmake/CLAM`) with the targets `CLAM::core`,
  `CLAM::processing`, `CLAM::audioio` and, when built, `CLAM::qtmonitors`;
- `clam_core.pc`, `clam_processing.pc` and `clam_audioio.pc`;
- the SCons tools in `share/clam/sconstools`, ported to Python 3 / SCons 4
  (`qt6` replaces `qt4`), for SCons projects that build against CLAM;
- the `clamrefactor`, `clamnetwork2code` and `clam_lv2_generator` scripts
  (Python 3) under `share/clam`, as commands in `bin`, with their man pages.

NetworkEditor's "Compile as LADSPA plugin" now generates a small CMake project
that uses the installed CMake package, instead of a Python 2 SConstruct.

## C++17

Dynamic exception specifications, `register`, `std::auto_ptr` and other
constructs removed from C++17 are gone from the compiled sources. The
libraries and applications build with GCC, clang and MSVC.

The `CLAM::Thread`, `Mutex`, `Condition` and `Lock` classes
(`CLAM/src/System/Threads`) are removed; their users, and programs built
against CLAM, use `std::thread`, `std::mutex` and `std::condition_variable`.

## Qt6

- Legacy includes such as `<QtGui/QWidget>` were rewritten to the Qt6 module
  layout.
- `QGLWidget` users now derive from `QOpenGLWidget`. The NetworkEditor
  widgets name `QOpenGLWidget` directly; the SMSTools plot library
  (`SMSTools/vmqt`) keeps a local `using QGLWidget = QOpenGLWidget;` alias.
  Text that `QGLWidget::renderText()` drew (the KeySpace, Tonnetz and
  QFirstPerson labels, the SMSTools plot tooltips) is now painted with
  `QPainter` over the GL drawing.
- GLU is no longer used: QFirstPerson draws its spheres with a small helper
  and projects its label positions itself.
- NetworkEditor reads the network version with `QDomDocument` instead of
  `QXmlQuery`.
- String-based connections to signals that Qt6 removed were moved to the
  Qt6 signals, mostly with pointer-to-member `connect()`.

## SMSTools

`main` had two SMSTools trees: `SMSTools/`, the original Qt3 application, and
`SMSTools-qt4/`, its Qt4 port that still used Qt3Support. This branch removes
the Qt3 application, moves `SMSTools-qt4/` to `SMSTools/` and ports it to
Qt6: the `Q3*` widgets are replaced by their Qt6 equivalents, and the
connections declared in the `.ui` files are made in the dialog constructors.
SMSConsole and SMSBatch do not use Qt.

## Status of the upstream components

| Component | SCons on `main` | This branch |
| --- | --- | --- |
| `clam_core`, `clam_processing`, `clam_audioio` | built | built with CMake |
| CLAM unit tests (`CLAM/test/UnitTests`) | built | built with CMake, run by CTest |
| CLAM functional tests, doxygen target | built | not ported |
| `clamrefactor`, `clamnetwork2code`, `clam_lv2_generator` scripts | installed (Python 2) | installed (Python 3) |
| CLAM plugins (`CLAM/plugins`) and examples (`CLAM/examples`) | SCons, against an installed CLAM | not ported; they can use the installed pkg-config files and SCons tool |
| NetworkEditor, `clam_qtmonitors` | built | built with CMake, Qt6 |
| Prototyper, OfflinePlayer, MonoOfflinePlayer, CheckClamNetwork | built | built with CMake |
| CLAMWidgets Qt Designer plugin | built | built with CMake |
| NetworkEditor network upgrader and IPython console | built by default (Python 2, PySide 1) | opt-in (`CLAM_NETWORKEDITOR_USE_PYTHON`: Python 3, Boost.Python, shiboken6); the upgrader is ported and tested, the console is not built in CI. Without Python, networks from older CLAM versions are loaded without upgrading them |
| NetworkEditor translations | built | built with CMake when Qt6 LinguistTools is found |
| SMSTools, SMSConsole, SMSBatch | `SMSTools-qt4/` | `SMSTools/`, built with CMake, Qt6 |
| Qt3 SMSTools (`SMSTools/` on `main`) | built | removed |
| Annotator, Voice2MIDI, chordata | SCons, Qt4 / Qt3Support | not ported; they still use `QGLWidget`, Qt3Support and SCons |
| pyclam, ipyclam, pyjack | SCons / Python 2 | SCons scripts updated to Python 3 and Qt6; not built by CMake |

## Known limitations

- The macOS and Windows packaging of the SCons scripts (bundles, NSIS,
  MinGW cross builds) has no CMake equivalent.
- `CLAM_WITH_DOUBLE=ON` (double precision `TData`) needs `CLAM_WITH_LADSPA=OFF`,
  because the LADSPA host and exporter only handle float samples. The
  libraries compile with it, but CI does not build or test it.
