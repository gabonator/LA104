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
#define MAXPATHLEN 255

int __target = -1;

void _readdi(int fdi, direntry& dirent, int direntofs, bool);

void get_name(char *fullname, struct direntry *dirent) 
{
    char name[9];
    char extension[4];
    int i;

    name[8] = ' ';
    extension[3] = ' ';
    memcpy(name, &(dirent->deName[0]), 8);
    memcpy(extension, dirent->deExtension, 3);

    /* names are space padded - remove the padding */
    for (i = 8; i > 0; i--) {
	if (name[i] == ' ') 
	    name[i] = '\0';
	else 
	    break;
    }

    /* extensions aren't normally space padded - but remove the
       padding anyway if it's there */
    for (i = 3; i > 0; i--) {
	if (extension[i] == ' ') 
	    extension[i] = '\0';
	else 
	    break;
    }
    fullname[0]='\0';
    strcat(fullname, name);

    /* append the extension if it's not a directory */
    if ((dirent->deAttributes & ATTR_DIRECTORY) == 0) {
	strcat(fullname, ".");
	strcat(fullname, extension);
    }
}

uint16_t get_fat_entry(uint16_t clusternum, 
		       int fd, struct bpb33* bpb)
{
    uint32_t offset;
    uint16_t value;
    uint8_t b1, b2;
    
    /* this involves some really ugly bit shifting.  This probably
       only works on a little-endian machine. */
    offset = bpb->bpbResSectors * bpb->bpbBytesPerSec * bpb->bpbSecPerClust 
	+ (3 * (clusternum/2));

    dprintf("Get fat entry 0x%x at 0x%x...", clusternum, offset);	

    lseek(fd, offset, SEEK_SET);

    unsigned char sector[3] = {0};
    if (read(fd, sector, 3) != 3 || errno != 0)
      fprintf(stderr, "Read error!\n");

    switch(clusternum % 2) {
    case 0:
	b1 = sector[0]; //*(image_buf + offset);
	b2 = sector[1]; //*(image_buf + offset + 1);
	/* mjh: little-endian CPUs are ugly! */
	value = ((0x0f & b2) << 8) | b1;
	break;
    case 1:
	b1 = sector[1]; //*(image_buf + offset + 1);
	b2 = sector[2]; //*(image_buf + offset + 2);
	value = b2 << 4 | ((0xf0 & b1) >> 4);
	break;
    }
    dprintf(" => 0x%x\n", value);
    return value;
}

void set_fat_entry(uint16_t clusternum, uint16_t value,
		   int fdi, struct bpb33* bpb)
{
    uint32_t offset;
    uint8_t *p1, *p2;
    
    /* this involves some really ugly bit shifting.  This probably
       only works on a little-endian machine. */
    offset = bpb->bpbResSectors * bpb->bpbBytesPerSec * bpb->bpbSecPerClust 
	+ (3 * (clusternum/2));

    dprintf("Set fat entry 0x%x at 0x%x value 0x%x...\n", clusternum, offset, value);	

    lseek(fdi, offset, SEEK_SET);

    unsigned char sector[3] = {0};
    if (read(fdi, sector, 3) != 3 || errno != 0)
      fprintf(stderr, "Read error!\n");

    switch(clusternum % 2) {
    case 0:
	p1 = &sector[0]; //image_buf + offset;
	p2 = &sector[1]; //image_buf + offset + 1;
	/* mjh: little-endian CPUs are really ugly! */
	*p1 = (uint8_t)(0xff & value);
	*p2 = (uint8_t)((0xf0 & (*p2)) | (0x0f & (value >> 8)));
	break;
    case 1:
	p1 = &sector[1]; //image_buf + offset + 1;
	p2 = &sector[2]; //image_buf + offset + 2;

	*p1 = (uint8_t)((0x0f & (*p1)) | ((0x0f & value) << 4));
	*p2 = (uint8_t)(0xff & (value >> 4));
	break;
    }

    lseek(fdi, offset, SEEK_SET);

    if (write(fdi, sector, 3) != 3 || errno != 0)
      fprintf(stderr, "Write error!\n");

    lseek(fdi, offset + 0x1800, SEEK_SET); // fat 2

    if (write(fdi, sector, 3) != 3 || errno != 0)
      fprintf(stderr, "Write error!\n");
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
	(bootsect->bsJump[0] == 0xeb && bootsect->bsJump[2] == 0x90)) {
	dprintf("Good jump inst\n");
    } else {
	fprintf(stderr, "illegal boot sector jump inst: %x%x%x\n", 
		bootsect->bsJump[0], bootsect->bsJump[1], 
		bootsect->bsJump[2]); 
        return NULL;
    } 

    dprintf("OemName: %s\n", bootsect->bsOemName);

    if (bootsect->bsBootSectSig0 == BOOTSIG0
	&& bootsect->bsBootSectSig0 == BOOTSIG0) {
	//Good boot sector sig;
	dprintf("Good boot sector signature\n");
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
    
    dprintf("Bytes per sector: %d\n", bpb2->bpbBytesPerSec);
    dprintf("Sectors per cluster: %d\n", bpb2->bpbSecPerClust);
    dprintf("Reserved sectors: %d\n", bpb2->bpbResSectors);
    dprintf("Number of FATs: %d\n", bpb->bpbFATs);
    dprintf("Number of root dir entries: %d\n", bpb2->bpbRootDirEnts);
    dprintf("Total number of sectors: %d\n", bpb2->bpbSectors);
    dprintf("Number of sectors per FAT: %d\n", bpb2->bpbFATsecs);
    dprintf("Number of hidden sectors: %d\n", bpb2->bpbHiddenSecs);

    return bpb2;
}

