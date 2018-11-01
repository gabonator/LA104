// elftest2.cpp : Defines the entry point for the console application.
//

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

#define EXPPROG 0

#define DUMP 1
#define EI_NIDENT 16

#pragma GCC diagnostic ignored "-Wwrite-strings"
//#pragma clang diagnostic ignored "no-write-strings"
//#pragma clang diagnostic ignored "write-strings"

#define _ASSERT assert
#define _T(a) a
class CString
{
    std::string data;
    
public:
    CString(char* str)
    {
        data = std::string(str);
    }
    CString(unsigned char* str)
    {
        data = std::string((char*)str);
    }
    CString(std::string str)
    {
        data = str;
    }
    /*
    operator const char*()
    {
        return data.c_str();
    }*/
    const char* GetBuffer()
    {
        return data.c_str();
    }

    bool operator == (const CString& other)
    {
        return other.data == data;
    }
    
    CString Left(int n)
    {
        return data.substr(0, n);
    }
    
    int GetLength()
    {
        return (int)data.size();
    }
};

template <class T>
class CArray
{
    std::vector<T> data;
public:
    int GetSize() { return data.size(); }
    int GetCount() { return data.size(); }
    T& operator[](int i) { return data[i]; }
    void Add(const T& o)
    {
        data.push_back(o);
    }
};

typedef uint32_t    Elf32_Addr;
typedef uint16_t    Elf32_Half;
typedef uint32_t    Elf32_Off;
typedef int32_t        Elf32_Sword;
typedef uint32_t    Elf32_Word;
typedef uint32_t    Elf32_Size;

