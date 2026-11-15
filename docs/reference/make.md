# make

[Index](../index.md)

Makes an empty arena over a block of memory.

```cpp
Arena make(void* memory, size_t size);
```

`<o2/utils/arena.h>`, namespace `o2::utils::arena`

## Parameters

| | |
|---|---|
| `memory` | The block the arena hands out. It must stay valid as long as the arena is used. |
| `size` | Size of the block in bytes. |

## Return value

An [Arena](Arena.md) over `memory`, with nothing used.

## Remarks

`make` doesn't allocate, clear or touch the block. Freeing the block, if it needs freeing, is up to you, after you're done with the arena.

## Example

```cpp
static uint8_t scratchMemory[64 * 1024];
o2::utils::arena::Arena scratch = o2::utils::arena::make(scratchMemory, sizeof(scratchMemory));
```

## See also

[Arena](Arena.md), [reset](reset.md)
