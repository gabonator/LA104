/* $FreeBSD: src/sys/fs/msdosfs/bootsect.h,v 1.11 2003/12/26 17:24:37 trhodes Exp $ */
/*	$NetBSD: bootsect.h,v 1.9 1997/11/17 15:36:17 ws Exp $	*/

/*
 * Written by Paul Popelka (paulp@uts.amdahl.com)
 *
 * You can do anything you want with this software, just don't say you wrote
 * it, and don't remove this notice.
 *
 * This software is provided "as is".
 *
 * The author supplies this software to be publicly redistributed on the
 * understanding that the author is not responsible for the correct
 * functioning of this software in any circumstances and is not liable for
 * any damages caused by this software.
 *
 * October 1992
 */

/*
 * Some useful cluster numbers.
 */
#define	MSDOSFSROOT	0		/* cluster 0 means the root dir */
#define	CLUST_FREE	0		/* cluster 0 also means a free cluster */
#define	MSDOSFSFREE	CLUST_FREE
#define	CLUST_FIRST	2		/* first legal cluster number */
#define	CLUST_RSRVD	0xfffffff6	/* reserved cluster range */
#define	CLUST_BAD	0xfffffff7	/* a cluster with a defect */
#define	CLUST_EOFS	0xfffffff8	/* start of eof cluster range */
#define	CLUST_EOFE	0xffffffff	/* end of eof cluster range */

#define	FAT12_MASK	0x00000fff	/* mask for 12 bit cluster numbers */
#define	FAT16_MASK	0x0000ffff	/* mask for 16 bit cluster numbers */
#define	FAT32_MASK	0x0fffffff	/* mask for FAT32 cluster numbers */

/*
 * Format of a boot sector.  This is the first sector on a DOS floppy disk
 * or the fist sector of a partition on a hard disk.  But, it is not the
 * first sector of a partitioned hard disk.
 */
struct bootsector33 {
	u_int8_t	bsJump[3];		/* jump inst E9xxxx or EBxx90 */
	int8_t		bsOemName[8];		/* OEM name and version */
	int8_t		bsBPB[19];		/* BIOS parameter block */
	int8_t		bsDriveNumber;		/* drive number (0x80) */
	int8_t		bsBootCode[479];	/* pad so struct is 512b */
	u_int8_t	bsBootSectSig0;
	u_int8_t	bsBootSectSig1;
#define	BOOTSIG0	0x55
#define	BOOTSIG1	0xaa
};

struct extboot {
	int8_t		exDriveNumber;		/* drive number (0x80) */
	int8_t		exReserved1;		/* reserved */
	int8_t		exBootSignature;	/* ext. boot signature (0x29) */
#define	EXBOOTSIG	0x29
	int8_t		exVolumeID[4];		/* volume ID number */
	int8_t		exVolumeLabel[11];	/* volume label */
	int8_t		exFileSysType[8];	/* fs type (FAT12 or FAT16) */
};

struct bootsector50 {
	u_int8_t	bsJump[3];		/* jump inst E9xxxx or EBxx90 */
	int8_t		bsOemName[8];		/* OEM name and version */
	int8_t		bsBPB[25];		/* BIOS parameter block */
	int8_t		bsExt[26];		/* Bootsector Extension */
	int8_t		bsBootCode[448];	/* pad so structure is 512b */
	u_int8_t	bsBootSectSig0;
	u_int8_t	bsBootSectSig1;
#define	BOOTSIG0	0x55
#define	BOOTSIG1	0xaa
};

struct bootsector710 {
	u_int8_t	bsJump[3];		/* jump inst E9xxxx or EBxx90 */
	int8_t		bsOEMName[8];		/* OEM name and version */
	int8_t		bsBPB[53];		/* BIOS parameter block */
	int8_t		bsExt[26];		/* Bootsector Extension */
	int8_t		bsBootCode[418];	/* pad so structure is 512b */
	u_int8_t	bsBootSectSig2;		/* 2 & 3 are only defined for FAT32? */
	u_int8_t	bsBootSectSig3;
	u_int8_t	bsBootSectSig0;
	u_int8_t	bsBootSectSig1;
#define	BOOTSIG0	0x55
#define	BOOTSIG1	0xaa
#define	BOOTSIG2	0
#define	BOOTSIG3	0
};

union bootsector {
	struct bootsector33 bs33;
	struct bootsector50 bs50;
	struct bootsector710 bs710;
};

