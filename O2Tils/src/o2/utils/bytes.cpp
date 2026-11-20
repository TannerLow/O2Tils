#include <o2/utils/bytes.h>
#include <assert.h>
#include <string.h>

// Every value is written and read one byte at a time with shifts, which work
// on the number's value, not on how the CPU stores it, so the bytes come out
// little-endian on every CPU. Compilers turn these into a single store or load
// on little-endian CPUs.
//
// Signed numbers and floats go through the unsigned integer of the same size
// with memcpy, which copies their bits exactly.

namespace o2 {
namespace utils {
namespace bytes {

    // Straight into memory

    void writeU16LE(uint8_t* bytes, uint16_t value) {
        assert(bytes != NULL);
        bytes[0] = (uint8_t)(value & 0xff);
        bytes[1] = (uint8_t)(value >> 8);
    }

    void writeU32LE(uint8_t* bytes, uint32_t value) {
        assert(bytes != NULL);
        bytes[0] = (uint8_t)(value & 0xff);
        bytes[1] = (uint8_t)((value >> 8) & 0xff);
        bytes[2] = (uint8_t)((value >> 16) & 0xff);
        bytes[3] = (uint8_t)(value >> 24);
    }

    void writeU64LE(uint8_t* bytes, uint64_t value) {
        // Written out rather than as a loop: compilers only turn the
        // written-out form into a single store
        assert(bytes != NULL);
        bytes[0] = (uint8_t)(value & 0xff);
        bytes[1] = (uint8_t)((value >> 8) & 0xff);
        bytes[2] = (uint8_t)((value >> 16) & 0xff);
        bytes[3] = (uint8_t)((value >> 24) & 0xff);
        bytes[4] = (uint8_t)((value >> 32) & 0xff);
        bytes[5] = (uint8_t)((value >> 40) & 0xff);
        bytes[6] = (uint8_t)((value >> 48) & 0xff);
        bytes[7] = (uint8_t)(value >> 56);
    }

    void writeI16LE(uint8_t* bytes, int16_t value) {
        uint16_t bits;
        memcpy(&bits, &value, sizeof(bits));
        writeU16LE(bytes, bits);
    }

    void writeI32LE(uint8_t* bytes, int32_t value) {
        uint32_t bits;
        memcpy(&bits, &value, sizeof(bits));
        writeU32LE(bytes, bits);
    }

    void writeI64LE(uint8_t* bytes, int64_t value) {
        uint64_t bits;
        memcpy(&bits, &value, sizeof(bits));
        writeU64LE(bytes, bits);
    }

    void writeF32LE(uint8_t* bytes, float value) {
        static_assert(sizeof(float) == 4, "float must be 32 bits");
        uint32_t bits;
        memcpy(&bits, &value, sizeof(bits));
        writeU32LE(bytes, bits);
    }

    void writeF64LE(uint8_t* bytes, double value) {
        static_assert(sizeof(double) == 8, "double must be 64 bits");
        uint64_t bits;
        memcpy(&bits, &value, sizeof(bits));
        writeU64LE(bytes, bits);
    }

    uint16_t readU16LE(const uint8_t* bytes) {
        assert(bytes != NULL);
        return (uint16_t)(bytes[0] | (bytes[1] << 8));
    }

    uint32_t readU32LE(const uint8_t* bytes) {
        assert(bytes != NULL);
        return (uint32_t)bytes[0]
            | ((uint32_t)bytes[1] << 8)
            | ((uint32_t)bytes[2] << 16)
            | ((uint32_t)bytes[3] << 24);
    }

    uint64_t readU64LE(const uint8_t* bytes) {
        // Written out rather than as a loop, as in writeU64LE
        assert(bytes != NULL);
        return (uint64_t)bytes[0]
            | ((uint64_t)bytes[1] << 8)
            | ((uint64_t)bytes[2] << 16)
            | ((uint64_t)bytes[3] << 24)
            | ((uint64_t)bytes[4] << 32)
            | ((uint64_t)bytes[5] << 40)
            | ((uint64_t)bytes[6] << 48)
            | ((uint64_t)bytes[7] << 56);
    }

