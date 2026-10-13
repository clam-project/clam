# CLAM — C++ Library for Audio and Music

CLAM is a framework for research and application development in the audio and
music domain. It bundles a set of C++ libraries for the analysis, synthesis
and processing of audio signals together with several end-user applications
built on top of them.

This branch ports the project to a modern toolchain: CMake build (replacing
SCons), C++17, and Qt6 for the GUI applications. The original project page is
at <https://clam-project.org>; what follows is the up-to-date build and usage
information for this tree.

## Components

### Libraries (built by default)

| Target | Description |
| --- | --- |
| `clam_core` | Core data types, flow control and networks, factories, XML storage, threading, LADSPA plugin host and network exporter |
| `clam_processing` | Analysis, synthesis and transformation processings (spectral, SMS, tonal analysis, controls, MIDI, SDIF) |
| `clam_audioio` | Audio file codecs (libsndfile, libvorbis, libmad, id3lib), MIDI devices, and the JACK, PortAudio and LV2 network back ends |

### Applications (opt-in)

| App | Build flag | Description |
| --- | --- | --- |
| **NetworkEditor** | `-DCLAM_BUILD_GUI_APPS=ON` | Qt6 visual editor for building audio processing networks. Networks can be compiled into LADSPA plugins (needs an installed CLAM, see below). |
| **Prototyper** | `-DCLAM_BUILD_GUI_APPS=ON` | Runs a network with a QML interface (`CLAM.Widgets`), or a legacy Qt Designer `.ui` file. |
| **OfflinePlayer**, **MonoOfflinePlayer** | `-DCLAM_BUILD_GUI_APPS=ON` | Command line players that run a network over audio files. |
| **CheckClamNetwork** | `-DCLAM_BUILD_GUI_APPS=ON` | Command line checker that loads a network and reports whether it can run. |
| CLAMWidgets Designer plugin | `-DCLAM_BUILD_GUI_APPS=ON` | Makes the CLAM monitors available in Qt Designer. |
| **SMSTools** | `-DCLAM_BUILD_SMSTOOLS=ON` | Qt6 Spectral Modeling Synthesis tools: sinusoidal-plus-residual analysis, transformation scores, resynthesis. |
| **SMSConsole** | `-DCLAM_BUILD_SMSTOOLS=ON` | The SMSTools engine without a GUI. It is menu driven: it reads the menu choices and file names from standard input. |
| **SMSBatch** | `-DCLAM_BUILD_SMSTOOLS=ON` | Menu-driven batch driver: analyzes and/or synthesizes every configuration file found under a directory. |

### Not ported

These parts of the upstream project still rely on SCons and are not built by
CMake: `Annotator/`, `Voice2MIDI/`, `chordata/`, the CLAM plugins in
`CLAM/plugins/`, the examples in `CLAM/examples/`, `pyclam/` and `ipyclam/`.
NetworkEditor's Python features (upgrading networks saved by older CLAM
versions, and the IPython console) are opt-in and not built in CI; without
them, older networks are loaded as they are. See
[`CMAKE-MIGRATION.md`](CMAKE-MIGRATION.md) for details.

## Building

### Dependencies

On Ubuntu 24.04 (the CI runner), `apt install` the following:

```
build-essential cmake ninja-build pkg-config
libxerces-c-dev libfftw3-dev libsndfile1-dev libvorbis-dev libmad0-dev
libid3-3.8.3-dev lv2-dev libjack-jackd2-dev portaudio19-dev
libasound2-dev libgl1-mesa-dev ladspa-sdk
qt6-base-dev qt6-base-dev-tools qt6-svg-dev qt6-tools-dev
qt6-declarative-dev qt6-declarative-dev-tools
qml6-module-qtquick qml6-module-qtquick-controls qml6-module-qtquick-layouts
qml6-module-qtquick-window qml6-module-qtquick-templates qml6-module-qtqml-workerscript
```

Optional: `qt6-l10n-tools` (NetworkEditor translations), `libcppunit-dev`
(unit tests), `xvfb` (the GUI smoke tests); `python3-dev`,
`libboost-python-dev` and shiboken6 (with its pkg-config file) for
NetworkEditor's Python scripting.

Only the libraries need the first three lines; the Qt and OpenGL packages are
for the applications. Each optional library dependency has a `CLAM_WITH_*`
switch (see below); CMake stops with an error naming the missing package when
a switch is `ON` and its dependency is not found. Equivalents are available
on most other Linux distributions, and on macOS through Homebrew (the CI
workflow lists the packages). A Windows (MSVC, vcpkg) job is in the workflow
but disabled.

