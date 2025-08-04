#ifndef IFF_H
#define IFF_H

#include "types.h"

#define CKID_TEXT 0x54584554

enum TEXTBOX_CMD
{
    TEXTBOX_CMD_UNKNOWN = 1,                // Hex: 0x01
    TEXTBOX_CMD_COLOUR = 2,                 // Hex: 0x02
    TEXTBOX_CMD_REDBUTTON = 3,              // Hex: 0x03
    TEXTBOX_CMD_GREENBUTTON = 4,            // Hex: 0x04
    TEXTBOX_CMD_YELLOWBUTTON = 5,           // Hex: 0x05
    TEXTBOX_CMD_BLUEBUTTON = 6,             // Hex: 0x06
    TEXTBOX_CMD_NEWLINE = 7,                // Hex: 0x07
    TEXTBOX_CMD_WINDOWWIDTH = 8,            // Hex: 0x08
    TEXTBOX_CMD_WINDOWHEIGHT = 9,           // Hex: 0x09
    TEXTBOX_CMD_FONTHEIGHT = 10,            // Hex: 0x0A
    TEXTBOX_CMD_BORDERSIZE = 11,            // Hex: 0x0B
    TEXTBOX_CMD_MODELSCALE = 12,            // Hex: 0x0C
    TEXTBOX_CMD_RESOURCEOFFSETY = 13,       // Hex: 0x0D
    TEXTBOX_CMD_RESOURCEOFFSETX = 14,       // Hex: 0x0E
    TEXTBOX_CMD_NEWPARAGRAPH = 15,          // Hex: 0x0F
    TEXTBOX_CMD_USEBUTTONS = 16,            // Hex: 0x10
    TEXTBOX_CMD_USEBUTTONSNOHELP = 17,      // Hex: 0x11
    TEXTBOX_CMD_WINDOWPOSITIONX = 18,       // Hex: 0x12
    TEXTBOX_CMD_WINDOWPOSITIONY = 19,       // Hex: 0x13
    TEXTBOX_CMD_PAUSEGAME = 20,             // Hex: 0x14
    TEXTBOX_CMD_CLEARWINDOW = 21,           // Hex: 0x15
    TEXTBOX_CMD_SCROLLING = 22,             // Hex: 0x16
    TEXTBOX_CMD_RESOURCE = 23,              // Hex: 0x17
    TEXTBOX_CMD_MODEL = 24,                 // Hex: 0x18
    TEXTBOX_CMD_JUSTIFY = 25,               // Hex: 0x19
    TEXTBOX_CMD_BORDER = 26,                // Hex: 0x1A
    TEXTBOX_CMD_CORNER = 27,                // Hex: 0x1B
    TEXTBOX_CMD_TITLE = 28,                 // Hex: 0x1C
    TEXTBOX_CMD_TITLECURVATURE = 29,        // Hex: 0x1D
    TEXTBOX_CMD_TITLESCALE = 30,            // Hex: 0x1E
    TEXTBOX_CMD_TITLEOFFSETY = 31,          // Hex: 0x1F
    TEXTBOX_CMD_ANIM = 32,                  // Hex: 0x20
    TEXTBOX_CMD_CSRY = 33,                  // Hex: 0x21
    TEXTBOX_CMD_WINDOWLIFETIME = 34,        // Hex: 0x22
    TEXTBOX_CMD_WINDOWLIFEMATCHAUDIO = 35,  // Hex: 0x23
    TEXTBOX_CMD_OPENRATE = 36,              // Hex: 0x24
    TEXTBOX_CMD_FADEIN = 37,                // Hex: 0x25
    TEXTBOX_CMD_SCALEIN = 38,               // Hex: 0x26
    TEXTBOX_CMD_APPEARANCECOUNT = 39,       // Hex: 0x27
    TEXTBOX_CMD_GLASS = 40,                 // Hex: 0x28
    TEXTBOX_CMD_AUTOFADE = 41,              // Hex: 0x29
    TEXTBOX_CMD_MORE = 42,                  // Hex: 0x2A
    TEXTBOX_CMD_PAUSE = 43,                 // Hex: 0x2B
    TEXTBOX_CMD_MOIGLEICON = 44,            // Hex: 0x2C
    TEXTBOX_CMD_PRIORITY = 45,              // Hex: 0x2D
    TEXTBOX_CMD_SPEECH = 46,                // Hex: 0x2E
    TEXTBOX_CMD_DIALOGRANDOM = 47,          // Hex: 0x2F
    TEXTBOX_CMD_DIALOGLIST = 48,            // Hex: 0x30
    TEXTBOX_CMD_DIALOGSECTION = 49,         // Hex: 0x31
    TEXTBOX_CMD_DIALOGSEQUENCE = 50,        // Hex: 0x32
    TEXTBOX_CMD_WAIT = 51,                  // Hex: 0x33
    TEXTBOX_CMD_IGNOREWAIT = 52,            // Hex: 0x34
    TEXTBOX_CMD_MAX = 53                    // Hex: 0x35
};
typedef enum TEXTBOX_CMD TEXTBOX_CMD;

#pragma pack(push, 1)

typedef struct IFF_CHUNK_HEADER IFF_CHUNK_HEADER;
typedef struct TEXT_CHUNK_HEADER TEXT_CHUNK_HEADER;
typedef struct TEXT_CHUNK_ENTRY TEXT_CHUNK_ENTRY;

struct IFF_CHUNK_HEADER // 0x10
{
/* 0x00 */ u32 id;
/* 0x04 */ u32 size;
/* 0x08 */ u32 unpack_size;
/* 0x0C */ u16 count;
/* 0x0E */ u8 version;
/* 0x0F */ u8 pad;
/* 0x10    u32 segments_sizes[num_packed_segments]; */
};

struct TEXT_CHUNK_ENTRY // 0x8
{
/* 0x00 */ u32 hash;
/* 0x04 */ u32 offset;
};

struct TEXT_CHUNK_HEADER // 0x8
{
/* 0x00 */ u32 language_offset;
/* 0x04 */ u32 num_strings;
/* 0x08    TEXT_CHUNK_ENTRY entries[num_strings]; */
};

#pragma pack(pop)

#endif // IFF_H