    int16_t readI16LE(const uint8_t* bytes) {
        uint16_t bits = readU16LE(bytes);
        int16_t value;
        memcpy(&value, &bits, sizeof(value));
        return value;
    }

    int32_t readI32LE(const uint8_t* bytes) {
        uint32_t bits = readU32LE(bytes);
        int32_t value;
        memcpy(&value, &bits, sizeof(value));
        return value;
    }

    int64_t readI64LE(const uint8_t* bytes) {
        uint64_t bits = readU64LE(bytes);
        int64_t value;
        memcpy(&value, &bits, sizeof(value));
        return value;
    }

    float readF32LE(const uint8_t* bytes) {
        uint32_t bits = readU32LE(bytes);
        float value;
        memcpy(&value, &bits, sizeof(value));
        return value;
    }

    double readF64LE(const uint8_t* bytes) {
        uint64_t bits = readU64LE(bytes);
        double value;
        memcpy(&value, &bits, sizeof(value));
        return value;
    }

    // Writer

    Writer makeWriter(void* memory, size_t capacity) {
        assert(memory != NULL || capacity == 0);
        Writer writer;
        writer.bytes = (uint8_t*)memory;
        writer.capacity = capacity;
        writer.size = 0;
        writer.failed = false;
        return writer;
    }

    // Takes size bytes at the end of what's written, or NULL, failing the
    // writer, if they don't fit. Compares without adding, so nothing wraps.
    static uint8_t* reserve(Writer* writer, size_t size) {
        if (writer->failed || size > writer->capacity - writer->size) {
            writer->failed = true;
            return NULL;
        }
        uint8_t* start = writer->bytes + writer->size;
        writer->size += size;
        return start;
    }

    bool writeU8(Writer* writer, uint8_t value) {
        uint8_t* at = reserve(writer, 1);
        if (at == NULL) {
            return false;
        }
        at[0] = value;
        return true;
    }

    bool writeI8(Writer* writer, int8_t value) {
        return writeU8(writer, (uint8_t)value);
    }

    bool writeU16LE(Writer* writer, uint16_t value) {
        uint8_t* at = reserve(writer, 2);
        if (at == NULL) {
            return false;
        }
        writeU16LE(at, value);
        return true;
    }

    bool writeU32LE(Writer* writer, uint32_t value) {
        uint8_t* at = reserve(writer, 4);
        if (at == NULL) {
            return false;
        }
        writeU32LE(at, value);
        return true;
    }

    bool writeU64LE(Writer* writer, uint64_t value) {
        uint8_t* at = reserve(writer, 8);
        if (at == NULL) {
            return false;
        }
        writeU64LE(at, value);
        return true;
    }

    bool writeI16LE(Writer* writer, int16_t value) {
        uint8_t* at = reserve(writer, 2);
        if (at == NULL) {
            return false;
        }
        writeI16LE(at, value);
        return true;
    }

    bool writeI32LE(Writer* writer, int32_t value) {
        uint8_t* at = reserve(writer, 4);
        if (at == NULL) {
            return false;
        }
        writeI32LE(at, value);
        return true;
    }

    bool writeI64LE(Writer* writer, int64_t value) {
        uint8_t* at = reserve(writer, 8);
        if (at == NULL) {
            return false;
        }
        writeI64LE(at, value);
        return true;
    }

    bool writeF32LE(Writer* writer, float value) {
        uint8_t* at = reserve(writer, 4);
        if (at == NULL) {
            return false;
        }
        writeF32LE(at, value);
        return true;
    }

    bool writeF64LE(Writer* writer, double value) {
        uint8_t* at = reserve(writer, 8);
        if (at == NULL) {
            return false;
        }
        writeF64LE(at, value);
        return true;
    }