int root_dir_addr(int fd, struct bpb33* bpb)
{
    uint32_t offset;
    offset = 
	(bpb->bpbBytesPerSec 
	 * (bpb->bpbResSectors + (bpb->bpbFATs * bpb->bpbFATsecs)));

    return offset;
}

int cluster_to_addr_i(uint16_t cluster, int fd, 
			 struct bpb33* bpb)
{

    int offset = root_dir_addr(fd, bpb);

    if (cluster != MSDOSFSROOT) {
	/* move to the end of the root directory */
	offset += bpb->bpbRootDirEnts * sizeof(struct direntry);
	/* move forward the right number of clusters */
	offset += bpb->bpbBytesPerSec * bpb->bpbSecPerClust 
	    * (cluster - CLUST_FIRST);
    }
  return offset;
}

uint8_t *cluster_to_addr(uint16_t cluster, int fd, 
			 struct bpb33* bpb)
{
    dprintf("Cluster %d", cluster);

    int offset = cluster_to_addr_i(cluster, fd,bpb);

    dprintf(", reading at %x...", offset);

    static unsigned char sector[512] = {0};
    lseek(fd, offset, SEEK_SET);
    if (read(fd, sector, 512) != 512 || errno != 0)
      fprintf(stderr, "Read error!\n");
    dprintf(" ok\n");
    return sector;
}

#define FIND_FILE 0
#define FIND_DIR 1

