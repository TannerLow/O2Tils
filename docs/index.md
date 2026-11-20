# O2Tils documentation

O2Tils is a small collection of utilities shared by the o2 libraries, such as O2Jam and O2Gui, written in C-style C++. It depends on nothing but the C and C++ runtime, and never allocates memory on its own.

The docs come in two parts:

- **Concepts** explain the ideas behind each utility. Read these first. The reference pages assume them.
- **Reference** has one page per function or struct, all with the same layout: what it does, its declaration, parameters, return value, remarks, an example, and related pages.

## Concepts

1. [Arenas](concepts/arenas.md): what an arena is, which memory fits one, and what happens when one runs out
2. [Bytes](concepts/bytes.md): turning numbers and strings into bytes for messages and files, little-endian on every CPU, and reading them back safely

## Reference

### Arena (`<o2/utils/arena.h>`, namespace `o2::utils::arena`)

| | |
|---|---|
| [Arena](reference/Arena.md) | A linear allocator over a block of memory you provide |
| [make](reference/make.md) | An empty arena over a block of memory |
| [reset](reference/reset.md) | Frees everything in an arena at once |
| [push](reference/push.md) | Takes bytes from an arena |
| [pushArray](reference/pushArray.md) | Takes room for an array from an arena |
| [pushAligned](reference/pushAligned.md) | Takes bytes from an arena, starting at an aligned address |
| [pushArrayAligned](reference/pushArrayAligned.md) | Takes room for an array from an arena, starting at an aligned address |

### Bytes (`<o2/utils/bytes.h>`, namespace `o2::utils::bytes`)

| | |
|---|---|
| [Writer](reference/Writer.md) | Writes values into a buffer you provide; also `makeWriter` |
| [writeU32LE](reference/writeU32LE.md) | Writes a number, little-endian; also every other number write, from `writeU8` to `writeF64LE` |
| [writeBytes](reference/writeBytes.md) | Writes bytes as they are; also `writeStringLE` |
| [Reader](reference/Reader.md) | Reads values from a buffer; also `makeReader`, `getRemaining` |
| [readU32LE](reference/readU32LE.md) | Reads a number, little-endian; also every other number read, from `readU8` to `readF64LE` |
| [readBytes](reference/readBytes.md) | Reads bytes as they are, without copying; also `readStringLE` |
