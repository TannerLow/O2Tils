# O2Tils

O2Tils holds small utilities that several o2 libraries and programs share, such as the arena allocator, so each library doesn't write its own. Other repositories pull it in as a dependency with PackageUpper, through the `packageupper.txt` at the repository root.

Because O2Tils sits at the bottom of the dependency tree, a change here reaches every library and program that uses it. Keep that in mind:
- O2Tils depends on nothing: no `dep` lines, no externals, no libraries to link beyond the C and C++ runtime
- A utility belongs here only if it's useful to more than one library. Code that only one library needs stays in that library
- Every project in a PackageUpper tree must agree on one O2Tils commit, so a breaking change to a header forces every library that uses O2Tils to update at once. Prefer adding functions over changing existing ones, and call out breaking changes in the pull request

# Coding style

Write C-style C++. The project uses C++ for its quality-of-life features, not its paradigm. Many C++ features add cognitive overhead and do things behind your back, so code should stay plain and explicit: a reader should be able to see what it does from what is written.

## C++ features

Fine to use:
- Namespaces
- Function overloading
- The C++ standard library, when reinventing the wheel isn't worth it

Avoid everything else by default, especially features that run or decide things implicitly, such as:
- Constructors, destructors, and RAII
- Inheritance and virtual functions
- Operator overloading and implicit conversions
- Exceptions
- Heavy template use

If a C++ feature seems worth using and isn't listed as fine, ask first.

The project builds as C++17, like every o2 library. That decides which features the compiler accepts, not which are fine to use: the lists above still decide that. A program compiles O2Tils's sources with its own settings, so it must build as C++17 too.

## Contain C++ usage

Keep C++ features from spreading. Using one in one place must not force the rest of the project, or the libraries that depend on O2Tils, to use C++ features too.
- Put C++ usage, including standard library types, inside the `.cpp` file that needs it, behind a C-like interface
- Headers expose plain structs and functions, not standard library types

## Project layout

Paths are relative to the project folder, `O2Tils/`:
- Header files go in `include/o2/utils/`
- Implementation files go in `src/o2/utils/`
- `include/` is on the include path, so include headers as `#include <o2/utils/name.h>`
- Add new `.cpp` files to `O2Tils.vcxproj` so they get compiled
- `main.cpp` is a demo of the library; other projects use only `include/` and `src/`
- Each utility gets its own header and `.cpp`, such as `arena.h` and `arena.cpp`, so a reader can find it by name

The `utils/` folders keep O2Tils's files apart from other o2 libraries, such as O2Jam and O2Gui, when one program uses several of them: two `o2/arena.h` files on the include path would silently shadow each other.

Visual Studio names each object file after its `.cpp` file only, so two `.cpp` files with the same name in one project overwrite each other's `.obj`, even in different folders. `O2Tils.vcxproj` must set Object File Name to `$(IntDir)%(RelativeDir)` so object files keep their folders. A program that compiles O2Tils's sources alongside another o2 library's needs the same setting; PackageUpper's `deps.props` does this for its dependencies.

## PackageUpper

`packageupper.txt` at the repository root says what O2Tils exports when another project depends on it. The format is described in PackageUpper's `README.md`.
- It exports `O2Tils/include` and `O2Tils/src`, and nothing else
- If a change adds a folder that dependents need, or moves one, update `packageupper.txt` in the same pull request
- `main.cpp` is outside the exported folders, so it is never copied into other projects

## Formatting

- When a function signature gets long, put one parameter per line, in the declaration and the definition:

```cpp
void* pushArrayAligned(
    Arena* arena,
    size_t sizePerElement,
    size_t count,
    size_t alignment
);
```

## Namespaces

Everything in these headers and their implementations goes in the `o2::utils` namespace, so the code can be reused in later projects without name conflicts, including with other o2 libraries. Each utility gets its own nested namespace named after it, such as `o2::utils::arena`.

## Memory

- Utilities never allocate on their own: no `malloc`, no `new`, no standard library containers. The caller provides the memory, as with the arena
- If a utility truly needs to allocate, ask first, and keep it to one clearly documented place
- Remember that standard library containers and some standard library functions allocate on their own

## Performance

- The libraries that use O2Tils run inside games whose loop runs very fast (1000+ frames per second). Functions called every frame, like pushing onto an arena, must be cheap: no allocation, no file access, no locks
- Keep hot functions small and predictable; check in Debug with `assert`, not with work that stays in Release builds
- Loading or setup functions are allowed to be slower; say so in their docs

## Documentation

The library is documented in `docs/` (repository root): concept pages in `docs/concepts/`, and one page per function or struct in `docs/reference/` with the sections Parameters, Return value, Remarks, Example and See also.
- Any change to a public header updates the matching docs pages in the same pull request
- New functions and structs get a reference page and an entry in `docs/index.md`
- New utilities get a line in the list of utilities in `README.md`
- Propose header changes, with their docs, before implementing them

## Workflow

- One feature per branch, with a descriptive branch name, and one pull request per feature
- For changes to public headers, open the pull request with the headers and docs only, as a proposal; once it's approved, add the implementation as a new commit on the same pull request
- Moving a utility out of another library, like the arena out of O2Gui, is one pull request here, then one in each library that switches to O2Tils
- Ask clarifying questions before designing a feature
