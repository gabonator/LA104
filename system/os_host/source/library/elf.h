#pragma once

typedef uint32_t	Elf32_Addr;
typedef uint16_t	Elf32_Half;
typedef uint32_t	Elf32_Off;
typedef int32_t		Elf32_Sword;
typedef uint32_t	Elf32_Word;
typedef uint32_t	Elf32_Size;

#pragma pack(push)
#pragma pack(2)
typedef struct {
        unsigned char   ident[16];
        Elf32_Half      type;
        Elf32_Half      machine;
        Elf32_Word      version;
        Elf32_Addr      entry;
        Elf32_Off       phoff;
        Elf32_Off       shoff;
        Elf32_Word      flags;
        Elf32_Half      ehsize;
        Elf32_Half      phentsize;
        Elf32_Half      phnum;
        Elf32_Half      shentsize;
        Elf32_Half      shnum;
        Elf32_Half      shtrndx;
} Elf32_Ehdr;

typedef struct {
	Elf32_Word	type;		/* Entry type. */
	Elf32_Off	offset;	/* File offset of contents. */
	Elf32_Addr	vaddr;	/* Virtual address in memory image. */
	Elf32_Addr	paddr;	/* Physical address (not used). */
	Elf32_Size	filesz;	/* Size of contents in file. */
	Elf32_Size	memsz;	/* Size of contents in memory. */
	Elf32_Word	flags;	/* Access permission flags. */
	Elf32_Size	align;	/* Alignment in memory and file. */
} Elf32_Phdr;

typedef struct
{
	Elf32_Word    name;                /* Section name (string tbl index) */
	Elf32_Word    type;                /* Section type */
	Elf32_Word    flags;               /* Section flags */
	Elf32_Addr    addr;                /* Section virtual addr at execution */
	Elf32_Off     offset;              /* Section file offset */
	Elf32_Word    size;                /* Section size in bytes */
	Elf32_Word    link;                /* Link to another section */
	Elf32_Word    info;                /* Additional section information */
	Elf32_Word    addralign;           /* Section alignment */
	Elf32_Word    entsize;             /* Entry size if section holds table */
} Elf32_Shdr;

typedef struct {
    Elf32_Word      st_name;
    Elf32_Addr      st_value;
    Elf32_Word      st_size;
    unsigned char   st_info;
    unsigned char   st_other;
    Elf32_Half      st_shndx;
} Elf32_Sym;

typedef struct
{
	Elf32_Addr    r_offset;               /* Address */
	Elf32_Word    r_info;                 /* Relocation type and symbol index */
} Elf32_Rel;

#pragma pack(pop)

enum Elf32_enums
{
	Elf32_PTypeNull = 0,
	Elf32_PTypeLoad = 1,
	Elf32_PTypeDynamic = 2,
	Elf32_PTypeInterpreter = 3,
	Elf32_PTypeHeader = 6
};