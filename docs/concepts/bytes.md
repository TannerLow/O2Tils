# Bytes

[Index](../index.md)

## Why numbers need packing

A network message or a file is a row of bytes. A number in memory is too, but which byte comes first depends on the CPU: x86 and ARM store the lowest byte first (little-endian), some other CPUs the highest first (big-endian). Copying a number's memory straight into a message would mean different things on different machines, and copying a struct also copies padding the compiler added.

The bytes utility writes each value byte by byte, in one fixed order, **little-endian**, whatever the CPU. A message written on one machine reads back the same on any other, and a server in another language reads it with that language's little-endian tools, such as Python's `struct.unpack("<I", ...)`.

Every function that writes or reads a multi-byte number says so in its name: `writeU32LE`, `readF32LE`. Single bytes (`writeU8`, `readI8`) have no order, so no `LE`.

It costs nothing: compilers turn the byte-by-byte code into a single load or store on little-endian CPUs, and it works at any address, aligned or not.

## Three ways to use it

**Straight into memory**, when the layout is fixed and the room is known:

```cpp
uint8_t header[6];
o2::utils::bytes::writeU16LE(header, messageType);
o2::utils::bytes::writeU32LE(header + 2, messageSize);
```

**With a [Writer](../reference/Writer.md)**, to pack a whole message into a buffer you provide, one value after another:

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

**With a [Reader](../reference/Reader.md)**, to unpack one:

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

## Checking once, at the end

A writer or reader never goes past the end of its buffer. When a value doesn't fit, or isn't all there, it sets `failed`, and from then on does nothing: writes write nothing and return `false`, reads return 0. So the code can write or read a whole message without checking each value, then check `failed` once.

That matters most for reading. Messages come from outside the program, from a server or a file someone could have edited, so they can be short or malformed. A reader can't be made to read out of bounds, however wrong the message is; it just fails.

## Strings

[writeStringLE](../reference/writeBytes.md) writes a string as its length, a `u16` little-endian, then its bytes, with no terminating zero. So strings can be up to 65535 bytes. [readStringLE](../reference/readBytes.md) reads one back as a pointer into the message and a length, without copying: it isn't followed by a zero, so print it with `%.*s`, or copy it where it needs to last.

## Memory

Nothing here allocates. A writer writes into memory you provide, such as a stack array or a block from an [arena](arenas.md); a reader reads memory you already have, and hands back pointers into it.
