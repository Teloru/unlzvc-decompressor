#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t uchar;
typedef uint8_t byte;
typedef uint16_t ushort;
typedef uint32_t uint;

// thanks ghidra :)
uint getword(uchar **buf)
{
  ushort uVar1;
  
  uVar1 = *(ushort *)*buf;
  *buf = (uchar *)((int)*buf + 2);
  return (uint)uVar1;
}

int unlzvc(uchar *ibuf,uchar *obuf)
{
  uchar uVar1;
  byte bVar2;
  bool bVar3;
  uint uVar4;
  uint uVar5;
  byte *pbVar6;
  uchar *puVar7;
  int iVar8;
  uchar *puVar9;
  uint dbits;
  uint uVar10;
  uint uVar11;
  byte *local_20 [4];
  
  local_20[0] = ibuf;
  uVar4 = getword(local_20);
  uVar5 = getword(local_20);
  uVar11 = 0;
  
  printf("Debug: uVar4=%u, uVar5=%u\n", uVar4, uVar5);
  if (true) {
    switch(uVar5) {
    case 0:
      bVar2 = *local_20[0];
      local_20[0] = local_20[0] + 1;
      uVar4 = (uint)bVar2;
      if ((bVar2 & 0x80) == 0) goto LAB_0c0632c0;
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
        while( true ) {
          bVar2 = *local_20[0];
          local_20[0] = local_20[0] + 1;
          uVar4 = (uint)bVar2;
          if ((bVar2 & 0x80) != 0) break;
LAB_0c0632c0:
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
      uVar4 = (uint)bVar2;
      if ((bVar2 & 0x80) == 0) goto LAB_0c063360;
      while (uVar5 = (uVar4 & 0x7f) >> 2, uVar5 != 0) {
        bVar2 = *local_20[0];
        local_20[0] = local_20[0] + 1;
        uVar10 = 0;
        if (uVar5 != 0xffffffff) {
          puVar9 = obuf + (uVar11 - ((uint)bVar2 + (uVar4 & 3) * 0x100));
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
        while( true ) {
          bVar2 = *local_20[0];
          local_20[0] = local_20[0] + 1;
          uVar4 = (uint)bVar2;
          if ((bVar2 & 0x80) != 0) break;
LAB_0c063360:
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
      uVar4 = (uint)bVar2;
      if ((bVar2 & 0x80) == 0) goto LAB_0c063420;
      while (uVar5 = (uVar4 & 0x7f) >> 3, uVar5 != 0) {
        bVar2 = *local_20[0];
        local_20[0] = local_20[0] + 1;
        uVar10 = 0;
        if (uVar5 != 0xffffffff) {
          puVar9 = obuf + (uVar11 - ((uint)bVar2 + (uVar4 & 7) * 0x100));
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
        while( true ) {
          bVar2 = *local_20[0];
          local_20[0] = local_20[0] + 1;
          uVar4 = (uint)bVar2;
          if ((bVar2 & 0x80) != 0) break;
LAB_0c063420:
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
        uVar4 = (uint)bVar2;
        do {
          if ((uVar4 & 1) == 0) {
            bVar2 = *local_20[0];
            local_20[0] = local_20[0] + 1;
            uVar5 = (uint)bVar2;
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
            uVar10 = (uint)(bVar2 >> 3);
            uVar5 = 0;
            if (uVar10 != 0xfffffffd) {
              puVar9 = obuf + (uVar11 - ((uint)*pbVar6 + (bVar2 & 7) * 0x100));
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
        uVar4 = (uint)bVar2;
        do {
          if ((uVar4 & 1) == 0) {
            bVar2 = *local_20[0];
            local_20[0] = local_20[0] + 1;
            uVar5 = (uint)bVar2;
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
            uVar10 = (uint)(bVar2 >> 4);
            uVar5 = 0;
            if (uVar10 != 0xfffffffd) {
              puVar9 = obuf + (uVar11 - ((uint)*pbVar6 + (bVar2 & 0xf) * 0x100));
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
  }
  return uVar11;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    FILE *input_file = fopen(argv[1], "rb");
    if (!input_file) {
        printf("Erreur: impossible d'ouvrir le fichier d'entrée '%s'\n", argv[1]);
        return 1;
    }
    fseek(input_file, 0, SEEK_END);
    long file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    uchar *input_buffer = malloc(file_size);
    if (!input_buffer) {
        printf("Erreur: impossible d'allouer la mémoire pour le buffer d'entrée\n");
        fclose(input_file);
        return 1;
    }
    
    size_t bytes_read = fread(input_buffer, 1, file_size, input_file);
    fclose(input_file);
    
    if (bytes_read != file_size) {
        printf("Erreur: lecture incomplète du fichier\n");
        free(input_buffer);
        return 1;
    }
    
    printf("Read file: %ld bytes\n", file_size);
    printf("Firsts bytes: %02X %02X %02X %02X\n", 
           input_buffer[0], input_buffer[1], input_buffer[2], input_buffer[3]);
    
    
    // lets say the decompression can be 10x the original size (this is arbitrary)
    uchar *output_buffer = malloc(file_size * 10);
    if (!output_buffer) {
        printf("Erreur: impossible d'allouer la mémoire pour le buffer de sortie\n");
        free(input_buffer);
        return 1;
    }

    int decompressed_size = unlzvc(input_buffer, output_buffer);
    
    if (decompressed_size <= 0) {
        printf("Erreur: échec de la décompression\n");
        free(input_buffer);
        free(output_buffer);
        return 1;
    }
    
    FILE *output_file = fopen(argv[2], "wb");
    if (!output_file) {
        printf("Erreur: impossible de créer le fichier de sortie '%s'\n", argv[2]);
        free(input_buffer);
        free(output_buffer);
        return 1;
    }
    
    size_t bytes_written = fwrite(output_buffer, 1, decompressed_size, output_file);
    fclose(output_file);
    
    if (bytes_written != decompressed_size) {
        printf("Erreur: écriture incomplète du fichier de sortie\n");
        free(input_buffer);
        free(output_buffer);
        return 1;
    }
    
    printf("Décompression réussie: %ld bytes -> %d bytes\n", file_size, decompressed_size);
    printf("Fichier de sortie: %s\n", argv[2]);
    
    free(input_buffer);
    free(output_buffer);
    
    return 0;
}

