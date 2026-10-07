# NDManager

NDManager (Neurophysiological Data Manager) is a simple graphical application designed to help
neurophysiologists manage their experimental recording parameters (e.g. acquisition system
sampling rate) and process their data (e.g. data filtering) with the
[NDManager plugins](https://github.com/neurosuite/ndmanager-plugins).

Developed by Lynn Hazan (main developer), Laurent Montel (Qt3 to Qt4/5 porting), David Faure
(Qt3 to Qt4/5 porting), Michaël Zugaro (plugins, maintenance), Florian Franzen (maintenance)
and Théotime de Charrin (Qt6 porting), distributed under the GNU General Public License v3 or
later.

If you use NDManager, please cite: L. Hazan, M. Zugaro, G. Buzsáki (2006). Klusters,
NeuroScope, NDManager: a free software suite for neurophysiological data processing and
visualization. *J Neurosci Methods* 155:207-216.

## Building

Requires CMake 3.16+, a C++17 compiler, Qt 6.4+ (Widgets, PrintSupport, Xml) and
[libneurosuite](https://github.com/neurosuite/libneurosuite) 3.x.

```bash
# with libneurosuite installed
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake --install build

# or let CMake fetch and build libneurosuite as part of NDManager
cmake -B build -S . -DNDMANAGER_BUNDLE_NEUROSUITE=ON
```

On Ubuntu 24.04 the build dependencies are `cmake ninja-build qt6-base-dev`.
With Nix: `nix build` or `nix develop`. See [CHANGELOG.md](CHANGELOG.md) for changes.