#if 0
/*
 * Shorthand for fields in the bpb.
 */
#define	bsBytesPerSec	bsBPB.bpbBytesPerSec
#define	bsSectPerClust	bsBPB.bpbSectPerClust
#define	bsResSectors	bsBPB.bpbResSectors
#define	bsFATS		bsBPB.bpbFATS
#define	bsRootDirEnts	bsBPB.bpbRootDirEnts
#define	bsSectors	bsBPB.bpbSectors
#define	bsMedia		bsBPB.bpbMedia
#define	bsFATsecs	bsBPB.bpbFATsecs
#define	bsSectPerTrack	bsBPB.bpbSectPerTrack
#define	bsHeads		bsBPB.bpbHeads
#define	bsHiddenSecs	bsBPB.bpbHiddenSecs
#define	bsHugeSectors	bsBPB.bpbHugeSectors
#endif

/*
 * BIOS Parameter Block (BPB) for DOS 3.3
 */
struct bpb33 {
	u_int16_t	bpbBytesPerSec;	/* bytes per sector */
	u_int8_t	bpbSecPerClust;	/* sectors per cluster */
	u_int16_t	bpbResSectors;	/* number of reserved sectors */
	u_int8_t	bpbFATs;	/* number of FATs */
	u_int16_t	bpbRootDirEnts;	/* number of root directory entries */
	u_int16_t	bpbSectors;	/* total number of sectors */
	u_int8_t	bpbMedia;	/* media descriptor */
	u_int16_t	bpbFATsecs;	/* number of sectors per FAT */
	u_int16_t	bpbSecPerTrack;	/* sectors per track */
	u_int16_t	bpbHeads;	/* number of heads */
	u_int16_t	bpbHiddenSecs;	/* number of hidden sectors */
};

/*
 * BPB for DOS 5.0 The difference is bpbHiddenSecs is a short for DOS 3.3,
 * and bpbHugeSectors is not in the 3.3 bpb.
 */
struct bpb50 {
	u_int16_t	bpbBytesPerSec;	/* bytes per sector */
	u_int8_t	bpbSecPerClust;	/* sectors per cluster */
	u_int16_t	bpbResSectors;	/* number of reserved sectors */
	u_int8_t	bpbFATs;	/* number of FATs */
	u_int16_t	bpbRootDirEnts;	/* number of root directory entries */
	u_int16_t	bpbSectors;	/* total number of sectors */
	u_int8_t	bpbMedia;	/* media descriptor */
	u_int16_t	bpbFATsecs;	/* number of sectors per FAT */
	u_int16_t	bpbSecPerTrack;	/* sectors per track */
	u_int16_t	bpbHeads;	/* number of heads */
	u_int32_t	bpbHiddenSecs;	/* # of hidden sectors */
	u_int32_t	bpbHugeSectors;	/* # of sectors if bpbSectors == 0 */
};

/*
 * BPB for DOS 7.10 (FAT32).  This one has a few extensions to bpb50.
 */
struct bpb710 {
	u_int16_t	bpbBytesPerSec;	/* bytes per sector */
	u_int8_t	bpbSecPerClust;	/* sectors per cluster */
	u_int16_t	bpbResSectors;	/* number of reserved sectors */
	u_int8_t	bpbFATs;	/* number of FATs */
	u_int16_t	bpbRootDirEnts;	/* number of root directory entries */
	u_int16_t	bpbSectors;	/* total number of sectors */
	u_int8_t	bpbMedia;	/* media descriptor */
	u_int16_t	bpbFATsecs;	/* number of sectors per FAT */
	u_int16_t	bpbSecPerTrack;	/* sectors per track */
	u_int16_t	bpbHeads;	/* number of heads */
	u_int32_t	bpbHiddenSecs;	/* # of hidden sectors */
	u_int32_t	bpbHugeSectors;	/* # of sectors if bpbSectors == 0 */
	u_int32_t	bpbBigFATsecs;	/* like bpbFATsecs for FAT32 */
	u_int16_t	bpbExtFlags;	/* extended flags: */
#define	FATNUM		0xf		/* mask for numbering active FAT */
#define	FATMIRROR	0x80		/* FAT is mirrored (like it always was) */
	u_int16_t	bpbFSVers;	/* filesystem version */
#define	FSVERS		0		/* currently only 0 is understood */
	u_int32_t	bpbRootClust;	/* start cluster for root directory */
	u_int16_t	bpbFSInfo;	/* filesystem info structure sector */
	u_int16_t	bpbBackup;	/* backup boot sector */
	/* There is a 12 byte filler here, but we ignore it */
};

