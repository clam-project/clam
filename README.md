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
| `clam_core` | Core data types, registries, XML storage, threading, factory system |
| `clam_processing` | Audio/spectral processing graph: analysis, synthesis, transformations, plugin export (LADSPA/LV2/VST hooks) |
| `clam_audioio` | Audio I/O backends (JACK, PortAudio, ALSA) and file codecs (libsndfile, libvorbis, libmad, id3lib) |

### Applications (opt-in)

| App | Build flag | Description |
| --- | --- | --- |
| **NetworkEditor** | `-DCLAM_BUILD_GUI_APPS=ON` | Qt6 visual editor for building audio processing networks. Networks can be exported as LADSPA / LV2 plugins. |
| **SMSTools** | `-DCLAM_BUILD_SMSTOOLS=ON` | Qt6 Spectral Modeling Synthesis tools: sinusoidal-plus-residual analysis, transformation scores, resynthesis. |
| **SMSBatch** | `-DCLAM_BUILD_SMSTOOLS=ON` | Console batch driver that walks a directory and applies SMS analysis/transformation/synthesis. |
| **SMSConsole** | `-DCLAM_BUILD_SMSTOOLS=ON` | Console SMS tool (no Qt) for scripted use. |

Other applications from the upstream project (`Annotator/`, `Voice2MIDI/`,
`chordata/`) still rely on SCons and have not been ported in this branch. See
[`CMAKE-MIGRATION.md`](CMAKE-MIGRATION.md) for the migration status.

## Building

### Dependencies

On Ubuntu 24.04 (`ubuntu-latest` in CI), `apt install` the following:

```
build-essential cmake ninja-build pkg-config
libxerces-c-dev libfftw3-dev libsndfile1-dev libvorbis-dev libmad0-dev
libid3-3.8.3-dev lv2-dev libjack-jackd2-dev portaudio19-dev
libasound2-dev libgl1-mesa-dev ladspa-sdk
qt6-base-dev qt6-base-dev-tools libqt6opengl6-dev qt6-svg-dev qt6-tools-dev
```

Equivalents are available on most other Linux distributions; the CMake checks
above will fail with a clear message if any required dep is missing.

### Configure and build

Libraries only (default):

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

Libraries + NetworkEditor:

```sh
cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCLAM_BUILD_GUI_APPS=ON
cmake --build build -j$(nproc)
```

Libraries + NetworkEditor + SMSTools (everything):

```sh
cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCLAM_BUILD_GUI_APPS=ON \
    -DCLAM_BUILD_SMSTOOLS=ON
cmake --build build -j$(nproc)
```

Run the binaries from the build tree:

```sh
./build/NetworkEditor/NetworkEditor
./build/SMSTools/SMSTools
./build/SMSTools/SMSBatch   <directory>
./build/SMSTools/SMSConsole <config.xml>
```

### Build options

| Option | Default | Effect |
| --- | --- | --- |
| `CLAM_BUILD_LIBRARIES` | `ON` | Build `clam_core`, `clam_processing`, `clam_audioio`. |
| `CLAM_BUILD_GUI_APPS` | `OFF` | Build `clam_qtmonitors` and `NetworkEditor`. |
| `CLAM_BUILD_SMSTOOLS` | `OFF` | Build `SMSTools`, `SMSBatch`, `SMSConsole`. |
| `CLAM_NETWORKEDITOR_BUILD_APPS` | `ON` (with GUI apps) | Whether to link the `NetworkEditor` executable in addition to `clam_qtmonitors`. |
| `CLAM_WITH_LV2` / `CLAM_WITH_JACK` / `CLAM_WITH_ID3` / `CLAM_WITH_MAD` / `CLAM_WITH_PORTAUDIO` / `CLAM_WITH_ALSA` / `CLAM_WITH_SNDFILE` / `CLAM_WITH_OGGVORBIS` / `CLAM_WITH_LADSPA` / `CLAM_WITH_FFTW3` / `CLAM_WITH_NR_FFT` | `ON` | Per-backend toggles. When `ON` and the dep is missing, CMake fails at configure time with the missing-package name. |
| `BUILD_SHARED_LIBS` | `ON` | Build the CLAM libraries as shared (`.so`) or static (`.a`). |

## Quick test recipe (SMSTools GUI)

```sh
./build/SMSTools/SMSTools
```

In the application:

1. **File → Load Configuration** → pick `SMSTools/example-data/Elvis-config.xml`.
2. **SMS Analysis → Analyze**.
3. **File → Load Transformation Score** → pick e.g.
   `<path-to-clam_data>/SMSTests/freqshift-transf.xml` (or any other
   `*-transf.xml` that does not need a morph sound).
4. **SMS Transformation → Apply**.
5. **SMS Synthesis → Synthesize**.

Bundled audio + matching configs live in `SMSTools/example-data/`.

## Continuous integration

`.github/workflows/build.yml` runs on every push, PR and manual dispatch. It
installs the apt deps listed above, configures with all three
`CLAM_BUILD_*` flags ON, builds with Ninja, and lists the produced binaries so
a green run visibly proves that `NetworkEditor`, `SMSTools`, `SMSBatch` and
`SMSConsole` all linked.

## License

CLAM is distributed under the **GNU General Public License**. See the
`COPYING` files in each subtree for the full text.

## Further reading

- [`CMAKE-MIGRATION.md`](CMAKE-MIGRATION.md) — what changed in this branch
  relative to the SCons-based upstream, and which apps still need porting.
- Upstream project page: <https://clam-project.org>
