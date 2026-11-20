// A demo of O2Tils: prints what each utility does, and checks it along the way.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <o2/utils/arena.h>
#include <o2/utils/bytes.h>

namespace arena = o2::utils::arena;
namespace bytes = o2::utils::bytes;

struct Note {
    double time;
    int column;
};

static void arenaDemo() {
    printf("Arena\n");

    const size_t memorySize = 1024;
    void* memory = malloc(memorySize);
    arena::Arena level = arena::make(memory, memorySize);

    // bytes: no alignment, starts right where the last push ended
    char* title = (char*)arena::push(&level, 6);
    assert(title != NULL);
    snprintf(title, 6, "Intro");
    printf("  pushed \"%s\", %zu of %zu bytes used\n", title, level.used, level.size);

    // structs: aligned, skipping a few bytes after the title
    Note* notes = (Note*)arena::pushArrayAligned(&level, sizeof(Note), 32, alignof(Note));
    assert(notes != NULL);
    assert((uintptr_t)notes % alignof(Note) == 0);
    for (int i = 0; i < 32; i++) {
        notes[i].time = i * 0.25;
        notes[i].column = i % 4;
    }
    printf("  pushed 32 notes, %zu of %zu bytes used\n", level.used, level.size);

    // running out returns NULL and leaves the arena as it was
    size_t usedBefore = level.used;
    void* tooBig = arena::push(&level, memorySize);
    assert(tooBig == NULL);
    assert(level.used == usedBefore);
    printf("  pushing %zu more bytes returned NULL, still %zu bytes used\n", memorySize, level.used);

    // a size too large for size_t returns NULL instead of wrapping around
    void* wrapped = arena::pushArray(&level, SIZE_MAX / 2 + 1, 2);
    assert(wrapped == NULL);
    printf("  pushing an array too large for size_t returned NULL\n");

    // a smaller push still fits after a failed one
    void* small = arena::push(&level, 16);
    assert(small != NULL);
    printf("  pushing 16 bytes after that worked, %zu bytes used\n", level.used);

    // reset frees everything at once
    arena::reset(&level);
    assert(level.used == 0);
    printf("  reset, %zu bytes used\n", level.used);

    free(memory);
}

static void printBytes(const char* label, const uint8_t* data, size_t size) {
    printf("  %s:", label);
    for (size_t i = 0; i < size; i++) {
        printf(" %02x", data[i]);
    }
    printf("\n");
}

static void bytesDemo() {
    printf("Bytes\n");

    // Straight into memory: the lowest byte comes first, on every CPU
    uint8_t number[4];
    bytes::writeU32LE(number, 1);
    printBytes("1 as a u32", number, sizeof(number));
    assert(number[0] == 0x01 && number[3] == 0x00);
    bytes::writeU32LE(number, 0x12345678);
    printBytes("0x12345678 as a u32", number, sizeof(number));
    assert(number[0] == 0x78 && number[3] == 0x12);
    assert(bytes::readU32LE(number) == 0x12345678);

    // A message, written with a Writer into a buffer it's sent from
    uint8_t message[32];
    bytes::Writer writer = bytes::makeWriter(message, sizeof(message));
    bytes::writeU32LE(&writer, 12);        // chart
    bytes::writeU32LE(&writer, 987654);    // score
    bytes::writeF32LE(&writer, 0.9875f);   // accuracy
    bytes::writeStringLE(&writer, "player1", 7);
    assert(!writer.failed);
    printBytes("a score message", message, writer.size);
    printf("  %zu bytes written\n", writer.size);

    // Read back with a Reader, straight from the message: nothing is copied
    bytes::Reader reader = bytes::makeReader(message, writer.size);
    uint32_t chart = bytes::readU32LE(&reader);
    uint32_t score = bytes::readU32LE(&reader);
    float accuracy = bytes::readF32LE(&reader);
    size_t nameLength = 0;
    const char* name = bytes::readStringLE(&reader, &nameLength);
    assert(!reader.failed && bytes::getRemaining(&reader) == 0);
    assert(chart == 12 && score == 987654 && accuracy == 0.9875f);
    assert(name == (const char*)message + 14 && nameLength == 7 && memcmp(name, "player1", 7) == 0);
    printf("  read back: chart %u, score %u, accuracy %.4f, player \"%.*s\"\n", chart, score, accuracy, (int)nameLength, name);

    // A message cut short can't make a Reader read past its end: it fails,
    // and every read after that returns 0
    bytes::Reader shortened = bytes::makeReader(message, 10);
    bytes::readU32LE(&shortened);
    bytes::readU32LE(&shortened);
    float missing = bytes::readF32LE(&shortened);
    assert(shortened.failed && missing == 0.0f);
    printf("  reading the first 10 bytes as a score message: failed, as it should\n");

    // A Writer that runs out of room writes nothing more, and says so
    uint8_t small[6];
    bytes::Writer full = bytes::makeWriter(small, sizeof(small));
    bytes::writeU32LE(&full, 1);
    bool fitted = bytes::writeU32LE(&full, 2);
    assert(!fitted && full.failed && full.size == 4);
    printf("  writing 8 bytes into 6: the second value didn't fit, %zu bytes written\n", full.size);
}

int main() {
    arenaDemo();
    bytesDemo();
    return 0;
}
