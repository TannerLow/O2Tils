# pushArrayAligned

[Index](../index.md)

Takes room for an array from an arena, starting at an address that's a multiple of `alignment`.

```cpp
void* pushArrayAligned(
    Arena* arena,
    size_t sizePerElement,
    size_t count,
    size_t alignment
);
```

`<o2/utils/arena.h>`, namespace `o2::utils::arena`

## Parameters

| | |
|---|---|
| `arena` | The arena to take from. |
| `sizePerElement` | Size of one element in bytes. |
| `count` | How many elements. |
| `alignment` | What the address must be a multiple of, usually `alignof(T)`. Must be a power of 2. |

## Return value

A pointer to `sizePerElement × count` bytes at an address that's a multiple of `alignment`, or `NULL` if the arena doesn't have room. On `NULL`, the arena is unchanged.

## Remarks

Like [pushAligned](pushAligned.md) with `sizePerElement × count`. If `sizePerElement × count` is too large to fit in a `size_t`, it returns `NULL` rather than taking a wrapped-around size.

## Example

```cpp
Note* notes = (Note*)o2::utils::arena::pushArrayAligned(&level, sizeof(Note), noteCount, alignof(Note));
if (notes == NULL) {
    return false;
}
```

## See also

[pushArray](pushArray.md), [pushAligned](pushAligned.md), [Arenas](../concepts/arenas.md)
