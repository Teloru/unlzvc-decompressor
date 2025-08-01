#include <stdio.h>
#include "types.h"
#include "iff.h"
#include "unlzvc.h"

#define STRINGS_FILE_SUFFIX ".strings.txt"

static void debug_init(const char *in_filename, const char *out_filename, u32 file_size, u8 *file_map)
{
    fprintf(
        stderr,
        "\n--- INIT ---\n" \
        "input file: %s\n" \
        "output file: %s\n" \
        "input file size: %u (%08X)\n" \
        "input file map addr: %08X\n\n",
        in_filename,
        out_filename,
        file_size, file_size,
        (u32)file_map
    );
}

static void debug_chunk(IFF_CHUNK_HEADER *ch, u32 num_packed_segments, u32 offset)
{
    u64 chunk_id;

    chunk_id = ch->id;

    fprintf(
        stderr,
        "--- CHUNK ---\n" \
        "id: %s\n" \
        "size: %08X\n" \
        "unpack size: %08X\n" \
        "count: %04X\n" \
        "version: %02X\n" \
        "num packed segments: %u\n" \
        "file offset: %08X\n\n",
        (char *)&chunk_id,
        ch->size,
        ch->unpack_size,
        ch->count,
        ch->version,
        num_packed_segments,
        offset
    );
}

static void debug_unpack(u32 segment_offset, u32 unpack_buf_offset, u32 actual_unpack_size, s32 i, u32 *segments_sizes)
{
    fprintf(
        stderr,
        "segment offset: %08X\n" \
        "unpack buf offset: %08X\n" \
        "actual unpack_size: %08X\n" \
        "segment %d size : %08X\n\n",
        segment_offset,
        unpack_buf_offset,
        actual_unpack_size,
        i, segments_sizes[i]
    );
}

static u8 *map_file(const char *path, u32 *osize)
{
    u8 *file_map;
    FILE *fp;
    s32 size;

    file_map = NULL;

    fp = fopen(path, "rb");
    if (!fp) goto F_OUT;
    
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    if (size == 0) goto F_FCLOSE;
    
    fseek(fp, 0, SEEK_SET);
    file_map = (u8 *)malloc(size);
    if (!file_map) goto F_FCLOSE;

    if (fread(file_map, sizeof(u8), size, fp) == size)
    {
        *osize = size;
    }
    else
    {
        free(file_map);
        file_map = NULL;
    }

F_FCLOSE:
    fclose(fp);
F_OUT:
    return file_map;
}

static char *get_out_filename(const char *in_filename)
{
    char *out_filename;
    u32 out_filename_size;

    out_filename_size = strlen(in_filename) + sizeof(STRINGS_FILE_SUFFIX);
    out_filename = (char *)malloc(out_filename_size);

    if (out_filename)
    {
        snprintf(out_filename, out_filename_size, "%s%s", in_filename, STRINGS_FILE_SUFFIX);
    }

    return out_filename;
}


void export_text_chunk(FILE *fp, u8 *text_chunk, u32 offset)
{
    TEXT_CHUNK_HEADER *tch;
    TEXT_CHUNK_ENTRY *tce;
    char *s;

    tch = (TEXT_CHUNK_HEADER *)text_chunk;
    tce = (TEXT_CHUNK_ENTRY *)(text_chunk + sizeof(TEXT_CHUNK_HEADER));

    fprintf(
        fp,
        "--- TEXT CHUNK ---\n" \
        "Language: %s\n" \
        "Strings count: %u\n" \
        "File offset: %08X\n\n",
        (char *)(text_chunk + tch->language_offset),
        tch->num_strings,
        offset
    );

    for (s32 i = 0; i < tch->num_strings; ++i)
    {
        s = (char *)(text_chunk + tce[i].offset);

        fprintf(
            fp,
            "String %d\n" \
            "Hash: %08X\n",
            i + 1,
            tce[i].hash
        );

        if (s[0])
        {
            fprintf(fp, "Data: %s\n\n", s);
        }
        else
        {
            fprintf(fp, "Data is empty\n\n");
        }
    }
}