void listfiles(uint16_t cluster, int fd, struct bpb33* bpb)
{
    char buf[MAXPATHLEN];
    char *seek_name, *next_name;
    int d;
    struct direntry dirent;
    uint16_t dir_cluster;
    dprintf("Listing files at cluster %d\n", cluster);
    int offset = cluster_to_addr_i(cluster, fd, bpb);
 
    /* find the first dirent in this directory */
    while (1) 
    {
	for (d = 0; d < bpb->bpbBytesPerSec * bpb->bpbSecPerClust; 
	     d += sizeof(struct direntry) /*, offset += sizeof(struct direntry)*/) 
        {
            _readdi(fd, dirent, offset, false);

	    if (dirent.deName[0] == SLOT_EMPTY) {
		/* we failed to find the file */
		return;
	    }

  printf("%c%c%c%c%c%c%c%c.%c%c%c attr:%02x low:%x, crea:%02x%02x%02x %02x%02x, high:%02x%02x, modif:%02x%02x %02x%02x, a:%02x%02x, start:%02x%02x size:%02x%02x%02x%02x\n",
  dirent.deName[0],  dirent.deName[1],  dirent.deName[2],  dirent.deName[3],
  dirent.deName[4],  dirent.deName[5],  dirent.deName[6],  dirent.deName[7],
  dirent.deExtension[0],  dirent.deExtension[1],  dirent.deExtension[2],
  dirent.deAttributes,
  dirent.deLowerCase, 
  dirent.deCHundredth, dirent.deCTime[0], dirent.deCTime[1], dirent.deCDate[0], dirent.deCDate[1],
  dirent.deHighClust[0], dirent.deHighClust[1],
  dirent.deMTime[0], dirent.deMTime[1],  dirent.deMDate[0], dirent.deMDate[1],
  dirent.deADate[0], dirent.deADate[1],
  dirent.deStartCluster[1], dirent.deStartCluster[0],
  dirent.deFileSize[3], dirent.deFileSize[2], dirent.deFileSize[1], dirent.deFileSize[0]);

	    if (dirent.deName[0] == SLOT_DELETED) {
		/* skip over a deleted file */
         	offset += sizeof(direntry);
		continue;
	    }

	    offset += sizeof(direntry);
	}
	/* we've reached the end of the cluster for this directory.
	   Where's the next cluster? */
	if (cluster == 0) {
	    // root dir is special
	    offset += sizeof(direntry);
	} else {
	    cluster = get_fat_entry(cluster, fd, bpb);
            offset = cluster_to_addr_i(cluster, fd, bpb);
	}
    }

}

/*struct direntry* */ int find_file(const char *infilename, uint16_t cluster,
			   int find_mode,
			   int fd, struct bpb33* bpb)
{
    char buf[MAXPATHLEN];
    char *seek_name, *next_name;
    int d;
    struct direntry dirent;
    uint16_t dir_cluster;
    char fullname[13];
    dprintf("Scanning file at cluster %d\n", cluster);
    int offset = cluster_to_addr_i(cluster, fd, bpb);
 
    /* first we need to split the file name we're looking for into the
       first part of the path, and the remainder.  We hunt through the
       current directory for the first part.  If there's a remainder,
       and what we find is a directory, then we recurse, and search
       that directory for the remainder */

    strncpy(buf, infilename, MAXPATHLEN);
    seek_name = buf;

    /* trim leading slashes */
    while (*seek_name == '/' || *seek_name == '\\') {
	seek_name++;
    }

    /* search for any more slashes - if so, it's a dirname */
    next_name = seek_name;
    while (1) {
	if (*next_name == '/' || *next_name == '\\') {
	    *next_name = '\0';
	    next_name ++;
	    break;
	}
	if (*next_name == '\0') {
	    /* end of name - no slashes found */
	    next_name = NULL;
	    if (find_mode == FIND_DIR) {
		return offset;
	    }
	    break;
	}
	next_name++;
    }

    /* find the first dirent in this directory */

    while (1) {

	/* hunt a cluster for the relevant dirent.  If we reach the
	   end of the cluster, we'll need to go to the next cluster
	   for this directory */
	for (d = 0; d < bpb->bpbBytesPerSec * bpb->bpbSecPerClust; 
	     d += sizeof(struct direntry) /*, offset += sizeof(struct direntry)*/) 
        {
            _readdi(fd, dirent, offset, true );

	    if (dirent.deName[0] == SLOT_EMPTY) {
		/* we failed to find the file */
		return NULL;
	    }
	    if (dirent.deName[0] == SLOT_DELETED) {
		/* skip over a deleted file */
         	offset += sizeof(direntry);
		continue;
	    }
	    get_name(fullname, &dirent);

	    if (strcasecmp(fullname, seek_name)==0) {
		/* found it! */
		if ((dirent.deAttributes & ATTR_DIRECTORY) != 0) {
		    /* it's a directory */
		    if (next_name == NULL) {
			fprintf(stderr, "Cannot copy out a directory\n");
			exit(1);
		    }
		    dir_cluster = getushort(dirent.deStartCluster);
		    return find_file(next_name, dir_cluster, 
				     find_mode, fd, bpb);
		} else if ((dirent.deAttributes & ATTR_VOLUME) != 0) {
		    /* it's a volume */
		    fprintf(stderr, "Cannot copy out a volume\n");
		    exit(1);
		} else {
		    /* assume it's a file */
		    return offset;//dirent;
		}
	    }
	    offset += sizeof(direntry);
	}
	/* we've reached the end of the cluster for this directory.
	   Where's the next cluster? */
	if (cluster == 0) {
	    // root dir is special
	    offset += sizeof(direntry);
	} else {
	    cluster = get_fat_entry(cluster, fd, bpb);
            dprintf("Scanning file at NEXT cluster %d\n", cluster);

            offset = cluster_to_addr_i(cluster, fd, bpb);
	}
    }
}

