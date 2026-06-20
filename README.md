# Avendish processor template (C++20 modules branch)

This provides a basic, canonical template for making objects with [Avendish](https://github.com/celtera/avendish).

This `cxx_modules` branch writes the object as a **C++20 module**: the processor
lives in [`src/Processor.cppm`](src/Processor.cppm) (`export module my_processor;
import halp;`) and every Avendish back-end's generated prototype `import`s it
instead of `#include`-ing a header. Avendish compiles the slim `halp` module and
the object into each back-end target in that back-end's own exception/rtti
dialect, so the one module serves the exception back-ends (dump / Python / ossia)
and the `-fno-exceptions -fno-rtti` plug-in back-ends (CLAP / VST3 / Pd / Max /
…) alike.

Two things differ from the header template:

* **Build with Ninja and the module scanner on** — the CMake here enforces this:
  ```bash
  cmake -G Ninja -DCMAKE_CXX_SCAN_FOR_MODULES=ON -B build -S .
  cmake --build build
  ```
  (The Makefiles generator silently drops `-fmodules`.) A compiler with C++20
  module support is required; validated with GCC 16.
* **Metadata is `static consteval`, not the `halp_meta` macro** — a module's
  `import` does not carry macros across, so the module spells out what
  `halp_meta` expands to. On a C++26-reflection compiler this becomes
  `[[=halp::meta(...)]]`.

Note that some libraries are needed to access the various back-ends. They can be passed to CMake.

These instructions are mostly useful on Windows / macOS as on Linux one can just install the packages from the OS package manager.

For instance, on Arch Linux:

```bash
$ sudo pacman -S vst3sdk pd pybind11
```

will install things.

To see a complete build procedure, one can refer to the [Github actions workflows](.github/workflows/), which compile 
the project on clean virtual machines.

## Python
 
[Get it there](https://github.com/pybind/pybind11) and pass to cmake:
```cpp
-Dpybind11_DIR=path/to/pybind11
```

## VST 3 SDK

[Get it there](https://github.com/steinbergmedia/vst3sdk) and pass to cmake:

```cpp
-DVST3_SDK_ROOT=path/to/vst3
```

## Max SDK

[Get it there](https://cycling74.com/downloads/sdk) and pass to cmake:
```cmake
-DAVND_MAXSDK_PATH=path/to/maxsdk
```

## PureData

[Get it there](https://github.com/pure-data/pure-data), build it, and pass to cmake:

```cmake
-DCMAKE_PREFIX_PATH=path/to/the/folder/containing/m_pd.h
```

## TODO

* Support importing them with a package manager
* Audio support for standalone