### Configure and build

Libraries only (default):

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

Everything, with the unit tests:

```sh
cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCLAM_BUILD_GUI_APPS=ON \
    -DCLAM_BUILD_SMSTOOLS=ON
cmake --build build -j$(nproc)
ctest --test-dir build --output-on-failure
```

The unit tests are built by default (`BUILD_TESTING`, which needs CppUnit);
`-DBUILD_TESTING=OFF` leaves them out. The unit tests that need the external
CLAM test data are skipped unless
`-DCLAM_TEST_DATA_PATH=<path to clam-test-data>` is given.

`CMAKE_BUILD_TYPE=Debug` also defines `_DEBUG`, which enables CLAM's debug
assertions (`CLAM_DEBUG_ASSERT`, array bounds checks), as the SCons debug
builds did.

### Running

The bundled example networks and SMS configurations refer to their audio
files with paths relative to `NetworkEditor/` and `SMSTools/`, so run the
applications from those directories (the applications' example file dialogs
open the installed copies under `<prefix>/share/networkeditor` and
`<prefix>/share/smstools`):

```sh
cd NetworkEditor && ../build/NetworkEditor/NetworkEditor example-data/tonalAnalysis-file.clamnetwork
cd SMSTools && ../build/SMSTools/SMSTools
```

Prototyper loads a same-named `.qml` interface automatically, then tries `.ui`
and finally its built-in playback interface. For example:

```sh
cd NetworkEditor
../build/NetworkEditor/Prototyper example-data/FilePlayer.clamnetwork
../build/NetworkEditor/Prototyper example-data/tonalAnalysis-file.clamnetwork --start-paused
# Select an interface explicitly, including legacy interfaces:
../build/NetworkEditor/Prototyper example-data/FilePlayer.clamnetwork example-data/FilePlayer.ui
```

Use `-b jack` or `-b portaudio` to select a backend; repeated
`-b` options try them in order. `--start-paused` waits for Play before
processing audio. `-o` runs a network without an interface. See
[`NetworkEditor/QML-WIDGETS.md`](NetworkEditor/QML-WIDGETS.md) for the component
catalog, binding properties and Qt Creator / QML preview workflow.

SMSConsole and SMSBatch print a menu and read the choices from standard
input, so they can also be scripted. For example, analyze and resynthesize
the bundled Elvis example (the output files are written to `example-data/`):

```sh
cd SMSTools
printf '%s\n' 1 example-data/Elvis-config.xml 3 9 10 0 | ../build/SMSTools/SMSConsole
```

### Installing

```sh
cmake --install build --prefix /usr/local
```

installs the libraries, the headers under `include/CLAM`, the applications,
their example data under `share/networkeditor` and `share/smstools`, desktop
files, man pages and translations. The libraries' SONAME carries the major
and minor version (`libclam_core.so.1.4`), as in the SCons builds, and the
installed programs find the libraries through a `$ORIGIN`-relative RPATH.

### Build options

