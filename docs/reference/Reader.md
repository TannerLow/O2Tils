# Reader

[Index](../index.md)

Reads values from a buffer, front to back. Also covers `makeReader` and `getRemaining`.

```cpp
struct Reader {
    const uint8_t* bytes;
    size_t size;
    size_t position;
    bool failed;
};

Reader makeReader(const void* memory, size_t size);
size_t getRemaining(const Reader* reader);
```

`<o2/utils/bytes.h>`, namespace `o2::utils::bytes`

## Members

| | |
|---|---|
| `bytes` | The buffer. |
| `size` | The buffer's size, in bytes. |
| `position` | Bytes read so far. The next value starts at `bytes + position`. |
| `failed` | `true` once a read went past the end. Nothing is read from then on. |

## Parameters

| | |
|---|---|
| `memory` | The buffer to read, such as a received message's data. It must stay valid as long as the reader, and pointers it returned, are used. Can be `NULL` when `size` is 0. |
| `size` | Its size, in bytes. |
| `reader` | The reader. |

## Return value

`makeReader`: a reader at the start of `memory`, not failed.

`getRemaining`: bytes left to read, `size - position`; 0 once the reader has failed.

## Remarks

Read from it with [readU32LE and the other number reads](readU32LE.md), [readBytes](readBytes.md) and [readStringLE](readBytes.md), in the order the values were written.

A read past the end reads nothing, returns 0 or `NULL`, and sets `failed`; so does every read after it. Read a whole message, then check `failed` once: when it's `false`, every value read was really there.

That makes a reader safe for data from outside the program, such as a network message or a file: however short or wrong it is, the reader never reads out of bounds. It still pays to check the values themselves, such as a count before using it to size a loop.

A message with bytes left over after everything expected was read is often worth rejecting too: `getRemaining` says how many.

## Example

```cpp
o2::utils::bytes::Reader reader = o2::utils::bytes::makeReader(message.data, message.size);
uint32_t chartId = o2::utils::bytes::readU32LE(&reader);
uint32_t score = o2::utils::bytes::readU32LE(&reader);
if (reader.failed || o2::utils::bytes::getRemaining(&reader) != 0) {
    return; // not a score message
}
showScore(chartId, score);
```

## See also

[readU32LE](readU32LE.md), [readBytes](readBytes.md), [Writer](Writer.md), [Bytes](../concepts/bytes.md)
