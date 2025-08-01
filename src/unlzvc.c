#include "types.h"
#include "unlzvc.h"

static u32 getword(u8 **buf)
{
    u16 uVar1;

    uVar1 = *(u16 *)*buf;
    *buf = (u8 *)((s32)*buf + 2);
    return (u32)uVar1;
}

u32 unlzvc_ghidra(u8 *ibuf, u8 *obuf)
{
    u8 uVar1;
    u8 bVar2;
    bool bVar3;
    u32 uVar4;
    u32 uVar5;
    u8 *pbVar6;
    u8 *puVar7;
    s32 iVar8;
    u8 *puVar9;
    u32 dbits;
    u32 local_r3_904;
    u32 uVar10;
    u32 uVar11;
    u8 *local_20[4];

    local_20[0] = ibuf;
    uVar4 = getword(local_20);
    uVar5 = getword(local_20);
    uVar11 = 0;
    switch (uVar5) {
    case 0:
        bVar2 = *local_20[0];
        local_20[0] = local_20[0] + 1;
        uVar4 = (u32)bVar2;
        if ((bVar2 & 0x80) == 0) goto LAB_1;
        while ((bVar2 & 0x7f) != 0) {
            uVar5 = getword(local_20);
            uVar10 = 0;
            if ((uVar4 & 0x7f) != 0xffffffff) {
                puVar9 = obuf + uVar5;
                puVar7 = obuf + uVar11;
                do {
                    uVar1 = *puVar9;
                    puVar9 = puVar9 + 1;
                    uVar11 = uVar11 + 1;
                    uVar10 = uVar10 + 1;
                    *puVar7 = uVar1;
                    puVar7 = puVar7 + 1;
                } while (uVar10 < (uVar4 & 0x7f) + 1);
            }
            while (true) {
                bVar2 = *local_20[0];
                local_20[0] = local_20[0] + 1;
                uVar4 = (u32)bVar2;
                if ((bVar2 & 0x80) != 0) break;
LAB_1:
                uVar5 = 0;
                if (uVar4 != 0xffffffff) {
                    pbVar6 = obuf + uVar11;
                    do {
                        bVar2 = *local_20[0];
                        local_20[0] = local_20[0] + 1;
                        uVar11 = uVar11 + 1;
                        uVar5 = uVar5 + 1;
                        *pbVar6 = bVar2;
                        pbVar6 = pbVar6 + 1;
                    } while (uVar5 < uVar4 + 1);
                }
            }
        }
        break;
    case 2:
        bVar2 = *local_20[0];
        local_20[0] = local_20[0] + 1;
        uVar4 = (u32)bVar2;
        if ((bVar2 & 0x80) == 0) goto LAB_2;
        while (uVar5 = (uVar4 & 0x7f) >> 2, uVar5 != 0) {
            bVar2 = *local_20[0];
            local_20[0] = local_20[0] + 1;
            uVar10 = 0;
            if (uVar5 != 0xffffffff) {
                puVar9 = obuf + (uVar11 - ((u32)bVar2 + (uVar4 & 3) * 0x100));
                puVar7 = obuf + uVar11;
                do {
                    uVar1 = *puVar9;
                    puVar9 = puVar9 + 1;
                    uVar11 = uVar11 + 1;
                    uVar10 = uVar10 + 1;
                    *puVar7 = uVar1;
                    puVar7 = puVar7 + 1;
                } while (uVar10 < uVar5 + 1);
            }
            while (true) {
                bVar2 = *local_20[0];
                local_20[0] = local_20[0] + 1;
                uVar4 = (u32)bVar2;
                if ((bVar2 & 0x80) != 0) break;
LAB_2:
                uVar5 = 0;
                if ((uVar4 & 0x7f) != 0xffffffff) {
                    pbVar6 = obuf + uVar11;
                    do {
                        bVar2 = *local_20[0];
                        local_20[0] = local_20[0] + 1;
                        uVar11 = uVar11 + 1;
                        uVar5 = uVar5 + 1;
                        *pbVar6 = bVar2;
                        pbVar6 = pbVar6 + 1;
                    } while (uVar5 < (uVar4 & 0x7f) + 1);
                }
            }
        }
        break;
    case 4:
        bVar2 = *local_20[0];
        local_20[0] = local_20[0] + 1;
        uVar4 = (u32)bVar2;
        if ((bVar2 & 0x80) == 0) goto LAB_3;
        while (uVar5 = (uVar4 & 0x7f) >> 3, uVar5 != 0) {
            bVar2 = *local_20[0];
            local_20[0] = local_20[0] + 1;
            uVar10 = 0;
            if (uVar5 != 0xffffffff) {
                puVar9 = obuf + (uVar11 - ((u32)bVar2 + (uVar4 & 7) * 0x100));
                puVar7 = obuf + uVar11;
                do {
                    uVar1 = *puVar9;
                    puVar9 = puVar9 + 1;
                    uVar11 = uVar11 + 1;
                    uVar10 = uVar10 + 1;
                    *puVar7 = uVar1;
                    puVar7 = puVar7 + 1;
                } while (uVar10 < uVar5 + 1);
            }
            while (true) {
                bVar2 = *local_20[0];
                local_20[0] = local_20[0] + 1;
                uVar4 = (u32)bVar2;
                if ((bVar2 & 0x80) != 0) break;
LAB_3:
                uVar5 = 0;
                if ((uVar4 & 0x7f) != 0xffffffff) {
                    pbVar6 = obuf + uVar11;
                    do {
                        bVar2 = *local_20[0];
                        local_20[0] = local_20[0] + 1;
                        uVar11 = uVar11 + 1;
                        uVar5 = uVar5 + 1;
                        *pbVar6 = bVar2;
                        pbVar6 = pbVar6 + 1;
                    } while (uVar5 < (uVar4 & 0x7f) + 1);
                }
            }
        }
        break;
    case 6:
        bVar3 = false;
        do {
            bVar2 = *local_20[0];
            local_20[0] = local_20[0] + 1;
            iVar8 = 8;
            uVar4 = (u32)bVar2;
            do {
                if ((uVar4 & 1) == 0) {
                    bVar2 = *local_20[0];
                    local_20[0] = local_20[0] + 1;
                    uVar5 = (u32)bVar2;
                    if (uVar5 == 0) {
                        bVar3 = true;
                        break;
                    }
                    uVar10 = 0;
                    if (uVar5 != 0) {
                        pbVar6 = obuf + uVar11;
                        do {
                            bVar2 = *local_20[0];
                            local_20[0] = local_20[0] + 1;
                            uVar11 = uVar11 + 1;
                            uVar10 = uVar10 + 1;
                            *pbVar6 = bVar2;
                            pbVar6 = pbVar6 + 1;
                        } while (uVar10 < uVar5);
                    }
                }
                else {
                    bVar2 = *local_20[0];
                    pbVar6 = local_20[0] + 1;
                    local_20[0] = local_20[0] + 2;
                    uVar10 = (u32)(bVar2 >> 3);
                    uVar5 = 0;
                    if (uVar10 != 0xfffffffd) {
                        puVar9 = obuf + (uVar11 - ((u32)*pbVar6 + (bVar2 & 7) * 0x100));
                        puVar7 = obuf + uVar11;
                        do {
                            uVar1 = *puVar9;
                            puVar9 = puVar9 + 1;
                            uVar11 = uVar11 + 1;
                            uVar5 = uVar5 + 1;
                            *puVar7 = uVar1;
                            puVar7 = puVar7 + 1;
                        } while (uVar5 < uVar10 + 3);
                    }
                }
                iVar8 = iVar8 + -1;
                uVar4 = uVar4 >> 1;
            } while (iVar8 != 0);
        } while (!bVar3);
        break;
    case 8:
        bVar3 = false;
        do {
            bVar2 = *local_20[0];
            local_20[0] = local_20[0] + 1;
            dbits = 8;
            uVar4 = (u32)bVar2;
            do {
                if ((uVar4 & 1) == 0) {
                    bVar2 = *local_20[0];
                    local_20[0] = local_20[0] + 1;
                    uVar5 = (u32)bVar2;
                    if (uVar5 == 0) {
                        bVar3 = true;
                        break;
                    }
                    uVar10 = 0;
                    if (uVar5 != 0) {
                        pbVar6 = obuf + uVar11;
                        do {
                            bVar2 = *local_20[0];
                            local_20[0] = local_20[0] + 1;
                            uVar11 = uVar11 + 1;
                            uVar10 = uVar10 + 1;
                            *pbVar6 = bVar2;
                            pbVar6 = pbVar6 + 1;
                        } while (uVar10 < uVar5);
                    }
                }
                else {
                    bVar2 = *local_20[0];
                    pbVar6 = local_20[0] + 1;
                    local_20[0] = local_20[0] + 2;
                    uVar10 = (u32)(bVar2 >> 4);
                    uVar5 = 0;
                    if (uVar10 != 0xfffffffd) {
                        local_r3_904 = (uVar11 - ((u32)*pbVar6 + (bVar2 & 0xf) * 0x100)) + (int)obuf;
                        puVar7 = obuf + uVar11;
                        do {
                            uVar1 = *(u8*)local_r3_904;
                            local_r3_904 = local_r3_904 + 1;
                            uVar11 = uVar11 + 1;
                            uVar5 = uVar5 + 1;
                            *puVar7 = uVar1;
                            puVar7 = puVar7 + 1;
                        } while (uVar5 < uVar10 + 3);
                    }
                }
                dbits = dbits - 1;
                uVar4 = uVar4 >> 1;
            } while (dbits != 0);
        } while (!bVar3);
        break;
    case 10:
        uVar5 = 0;
        uVar11 = uVar4;
        if (uVar4 != 0) {
            do {
                bVar2 = *local_20[0];
                local_20[0] = local_20[0] + 1;
                uVar5 = uVar5 + 1;
                *obuf = bVar2;
                obuf = obuf + 1;
            } while (uVar5 < uVar4);
        }
    }
    return uVar11;
}
