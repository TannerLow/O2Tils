# pushAligned

[Index](../index.md)

Takes bytes from an arena, starting at an address that's a multiple of `alignment`.

```cpp
void* pushAligned(Arena* arena, size_t size, size_t alignment);
```

`<o2/utils/arena.h>`, namespace `o2::utils::arena`

## Parameters

| | |
|---|---|
| `arena` | The arena to take from. |
| `size` | How many bytes to take. |
| `alignment` | What the address must be a multiple of, usually `alignof(T)`. Must be a power of 2. |

## Return value

A pointer to `size` bytes at an address that's a multiple of `alignment`, or `NULL` if the arena doesn't have room. On `NULL`, the arena is unchanged.

## Remarks

To reach an aligned address, `pushAligned` may skip up to `alignment - 1` bytes, which count as used. The skipped bytes count when deciding whether there's room.

An `alignment` that isn't a power of 2 is a bug in the calling code, not running out of room: it asserts in Debug builds, and the address returned in Release builds isn't guaranteed to be aligned.

The memory isn't cleared.

## Example

```cpp
o2::gui::Context* ui = (o2::gui::Context*)o2::utils::arena::pushAligned(
    &permanent,
    sizeof(o2::gui::Context),
    alignof(o2::gui::Context)
);
assert(ui != NULL);
```

## See also

[push](push.md), [pushArrayAligned](pushArrayAligned.md), [Arenas](../concepts/arenas.md)