uint16_t copy_in_file(FILE* fd, int fdi, struct bpb33* bpb, 
		      uint32_t *size)
{
    uint32_t clust_size, total_clusters, i;
    uint8_t *buf;
    size_t bytes;
    uint16_t start_cluster = 0;
    uint16_t prev_cluster = 0;
    int lastWritten = __target-1;

    clust_size = bpb->bpbSecPerClust * bpb->bpbBytesPerSec;
    total_clusters = bpb->bpbSectors / bpb->bpbSecPerClust;
    buf = (uint8_t*)malloc(clust_size);

    *size = 0;

    while(1) 
    {
        memset(buf, 0, clust_size);

	/* read a block of data, and store it */
	bytes = fread(buf, 1, clust_size, fd);
	if (bytes > 0) {
	    *size += bytes;

	    /* find a free cluster */

	    for (i = 2; i < total_clusters; i++) {
	        if (__target != -1)
                {
                  if (i > lastWritten)
                  {
                    lastWritten = i;
                    break;
                  }
                } else
		if (get_fat_entry(i, fdi, bpb) == CLUST_FREE) {
		    break;
		}
	    }

	    if (i == total_clusters) {
		/* oops - we ran out of disk space */
		fprintf(stderr, "No more space in filesystem\n");
		/* we should clean up here, rather than just exit */ 
		exit(1);
	    }

            dprintf("Found free cluster %d\n", i);

	    /* remember the first cluster, as we need to store this in
	       the dirent */
	    if (start_cluster == 0) {
		start_cluster = i;
          	set_fat_entry(i, FAT12_MASK&CLUST_EOFE, fdi, bpb);

	    } else {
		/* link the previous cluster to this one in the FAT */
		assert(prev_cluster != 0);
    	        set_fat_entry(i, FAT12_MASK&CLUST_EOFE, fdi, bpb);
		set_fat_entry(prev_cluster, i, fdi, bpb);
	    }
	    /* make sure we've recorded this cluster as used */

	    /* copy the data into the cluster */
            assert(clust_size == 512);

            int targetOfs = cluster_to_addr_i(i, fdi, bpb);

            dprintf("Writing %d bytes cluster %x at %x... ", clust_size, i, targetOfs);
            lseek(fdi, targetOfs, SEEK_SET);
            if (write(fdi, buf, clust_size) != clust_size || errno != 0)
              fprintf(stderr, "Write error!\n");

            dprintf(" ok\n");
 	}

	if (bytes < clust_size) {
	    /* We didn't real a full cluster, so we either got a read
	       error, or reached end of file.  We exit anyway */
	    break;
	}
	prev_cluster = i;
    }

    free(buf);
    return start_cluster;
}