/*
 * The following structures represent how the bpb's look on disk.  shorts
 * and longs are just character arrays of the appropriate length.  This is
 * because the compiler forces shorts and longs to align on word or
 * halfword boundaries.
 *
 * XXX The little-endian code here assumes that the processor can access
 * 16-bit and 32-bit quantities on byte boundaries.  If this is not true,
 * use the macros for the big-endian case.
 */

#include "i386/endian.h"

#ifdef __i386__
#define	UNLALIGNED_ACCESS
#endif

#if (BYTE_ORDER == LITTLE_ENDIAN) && defined(UNALIGNED_ACCESS)
#define	getushort(x)	*((u_int16_t *)(x))
#define	getulong(x)	*((u_int32_t *)(x))
#define	putushort(p, v)	(*((u_int16_t *)(p)) = (v))
#define	putulong(p, v)	(*((u_int32_t *)(p)) = (v))
#else
#define getushort(x)	(((u_int8_t *)(x))[0] + (((u_int8_t *)(x))[1] << 8))
#define getulong(x)	(((u_int8_t *)(x))[0] + (((u_int8_t *)(x))[1] << 8) \
			 + (((u_int8_t *)(x))[2] << 16)	\
			 + (((u_int8_t *)(x))[3] << 24))
#define putushort(p, v)	(((u_int8_t *)(p))[0] = (v),	\
			 ((u_int8_t *)(p))[1] = (v) >> 8)
#define putulong(p, v)	(((u_int8_t *)(p))[0] = (v),	\
			 ((u_int8_t *)(p))[1] = (v) >> 8, \
			 ((u_int8_t *)(p))[2] = (v) >> 16,\
			 ((u_int8_t *)(p))[3] = (v) >> 24)
#endif

/*
 * BIOS Parameter Block (BPB) for DOS 3.3
 */
struct byte_bpb33 {
	int8_t bpbBytesPerSec[2];	/* bytes per sector */
	int8_t bpbSecPerClust;		/* sectors per cluster */
	int8_t bpbResSectors[2];	/* number of reserved sectors */
	int8_t bpbFATs;			/* number of FATs */
	int8_t bpbRootDirEnts[2];	/* number of root directory entries */
	int8_t bpbSectors[2];		/* total number of sectors */
	int8_t bpbMedia;		/* media descriptor */
	int8_t bpbFATsecs[2];		/* number of sectors per FAT */
	int8_t bpbSecPerTrack[2];	/* sectors per track */
	int8_t bpbHeads[2];		/* number of heads */
	int8_t bpbHiddenSecs[2];	/* number of hidden sectors */
};

/*
 * BPB for DOS 5.0 The difference is bpbHiddenSecs is a short for DOS 3.3,
 * and bpbHugeSectors is not in the 3.3 bpb.
 */
struct byte_bpb50 {
	int8_t bpbBytesPerSec[2];	/* bytes per sector */
	int8_t bpbSecPerClust;		/* sectors per cluster */
	int8_t bpbResSectors[2];	/* number of reserved sectors */
	int8_t bpbFATs;			/* number of FATs */
	int8_t bpbRootDirEnts[2];	/* number of root directory entries */
	int8_t bpbSectors[2];		/* total number of sectors */
	int8_t bpbMedia;		/* media descriptor */
	int8_t bpbFATsecs[2];		/* number of sectors per FAT */
	int8_t bpbSecPerTrack[2];	/* sectors per track */
	int8_t bpbHeads[2];		/* number of heads */
	int8_t bpbHiddenSecs[4];	/* number of hidden sectors */
	int8_t bpbHugeSectors[4];	/* # of sectors if bpbSectors == 0 */
};

/*
 * BPB for DOS 7.10 (FAT32).  This one has a few extensions to bpb50.
 */
