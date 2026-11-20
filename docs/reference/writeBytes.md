# writeBytes

[Index](../index.md)

Writes bytes as they are, or a string with its length. Also covers `writeStringLE`.

```cpp
bool writeBytes(
    Writer* writer,
    const void* data,
    size_t size
);

bool writeStringLE(
    Writer* writer,
    const char* text,
    size_t length
);
```

`<o2/utils/bytes.h>`, namespace `o2::utils::bytes`

## Parameters

| | |
|---|---|
| `writer` | The [Writer](Writer.md) to write into. |
| `data` | The bytes. Can be `NULL` when `size` is 0. |
| `size` | How many. |
| `text` | The string's bytes, such as UTF-8 text. Doesn't need a terminating zero. Can be `NULL` when `length` is 0. |
| `length` | Bytes in `text`, at most 65535. |

## Return value

`true` if everything was written. `false`, writing nothing, if it doesn't fit, `length` is more than 65535, or the writer has already failed; the writer's `failed` is then `true`.

## Remarks

`writeBytes` writes the bytes and nothing else, so whoever reads them must know how many there are, such as from a count written before them, or because they're the rest of the message.

`writeStringLE` writes the length first, as a `u16` little-endian, then the bytes, without a terminating zero. [readStringLE](readBytes.md) reads it back. A string longer than 65535 bytes fails rather than being cut short.

## Example

```cpp
o2::utils::bytes::writeStringLE(&writer, chat, strlen(chat));

// A replay's hit timings: a count, then the raw values
o2::utils::bytes::writeU32LE(&writer, hitCount);
for (uint32_t i = 0; i < hitCount; i++) {
    o2::utils::bytes::writeI16LE(&writer, hitOffsetsMs[i]);
}
```

## See also

[readBytes](readBytes.md), [Writer](Writer.md), [writeU32LE](writeU32LE.md)
