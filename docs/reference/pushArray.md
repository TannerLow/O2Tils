# pushArray

[Index](../index.md)

Takes room for an array from an arena.

```cpp
void* pushArray(Arena* arena, size_t sizePerElement, size_t count);
```

`<o2/utils/arena.h>`, namespace `o2::utils::arena`

## Parameters

| | |
|---|---|
| `arena` | The arena to take from. |
| `sizePerElement` | Size of one element in bytes. |
| `count` | How many elements. |

## Return value

A pointer to `sizePerElement × count` bytes, or `NULL` if the arena doesn't have room. On `NULL`, the arena is unchanged.

## Remarks

Like [push](push.md) with `sizePerElement × count`, without alignment. If `sizePerElement × count` is too large to fit in a `size_t`, it returns `NULL` rather than taking a wrapped-around size.

Use [pushArrayAligned](pushArrayAligned.md) for arrays of structs.

## Example

```cpp
uint8_t* flags = (uint8_t*)o2::utils::arena::pushArray(&level, sizeof(uint8_t), columnCount);
```

## See also

[push](push.md), [pushArrayAligned](pushArrayAligned.md), [Arenas](../concepts/arenas.md)