typedef struct {
    unsigned char   ident[EI_NIDENT];
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
    Elf32_Word    type;        /* Entry type. */
    Elf32_Off    offset;    /* File offset of contents. */
    Elf32_Addr    vaddr;    /* Virtual address in memory image. */
    Elf32_Addr    paddr;    /* Physical address (not used). */
    Elf32_Size    filesz;    /* Size of contents in file. */
    Elf32_Size    memsz;    /* Size of contents in memory. */
    Elf32_Word    flags;    /* Access permission flags. */
    Elf32_Size    align;    /* Alignment in memory and file. */
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

class CElf
{
public:
    typedef Elf32_Ehdr FileHeader;
    typedef Elf32_Phdr ProgramHeader;
    typedef Elf32_Shdr SectionHeader;
    
    class CElfProgram
    {
    public:
        CElfProgram()
        {
            m_pDataPtr = NULL;
            m_nTargetSec = -1;
        }
        CElfProgram( CElf* pElf, ProgramHeader* phdr )
        {
            m_pElf = pElf;
            m_Header = *phdr;
            m_pDataPtr = pElf->m_elfHeader.m_pFileData + phdr->offset;
        }
        void Dump();
        ProgramHeader m_Header;
        
        unsigned char* m_pDataPtr;
        CElf* m_pElf;
        
        int m_nTargetSec;
    };
    
    class CElfSection
    {
    public:
        CElfSection()
        {
            m_strName[0] = 0;
            m_nExTargetProg = -1;
            m_pDataPtr = NULL;
        }
        CElfSection( CElf* pElf, SectionHeader* shdr )
        {
            m_pElf = pElf;
            m_Header = *shdr;
            strcpy(m_strName, m_pElf->m_elfHeader.m_pNamesData + m_Header.name);
            m_pDataPtr = m_pElf->m_elfHeader.m_pFileData + shdr->offset;
            m_nExTargetProg = -1;
        }
        /*
         CElfProgram CreateProgram()
         {
         CElfProgram prog = m_pElf.GetProgramBySection( this );
         return prog;
         }*/
        void Dump();
        void Export(CString strFile);
        SectionHeader m_Header;
        char m_strName[128];
        
        unsigned char* m_pDataPtr;
        CElf* m_pElf;
        
        //
        int m_nExTargetProg;
        int m_nExTargetOfs;
    };
    
    class CElfBinary
    {
    public:
        CElfBinary()
        {
            m_pFileData = NULL;
            m_pElf = NULL;
        }
        
        ~CElfBinary()
        {
            if ( m_pFileData )
                delete m_pFileData;
        }
        
        void Dump();
        
        Elf32_Ehdr m_Header;
        unsigned char* m_pFileData;
        char* m_pNamesData;
        int m_nFileLen;
        CElf* m_pElf;
    };
    
private:
    CElfBinary m_elfHeader;
    CArray<CElfProgram> m_arrProgram;
    CArray<CElfSection> m_arrSection;
    
public:
    CElf();
    void Load(CString strFileName);
    void Save(CString strFileName);
    void Dump();
    void Process();
    void BuildNames(CElfSection secOrig, CElfSection secDynSym);
    
    CElfProgram GetProgramBySection( const CElfSection& sec );
    CElfSection GetSection(CString strName);
    CElfSection GetSectionMerged(CString strName);
    bool HasSection(CString strName);
    
    int GetSections() { return m_arrSection.GetSize(); }
    CElfSection GetSection(int i) { return m_arrSection[i]; }
    
    void PairSection( CString strName, CString strLink );
    CElfBinary& GetHeader()
    {
        return m_elfHeader;
    }
    
public:
    void operator << (const CElfBinary& elf);
    void operator << (CElfSection hdr);
    //void operator << (SectionHeader& hdr);
    
private:
    int GetSectionByName( CString strName );
    int GetProgramBySection( int n );
};

void CElf::PairSection( CString strName, CString strLink )
{
    m_arrSection[GetSectionByName( strName )].m_Header.link = GetSectionByName( strLink );
}

void CElf::Process()
{
    m_elfHeader.m_Header.phnum = m_arrProgram.GetSize();
    m_elfHeader.m_Header.shnum = m_arrSection.GetSize();
    // recalculate offsets
    int nOffset = 0;
    nOffset += sizeof(CElf::FileHeader);
    m_elfHeader.m_Header.phoff = nOffset;
    nOffset += m_arrProgram.GetSize() * sizeof(CElf::ProgramHeader);
    m_elfHeader.m_Header.shoff = nOffset;
    nOffset += m_arrSection.GetSize() * sizeof(CElf::SectionHeader);
#if EXPPROG
    for ( int i=0; i<m_arrProgram.GetSize(); i++)
    {
        m_arrProgram[i].m_Header.offset = nOffset;
        nOffset += m_arrProgram[i].m_Header.filesz;
    }
    int nLastOfs = 0;
    // update section offsets
    for ( int i=0; i<m_arrSection.GetSize(); i++)
    {
        CElf::CElfSection& sec = m_arrSection[i];
        if ( CString(sec.m_strName) == ".shstrtab" )
        {
            m_elfHeader.m_Header.shtrndx = i;
            sec.m_Header.offset = nOffset;
            continue;
        }
        if ( sec.m_Header.type == 8 ) // NOBITS
        {
            sec.m_Header.offset = nLastOfs;
            sec.m_pDataPtr = NULL;
            continue;
        }
        _ASSERT( sec.m_nExTargetProg != -1 );
        sec.m_Header.offset = m_arrProgram[sec.m_nExTargetProg].m_Header.offset + sec.m_nExTargetOfs;
        nLastOfs = sec.m_Header.offset + sec.m_Header.size;
    }
#else
    // update section offsets
    for ( int i=0; i<m_arrSection.GetSize(); i++)
    {
        CElf::CElfSection& sec = m_arrSection[i];
        if ( CString(sec.m_strName) == ".shstrtab" )
        {
            m_elfHeader.m_Header.shtrndx = i;
            sec.m_Header.offset = nOffset;
            continue;
        }
        if ( sec.m_Header.type == 8 ) // NOBITS
        {
            sec.m_Header.offset = nOffset;
            sec.m_pDataPtr = NULL;
            continue;
        }
        sec.m_Header.offset = nOffset;
        nOffset += sec.m_Header.size;
    }
    for ( int i=0; i<m_arrProgram.GetSize(); i++)
    {
        CElf::CElfProgram& prog = m_arrProgram[i];
        if ( prog.m_nTargetSec != -1 )
            prog.m_Header.offset = m_arrSection[prog.m_nTargetSec].m_Header.offset;
    }
#endif
}

void CElf::BuildNames(CElfSection secOrig, CElfSection newSymtab)
{
    char strNames[1024];
    char *strCurName = strNames;
    secOrig.m_Header.name = 0;
    
    if ( newSymtab.m_pDataPtr )
    {
        Elf32_Sym* pSym = (Elf32_Sym*)newSymtab.m_pDataPtr;
        for (int i=0, j=0; i<newSymtab.m_Header.size; i+=sizeof(Elf32_Sym), pSym++, j++)
        {
            int nOrigOfs = pSym->st_name;
            pSym->st_name = strCurName - strNames;
            strcpy(strCurName, (char*)(secOrig.m_pElf->GetSection(".strtab").m_pDataPtr + nOrigOfs));
            strCurName += strlen(strCurName) + 1;
        }
    } else
    {
        strcpy(strCurName, secOrig.m_strName);
        strCurName += strlen(secOrig.m_strName) + 1;
        for (int i=0; i<m_arrSection.GetSize(); i++)
        {
            m_arrSection[i].m_Header.name = strCurName - strNames;
            strcpy(strCurName, m_arrSection[i].m_strName);
            strCurName += strlen(m_arrSection[i].m_strName) + 1;
        }
    }
    
    int nLen = strCurName-strNames;
    
    secOrig.m_pDataPtr = new unsigned char[nLen];
    memcpy(secOrig.m_pDataPtr, strNames, nLen); // mleak
    secOrig.m_Header.size = nLen;
    m_arrSection.Add( secOrig );
}

CElf::CElfProgram CElf::GetProgramBySection( const CElf::CElfSection& sec )
{
    int nBegin = sec.m_Header.offset;
    int nEnd = nBegin + sec.m_Header.size;
    
    for (int i=0; i<m_arrProgram.GetCount(); i++)
    {
        Elf32_Phdr& phdr = m_arrProgram[i].m_Header;
        if ( nBegin >= (int)phdr.offset && nBegin <= (int)phdr.offset + (int)phdr.filesz )
        {
            if ( nEnd <= (int)phdr.offset + (int)phdr.memsz )
                return m_arrProgram[i];
        }
    }
    _ASSERT(0);
    return CElf::CElfProgram();
}

bool CElf::HasSection(CString strName)
{
    for (int i=0; i<m_arrSection.GetSize(); i++)
        if ( CString(m_arrSection[i].m_strName) == strName )
            return true;
    return false;
}

CElf::CElfSection CElf::GetSection(CString strName)
{
    for (int i=0; i<m_arrSection.GetSize(); i++)
        if ( CString(m_arrSection[i].m_strName) == strName )
            return m_arrSection[i];

    printf("Cant find section '%s', available sections:\n", strName.GetBuffer());
    for (int i=0; i<m_arrSection.GetSize(); i++)
      printf("  %d: %s\n", i, m_arrSection[i].m_strName);

    _ASSERT(0);
    return CElfSection();
}

CElf::CElfSection CElf::GetSectionMerged(CString strName)
{
    for (int i=0; i<m_arrSection.GetSize(); i++)
        if ( CString(m_arrSection[i].m_strName).Left(strName.GetLength()) == strName )
        {
            int nLast = i;
            int nTotalSize = m_arrSection[i].m_Header.size;
            // starting from this section
            for ( int j=i+1; j<m_arrSection.GetSize(); j++ )
            {
                if ( CString(m_arrSection[i].m_strName).Left(strName.GetLength()) == strName )
                {
                    CElf::SectionHeader& prev = m_arrSection[j-1].m_Header;
                    CElf::SectionHeader& cur = m_arrSection[j].m_Header;
                    
                    nTotalSize += cur.size;
                    _ASSERT( prev.offset + prev.size == cur.offset );
                    _ASSERT( prev.addr + prev.size == cur.addr );
                    nLast = j;
                }
                else
                    break;
            }
            if ( i==nLast )
                return m_arrSection[i];
            
            CElf::SectionHeader merged = m_arrSection[i].m_Header;
            merged.size = nTotalSize;
        }
    _ASSERT(0);
    return CElfSection();
}

void CElf::CElfBinary::Dump()
{
    printf("num programs = %d, num sections = %d, ", m_Header.phnum, m_Header.shnum);
    printf("entry addr = %08x\n", m_Header.entry);
}

void CElf::CElfProgram::Dump()
{
    CElf::ProgramHeader* ph = &m_Header;
    char *type = "?      ";
    switch (ph->type)
    {
        case 0: type = "NULL   "; break;
        case 1: type = "LOAD   "; break;
        case 2: type = "DYNAMIC"; break;
        case 3: type = "INTERPR"; break;
        case 6: type = "PHEADER"; break;
    }
    printf(" type %2d (%s)", ph->type, type);
    printf(" offset %6d", ph->offset);
    printf(" lengthInFile %5d", ph->filesz);
    printf(" startAddress %08x", ph->vaddr);
    printf(" sizeInMemory %5d", ph->memsz);
    printf(" protFlags %d %c%c%c", ph->flags, (ph->flags & 1) ? 'X' : ' ', (ph->flags & 2) ? 'W' : ' ', (ph->flags & 4) ? 'R' : ' ');
    printf("\n");
}

void CElf::CElfSection::Export(CString strFile)
{
    FILE* f;
    f = fopen(strFile.GetBuffer(), "wb");
    fwrite(m_pDataPtr, m_Header.size, 1, f);
    fclose(f);
    
    //CFile f;
    //f.Open( strFile, CFile::modeCreate | CFile::modeWrite );
    //f.Write( m_pDataPtr, m_Header.size );
    //f.Close();
}

void CElf::CElfSection::Dump()
{
    CElf::SectionHeader* sh = &m_Header;
    
    char *type = "?";
    switch (sh->type)
    {
        case 0: type = "NULL"; break;
        case 1: type = "PROGBITS"; break;
        case 2: type = "SYMTAB"; break;
        case 3: type = "STRTAB"; break;
        case 4: type = "RELA"; break;
        case 5: type = "HASH"; break;
        case 6: type = "DYNAMIC"; break;
        case 7: type = "NOTE"; break;
        case 8: type = "NOBITS"; break;
        case 9: type = "REL"; break;
    }
    
    printf(" type %2d (%8s) ", sh->type, type);
    printf("  addr 0x%08x", sh->addr);
    printf("  offset %6d", sh->offset);
    printf("  size %6d", sh->size);
    printf("  name '%s'", m_strName);
    printf("\n");
}

void CElf::operator << (const CElf::CElfBinary& bin)
{
    m_elfHeader = bin;
    m_elfHeader.m_pFileData = NULL; // ble!
    m_elfHeader.m_Header.phnum = 0;
    m_elfHeader.m_Header.shnum = 0;
    m_elfHeader.m_Header.shtrndx = -1;
}

void CElf::operator << (CElf::CElfSection sec)
{
#if !EXPPROG
    if ( CString(sec.m_strName) == ".interp" )
    {
        CElf::CElfProgram prog = sec.m_pElf->GetProgramBySection(sec);
        prog.m_nTargetSec = m_arrSection.GetSize();
        m_arrProgram.Add( prog );
    }
    m_arrSection.Add(sec);
    return;
#endif
    if ( sec.m_Header.type == 8 ) // 8 = NOBITS
    {
        m_arrSection.Add( sec );
        return;
    }
    CElf::CElfProgram prog = sec.m_pElf->GetProgramBySection(sec);
    // trim program
    _ASSERT( sec.m_Header.offset >= prog.m_Header.offset );
    _ASSERT( sec.m_Header.offset + sec.m_Header.size <= prog.m_Header.offset + prog.m_Header.filesz );
    int nMove = sec.m_Header.offset - prog.m_Header.offset;
    prog.m_Header.offset += nMove;
    prog.m_Header.paddr += nMove;
    prog.m_Header.vaddr += nMove;
    if ( prog.m_Header.offset + prog.m_Header.filesz == sec.m_Header.offset + sec.m_Header.size )
    {
        // section points to a part at end of program?
        prog.m_Header.filesz -= nMove;
        prog.m_Header.memsz -= nMove;
    } else
    {
        prog.m_Header.filesz = sec.m_Header.size;
        prog.m_Header.memsz = sec.m_Header.size;
    }
    prog.m_pDataPtr += nMove;
    
    m_arrProgram.Add( prog );
    
    sec.m_nExTargetProg = m_arrProgram.GetSize()-1;
    sec.m_nExTargetOfs = 0;
    m_arrSection.Add( sec );
}

CElf::CElf()
{
    m_elfHeader.m_pElf = this;
}

/*
 void CElf::operator << (CElf& elf)
 {
 this->m_pFileData = elf.m_pFileData;
 this->m_nFileLen = elf.m_nFileLen;
 }
 
 void CElf::operator << (FileHeader& hdr)
 {
 m_elfHeader = hdr;
 m_elfHeader.shnum = 0;
 m_elfHeader.phnum = 0;
 m_elfHeader.shtrndx = -1;
 }
 
 void CElf::operator << (SectionHeader& hdr)
 {
 // copy section + create corresponding program
 
 m_elfHeader = hdr;
 m_elfHeader.shnum = 0;
 m_elfHeader.phnum = 0;
 m_elfHeader.shtrndx = -1;
 }
 */

int CElf::GetSectionByName( CString strName )
{
    for (int i=0; i<m_arrSection.GetCount(); i++)
        if ( CString(m_arrSection[i].m_strName) == strName )
            return i;
    _ASSERT(0);
    return -1;
}

int CElf::GetProgramBySection( int n )
{
    int nBegin = m_arrSection[n].m_Header.offset;
    int nEnd = nBegin + m_arrSection[n].m_Header.size;
    
    for (int i=0; i<m_arrProgram.GetCount(); i++)
    {
        Elf32_Phdr& phdr = m_arrProgram[i].m_Header;
        if ( nBegin >= (int)phdr.offset && nBegin <= (int)phdr.offset + (int)phdr.filesz )
        {
            if ( nEnd <= (int)phdr.offset + (int)phdr.memsz )
                return i;
        }
    }
    //_ASSERT(0);
    return -1;
}

void CElf::Save(CString strFileName)
{
    FILE* f = fopen(strFileName.GetBuffer(), "wb");
    fwrite(&m_elfHeader, sizeof(CElf::FileHeader), 1, f);
    for ( int i=0; i<m_arrProgram.GetCount(); i++ )
        fwrite(&m_arrProgram[i].m_Header, sizeof(CElf::ProgramHeader), 1, f);
    for ( int i=0; i<m_arrSection.GetCount(); i++ )
        fwrite(&m_arrSection[i].m_Header, sizeof(CElf::SectionHeader), 1, f);
    for ( int i=0; i<m_arrSection.GetCount(); i++ )
    {
        if ( m_arrSection[i].m_pDataPtr )
            fwrite(m_arrSection[i].m_pDataPtr, m_arrSection[i].m_Header.size, 1, f);
    }
    fclose(f);
}

void CElf::Load(CString strFileName)
{
    FILE* f = fopen(strFileName.GetBuffer(), "rb");
    fseek( f, 0, SEEK_END );
    m_elfHeader.m_nFileLen = ftell( f );
    rewind(f);
    
    m_elfHeader.m_pFileData = (unsigned char*)malloc(m_elfHeader.m_nFileLen);
    fread(m_elfHeader.m_pFileData, 1, m_elfHeader.m_nFileLen, f);
    fclose(f);
    
    // copy
    CElf::FileHeader& elfHeader = m_elfHeader.m_Header;
    memcpy( &elfHeader, m_elfHeader.m_pFileData, sizeof(CElf::FileHeader) );
    
    //check the elf-identification
    if ( elfHeader.ident[0] != 0x7F || elfHeader.ident[1] != 'E' || elfHeader.ident[2] != 'L' || elfHeader.ident[3] != 'F')
    {
        printf("not an executable file\n");
        return;
    }
    if ( elfHeader.ident[4] == 0 || elfHeader.ident[4] == 2 )
    {
        printf("invalid class or 64bit object\n");
        return;
    }
    
    if ( elfHeader.shtrndx >= 0 && elfHeader.shtrndx < elfHeader.shnum )
    {
        Elf32_Shdr* sh = (Elf32_Shdr*) (m_elfHeader.m_pFileData+elfHeader.shoff+elfHeader.shentsize*elfHeader.shtrndx);
        m_elfHeader.m_pNamesData = (char*)(m_elfHeader.m_pFileData + sh->offset);
    }
    
    if ( elfHeader.phoff == 0 )
    {
        printf("no program header available\n");
        return;
    }
    
    _ASSERT( m_elfHeader.m_Header.phentsize == sizeof(Elf32_Phdr) );
    
    int nextProgramHeaderOffset = elfHeader.phoff;
    int i;
    for(i=0; i<elfHeader.phnum; i++)
    {
        CElf::ProgramHeader* ph = (CElf::ProgramHeader*) (m_elfHeader.m_pFileData + nextProgramHeaderOffset);
        m_arrProgram.Add( CElf::CElfProgram( this, ph ) );
        /*
         m_arrProgramMin.Add( -1 );
         m_arrProgramMax.Add( -1 );
         m_arrProgramData.Add( m_pFileData + ph->offset );)
         */
        //get the address of the next programHeader
        nextProgramHeaderOffset += elfHeader.phentsize;
    }
    
    _ASSERT( m_elfHeader.m_pNamesData );
    _ASSERT( elfHeader.shentsize == sizeof(Elf32_Shdr) );
    int nextSectionHeaderOffset = elfHeader.shoff;
    for (i=0; i<elfHeader.shnum; i++)
    {
        //map the programHeader to the exeFileData
        CElf::SectionHeader* sh = (CElf::SectionHeader*) (m_elfHeader.m_pFileData+nextSectionHeaderOffset);
        //m_arrSectionNames.Add(  CString( m_pNamesData + sh->name ) );
        m_arrSection.Add( CElf::CElfSection( this, sh ) );
        /*
         int nProg = GetProgramBySection(i);
         if ( nProg != -1 && sh->addr != 0 )
         {
         int& nMin = m_arrProgramMin[nProg];
         int& nMax = m_arrProgramMax[nProg];
         
         if ( nMin == -1 )
         {
         nMin = sh->offset;
         nMax = sh->offset + sh->size;
         } else
         {
         nMin = min(nMin, sh->offset);
         nMax = max(nMax, sh->offset + sh->size);
         }
         }*/
        /*
         printf("Section %2d:", i);
         printf("  addr 0x%08x", sh->addr);
         printf("  offset %6d", sh->offset);
         printf("  size %6d", sh->size);
         printf("  name '%s'", m_elfHeader.m_pNamesData + sh->name);
         printf("  -> Prog[%d]\n", nProg );
         */
        nextSectionHeaderOffset += elfHeader.shentsize;
    }
}

void CElf::Dump()
{
    printf("-----------------\n");
    m_elfHeader.Dump();
    for ( int i=0; i<m_arrProgram.GetSize(); i++)
    {
        printf("Program %2d:", i);
        m_arrProgram[i].Dump();
    }
    for ( int i=0; i<m_arrSection.GetSize(); i++)
    {
        printf("Section %2d:", i);
        m_arrSection[i].Dump();
    }
}
#if 0
void CElf::Process()
{
    /*
     for ( int i=0; i<m_arrProgram.GetCount(); i++ )
     {
     Elf32_Phdr& phdr = m_arrProgram[i];
     int& nMin = m_arrProgramMin[i];
     int& nMax = m_arrProgramMax[i];
     
     printf("ofs:%d fsz:%d min:%d len:%d", phdr.offset, phdr.filesz, nMin, nMax-nMin);
     if ( phdr.offset == 0 )
     {
     printf(" **");
     int nShift = nMin - phdr.offset;
     phdr.offset += nShift;
     phdr.filesz -= nShift;
     m_arrProgramData[i] = m_pFileData + nMin;
     // fix all section offsets
     
     for ( int j=0; j<m_arrSection.GetCount(); j++ )
     if ( GetProgramBySection( j ) == i )
     {
     Elf32_Shdr& shdr = m_arrSection[j];
     shdr.offset += nShift;
     }
     }
     printf("\n");
     }
     
     */
    /*
     CArray<Elf32_Phdr> m_arrProgram;
     CArray<LPVOID> m_arrProgramData;
     CArray<int> m_arrProgramMin;
     CArray<int> m_arrProgramMax;
     
     CArray<Elf32_Shdr> m_arrSection;
     CArray<CString> m_arrSectionNames;
     
     
     */
    /*
     m_elfHeader.shtrndx = -1;
     m_arrSection.RemoveAt(0);
     m_arrSectionNames.RemoveAt(0);
     m_arrSection.RemoveAt(m_arrSection.GetCount()-1);
     m_arrSectionNames.RemoveAt(m_arrSectionNames.GetCount()-1);
     m_arrProgram.RemoveAt(0);
     m_arrProgramData.RemoveAt(0);
     m_arrProgramMin.RemoveAt(0);
     m_arrProgramMax.RemoveAt(0);
     */
}
#endif

void DumpSym( CElf::CElfSection sec, CElf::CElfSection nam)
{
    Elf32_Sym* pSym = (Elf32_Sym*)sec.m_pDataPtr;
    for (int i=0, j=0; i<sec.m_Header.size; i+=sizeof(Elf32_Sym), pSym++, j++)
    {
        printf("Symbol %d: '%s' value:%08x, size:%d, info:%d, other:%d, shndx:%d\n", j,
               nam.m_pDataPtr + pSym->st_name, pSym->st_value, pSym->st_size, pSym->st_info,
               pSym->st_other, pSym->st_shndx);
    }
}

void DumpRel( CElf::CElfSection sec, CElf::CElfSection nam)
{
    Elf32_Rel* pRel = (Elf32_Rel*)sec.m_pDataPtr;
    for (int i=0, j=0; i<sec.m_Header.size; i+=sizeof(Elf32_Rel), pRel++, j++)
    {
        printf("Relocation %d: addr:%08x, info:%08x, index:%d\n", j,
               pRel->r_offset, pRel->r_info, pRel->r_info>>8);
    }
}

CElf::CElfSection CreateSymTab( CElf::CElfSection sec, CElf::CElfSection nam )
{
    CElf::CElfSection result(sec);
    result.m_pDataPtr = NULL;
    Elf32_Sym* pSym = (Elf32_Sym*)sec.m_pDataPtr;
    for (int i=0, j=0; i<sec.m_Header.size; i+=sizeof(Elf32_Sym), pSym++, j++)
    {
        if ( CString(nam.m_pDataPtr + pSym->st_name) == "_estack" )
        {
            Elf32_Sym* pNewSym = new Elf32_Sym();
            memcpy( pNewSym, pSym, sizeof(Elf32_Sym) );
            result.m_pDataPtr = (unsigned char*)pNewSym;
            result.m_Header.size = sizeof(Elf32_Sym);
        } else
            continue;
        
        printf("CopyDynSym: '%s' value:%08x, size:%d, info:%d, other:%d, shndx:%d\n",
               nam.m_pDataPtr + pSym->st_name, pSym->st_value, pSym->st_size, pSym->st_info,
               pSym->st_other, pSym->st_shndx);
    }
    return result;
}

//#define src "/Users/gabrielvalky/Documents/git/Work-in-progress/LA104/dynamic/build/mains.elf" //argv[1]
//#define out argv[2] //"C:\\Data\\Devel\\Github\\ds203\\Resources\\SampleAppDyn\\bin\\APP_2o.elf"
//#define out "/Users/gabrielvalky/Documents/git/Work-in-progress/LA104/dynamic/build/mainss.elf"


//#define src "/Users/gabrielvalky/Documents/git/Work-in-progress/LA104/dynamic/advancedapp/build/mains.elf" //argv[1]
//#define out "/Users/gabrielvalky/Documents/git/Work-in-progress/LA104/dynamic/advancedapp/build/mainss.elf"

#define src argv[1]
#define out argv[2]

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{
    if (argc < 2)
        return 1;
    
    CElf elfIn;
    elfIn.Load(src);
    elfIn.Dump();
    
    /*
     elfIn.GetSection(".rel.plt").Export("relplt.x");
     elfIn.GetSection(".plt").Export("plt.x");
     elfIn.GetSection(".dynamic").Export("dynamic.x");
     elfIn.GetSection(".dynsym").Export("dynsym.x");
     elfIn.GetSection(".dynstr").Export("dynstr.x");
     elfIn.GetSection(".got").Export("got.x");
     elfIn.GetSection(".text").Export("text.x");
     */
    
    // .dynsym - list of imported symbols + names as offsets in .dynstr string
    // .rel.plt. - list of imported symbols - addresses of locations pointing to real function bodies, indices to .dynsym
    // .plt - actual code referring to function addresses at .rel.plt
    // .got - at this memory location the addresses being paired with real function locations are placed, here .rel.plt points
    

    if (0)
    {
      if ( elfIn.HasSection(".dynstr") )
          DumpSym( elfIn.GetSection(".dynsym"), elfIn.GetSection(".dynstr") );
      if ( elfIn.HasSection(".rel.plt") )
          DumpRel( elfIn.GetSection(".rel.plt"), elfIn.GetSection(".dynstr") );
    }
    //elfIn.GetSection(".rel.plt").Export("relplt.x");
    
    CElf elfOut;
    elfOut << elfIn.GetHeader();
    elfOut << elfIn.GetSection(".text");
    if ( elfIn.HasSection(".data") )
    {
        for (int i=0; i<elfIn.GetSections(); i++)
            if ( CString(elfIn.GetSection(i).m_strName).Left(5) == _T(".data") )
                elfOut << elfIn.GetSection(i);
    }
    if ( elfIn.HasSection(".rodata") )
    {
        for (int i=0; i<elfIn.GetSections(); i++)
            if ( CString(elfIn.GetSection(i).m_strName).Left(7) == _T(".rodata") )
                elfOut << elfIn.GetSection(i);
    }
    //elfOut << elfIn.GetSectionMerged(".data");
    if ( elfIn.HasSection(".bss") )
    {
        for (int i=0; i<elfIn.GetSections(); i++)
            if ( CString(elfIn.GetSection(i).m_strName).Left(4) == _T(".bss") )
                elfOut << elfIn.GetSection(i);
    }
    //elfOut << elfIn.GetSectionMerged(".bss");
    if ( elfIn.HasSection(".init_array") )
        elfOut << elfIn.GetSection(".init_array");
    
    if ( elfIn.HasSection(".plt") )
        elfOut << elfIn.GetSection(".plt"); // code
    if ( elfIn.HasSection(".rel.plt") )
        elfOut << elfIn.GetSection(".rel.plt"); // for OS pairing process
    // got will be placed in RAM to be easily relocated. Flash as last section
    if ( elfIn.HasSection(".got") )
        elfOut << elfIn.GetSection(".got"); // table of pointers to functions, OS should fill these with real addresses
    
    if ( elfIn.HasSection(".dynsym") && elfIn.GetSection(".dynsym").m_Header.size > 0 )
    {
        CElf::CElfSection sDynSym = elfIn.GetSection(".dynsym"); // for OS pairing process
        sDynSym.m_Header.addr = 0; // override load ptr, do not load
        elfOut << sDynSym;
    }
    if ( elfIn.HasSection(".rel.plt") )
    {
        CElf::CElfSection sDynStr = elfIn.GetSection(".dynstr"); // for OS pairing process
        sDynStr.m_Header.addr = 0; // override load ptr, do not load
        elfOut << sDynStr;
    }
    
    //    if ( elfIn.HasSection(".dynamic") )
    //        elfOut << elfIn.GetSection(".dynamic");
    if ( elfIn.HasSection(".symtab") )
    {
        CElf::CElfSection sTab = CreateSymTab( elfIn.GetSection(".symtab"), elfIn.GetSection(".strtab") );
        if ( sTab.m_pDataPtr ) // tu je ulozeny iba _stack
            elfOut << sTab;
    }
    
    if ( elfIn.HasSection(".inetrp") )
        elfOut << elfIn.GetSection(".interp");
    
    if ( elfOut.HasSection( ".dynstr" ) && elfOut.HasSection( ".dynsym" ) )
        elfOut.PairSection( ".dynsym", ".dynstr");
    if ( elfOut.HasSection( ".rel.plt" ) && elfOut.HasSection( ".dynsym" ) )
        elfOut.PairSection( ".rel.plt", ".dynsym");
    
    if ( elfIn.HasSection(".symtab") && elfOut.HasSection(".symtab"))
    {
        elfOut.BuildNames( elfIn.GetSection(".strtab"), elfOut.GetSection(".symtab") );
        elfOut.PairSection( ".symtab", ".strtab");
    }
    elfOut.BuildNames( elfIn.GetSection(".shstrtab"), CElf::CElfSection() );
    elfOut.Process();
    elfOut.Dump();
    elfOut.Save(out);
    //elf.Process();
    //elf.Save(out);
}
