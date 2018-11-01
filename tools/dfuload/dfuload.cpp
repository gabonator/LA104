#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>                     
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <ctype.h>
#include <strings.h>

#include "fat12.h"

//#define dprintf(...)
#define dprintf printf

int root_dir_addr(int fd, struct bpb33* bpb)
{
    uint32_t offset;
    offset = 
	(bpb->bpbBytesPerSec 
	 * (bpb->bpbResSectors + (bpb->bpbFATs * bpb->bpbFATsecs)));

    return offset;
}

void _readdi(int fdi, direntry& dirent, int direntofs, bool verb)
{
    dprintf("Read direntry at %x", direntofs);
    lseek(fdi, direntofs, SEEK_SET);
    if (read(fdi, &dirent, sizeof(direntry)) != sizeof(direntry) || errno != 0)
        fprintf(stderr, "Read error! errno=%d \n", errno);
    dprintf(" ok\n");
}

void _writedi(int fdi, direntry& dirent, int direntofs)
{
    dprintf("Updating direntry at %x", direntofs);
    lseek(fdi, direntofs, SEEK_SET);
    if (write(fdi, &dirent, sizeof(direntry)) != sizeof(direntry) || errno != 0)
        fprintf(stderr, "Write error!\n");
    dprintf(" ok\n");
}

struct bpb33* check_bootsector(int fd)
{
    struct bootsector33* bootsect;
    struct byte_bpb33* bpb;  /* BIOS parameter block */
    struct bpb33* bpb2;

    unsigned char sector0[512] = {0};
    if (read(fd, sector0, 512) != 512 || errno != 0)
    {
        fprintf(stderr, "Read error!\n");
        return NULL;
    }

    bootsect = (struct bootsector33*)sector0;

    if (bootsect->bsJump[0] == 0xe9 ||
	(bootsect->bsJump[0] == 0xeb && bootsect->bsJump[2] == 0x90)) 
    {
      // Good jump instruction
    } else {
	fprintf(stderr, "illegal boot sector jump inst: %x%x%x\n", 
		bootsect->bsJump[0], bootsect->bsJump[1], 
		bootsect->bsJump[2]); 
        return NULL;
    } 

    dprintf("OemName: %s\n", bootsect->bsOemName);

    if (bootsect->bsBootSectSig0 == BOOTSIG0
	&& bootsect->bsBootSectSig0 == BOOTSIG0) 
    {
	//Good boot sector sig;
    } else {
	fprintf(stderr, "Boot boot sector signature %x%x\n", 
		bootsect->bsBootSectSig0, 
		bootsect->bsBootSectSig1);
        return NULL;
    }

    bpb = (struct byte_bpb33*)&(bootsect->bsBPB[0]);

    /* bpb is a byte-based struct, because this data is unaligned.
       This makes it hard to access the multi-byte fields, so we copy
       it to a slightly larger struct that is word-aligned */
    bpb2 = (bpb33*)malloc(sizeof(struct bpb33));

    bpb2->bpbBytesPerSec = getushort(bpb->bpbBytesPerSec);
    bpb2->bpbSecPerClust = bpb->bpbSecPerClust;
    bpb2->bpbResSectors = getushort(bpb->bpbResSectors);
    bpb2->bpbFATs = bpb->bpbFATs;
    bpb2->bpbRootDirEnts = getushort(bpb->bpbRootDirEnts);
    bpb2->bpbSectors = getushort(bpb->bpbSectors);
    bpb2->bpbFATsecs = getushort(bpb->bpbFATsecs);
    bpb2->bpbHiddenSecs = getushort(bpb->bpbHiddenSecs);
    return bpb2;
}

bool checkCompatibility(int fdi, bpb33* bpb)
{
    if (bpb->bpbBytesPerSec != 512 || bpb->bpbSecPerClust != 1 || bpb->bpbSectors != 4096)
    {
      fprintf(stderr, "Incompatible device\n");
      return false;
    }

    int offset = root_dir_addr(fdi, bpb);
    direntry dirent; 
    _readdi(fdi, dirent, offset, false);
    return memcmp(dirent.deName, "DFU V", 5) == 0;
}

void fill_clusters(FILE*fd, int fdi, struct bpb33* bpb)
{
    uint32_t clust_size;
    uint8_t *buf;
    uint32_t rdl;
    uint32_t total = 0;

    clust_size = bpb->bpbSecPerClust * bpb->bpbBytesPerSec;
    buf = (uint8_t*)malloc(clust_size);

    lseek(fdi, 0xf000, SEEK_SET);
    do
    {
	rdl = fread(buf, 1, clust_size, fd); 
	if (rdl == 0)
          break;

        dprintf(".");

        if (write(fdi, buf, rdl) != rdl || errno != 0)
          fprintf(stderr, "Write error e:%d!\n", errno);
        total += rdl;
    } while (rdl == clust_size);

    dprintf("\nFinished writing %d bytes\n", total);

    free(buf);
}

void prepare_dirent(struct direntry *dirent)
{
    dirent->deName[0] = 'A';
    dirent->deName[1] = ' ';
    dirent->deName[2] = ' ';
    dirent->deName[3] = ' ';
    dirent->deName[4] = ' ';
    dirent->deName[5] = ' ';
    dirent->deName[6] = ' ';
    dirent->deName[7] = ' ';
    dirent->deExtension[0] = 'H';
    dirent->deExtension[1] = 'E';
    dirent->deExtension[2] = 'X';

    dirent->deAttributes = ATTR_ARCHIVE;
    putushort(dirent->deStartCluster, 1);
    putulong(dirent->deFileSize, 0);
}

int get_empty_dirent(int direntofs, int fdi, struct bpb33* bpb)
{
    direntry dirent;
    while(1) 
    {
        _readdi(fdi, dirent, direntofs, true);
	if (dirent.deName[0] == SLOT_EMPTY) 
        {
            return direntofs;
	}
	direntofs += sizeof(direntry);
    }
    return 0;
}

int main(int argc, const char * argv[]) 
{
    if (argc < 3)
    {
      printf("Usage:\n");
      printf("\n");
      printf("  sudo ./dfuload /dev/disk2 cp input.hex\n");
      return 1;
    }

    int fd = open(argv[1], O_RDWR);
    fcntl(fd, F_NOCACHE, 1);

    if (fd<0)
    {
      fprintf(stderr, "Failed to open disk '%s', errno:%d\n", argv[1], errno);
      return 1;
    }

    bpb33* bpb = check_bootsector(fd);

    if (strcmp(argv[2], "cp") == 0 && argc == 4)
    { 
      if (checkCompatibility(fd, bpb))
      {
        FILE* source = fopen(argv[3], "r");
        if (source == NULL) 
        {
  	  fprintf(stderr, "Can't open file %s for reading\n", argv[3]);
	  exit(1); 
        }

        int direntofs = get_empty_dirent(root_dir_addr(fd, bpb), fd, bpb);
        direntry dirent;
	prepare_dirent(&dirent);

	// writing operations
        fill_clusters(source, fd, bpb);
	_writedi(fd, dirent, direntofs);

      fprintf(stderr, "Closing data ");
        fclose(source);
      fprintf(stderr, "Ok\n");
      }
    } else
    {
      fprintf(stderr, "Nothing to do.\n");
    }

      fprintf(stderr, "Closing disk ");
    close(fd);
      fprintf(stderr, "Ok\n");
    free(bpb);

    return 0;
}