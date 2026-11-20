# O2Tils

O2Tils is a small collection of utilities shared by the o2 libraries, such as O2Jam and O2Gui, and by the programs that use them. It's written in C-style C++: plain structs and functions, no hidden allocation, nothing running behind your back.

It depends on nothing but the C and C++ runtime, so any library can pull it in without pulling in anything else.

## Utilities

| Utility | Header | Namespace | What it is |
|---|---|---|---|
| Arena | `<o2/utils/arena.h>` | `o2::utils::arena` | A linear allocator over a block of memory you provide. Returns `NULL` when it runs out. |
| Bytes | `<o2/utils/bytes.h>` | `o2::utils::bytes` | Numbers and strings packed into a byte buffer and read back, little-endian on every CPU, for messages and files. Never reads or writes past the end. |

Every utility lives in `O2Tils/include/o2/utils/` and `O2Tils/src/o2/utils/`, in its own namespace under `o2::utils`. The full documentation is in [docs/](docs/index.md).

## Using O2Tils

### With PackageUpper

Add O2Tils to your project's `packageupper.txt`, pinned to a full commit SHA:

```
dep      https://github.com/TannerLow/O2Tils/tree/<full commit sha>
```

Then run PackageUpper's `install` from your project. O2Tils is copied into `deps/O2Tils/`, and `deps/deps.props` adds its include path and sources to your Visual Studio project.

If your project also depends on a library that uses O2Tils, such as O2Gui, both must name the same O2Tils commit. PackageUpper reports a conflict otherwise, listing which repository needs which commit.

### By hand

Put `O2Tils/include` on your include path and add the `.cpp` files from `O2Tils/src` to your project. Include headers as `#include <o2/utils/arena.h>`.

If your project also compiles another o2 library's sources, set Object File Name to `$(IntDir)%(RelativeDir)` (Project Properties → C/C++ → Output Files), so `.cpp` files with the same name in different libraries don't overwrite each other's object files.

## Repository layout

```
packageupper.txt         what O2Tils exports to projects that depend on it
O2Tils.sln
O2Tils/
    include/o2/utils/    public headers
    src/o2/utils/        implementations
    main.cpp             a demo; not exported
docs/
    index.md
    concepts/
    reference/           one page per function or struct
```

## Building

Open `O2Tils.sln` in Visual Studio 2022. Windows x64 is the main target; x86 builds too. The project builds `main.cpp`, a small demo of the utilities.

O2Tils builds as C++17, like the other o2 libraries. A program that compiles O2Tils's sources, directly or through PackageUpper, must build as C++17 too.

## Contributing

`CLAUDE.md` describes the coding style, layout and workflow. In short: one feature per branch and pull request, header changes are proposed with their docs before they're implemented, and a utility belongs here only when more than one library needs it.
