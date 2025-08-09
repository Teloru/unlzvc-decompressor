#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "iff.h"
#include "unlzvc.h"

#define STRINGS_FILE_SUFFIX ".strings.txt"
#define MAX_DECODED_STRING_SIZE 4096

// Command name mapping from iff.h
static const char* get_command_name(u8 cmd_byte)
{
    switch(cmd_byte)
    {
        case TEXTBOX_CMD_UNKNOWN: return "UNKNOWN";
        case TEXTBOX_CMD_COLOUR: return "COLOR";
        case TEXTBOX_CMD_REDBUTTON: return "REDBUTTON";
        case TEXTBOX_CMD_GREENBUTTON: return "GREENBUTTON";
        case TEXTBOX_CMD_YELLOWBUTTON: return "YELLOWBUTTON";
        case TEXTBOX_CMD_BLUEBUTTON: return "BLUEBUTTON";
        case TEXTBOX_CMD_NEWLINE: return "NEWLINE";
        case TEXTBOX_CMD_WINDOWWIDTH: return "WINDOWWIDTH";
        case TEXTBOX_CMD_WINDOWHEIGHT: return "WINDOWHEIGHT";
        case TEXTBOX_CMD_FONTHEIGHT: return "FONTHEIGHT";
        case TEXTBOX_CMD_BORDERSIZE: return "BORDERSIZE";
        case TEXTBOX_CMD_MODELSCALE: return "MODELSCALE";
        case TEXTBOX_CMD_RESOURCEOFFSETY: return "RESOURCEOFFSETY";
        case TEXTBOX_CMD_RESOURCEOFFSETX: return "RESOURCEOFFSETX";
        case TEXTBOX_CMD_NEWPARAGRAPH: return "NEWPARAGRAPH";
        case TEXTBOX_CMD_USEBUTTONS: return "USEBUTTONS";
        case TEXTBOX_CMD_USEBUTTONSNOHELP: return "USEBUTTONSNOHELP";
        case TEXTBOX_CMD_WINDOWPOSITIONX: return "WINDOWPOSITIONX";
        case TEXTBOX_CMD_WINDOWPOSITIONY: return "WINDOWPOSITIONY";
        case TEXTBOX_CMD_PAUSEGAME: return "PAUSEGAME";
        case TEXTBOX_CMD_CLEARWINDOW: return "CLEARWINDOW";
        case TEXTBOX_CMD_SCROLLING: return "SCROLLING";
        case TEXTBOX_CMD_RESOURCE: return "RESOURCE";
        case TEXTBOX_CMD_MODEL: return "MODEL";
        case TEXTBOX_CMD_JUSTIFY: return "JUSTIFY";
        case TEXTBOX_CMD_BORDER: return "BORDER";
        case TEXTBOX_CMD_CORNER: return "CORNER";
        case TEXTBOX_CMD_TITLE: return "TITLE";
        case TEXTBOX_CMD_TITLECURVATURE: return "TITLECURVATURE";
        case TEXTBOX_CMD_TITLESCALE: return "TITLESCALE";
        case TEXTBOX_CMD_TITLEOFFSETY: return "TITLEOFFSETY";
        case TEXTBOX_CMD_ANIM: return "ANIM";
        case TEXTBOX_CMD_CSRY: return "CSRY";
        case TEXTBOX_CMD_WINDOWLIFETIME: return "WINDOWLIFETIME";
        case TEXTBOX_CMD_WINDOWLIFEMATCHAUDIO: return "WINDOWLIFEMATCHAUDIO";
        case TEXTBOX_CMD_OPENRATE: return "OPENRATE";
        case TEXTBOX_CMD_FADEIN: return "FADEIN";
        case TEXTBOX_CMD_SCALEIN: return "SCALEIN";
        case TEXTBOX_CMD_APPEARANCECOUNT: return "APPEARANCECOUNT";
        case TEXTBOX_CMD_GLASS: return "GLASS";
        case TEXTBOX_CMD_AUTOFADE: return "AUTOFADE";
        case TEXTBOX_CMD_MORE: return "MORE";
        case TEXTBOX_CMD_PAUSE: return "PAUSE";
        case TEXTBOX_CMD_MOIGLEICON: return "MOIGLEICON";
        case TEXTBOX_CMD_PRIORITY: return "PRIORITY";
        case TEXTBOX_CMD_SPEECH: return "SPEECH";
        case TEXTBOX_CMD_DIALOGRANDOM: return "DIALOGRANDOM";
        case TEXTBOX_CMD_DIALOGLIST: return "DIALOGLIST";
        case TEXTBOX_CMD_DIALOGSECTION: return "DIALOGSECTION";
        case TEXTBOX_CMD_DIALOGSEQUENCE: return "DIALOGSEQUENCE";
        case TEXTBOX_CMD_WAIT: return "WAIT";
        case TEXTBOX_CMD_IGNOREWAIT: return "IGNOREWAIT";
        default:
            return NULL;
    }
}