int main(int argc, char **argv)
{
    u8 *file_map;
    u8 *unpack_buf;
    u8 *text_chunk;
    char *out_filename;
    FILE *fp;
    u32 file_size;
    u32 offset;
    u32 unpack_buf_offset;
    u32 *segments_sizes;
    u32 segment_offset;
    u32 num_packed_segments;
    u32 actual_unpack_size;
    u32 num_text_chunks;
    u32 num_chunks;
    IFF_CHUNK_HEADER *ch;

    file_map = map_file(argv[1], &file_size);
    if (!file_map)
    {
        fprintf(stderr, "\nERROR: Input file cannot be read properly\n");
        goto F_OUT;
    }
    
    out_filename = get_out_filename(argv[1]);
    if (!out_filename)
    {
        fprintf(stderr, "\nERROR: Output filename cannot be created properly\n");
        goto F_FREE_MAP;
    }

    debug_init(argv[1], out_filename, file_size, file_map);

    fp = fopen(out_filename, "w+b");
    if (!fp)
    {
        fprintf(stderr, "\nERROR: Could not create output file\n");
        goto F_FREE_NAME;
    }

    offset = 0;
    num_chunks = 0;
    num_text_chunks = 0;

    while (offset < file_size)
    {
        ch = (IFF_CHUNK_HEADER *)(file_map + offset);
        num_packed_segments = (ch->unpack_size > 0) * ((ch->unpack_size >> 0xF) + ((ch->unpack_size & 0x7FFF) != 0));

        ++num_chunks;

        // Display debug information for all chunk types
        debug_chunk(ch, num_packed_segments, offset);
            
        if (ch->id != CKID_TEXT) goto NEXT_CHUNK;

        ++num_text_chunks;

        // Display debug information for TEXT chunks only
        // debug_chunk(ch, num_packed_segments, offset);

        if (num_packed_segments > 0)
        {
            unpack_buf = (u8 *)malloc(ch->unpack_size);
            if (!unpack_buf)
            {
                fprintf(stderr, "ERROR: Memory allocation failed for current TEXT chunk\n");
                goto NEXT_CHUNK;
            }

            segment_offset = offset + sizeof(IFF_CHUNK_HEADER) + (num_packed_segments * sizeof(u32));
            segments_sizes = (u32 *)(file_map + offset + sizeof(IFF_CHUNK_HEADER));
            unpack_buf_offset = 0;

            for (s32 i = 0; i < num_packed_segments; ++i)
            {
                actual_unpack_size = unlzvc_ghidra(file_map + segment_offset, unpack_buf + unpack_buf_offset);
                segment_offset += segments_sizes[i];
                unpack_buf_offset += actual_unpack_size;
                debug_unpack(segment_offset, unpack_buf_offset, actual_unpack_size, i, segments_sizes);
            }
            
            text_chunk = unpack_buf;
        }
        else
        {
            text_chunk = file_map + offset + sizeof(IFF_CHUNK_HEADER);
        }

        export_text_chunk(fp, text_chunk, offset);

NEXT_CHUNK:
        if (offset == 0x0029D194)
        {
            FILE *fp2;
            fp2 = fopen("blablabla.bin", "w+b");
            fwrite(unpack_buf, sizeof(u8), ch->unpack_size, fp2);
            fclose(fp2);
        }
        offset += (sizeof(IFF_CHUNK_HEADER) + ch->size);
    }

    fclose(fp);
    fprintf(stderr, "Found %u TEXT chunk(s) (Total chunks: %u)\n", num_text_chunks, num_chunks);

F_FREE_NAME:
    free(out_filename);
F_FREE_MAP:
    free(file_map);
F_OUT:
    return 0;
}
