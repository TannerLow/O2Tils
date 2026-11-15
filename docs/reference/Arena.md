# Arena

[Index](../index.md)

A linear allocator over a block of memory you provide.

```cpp
struct Arena {
    uint8_t* base;
    size_t size;
    size_t used;
};
```

`<o2/utils/arena.h>`, namespace `o2::utils::arena`

## Members

| | |
|---|---|
| `base` | Start of the memory block. |
| `size` | Size of the block in bytes. |
| `used` | Bytes handed out so far, including bytes skipped for alignment. The next allocation starts at `base + used`. |

## Remarks

Make one with [make](make.md) and take memory from it with [push](push.md), [pushArray](pushArray.md), [pushAligned](pushAligned.md) and [pushArrayAligned](pushArrayAligned.md). [reset](reset.md) frees everything at once. See [Arenas](../concepts/arenas.md).

The arena is a plain struct with no hidden state: it can live on the stack, in a global, or inside another struct, and copying it copies the counter, not the memory. Two copies of one arena hand out the same bytes, so pass it by pointer.

`size - used` is how many bytes are left, before any alignment.

## Example

```cpp
const size_t memorySize = 4 * 1024 * 1024;
void* memory = malloc(memorySize);
o2::utils::arena::Arena permanent = o2::utils::arena::make(memory, memorySize);

printf("%zu of %zu bytes used\n", permanent.used, permanent.size);
```

## See also

[make](make.md), [reset](reset.md), [push](push.md), [Arenas](../concepts/arenas.md)
