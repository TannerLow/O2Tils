# writeU32LE

[Index](../index.md)

Writes a number, little-endian. Covers every number write: `writeU8`, `writeI8`, `writeU16LE`, `writeU32LE`, `writeU64LE`, `writeI16LE`, `writeI32LE`, `writeI64LE`, `writeF32LE` and `writeF64LE`.

```cpp
// Straight into memory
void writeU16LE(uint8_t* bytes, uint16_t value);
void writeU32LE(uint8_t* bytes, uint32_t value);
void writeU64LE(uint8_t* bytes, uint64_t value);
void writeI16LE(uint8_t* bytes, int16_t value);
void writeI32LE(uint8_t* bytes, int32_t value);
void writeI64LE(uint8_t* bytes, int64_t value);
void writeF32LE(uint8_t* bytes, float value);
void writeF64LE(uint8_t* bytes, double value);

// With a Writer
bool writeU8(Writer* writer, uint8_t value);
bool writeI8(Writer* writer, int8_t value);
bool writeU16LE(Writer* writer, uint16_t value);
bool writeU32LE(Writer* writer, uint32_t value);
bool writeU64LE(Writer* writer, uint64_t value);
bool writeI16LE(Writer* writer, int16_t value);
bool writeI32LE(Writer* writer, int32_t value);
bool writeI64LE(Writer* writer, int64_t value);
bool writeF32LE(Writer* writer, float value);
bool writeF64LE(Writer* writer, double value);
```

`<o2/utils/bytes.h>`, namespace `o2::utils::bytes`

## Parameters

| | |
|---|---|
| `bytes` | Where to write. Must have room for the value: 2, 4 or 8 bytes. Any address, aligned or not. |
| `writer` | The [Writer](Writer.md) to write into. |
| `value` | The number. |

## Return value

Straight into memory: none.

With a `Writer`: `true` if the value was written. `false`, writing nothing, if it doesn't fit, or the writer has already failed; the writer's `failed` is then `true`.

## Remarks

Every multi-byte number is written little-endian: lowest byte first, whatever the CPU. `U` numbers are unsigned, `I` numbers signed (two's complement), and the digits are the size in bits. `F32` and `F64` are `float` and `double`, written bit for bit in their IEEE 754 form, so they read back exactly, including infinities and NaN.

The straight-into-memory forms check nothing: `bytes` must have room. They suit fixed layouts, such as a header of known size. In Debug builds they assert that `bytes` isn't `NULL`.

## Example

```cpp
// A fixed 8-byte header, straight into memory
uint8_t header[8];
o2::utils::bytes::writeU16LE(header, version);
o2::utils::bytes::writeU16LE(header + 2, flags);
o2::utils::bytes::writeU32LE(header + 4, payloadSize);

// A message of several values, with a Writer
o2::utils::bytes::writeF32LE(&writer, accuracy);
o2::utils::bytes::writeU8(&writer, grade);
```

## See also

[readU32LE](readU32LE.md), [Writer](Writer.md), [writeBytes](writeBytes.md), [Bytes](../concepts/bytes.md)
