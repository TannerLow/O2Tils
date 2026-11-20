# readBytes

[Index](../index.md)

Reads bytes as they are, or a string with its length, without copying. Also covers `readStringLE`.

```cpp
const uint8_t* readBytes(
    Reader* reader,
    size_t size
);

const char* readStringLE(
    Reader* reader,
    size_t* length
);
```

`<o2/utils/bytes.h>`, namespace `o2::utils::bytes`

## Parameters

| | |
|---|---|
| `reader` | The [Reader](Reader.md) to read from. |
| `size` | How many bytes to read. |
| `length` | Filled in with the string's length, in bytes. |

## Return value

`readBytes`: a pointer to the next `size` bytes in the reader's buffer. `NULL`, setting the reader's `failed`, if they aren't all there or the reader has already failed. `NULL` without failing when `size` is 0.

`readStringLE`: a pointer to the string's bytes in the reader's buffer, with `length` set. `NULL` with `length` 0, setting `failed`, if the string isn't all there or the reader has already failed. `NULL` with `length` 0, without failing, for an empty string.

## Remarks

Nothing is copied: the pointer is into the reader's buffer, so it's valid as long as that buffer is. Copy what needs to last longer.

`readStringLE` reads what [writeStringLE](writeBytes.md) wrote: a `u16` little-endian length, then that many bytes. The string isn't followed by a terminating zero: print it with `%.*s`, or copy it and add one. Its bytes are whatever was written, so text from outside the program should be checked, such as being valid UTF-8, before it's shown.

## Example

```cpp
size_t nameLength = 0;
const char* name = o2::utils::bytes::readStringLE(&reader, &nameLength);
uint32_t hitCount = o2::utils::bytes::readU32LE(&reader);
if (reader.failed || hitCount > MAX_HITS) {
    return;
}
printf("%.*s hit %u notes\n", (int)nameLength, name, hitCount);
```

## See also

[writeBytes](writeBytes.md), [Reader](Reader.md), [readU32LE](readU32LE.md)