/* write the values into a directory entry */
void write_dirent(struct direntry *dirent, const char *filename, 
		   uint16_t start_cluster, uint32_t size)
{
    char *p, *p2;
    char *uppername;
    int len, i;

    /* clean out anything old that used to be here */
    memset(dirent, 0, sizeof(struct direntry));

    /* extract just the filename part */
    uppername = strdup(filename);
    p2 = uppername;
    for (i = 0; i < strlen(filename); i++) {
	if (p2[i] == '/' || p2[i] == '\\') {
	    uppername = p2+i+1;
	}
    }

    /* convert filename to upper case */
    for (i = 0; i < strlen(uppername); i++) {
	uppername[i] = toupper(uppername[i]);
    }

    /* set the file name and extension */
    memset(dirent->deName, ' ', 8);
    p = strchr(uppername, '.');
    memcpy(dirent->deExtension, "___", 3);
    if (p == NULL) {
	fprintf(stderr, "No filename extension given - defaulting to .___\n");
    } else {
	*p = '\0';
	p++;
	len = strlen(p);
	if (len > 3) len = 3;
	memcpy(dirent->deExtension, p, len);
    }
    if (strlen(uppername)>8) {
	uppername[8]='\0';
    }
    memcpy(dirent->deName, uppername, strlen(uppername));
    free(p2);

    /* set the attributes and file size */
    dirent->deAttributes = ATTR_ARCHIVE;
    putushort(dirent->deStartCluster, start_cluster);
    putulong(dirent->deFileSize, size);

    dirent->deCHundredth = 0x16; dirent->deCTime[0] = 0xcf; dirent->deCTime[1] = 0x96; dirent->deCDate[0] = 0x3c; dirent->deCDate[1] = 0x4d;
    dirent->deMTime[0] = 0xcb; dirent->deMTime[1] = 0x96;  dirent->deMDate[0] = 0x3c; dirent->deMDate[1] = 0x4d;
    dirent->deADate[0] = 0x3c; dirent->deADate[1] = 0x4d;
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

void create_dirent(int direntofs, const char *filename, 
		   uint16_t start_cluster, uint32_t size,
		   int fdi, struct bpb33* bpb)
{
    direntry dirent;

    while(1) 
    {
        _readdi(fdi, dirent, direntofs, true);

	if (dirent.deName[0] == SLOT_EMPTY) {
	    /* we found an empty slot at the end of the directory */
	    write_dirent(&dirent, filename, start_cluster, size);
	    _writedi(fdi, dirent, direntofs);

	    direntofs += sizeof(direntry);

	    /* make sure the next dirent is set to be empty, just in
	       case it wasn't before */
	    memset((uint8_t*)&dirent, 0, sizeof(struct direntry));
	    dirent.deName[0] = SLOT_EMPTY;

	    _writedi(fdi, dirent, direntofs);

	    return;
	}
	if (0 && dirent.deName[0] == SLOT_DELETED) {
	    /* we found a deleted entry - we can just overwrite it */
	    write_dirent(&dirent, filename, start_cluster, size);
	    _writedi(fdi, dirent, direntofs);
	    return;
	}

	direntofs += sizeof(direntry);
    }
}

void copyin(const char *infilename, const char* outfilename,
	     int fdi, struct bpb33* bpb)
{
    int direntofs = 0;

    FILE *fd;
    uint16_t start_cluster;
    uint32_t size = 0;

    /* check that the file doesn't already exist */
    direntofs = find_file(outfilename, 0, FIND_FILE, fdi, bpb);
    if (direntofs != 0) {
	fprintf(stderr, "File %s already exists\n", outfilename);
	exit(1);
    }

    /* find the dirent of the directory to put the file in */
    direntofs = find_file(outfilename, 0, FIND_DIR, fdi, bpb);
    if (direntofs == 0) {
	fprintf(stderr, "Directory does not exists in the disk image\n");
	exit(1);
    }

    /* open the real file for reading */
    fd = fopen(infilename, "r");
    if (fd == NULL) {
	fprintf(stderr, "Can't open file %s to copy data in\n",
		infilename);
	exit(1); 
    }
/*
    fseek(fd, 0, SEEK_END);
    size = ftell(fd);
    rewind(fd);
    start_cluster = __target;
    set_fat_entry(start_cluster, FAT12_MASK&CLUST_EOFE, fdi, bpb);
*/
    /* do the actual copy in*/
    start_cluster = copy_in_file(fd, fdi, bpb, &size);

    /* create the directory entry */
    create_dirent(direntofs, outfilename, start_cluster, size, fdi, bpb);

    fclose(fd);
}

bool checkCompatibility(int fdi, bpb33* bpb)
{
    if (bpb->bpbBytesPerSec != 512 || bpb->bpbSecPerClust != 1 || bpb->bpbSectors != 4096)
    {
      fprintf(stderr, "Incompatible device\n");
      return false;
    }
    int offset = cluster_to_addr_i(0, fdi, bpb);
    direntry dirent; 
    _readdi(fdi, dirent, offset, false);
    return memcmp(dirent.deName, "DFU V", 5) == 0;
}

int main(int argc, const char * argv[]) 
{
    if (argc < 3)
    {
      printf("Usage:\n");
      printf("\n");
      printf("  sudo ./fat12upload /dev/disk2 info\n");
      printf("  sudo ./fat12upload /dev/disk2 ls\n");
      printf("  sudo ./fat12upload /dev/disk2 cp from.xyz to.xyz -1\n");
      return 1;
    }

    int fd = open(argv[1], O_RDWR);
    fcntl(fd, F_NOCACHE, 1);
//    setvbuf(fileno(fd), NULL, _IONBF, 0);

    if (fd<0)
    {
      fprintf(stderr, "Failed to open disk '%s', errno:%d\n", argv[1], errno);
      return 1;
    }

    bpb33* bpb = 0;
    bpb = check_bootsector(fd);

    if (strcmp(argv[2], "info") == 0)
    {
       printf("Device: %s\n", argv[1]);
       printf("Bytes per sector: %d\n", bpb->bpbBytesPerSec);
       printf("Sectors per cluster: %d\n", bpb->bpbSecPerClust);
       printf("Reserved sectors: %d\n", bpb->bpbResSectors);
       printf("Number of root dir entries: %d\n", bpb->bpbRootDirEnts);
       printf("Total number of sectors: %d\n", bpb->bpbSectors);
       printf("Number of sectors per FAT: %d\n", bpb->bpbFATsecs);
       printf("Number of hidden sectors: %d\n", bpb->bpbHiddenSecs);
    } else
    if (strcmp(argv[2], "ls") == 0)
    {
      int root = 0;
      if (argc == 4)
        root = atoi(argv[3]);
      listfiles(root, fd, bpb);
    } else
    if (strcmp(argv[2], "cp") == 0 && argc == 4)
    { 
      if (checkCompatibility(fd, bpb))
        copyin(argv[3], argv[3], fd, bpb);
    } else
    if (strcmp(argv[2], "cp") == 0 && argc == 5)
    { 
      if (checkCompatibility(fd, bpb))
        copyin(argv[3], argv[4], fd, bpb);
    } else
    if (strcmp(argv[2], "cp") == 0 && argc == 6)
    { 
      __target = atoi(argv[5]);
      if (checkCompatibility(fd, bpb))
        copyin(argv[3], argv[4], fd, bpb);
    } else
    if (strcmp(argv[2], "fat") == 0)
    { 
      int n = 32;
      if (argc == 4)
        n = atoi(argv[3]);
      for (int i=0; i<n; i++)
        printf("cluster 0x%x: %0x\n", i, get_fat_entry(i, fd, bpb));
    } else
    if (strcmp(argv[2], "read") == 0 && argc == 4)
    {
      int cluster = atoi(argv[3]);
 
      int offset = cluster_to_addr_i(cluster, fd, bpb);
      printf("Reading cluster 0x%x at 0x%0x:\n", cluster, offset);
      lseek(fd, offset, SEEK_SET);

      unsigned char sector[512] = {0};
      if (read(fd, sector, 512) != 512 || errno != 0)
      {
          fprintf(stderr, "Read error!\n");
          return NULL;
      }

      for (int i=0; i<512; i++)
      {
        char c = sector[i];
        printf("%c", (isprint(c) || c == 0x0d || c == 0x0a) ? c : '?');
      }
      printf("\n");
    } else 
    if (strcmp(argv[2], "clfat") == 0)
    {
      int total_clusters = bpb->bpbSectors / bpb->bpbSecPerClust;
      for (int i=1; i<total_clusters; i++)
        set_fat_entry(i, 0, fd, bpb);
    } else
    if (strcmp(argv[2], "cldir") == 0)
    {
      direntry dirent; 
      memset((uint8_t*)&dirent, 0, sizeof(struct direntry));
      dirent.deName[0] = SLOT_EMPTY;

      int root = cluster_to_addr_i(0, fd, bpb);
      for (int i=1; i<20; i++)
	_writedi(fd, dirent, root + i*sizeof(direntry));
    } else
    {
      fprintf(stderr, "Nothing to do.\n");
    }

    close(fd);
    free(bpb);

    return 0;
}
