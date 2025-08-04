# UNLZVC Decompressor

A reverse-engineered decompressor for Visual Concepts' proprietary LZVC compression format, specifically designed to extract **dialogue strings** from Floigan Bros (and other Dreamcast games ?).

## About

This tool was developed through reverse engineering of the Floigan Bros Dreamcast game binary using Ghidra & IDA. During reverse engineering, we discovered a function labeled `unlzvc` in the disassembly, which led to naming this project UNLZVC. However, the underlying compression format appears to be LZVC - likely Visual Concepts' custom implementation of LZX compression.

The LZVC format uses IFF-style chunks with segmented compression, where each file contains multiple compressed segments that need to be decompressed individually.

**Our current focus:** This decompressor is specifically designed and tested for extracting dialogue strings from Floigan Bros. While the LZVC format could potentially be used for other game assets (textures, animations, camera data, etc.), we haven't implemented support for those use cases (yet).

**Special thanks to [Nahelam](https://github.com/Nahelam) who did most of the heavy lifting on this project!** 🙏

## Usage

### Quick Start (recommended)

1. **Place your IFF files** in the `scripts` folder alongside `process_iff_files.bat`
2. **Double-click** `process_iff_files.bat`
3. **Wait** for the 3-step processing to complete:
   - Step 1: Extract compressed data from all IFF files
   - Step 2: Parse and extract Data fields 
   - Step 3: Filter and extract clean dialogue
4. **Find your extracted dialogue** in `floigan_bros_all_dialogues.txt`

The batch script will automatically:
- Process all `.IFF` files in the scripts folder
- Extract and decompress the dialogue data  
- Filter out UI elements and menu text
- Generate clean dialogue output in `floigan_bros_all_dialogues.txt`
- Clean up temporary files

### Manual Usage

If you prefer to run the tools manually:

```bash
# 1. Extract compressed data from IFF files
main.exe your_file.IFF

# 2. Process the extracted data to get clean dialogue
cd scripts
python process_all_iff.py
```

### Output

The final `floigan_bros_all_dialogues.txt` will contain clean dialogue lines like:
```
Hey Moigle, I found some neat stuff in here!
I think this junk might be worth something.
Spitz, you're always looking for treasure.
```

## Build

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

✅ **Fully functional dialogue extraction pipeline**
- UNLZVC decompression working for IFF files
- Automated batch processing script
- Successfully extracts 389+ dialogue lines from Floigan Bros

🔧 **What works:**
- Processing IFF files from Floigan Bros
- Automatic decompression and text extraction
- One-click batch processing via `process_iff_files.bat`

## Disclaimer

This tool is for educational and preservation purposes. Floigan Bros is owned by Visual Concepts/2K Games.