struct byte_bpb710 {
	u_int8_t bpbBytesPerSec[2];	/* bytes per sector */
	u_int8_t bpbSecPerClust;	/* sectors per cluster */
	u_int8_t bpbResSectors[2];	/* number of reserved sectors */
	u_int8_t bpbFATs;		/* number of FATs */
	u_int8_t bpbRootDirEnts[2];	/* number of root directory entries */
	u_int8_t bpbSectors[2];		/* total number of sectors */
	u_int8_t bpbMedia;		/* media descriptor */
	u_int8_t bpbFATsecs[2];		/* number of sectors per FAT */
	u_int8_t bpbSecPerTrack[2];	/* sectors per track */
	u_int8_t bpbHeads[2];		/* number of heads */
	u_int8_t bpbHiddenSecs[4];	/* # of hidden sectors */
	u_int8_t bpbHugeSectors[4];	/* # of sectors if bpbSectors == 0 */
	u_int8_t bpbBigFATsecs[4];	/* like bpbFATsecs for FAT32 */
	u_int8_t bpbExtFlags[2];	/* extended flags: */
	u_int8_t bpbFSVers[2];		/* filesystem version */
	u_int8_t bpbRootClust[4];	/* start cluster for root directory */
	u_int8_t bpbFSInfo[2];		/* filesystem info structure sector */
	u_int8_t bpbBackup[2];		/* backup boot sector */
	/* There is a 12 byte filler here, but we ignore it */
};

/*
 * FAT32 FSInfo block.
 */
struct fsinfo {
	u_int8_t fsisig1[4];
	u_int8_t fsifill1[480];
	u_int8_t fsisig2[4];
	u_int8_t fsinfree[4];
	u_int8_t fsinxtfree[4];
	u_int8_t fsifill2[12];
	u_int8_t fsisig3[4];
	u_int8_t fsifill3[508];
	u_int8_t fsisig4[4];
};

/* 
 * Trimmed down by mjh, Nov 2005
 */

/*
 * Structure of a dos directory entry.
 */
struct direntry {
	u_int8_t	deName[8];	/* filename, blank filled */
#define	SLOT_EMPTY	0x00		/* slot has never been used */
#define	SLOT_E5		0x05		/* the real value is 0xe5 */
#define	SLOT_DELETED	0xe5		/* file in this slot deleted */
	u_int8_t	deExtension[3];	/* extension, blank filled */
	u_int8_t	deAttributes;	/* file attributes */
#define	ATTR_NORMAL	0x00		/* normal file */
#define	ATTR_READONLY	0x01		/* file is readonly */
#define	ATTR_HIDDEN	0x02		/* file is hidden */
#define	ATTR_SYSTEM	0x04		/* file is a system file */
#define	ATTR_VOLUME	0x08		/* entry is a volume label */
#define	ATTR_DIRECTORY	0x10		/* entry is a directory name */
#define	ATTR_ARCHIVE	0x20		/* file is new or modified */
	u_int8_t	deLowerCase;	/* NT VFAT lower case flags */
#define	LCASE_BASE	0x08		/* filename base in lower case */
#define	LCASE_EXT	0x10		/* filename extension in lower case */
	u_int8_t	deCHundredth;	/* hundredth of seconds in CTime */
	u_int8_t	deCTime[2];	/* create time */
	u_int8_t	deCDate[2];	/* create date */
	u_int8_t	deADate[2];	/* access date */
	u_int8_t	deHighClust[2];	/* high bytes of cluster number */
	u_int8_t	deMTime[2];	/* last update time */
	u_int8_t	deMDate[2];	/* last update date */
	u_int8_t	deStartCluster[2]; /* starting cluster of file */
	u_int8_t	deFileSize[4];	/* size of file in bytes */
};


/*
 * This is the format of the contents of the deTime field in the direntry
 * structure.
 * We don't use bitfields because we don't know how compilers for
 * arbitrary machines will lay them out.
 */
#define DT_2SECONDS_MASK	0x1F	/* seconds divided by 2 */
#define DT_2SECONDS_SHIFT	0
#define DT_MINUTES_MASK		0x7E0	/* minutes */
#define DT_MINUTES_SHIFT	5
#define DT_HOURS_MASK		0xF800	/* hours */
#define DT_HOURS_SHIFT		11

/*
 * This is the format of the contents of the deDate field in the direntry
 * structure.
 */
#define DD_DAY_MASK		0x1F	/* day of month */
#define DD_DAY_SHIFT		0
#define DD_MONTH_MASK		0x1E0	/* month */
#define DD_MONTH_SHIFT		5
#define DD_YEAR_MASK		0xFE00	/* year - 1980 */
#define DD_YEAR_SHIFT		9

