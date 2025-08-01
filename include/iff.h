#ifndef IFF_H
#define IFF_H

#include "types.h"

#define CKID_TEXT 0x54584554

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
