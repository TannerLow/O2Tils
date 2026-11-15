# Arenas

[Index](../index.md)

## What an arena is

An [Arena](../reference/Arena.md) hands out memory from one block that you provide, front to back. Allocating is just moving a counter forward, so it's very fast and can't fragment. There's no way to free one allocation; everything goes at once with [reset](../reference/reset.md).

```
base                         base + used                  base + size
|------- handed out --------|---------- free ------------|
```

The arena never allocates or frees the block itself. Where the block comes from is up to you: `malloc` once at startup, a static array, or a piece of another arena. It must stay valid as long as the arena is used.

## Which memory fits an arena

Arenas fit memory whose lifetime is clear and shared by everything in it:

- **Permanent memory**: things that live for the whole program, pushed once at startup and never reset
- **Per-frame memory**: things that are only needed for one frame, such as O2Gui's draw commands, pushed during the frame and reset at the start of the next one
- **Per-level or per-song memory**: things loaded together and thrown away together, reset when the level or song ends

Memory that's freed piece by piece, in no particular order, doesn't fit an arena.

## Alignment

[push](../reference/push.md) and [pushArray](../reference/pushArray.md) don't align: they start right where the last allocation ended. That's fine for bytes, such as a copy of a string, but a struct needs an address that's a multiple of its alignment. Use [pushAligned](../reference/pushAligned.md) or [pushArrayAligned](../reference/pushArrayAligned.md) with `alignof(T)` for structs:

```cpp
Player* player = (Player*)o2::utils::arena::pushAligned(&permanent, sizeof(Player), alignof(Player));
```

Aligning may skip a few bytes, which count as used.

## Running out

When an arena doesn't have room for a push, the push returns `NULL` and leaves the arena unchanged, in Debug and Release builds alike. Nothing is written past the end of the block. A failed push doesn't spoil the arena: a smaller push afterwards can still succeed.

Check the result wherever running out is possible. Where running out would be a bug, such as memory sized up front for a known amount of data, an `assert` on the result is enough:

```cpp
Note* notes = (Note*)o2::utils::arena::pushArrayAligned(&level, sizeof(Note), noteCount, alignof(Note));
if (notes == NULL) {
    // the chart has more notes than the level arena has room for
    return false;
}
```

The memory isn't cleared, neither when it's pushed nor when the arena is reset. Set every field of what you push.

## Cost

Every push is a few additions and comparisons. It doesn't allocate, lock or touch the memory it hands out, so it's fine to call many times a frame, inside game loops running at 1000+ frames per second.
