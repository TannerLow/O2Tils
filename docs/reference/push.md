# push

[Index](../index.md)

Takes bytes from an arena.

```cpp
void* push(Arena* arena, size_t size);
```

`<o2/utils/arena.h>`, namespace `o2::utils::arena`

## Parameters

| | |
|---|---|
| `arena` | The arena to take from. |
| `size` | How many bytes to take. |

## Return value

A pointer to `size` bytes, or `NULL` if the arena doesn't have room. On `NULL`, the arena is unchanged.

## Remarks

The bytes start right where the last allocation ended, without alignment, so use `push` for bytes such as a copy of a string. Use [pushAligned](pushAligned.md) for structs.

The memory isn't cleared. A `size` of 0 succeeds without taking anything, and returns where the next allocation will start.

## Example

```cpp
size_t length = strlen(name);
char* copy = (char*)o2::utils::arena::push(&frame, length + 1);
if (copy != NULL) {
    memcpy(copy, name, length + 1);
}
```

## See also

[pushArray](pushArray.md), [pushAligned](pushAligned.md), [Arenas](../concepts/arenas.md)
