#pragma once
#include <stddef.h>
#include <stdint.h>

// Bytes: numbers and strings packed into a byte buffer, and read back, in
// little-endian byte order (lowest byte first) on every CPU. For network
// messages and files, which must mean the same on every machine. Functions
// ending in LE use little-endian order; 8-bit values have no byte order.
//
// Writer and Reader never go past the end of their buffer. When something
// doesn't fit, they set failed, and do nothing from then on, so a whole
// message can be written or read, and checked once at the end.

namespace o2 {
namespace utils {
namespace bytes {

    // Straight into memory, at any address, aligned or not. bytes must have
    // room for the value: 2, 4 or 8 bytes.

    void writeU16LE(uint8_t* bytes, uint16_t value);
    void writeU32LE(uint8_t* bytes, uint32_t value);
    void writeU64LE(uint8_t* bytes, uint64_t value);
    void writeI16LE(uint8_t* bytes, int16_t value);
    void writeI32LE(uint8_t* bytes, int32_t value);
    void writeI64LE(uint8_t* bytes, int64_t value);
    void writeF32LE(uint8_t* bytes, float value);  // bit for bit, IEEE 754
    void writeF64LE(uint8_t* bytes, double value);

    uint16_t readU16LE(const uint8_t* bytes);
    uint32_t readU32LE(const uint8_t* bytes);
    uint64_t readU64LE(const uint8_t* bytes);
    int16_t readI16LE(const uint8_t* bytes);
    int32_t readI32LE(const uint8_t* bytes);
    int64_t readI64LE(const uint8_t* bytes);
    float readF32LE(const uint8_t* bytes);
    double readF64LE(const uint8_t* bytes);

    // Writes into a buffer you provide, front to back.
    struct Writer {
        uint8_t* bytes;
        size_t capacity; // the buffer's size
        size_t size;     // bytes written so far
        bool failed;     // a write didn't fit; nothing is written from then on
    };

    Writer makeWriter(void* memory, size_t capacity);

    // Each returns false, writing nothing, if the value doesn't fit or the
    // writer has already failed.
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

    // Bytes as they are, with nothing before them. data can be NULL when
    // size is 0.
    bool writeBytes(
        Writer* writer,
        const void* data,
        size_t size
    );

    // A string: its length as a u16, little-endian, then its bytes, without
    // a terminating zero. Fails if length is more than 65535.
    bool writeStringLE(
        Writer* writer,
        const char* text,
        size_t length
    );

    // Reads from a buffer, front to back.
    struct Reader {
        const uint8_t* bytes;
        size_t size;     // the buffer's size
        size_t position; // bytes read so far
        bool failed;     // a read went past the end; nothing is read from then on
    };

    Reader makeReader(const void* memory, size_t size);

    // Each returns 0, and sets failed, if the value isn't all there or the
    // reader has already failed.
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

    // The next size bytes, as a pointer into the reader's buffer: nothing
    // is copied. NULL, and sets failed, if they aren't all there. NULL when
    // size is 0 too, without failing.
    const uint8_t* readBytes(
        Reader* reader,
        size_t size
    );

    // A string written by writeStringLE, as a pointer into the reader's
    // buffer and its length. Not followed by a terminating zero. NULL, with
    // length 0, and sets failed, if it isn't all there; NULL with length 0,
    // without failing, for an empty string.
    const char* readStringLE(
        Reader* reader,
        size_t* length
    );

    // Bytes left to read: 0 once the reader has failed.
    size_t getRemaining(const Reader* reader);

}
}
}
