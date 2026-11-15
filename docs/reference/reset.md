# reset

[Index](../index.md)

Frees everything in an arena at once.

```cpp
void reset(Arena* arena);
```

`<o2/utils/arena.h>`, namespace `o2::utils::arena`

## Parameters

| | |
|---|---|
| `arena` | The arena to empty. |

## Return value

None.

## Remarks

`reset` sets `used` back to 0, so the next push starts at the beginning of the block again. Every pointer handed out before is now pointing at memory that the next pushes reuse: stop using them.

The memory isn't cleared.

## Example

```cpp
while (running) {
    o2::utils::arena::reset(&frame);

    // everything pushed onto frame from here on lives until the next frame
    updateAndDraw(&frame);
}
```

## See also

[Arena](Arena.md), [make](make.md), [Arenas](../concepts/arenas.md)
