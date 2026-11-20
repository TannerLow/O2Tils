# readU32LE

[Index](../index.md)

Reads a number, little-endian. Covers every number read: `readU8`, `readI8`, `readU16LE`, `readU32LE`, `readU64LE`, `readI16LE`, `readI32LE`, `readI64LE`, `readF32LE` and `readF64LE`.

```cpp
// Straight from memory
uint16_t readU16LE(const uint8_t* bytes);
uint32_t readU32LE(const uint8_t* bytes);
uint64_t readU64LE(const uint8_t* bytes);
int16_t readI16LE(const uint8_t* bytes);
int32_t readI32LE(const uint8_t* bytes);
int64_t readI64LE(const uint8_t* bytes);
float readF32LE(const uint8_t* bytes);
double readF64LE(const uint8_t* bytes);

// With a Reader
uint8_t readU8(Reader* reader);
int8_t readI8(Reader* reader);
uint16_t readU16LE(Reader* reader);
uint32_t readU32LE(Reader* reader);
uint64_t readU64LE(Reader* reader);
int16_t readI16LE(Reader* reader);
int32_t readI32LE(Reader* reader);
int64_t readI64LE(Reader* reader);
float readF32LE(Reader* reader);
double readF64LE(Reader* reader);
```

`<o2/utils/bytes.h>`, namespace `o2::utils::bytes`

## Parameters

| | |
|---|---|
| `bytes` | Where to read. Must hold the whole value: 2, 4 or 8 bytes. Any address, aligned or not. |
| `reader` | The [Reader](Reader.md) to read from. |

## Return value

The number. With a `Reader`: 0 if the value isn't all there, or the reader has already failed; the reader's `failed` is then `true`.

## Remarks

Every multi-byte number is read little-endian, as the matching [write](writeU32LE.md) wrote it, whatever the CPU. Read values in the order they were written.

A 0 from a `Reader` can be a real 0: check `failed`, once, after reading everything, to tell them apart.

The straight-from-memory forms check nothing: `bytes` must hold the value. For data from outside the program, such as a network message, use a `Reader`, which never reads out of bounds. In Debug builds they assert that `bytes` isn't `NULL`.

## Example

```cpp
o2::utils::bytes::Reader reader = o2::utils::bytes::makeReader(message.data, message.size);
uint16_t playerCount = o2::utils::bytes::readU16LE(&reader);
float averageAccuracy = o2::utils::bytes::readF32LE(&reader);
if (!reader.failed) {
    showLobby(playerCount, averageAccuracy);
}
```

## See also

[writeU32LE](writeU32LE.md), [Reader](Reader.md), [readBytes](readBytes.md), [Bytes](../concepts/bytes.md)
