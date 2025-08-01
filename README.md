# UNLZVC Decompressor

A reverse-engineered decompressor for Visual Concepts' proprietary LZVC compression format, specifically designed to extract **dialogue strings** from Floigan Bros (and other Dreamcast games ?).

## About

This tool was developed through reverse engineering of the Floigan Bros Dreamcast game binary using Ghidra & IDA. During reverse engineering, we discovered a function labeled `unlzvc` in the disassembly, which led to naming this project UNLZVC. However, the underlying compression format appears to be LZVC - likely Visual Concepts' custom implementation of LZX compression.

The LZVC format uses IFF-style chunks with segmented compression, where each file contains multiple compressed segments that need to be decompressed individually.

**Our current focus:** This decompressor is specifically designed and tested for extracting dialogue strings from Floigan Bros. While the LZVC format could potentially be used for other game assets (textures, animations, camera data, etc.), we haven't implemented support for those use cases (yet).

**Special thanks to [Nahelam](https://github.com/Nahelam) who did most of the heavy lifting on this project!** 🙏

## Usage

```bash
# Compile the decompressor
cl /TC /GS- /GR- src/main.c src/unlzvc.c /Iinclude /link

# Extract compressed data
main.exe xxx.IFF
```

## File Format

Each LZVC file contains:
1. **IFF Chunk Header** (16 bytes) - metadata about the compressed data
2. **Segment Table** - array of uint values indicating segment sizes  
3. **Compressed Segments** - the actual compressed data

### Header Structure

```c
struct IFF_CHUNK_HEADER {
    uint id;           // Chunk identifier
    uint size;         // Compressed data size
    uint unpacksize;   // Uncompressed size + segment info
    ushort count;      // Number of elements
    uchar version;     // Format version
    uchar pad;         // Padding byte
};
```

**Segment calculation:** `num_segments = (unpacksize >> 15) + ((unpacksize & 0x7fff) != 0)`

## Current Status

- todo

## Disclaimer

This tool is for educational and preservation purposes. Floigan Bros is owned by Visual Concepts/2K Games.

