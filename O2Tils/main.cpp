// A demo of O2Tils: prints what each utility does, and checks it along the way.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <o2/utils/arena.h>

namespace arena = o2::utils::arena;

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

int main() {
    arenaDemo();
    return 0;
}