// Decode binary commands to readable text format
static char* decode_commands(const char* input)
{
    static char decoded[MAX_DECODED_STRING_SIZE];
    const char* src = input;
    char* dst = decoded;
    size_t remaining = sizeof(decoded) - 1;
    
    while (*src && remaining > 1)
    {
        if (*src == '@')
        {
            // Find the end of the command
            const char* cmd_end = strchr(src + 1, '@');
            if (!cmd_end)
            {
                // Malformed command, copy as-is
                *dst++ = *src++;
                remaining--;
                continue;
            }
            
            // Check if this is a binary command (first char after @ is a control character)
            if ((src + 1) < cmd_end && (u8)src[1] < 32)
            {
                u8 cmd_byte = (u8)src[1];
                const char* cmd_name = get_command_name(cmd_byte);
                
                if (cmd_name)
                {
                    // Convert binary command to text format
                    size_t param_len = cmd_end - src - 2;
                    if (param_len > 0)
                    {
                        // Format: @COMMAND:param@
                        int written = snprintf(dst, remaining, "@%s:", cmd_name);
                        if (written > 0 && written < remaining)
                        {
                            dst += written;
                            remaining -= written;
                            
                            // Copy parameter (skip the command byte)
                            size_t copy_len = (param_len < remaining - 1) ? param_len : remaining - 1;
                            memcpy(dst, src + 2, copy_len);
                            dst += copy_len;
                            remaining -= copy_len;
                            
                            // Add closing @
                            if (remaining > 1)
                            {
                                *dst++ = '@';
                                remaining--;
                            }
                        }
                    }
                    else
                    {
                        // Command without parameter
                        int written = snprintf(dst, remaining, "@%s@", cmd_name);
                        if (written > 0 && written < remaining)
                        {
                            dst += written;
                            remaining -= written;
                        }
                    }
                    
                    src = cmd_end + 1;
                }
                else
                {
                    // Unknown command, copy as-is
                    size_t cmd_len = cmd_end - src + 1;
                    size_t copy_len = (cmd_len < remaining) ? cmd_len : remaining;
                    memcpy(dst, src, copy_len);
                    dst += copy_len;
                    remaining -= copy_len;
                    src = cmd_end + 1;
                }
            }
            else
            {
                // Text command or already in readable format, copy as-is
                size_t cmd_len = cmd_end - src + 1;
                size_t copy_len = (cmd_len < remaining) ? cmd_len : remaining;
                memcpy(dst, src, copy_len);
                dst += copy_len;
                remaining -= copy_len;
                src = cmd_end + 1;
            }
        }
        else
        {
            // Regular character, copy as-is
            *dst++ = *src++;
            remaining--;
        }
    }
    
    *dst = '\0';
    return decoded;
}

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


void export_text_chunk(FILE *fp, u8 *text_chunk, u32 offset, const char *filename)
{
    TEXT_CHUNK_HEADER *tch;
    TEXT_CHUNK_ENTRY *tce;
    char *s;
    char *decoded_s;

    tch = (TEXT_CHUNK_HEADER *)text_chunk;
    tce = (TEXT_CHUNK_ENTRY *)(text_chunk + sizeof(TEXT_CHUNK_HEADER));

    fprintf(
        fp,
        "\n=== FILE: %s ===\n" \
        "--- TEXT CHUNK ---\n" \
        "Language: %s\n" \
        "Strings count: %u\n" \
        "File offset: %08X\n\n",
        filename,
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
            decoded_s = decode_commands(s);
            fprintf(fp, "Data: %s\n\n", decoded_s);
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
    u32 total_text_chunks = 0;
    u32 total_chunks = 0;
    IFF_CHUNK_HEADER *ch;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s file1.iff [file2.iff ...]\n", argv[0]);
        return 1;
    }

    // create fixed output filename
    size_t out_filename_len = strlen("all_files") + strlen(STRINGS_FILE_SUFFIX) + 1;
    out_filename = (char *)malloc(out_filename_len);
    if (!out_filename)
    {
        fprintf(stderr, "\nERROR: Output filename cannot be created properly\n");
        return 1;
    }
    snprintf(out_filename, out_filename_len, "%s%s", "all_files", STRINGS_FILE_SUFFIX);

    fp = fopen(out_filename, "w+b");
    if (!fp)
    {
        fprintf(stderr, "\nERROR: Could not create output file\n");
        free(out_filename);
        return 1;
    }

    // process each input file
    for (int file_idx = 1; file_idx < argc; file_idx++) {
        fprintf(stderr, "\n=== Processing file %d/%d: %s ===\n", file_idx, argc-1, argv[file_idx]);
        
        file_map = map_file(argv[file_idx], &file_size);
        if (!file_map)
        {
            fprintf(stderr, "\nERROR: Input file %s cannot be read properly, skipping...\n", argv[file_idx]);
            continue;
        }

        debug_init(argv[file_idx], out_filename, file_size, file_map);

        offset = 0;
        num_chunks = 0;
        num_text_chunks = 0;

        while (offset < file_size)
        {
            ch = (IFF_CHUNK_HEADER *)(file_map + offset);
            num_packed_segments = (ch->unpack_size > 0) * ((ch->unpack_size >> 0xF) + ((ch->unpack_size & 0x7FFF) != 0));

            ++num_chunks;

            // Display debug information for all chunk types
            // debug_chunk(ch, num_packed_segments, offset);
                
            if (ch->id != CKID_TEXT) goto NEXT_CHUNK;

            ++num_text_chunks;

            // Display debug information for TEXT chunks only
            debug_chunk(ch, num_packed_segments, offset);

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

            export_text_chunk(fp, text_chunk, offset, argv[file_idx]);

            if (num_packed_segments > 0)
            {
                free(unpack_buf);
            }

    NEXT_CHUNK:
            offset += (sizeof(IFF_CHUNK_HEADER) + ch->size);
        }

        fprintf(stderr, "File %s: Found %u TEXT chunk(s) (Total chunks: %u)\n", argv[file_idx], num_text_chunks, num_chunks);
        total_text_chunks += num_text_chunks;
        total_chunks += num_chunks;
        
        free(file_map);
    }

    fclose(fp);
    fprintf(stderr, "\n=== SUMMARY ===\n");
    fprintf(stderr, "Processed %d files\n", argc-1);
    fprintf(stderr, "Total TEXT chunks: %u (Total chunks: %u)\n", total_text_chunks, total_chunks);
    fprintf(stderr, "Output written to: %s\n", out_filename);

    free(out_filename);
    return 0;
}
