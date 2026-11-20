# Writer

[Index](../index.md)

Writes values into a buffer you provide, front to back. Also covers `makeWriter`.

```cpp
struct Writer {
    uint8_t* bytes;
    size_t capacity;
    size_t size;
    bool failed;
};

Writer makeWriter(void* memory, size_t capacity);
```

`<o2/utils/bytes.h>`, namespace `o2::utils::bytes`

## Members

| | |
|---|---|
| `bytes` | The buffer. |
| `capacity` | The buffer's size, in bytes. |
| `size` | Bytes written so far. The next value goes at `bytes + size`. |
| `failed` | `true` once a write didn't fit. Nothing is written from then on. |

## Parameters

| | |
|---|---|
| `memory` | The buffer to write into. It must stay valid as long as the writer is used. |
| `capacity` | Its size, in bytes. |

## Return value

`makeWriter`: an empty writer over `memory`: `size` 0, not failed.

## Remarks

Write into it with [writeU32LE and the other number writes](writeU32LE.md), [writeBytes](writeBytes.md) and [writeStringLE](writeBytes.md). Each value goes right after the last, with nothing between them.

A write that doesn't fit writes nothing, sets `failed`, and returns `false`; so does every write after it. Write a whole message, then check `failed` once: when it's `false`, `bytes` holds `size` bytes of the message.

`makeWriter` doesn't clear or touch the buffer. The writer is a plain struct: to write the same buffer again from the start, make a new one.

## Example

```cpp
uint8_t buffer[64];
o2::utils::bytes::Writer writer = o2::utils::bytes::makeWriter(buffer, sizeof(buffer));
o2::utils::bytes::writeU32LE(&writer, chartId);
o2::utils::bytes::writeU32LE(&writer, score);
o2::utils::bytes::writeU16LE(&writer, maxCombo);
if (!writer.failed) {
    sendMessage(MESSAGE_SCORE, buffer, writer.size);
}
```

## See also

[writeU32LE](writeU32LE.md), [writeBytes](writeBytes.md), [Reader](Reader.md), [Bytes](../concepts/bytes.md)