| Option | Default | Effect |
| --- | --- | --- |
| `CLAM_BUILD_LIBRARIES` | `ON` | Build `clam_core`, `clam_processing`, `clam_audioio`. `OFF` builds the applications against an installed CLAM (`find_package(CLAM)`). |
| `CLAM_BUILD_GUI_APPS` | `OFF` | Build `clam_qtmonitors`, NetworkEditor, Prototyper, the command line tools and the Designer plugin. |
| `CLAM_BUILD_SMSTOOLS` | `OFF` | Build SMSTools, SMSConsole and SMSBatch. |
| `BUILD_TESTING` | `ON` | Build the CppUnit unit tests and the threading test and register them with CTest. Needs CppUnit. |
| `CLAM_TEST_DATA_PATH` | empty | Path to the CLAM test data; enables the unit tests that need it. |
| `CLAM_NETWORKEDITOR_BUILD_APPS` | `ON` | With GUI apps: build NetworkEditor, Prototyper and the command line tools, not only `clam_qtmonitors`. |
| `CLAM_NETWORKEDITOR_BUILD_DESIGNER_PLUGIN` | `ON` | With GUI apps: build the CLAMWidgets Qt Designer plugin. |
| `CLAM_NETWORKEDITOR_USE_PYTHON` | `OFF` | With GUI apps: Python scripting in NetworkEditor (the network upgrader and the IPython console). Needs python3-embed, Boost.Python and shiboken6; not built in CI. |
| `CLAM_XML_BACKEND` | `xercesc` | `xercesc`, or `none` to build without XML support. |
| `CLAM_WITH_FFTW3` / `CLAM_WITH_NR_FFT` | `ON` | FFT implementations. |
| `CLAM_WITH_OGGVORBIS` / `CLAM_WITH_MAD` / `CLAM_WITH_ID3` | `ON` | Ogg Vorbis and MP3 codecs; MP3 (libmad) needs id3lib. |
| `CLAM_WITH_JACK` / `CLAM_WITH_PORTAUDIO` / `CLAM_WITH_ALSA` | `ON` | Audio and MIDI back ends. |
| `CLAM_WITH_LADSPA` / `CLAM_WITH_LV2` | `ON` | LADSPA host and exporter, LV2 back end. |
| `CLAM_WITH_PORTMIDI` | `OFF` | PortMidi MIDI devices. |
| `CLAM_WITH_CHECKS` | `ON` | `OFF` defines `CLAM_DISABLE_CHECKS`, which removes the runtime checks. |
| `CLAM_WITH_RELEASE_ASSERTS` | `OFF` | Use the release assertions (no debug checks) in Debug builds too. |
| `CLAM_WITH_OPTIMIZE` | `OFF` | `CLAM_OPTIMIZE`: faster approximate math functions, losing precision. |
| `CLAM_WITH_DOUBLE` | `OFF` | Use `double` instead of `float` for `CLAM::TData`. Needs `CLAM_WITH_LADSPA=OFF`. |

libsndfile is always required. The CLAM libraries are always shared
libraries: their processings register themselves in factories from static
initializers, which a static link would drop.

## Using CLAM from other projects

An installed CLAM provides a CMake package:

```cmake
find_package(CLAM CONFIG REQUIRED)
target_link_libraries(myapp PRIVATE CLAM::processing CLAM::audioio)
# CLAM::qtmonitors and CLAM::qmlwidgets too, with the GUI apps
```

and pkg-config files (`clam_core`, `clam_processing`, `clam_audioio`), plus
the SCons tools in `share/clam/sconstools` (Python 3 / SCons 4) for SCons
projects such as the CLAM plugins, and the `clamrefactor`,
`clamnetwork2code` and `clam_lv2_generator` commands.
`CLAM/test/InstallConsumer` is a minimal example of the CMake package and
the pkg-config files.

A program that creates processings only through the factories (for example
from a network file) references no symbol of `clam_processing` or
`clam_audioio`, so linkers that default to `--as-needed` drop them; link such
programs with `-Wl,--no-as-needed`.

## Quick test recipe (SMSTools GUI)

```sh
cd SMSTools
../build/SMSTools/SMSTools
```

In the application:

1. **File → Load Configuration...** → pick `example-data/Elvis-config.xml`.
2. **SMS Analysis → Analyze**.
3. **SMS Transformation → Load Score...** → pick
   `example-data/tranformation-femaleAndTimeStretch.xml`.
4. **SMS Transformation → Apply**.
5. **SMS Synthesis → Synthesize**, then play the result from the view.

## Continuous integration

`.github/workflows/build.yml` runs on every push, pull request and manual
dispatch:

- Ubuntu 24.04, GCC, Release, everything enabled: builds, runs the unit
  tests and the smoke tests in `.github/scripts/smoke-tests.sh` (SMSConsole
  analysis, transformation and synthesis of the bundled example,
  `CheckClamNetwork` on the example networks, and NetworkEditor and SMSTools
  started under Xvfb, failing on Qt signal/slot connection errors),
  exercises Prototyper startup, legacy interfaces, every QML component and
  live network bindings in CTest, installs, and builds
  `CLAM/test/InstallConsumer` against the installed CMake package and
  pkg-config files.
- Ubuntu 24.04, clang, Debug, libraries and unit tests only, with the
  optional dependencies switched off.
- macOS, Release, everything enabled (Homebrew packages, without LADSPA and
  ALSA).
- A Windows job (MSVC, vcpkg, Qt from the online installer) is written but
  disabled.

## License

CLAM is distributed under the **GNU General Public License**, version 2 or
later, as stated in the header of each source file. See
`NetworkEditor/COPYING` and `SMSTools/COPYING` for the license notes.

## Further reading

- [`CMAKE-MIGRATION.md`](CMAKE-MIGRATION.md) — what changed in this branch
  relative to the SCons-based upstream, and what still needs porting.
- Upstream project page: <https://clam-project.org>
