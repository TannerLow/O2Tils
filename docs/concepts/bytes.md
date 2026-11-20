# Bytes

[Index](../index.md)

## What it does

A network message or a file is a row of bytes, and both ends have to agree on what each byte means. The bytes utility turns numbers and strings into bytes in an order fixed by the utility, not by the CPU, and turns those bytes back into numbers and strings.

## Why not copy the memory

A number in memory is already bytes, so it's tempting to copy them straight into a message, or to copy a whole struct with `memcpy`. Two things go wrong:

- **Byte order depends on the CPU.** A `uint32_t` holding 1 is stored as `01 00 00 00` on x86 and ARM, which put the lowest byte first (little-endian), but as `00 00 00 01` on CPUs that put the highest first (big-endian). The same bytes would mean 1 to one machine and 16,777,216 to the other
- **A struct has gaps.** The compiler adds padding between fields to align them, and how much depends on the compiler and the platform. A Python server, for example, doesn't know where the gaps are

So the bytes utility writes each value by itself, byte by byte, in one fixed order, **little-endian**, whatever the CPU: 1 is always `01 00 00 00`. A message written on one machine reads back the same on any other, and a server in another language reads it with that language's little-endian tools, such as Python's `struct.unpack("<I", ...)`.

Every function that writes or reads a multi-byte number says so in its name: `writeU32LE`, `readF32LE`. Single bytes (`writeU8`, `readI8`) have no order, so no `LE`.

It costs nothing: compilers turn the byte-by-byte code into a single load or store on little-endian CPUs, and it works at any address, aligned or not.

## Three ways to use it

**Straight into memory**, when the layout is fixed and the room is known:

```cpp
uint8_t header[6];
o2::utils::bytes::writeU16LE(header, messageType);
o2::utils::bytes::writeU32LE(header + 2, messageSize);
```

**With a [Writer](../reference/Writer.md)**, to write a whole message into a buffer you provide, one value after another:

```cpp
uint8_t buffer[256];
o2::utils::bytes::Writer writer = o2::utils::bytes::makeWriter(buffer, sizeof(buffer));
o2::utils::bytes::writeU32LE(&writer, chartId);
o2::utils::bytes::writeU32LE(&writer, score);
o2::utils::bytes::writeStringLE(&writer, playerName, nameLength);
if (!writer.failed) {
    send(buffer, writer.size);
}
```

**With a [Reader](../reference/Reader.md)**, to read one back:

```cpp
o2::utils::bytes::Reader reader = o2::utils::bytes::makeReader(message.data, message.size);
uint32_t chartId = o2::utils::bytes::readU32LE(&reader);
uint32_t score = o2::utils::bytes::readU32LE(&reader);
size_t nameLength = 0;
const char* name = o2::utils::bytes::readStringLE(&reader, &nameLength);
if (reader.failed) {
    // The message was shorter than it should be: ignore it
}
```

## No extra copies

A Writer or Reader adds no buffer of its own.

**A Reader** is only a position and a size over bytes you already have, such as a received message's data. Each read turns bytes from there straight into the value it returns, so reading a score message into a struct is one step per field, with nothing copied in between:

```cpp
o2::utils::bytes::Reader reader = o2::utils::bytes::makeReader(message.data, message.size);
score.chartId = o2::utils::bytes::readU32LE(&reader);
score.points = o2::utils::bytes::readU32LE(&reader);
```

What it adds over reading at fixed offsets, such as `readU32LE(message.data + 4)`, is keeping track of where the next value starts, and checking the message is long enough, so the program doesn't have to.

**A Writer** writes into memory you provide, such as the array a message is sent from. That array is the message, ready to send; the Writer just keeps track of where the next value goes, and that it fits.

## Checking once, at the end

A writer or reader never goes past the end of its buffer. When a value doesn't fit, or isn't all there, it sets `failed`, and from then on does nothing: writes write nothing and return `false`, reads return 0. So the code can write or read a whole message without checking each value, then check `failed` once.

That matters most for reading. Messages come from outside the program, from a server or a file someone could have edited, so they can be short or malformed. A reader can't be made to read out of bounds, however wrong the message is; it just fails.

## Strings

[writeStringLE](../reference/writeBytes.md) writes a string as its length, a `u16` little-endian, then its bytes, with no terminating zero. So strings can be up to 65535 bytes. [readStringLE](../reference/readBytes.md) reads one back as a pointer into the message and a length, without copying: it isn't followed by a zero, so print it with `%.*s`, or copy it where it needs to last.

## Memory

Nothing here allocates. A writer writes into memory you provide, such as a stack array or a block from an [arena](arenas.md); a reader reads memory you already have, and hands back pointers into it.