    bool writeBytes(
        Writer* writer,
        const void* data,
        size_t size
    ) {
        if (size > 0 && data == NULL) {
            assert(false && "writeBytes: data is NULL but size isn't 0");
            writer->failed = true;
            return false;
        }
        if (size == 0) {
            // Nothing to write: fine, even for an empty writer whose memory is NULL
            return !writer->failed;
        }
        uint8_t* at = reserve(writer, size);
        if (at == NULL) {
            return false;
        }
        memcpy(at, data, size);
        return true;
    }

    bool writeStringLE(
        Writer* writer,
        const char* text,
        size_t length
    ) {
        if (length > 0xffff || (length > 0 && text == NULL)) {
            writer->failed = true;
            return false;
        }
        // The length and the bytes together, so it's all written or none of it
        uint8_t* at = reserve(writer, 2 + length);
        if (at == NULL) {
            return false;
        }
        writeU16LE(at, (uint16_t)length);
        if (length > 0) {
            memcpy(at + 2, text, length);
        }
        return true;
    }

    // Reader

    Reader makeReader(const void* memory, size_t size) {
        assert(memory != NULL || size == 0);
        Reader reader;
        reader.bytes = (const uint8_t*)memory;
        reader.size = size;
        reader.position = 0;
        reader.failed = false;
        return reader;
    }

    // Takes the next size bytes, or NULL, failing the reader, if they aren't
    // all there. Compares without adding, so nothing wraps.
    static const uint8_t* take(Reader* reader, size_t size) {
        if (reader->failed || size > reader->size - reader->position) {
            reader->failed = true;
            return NULL;
        }
        const uint8_t* start = reader->bytes + reader->position;
        reader->position += size;
        return start;
    }

    uint8_t readU8(Reader* reader) {
        const uint8_t* at = take(reader, 1);
        return at != NULL ? at[0] : 0;
    }

    int8_t readI8(Reader* reader) {
        return (int8_t)readU8(reader);
    }

    uint16_t readU16LE(Reader* reader) {
        const uint8_t* at = take(reader, 2);
        return at != NULL ? readU16LE(at) : 0;
    }

    uint32_t readU32LE(Reader* reader) {
        const uint8_t* at = take(reader, 4);
        return at != NULL ? readU32LE(at) : 0;
    }

    uint64_t readU64LE(Reader* reader) {
        const uint8_t* at = take(reader, 8);
        return at != NULL ? readU64LE(at) : 0;
    }

    int16_t readI16LE(Reader* reader) {
        const uint8_t* at = take(reader, 2);
        return at != NULL ? readI16LE(at) : 0;
    }

    int32_t readI32LE(Reader* reader) {
        const uint8_t* at = take(reader, 4);
        return at != NULL ? readI32LE(at) : 0;
    }

    int64_t readI64LE(Reader* reader) {
        const uint8_t* at = take(reader, 8);
        return at != NULL ? readI64LE(at) : 0;
    }

    float readF32LE(Reader* reader) {
        const uint8_t* at = take(reader, 4);
        return at != NULL ? readF32LE(at) : 0.0f;
    }

    double readF64LE(Reader* reader) {
        const uint8_t* at = take(reader, 8);
        return at != NULL ? readF64LE(at) : 0.0;
    }

    const uint8_t* readBytes(
        Reader* reader,
        size_t size
    ) {
        if (size == 0) {
            return NULL;
        }
        return take(reader, size);
    }

    const char* readStringLE(
        Reader* reader,
        size_t* length
    ) {
        assert(length != NULL);
        *length = 0;
        uint16_t stored = readU16LE(reader);
        if (reader->failed || stored == 0) {
            return NULL;
        }
        const uint8_t* text = take(reader, stored);
        if (text == NULL) {
            return NULL;
        }
        *length = stored;
        return (const char*)text;
    }

    size_t getRemaining(const Reader* reader) {
        return reader->failed ? 0 : reader->size - reader->position;
    }

}
}
}
