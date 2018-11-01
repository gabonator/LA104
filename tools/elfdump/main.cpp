//
//  main.cpp
//  testelf
//
//  Created by Gabriel Valky on 10/13/18.
//  Copyright © 2018 Gabriel Valky. All rights reserved.
//

#include <iostream>


#include "elf.h"
#include "BufferedIo.h"
#define COUNT(a) (sizeof(a)/sizeof(a[0]))

namespace BIOS
{
    namespace MEMORY
    {
        void LinearStart() {}
        void LinearFinish() {}
        bool LinearProgram(int, ui8*, int) { return true;}
    }
    namespace DBG
    {
        /*static*/ void Print(const char * format, ...)
        {
            va_list arglist;
            
            //printf( "Error: " );
            va_start( arglist, format );
            //vprintf( format, arglist );
            va_end( arglist );
            
        }
    }
    namespace LCD
    {
        void Clear(int) {}
    }
    namespace SYS
    {
        void DelayMs(int) {}
        ui32 GetProcAddress(char*) { return 1; }
        int Execute(ui32 addr) {return 0;}
    }
}

class CFlasherProto
{
public:
    virtual void FlashData( CBufferedReader2& f, Elf32_Shdr& elfSection ) = 0;
    virtual void FlashBss( CBufferedReader2& f, Elf32_Shdr& elfSection ) = 0;
    virtual void ExecuteInit( ui32 addr ) = 0;
    virtual int Execute( ui32 addr ) = 0;
    virtual void WriteDword( ui32 addr, ui32 data ) = 0;
    virtual ui32 ReadDword( ui32 addr ) = 0;
};

class CFlasherCode : public CFlasherProto
{
    int memSize;
    uint8_t* pMemory;
    uint32_t base;

public:
    CFlasherCode()
    {
        base = 0;
//        base = 0x20005000;
        memSize = 0x2000;
        pMemory = new uint8_t[memSize];
        memset(pMemory, 0, sizeof(memSize));
    }
    
    ~CFlasherCode()
    {
        delete pMemory;
    }
    
    virtual void FlashData( CBufferedReader2& f, Elf32_Shdr& elfSection )
    {
        //printf("Flash %x bytes at %08x\n", elfSection.size, elfSection.addr);
        
        printf("uint8_t data_%08x[0x%x] = {\n  ", elfSection.addr, elfSection.size);

        f.Seek( elfSection.offset );
        int addr = elfSection.addr;
        for (int i=0; i<(int)elfSection.size; i++)
        {
            ui8 bData;
            f >> bData;
            printf("0x%02x, ", bData);
            if (i%16==15)
                printf("\n  ");
            WriteByte(addr++, bData);
        }
        printf("\n};\n");
        printf("WriteMem(data_%08x, 0x%x, 0x%08x);\n", elfSection.addr, elfSection.size, elfSection.addr);
    }
    
    virtual void FlashBss( CBufferedReader2& f, Elf32_Shdr& elfSection )
    {
        printf("Clear %x bytes at %08x\n", elfSection.size, elfSection.addr);
    }
    
    virtual void ExecuteInit( ui32 addr )
    {
        printf("ExecInit(0x%08x);\n", addr);
    }
    
    virtual int Execute( ui32 addr )
    {
        printf("ExecEntry(0x%08x);\n", addr +1 ); // +1 for thumb!!!
        return 0;
    }
    
    virtual void WriteDword( ui32 addr, ui32 data)
    {
        printf("WriteDword(0x%08x, 0x%08x);\n", addr, data);

        WriteByte(addr+3, data >> 24);
        WriteByte(addr+2, data >> 16);
        WriteByte(addr+1, data >> 8);
        WriteByte(addr+0, data);
    }
    
    virtual ui32 ReadDword( ui32 addr )
    {
        uint32_t v;
        v = ReadByte(addr+3);
        v <<= 8;
        v |= ReadByte(addr+2);
        v <<= 8;
        v |= ReadByte(addr+1);
        v <<= 8;
        v |= ReadByte(addr+0);
        return v;
    }
    
    virtual ui8 ReadByte( ui32 addr )
    {
/*
        addr -= base;
        assert(addr < memSize);
        return pMemory[addr];
*/
        return 0;
    }
    
    virtual void WriteByte( ui32 addr, ui8 data )
    {
/*
        if (base == 0)
        {
          printf("Setting emulation base addr: %08x\n", addr);
          base = addr;
        }
        addr -= base;
        assert(addr < memSize);
        pMemory[addr] = data;
*/
    }
};
#if 0
class CFlasherArm : public CFlasherProto
{
    bool Verify( CBufferedReader2& f, Elf32_Shdr& elfSection )
    {
        f.Seek( elfSection.offset );
        
        ui8* pMem = (ui8*)elfSection.addr;
        for (int i=0; i<(int)elfSection.size; i++, pMem++)
        {
            ui8 bData;
            f >> bData;
            if ( bData != *pMem )
                return false;
        }
        return true;
    }
    
    bool VerifyZero( CBufferedReader2& f, Elf32_Shdr& elfSection )
    {
        ui8* pMem = (ui8*)elfSection.addr;
        for (int i=0; i<(int)elfSection.size; i++, pMem++)
        {
            if ( 0 != *pMem )
                return false;
        }
        return true;
    }
    
    void FlashRam( CBufferedReader2& f, Elf32_Shdr& elfSection )
    {
        f.Seek( elfSection.offset );
        ui8* pWriteTo = (ui8*)elfSection.addr;
        for (int i=0; i<(int)elfSection.size; i++)
        {
            ui8 bData;
            f >> bData;
            *pWriteTo++ = bData;
        }
    }
    
    void ZeroRam( CBufferedReader2& f, Elf32_Shdr& elfSection )
    {
        // section length should be aligned to 32bits!?
        ui8* pWriteTo = (ui8*)elfSection.addr;
        for (int i=0; i<(int)elfSection.size; i++)
            *pWriteTo++ = 0;
    }
    
    void FlashRom( CBufferedReader2& f, Elf32_Shdr& elfSection )
    {
        int nLength = (int)elfSection.size;
        ui32 dwAddr = elfSection.addr;
        
        f.Seek( elfSection.offset );
        BIOS::MEMORY::LinearStart();
        for ( int i=0; i<nLength; )
        {
            ui8 buffer[64];
            int nToLoad = nLength-i;
            if ( nToLoad > 64 )
                nToLoad = 64;
            f >> CStream(buffer, nToLoad);
            if ( !BIOS::MEMORY::LinearProgram( dwAddr, buffer, nToLoad) )
            {
                _ASSERT(!!!"LinearProgram failed");
            }
            i += nToLoad;
            dwAddr += nToLoad;
        }
        BIOS::MEMORY::LinearFinish();
    }
    
    void ZeroRom( CBufferedReader2& f, Elf32_Shdr& elfSection )
    {
        int nLength = (int)elfSection.size;
        ui32 dwAddr = elfSection.addr;
        
        ui8 buffer[64] = {0};
        
        BIOS::MEMORY::LinearStart();
        for ( int i=0; i<nLength; )
        {
            int nToLoad = nLength-i;
            if ( nToLoad > 64 )
                nToLoad = 64;
            if ( !BIOS::MEMORY::LinearProgram( dwAddr, buffer, nToLoad) )
            {
                _ASSERT(!!!"LinearProgram failed");
            }
            i += nToLoad;
            dwAddr += nToLoad;
        }
        BIOS::MEMORY::LinearFinish();
    }

public:
    //LINKERSECTION(".gbios")
    virtual void /*CWndUserManager::*/FlashData( CBufferedReader2& f, Elf32_Shdr& elfSection ) override
    {
        if ( Verify( f, elfSection ) )
        {
            BIOS::DBG::Print("Ignoring %d bytes at %08x.", elfSection.size, elfSection.addr );
            return;
        }
        BIOS::DBG::Print("Flashing %d bytes at %08x.", elfSection.size, elfSection.addr );
        
#ifndef _WIN32
        if ( (elfSection.addr >> 24) == 0x20 )
        {
            FlashRam( f, elfSection );
        } else
            if ( (elfSection.addr >> 24) == 0x08 )
            {
                FlashRom( f, elfSection );
            } else
                _ASSERT(!!!"Unrecognized memory location");
#endif
    }
    
    //LINKERSECTION(".gbios")
    virtual void /*CWndUserManager::*/FlashBss( CBufferedReader2& f, Elf32_Shdr& elfSection ) override
    {
        if ( VerifyZero( f, elfSection ) )
        {
            BIOS::DBG::Print("Ignoring %d bytes at %08x.", elfSection.size, elfSection.addr );
            return;
        }
        BIOS::DBG::Print("Filling %d bytes at %08x.", elfSection.size, elfSection.addr );
#ifndef _WIN32
        if ( (elfSection.addr >> 24) == 0x20 )
        {
            ZeroRam( f, elfSection );
        } else
            if ( (elfSection.addr >> 24) == 0x08 )
            {
                ZeroRom( f, elfSection );
            } else
                _ASSERT(!!!"Unrecognized memory location");
#endif
    }
    
    virtual void ExecuteInit( ui32 addr ) override
    {
        typedef void (*TInitFunc)();
        TInitFunc InitFunc = (TInitFunc)addr;
        InitFunc();
    }
    virtual int Execute( ui32 addr ) override
    {
        // TODO: set stack pointer!
        return BIOS::SYS::Execute( addr );
    }
    virtual void WriteDword( ui32 addr, ui32 data ) override
    {
        
    }
    virtual ui32 ReadDword( ui32 addr ) override
    {
        return 0;
    }
};
#endif

bool CWndUserManager_ElfGetInterpreter( char* strName, char* strInterpreter )
{
    CBufferedReader2 fw;
    if ( !fw.Open( strName ) )
        return false;
    
    Elf32_Ehdr elfHeader;
    Elf32_Phdr elfProgram;
    
    fw >> CStream(&elfHeader, sizeof(Elf32_Ehdr));
    
    _ASSERT( sizeof(Elf32_Phdr) == elfHeader.phentsize );
    
    fw.Seek( elfHeader.phoff );
    for (int i=0; i<elfHeader.phnum; i++)
    {
        fw >> CStream(&elfProgram, sizeof(Elf32_Phdr));
        if ( elfProgram.type == Elf32_PTypeInterpreter )
        {
            fw.Seek( elfProgram.offset );
            _ASSERT( elfProgram.filesz < 31 );
            fw >> CStream( strInterpreter, elfProgram.filesz );
            strInterpreter[elfProgram.filesz] = 0;
            fw.Close();
            return true;
        }
    }
    fw.Close();
    return false;
}


//LINKERSECTION(".gbios")
void CWndUserManager_ElfExecute( char* strName, CFlasherProto& flash )
{
    /*
     all variables used in this routine + (used by BIOS functions) must be placed
     at memory location not colliding with loaded image! Same limitation apply to the
     code area in flash occupied by this function!
     */
    BIOS::LCD::Clear(0);
    BIOS::DBG::Print("Executing ELF image\n");
    CBufferedReader2 fw;
    if ( !fw.Open( strName ) )
        return;
    
    Elf32_Ehdr elfHeader;
    Elf32_Shdr elfSection;
    
    fw >> CStream(&elfHeader, sizeof(Elf32_Ehdr));
    int nStringTableSectionOfs = elfHeader.shoff + elfHeader.shtrndx * sizeof(Elf32_Shdr);
    
    fw.Seek( nStringTableSectionOfs );
    fw >> CStream(&elfSection, sizeof(Elf32_Shdr));
    int nStringTableOfs = elfSection.offset;
    int nStringTableLen = elfSection.size;
    
    char strSymbolNames[128];
    
    /*
     _ASSERT( nStringTableLen < 128 );
     char strSectionNames[128];
     char* strSymbolNames = strSectionNames;
     fw.Seek(nStringTableOfs);
     fw >> CStream(strSectionNames, nStringTableLen);
     */
    
    enum SecType {
        SecNone = 0,
        SecText = 1,
        SecData = 2,
        SecBss = 3,
        SecPlt = 4,
        SecGot = 5,
        SecDynamic = 6,
        SecDynStr = 7, // we need to load symbol list before DynSym
        SecDynSym = 8,
        SecRelPlt = 9, // process as last
        SecInterp = 10,
        SecStringTab = 11,
        SecInit = 12,
        SecStrTab = 13,
        SecSymTab = 14,
        SecSymRodata = 15
    };
    const char* arrSecNames[] = {"none", ".text", ".data", ".bss", ".plt", ".got", ".dynamic", ".dynstr", ".dynsym", ".rel.plt", ".interp", ".shstrtab", ".init_array", ".strtab", ".symtab", ".rodata"};
    
    int arrSectionIndex[COUNT(arrSecNames)];
    int arrSectionOffset[COUNT(arrSecNames)];
    for (int i=0; i<COUNT(arrSectionIndex); i++)
    {
        arrSectionIndex[i] = -1;
        arrSectionOffset[i] = -1;
    }
    //fw.Seek( elfHeader.shoff );
    for (int i=0; i<elfHeader.shnum; i++)
    {
        fw.Seek( elfHeader.shoff + i * sizeof(Elf32_Shdr) );
        fw >> CStream(&elfSection, sizeof(Elf32_Shdr));
        
        // get section name
        char strSectionName[15];
        int nSectionNameMaxLen = nStringTableLen - elfSection.name;
        if ( nSectionNameMaxLen > 14 )
            nSectionNameMaxLen = 14;
        strSectionName[14] = 0;
        
        fw.Seek(nStringTableOfs + elfSection.name);
        fw >> CStream( strSectionName, nSectionNameMaxLen );

        if (elfSection.addr != 0)
            BIOS::DBG::Print("%s> %08x..%08x\n", strSectionName, elfSection.addr, elfSection.addr + elfSection.size);
//        printf("// %s\n", strSectionName);
        //        char* strSectionName = strSectionNames + elfSection.name;
        
        if ( strncmp( strSectionName, ".text", 5 ) == 0 )
        {
            printf("// %s\n", strSectionName);
            BIOS::DBG::Print("%s>", strSectionName);
            // flash code
            flash.FlashData( fw, elfSection );
            BIOS::DBG::Print("\n");
            continue;
        }
        
        if ( strncmp( strSectionName, ".data", 5 ) == 0 || strncmp( strSectionName, ".rodata", 7 ) == 0)
        {
            printf("// %s\n", strSectionName);
            BIOS::DBG::Print("%s>", strSectionName);
            // flash data
            flash.FlashData( fw, elfSection );
            BIOS::DBG::Print("\n");
            continue;
        }
        
        if ( strncmp( strSectionName, ".bss", 4 ) == 0 )
        {
            printf("// %s\n", strSectionName);
            BIOS::DBG::Print("%s>", strSectionName);
            // flash bss
            flash.FlashBss( fw, elfSection );
            BIOS::DBG::Print("\n");
            continue;
        }
        
        SecType sectionType = SecNone;
        for (int j=0; j<COUNT(arrSecNames); j++)
            if ( strcmp( strSectionName, arrSecNames[j] ) == 0 )
            {
                sectionType = (SecType)j;
                break;
            }
        
        _ASSERT( sectionType > 0 );
        if ( sectionType > 0 )
        {
            BIOS::DBG::Print("%s assigning\n", strSectionName);
            arrSectionIndex[sectionType] = i;
            arrSectionOffset[sectionType] = elfSection.offset;
        } else
        {
            BIOS::DBG::Print("UNKNOWN SECTION NAME: '%s'\n", strSectionName );
            BIOS::SYS::DelayMs(4000);
        }
        /*
         BIOS::DBG::Print("Section%d '%s' ofs=%d addr=%08x len=%d\n", i,
         strSectionNames+elfSection.name, elfSection.offset, elfSection.addr,
         elfSection.size);
         */
    }
    
    for ( int i=0; i<(int)COUNT(arrSectionIndex); i++)
    {
        if ( arrSectionIndex[i] == -1 )
            continue;
        
        fw.Seek( elfHeader.shoff + arrSectionIndex[i] * sizeof(Elf32_Shdr) );
        fw >> CStream(&elfSection, sizeof(Elf32_Shdr));
        
        BIOS::DBG::Print("%s>", arrSecNames[i]);
        switch ( i )
        {
            case SecText:
            case SecData:
            {
                _ASSERT( 0 );
                flash.FlashData( fw, elfSection );
                break;
            }
            case SecPlt:
            case SecGot:
            case SecDynamic:
            {
                printf("// %s\n", arrSecNames[i]);
                flash.FlashData( fw, elfSection );
                break;
            }
            case SecBss:
            {
                _ASSERT( 0 );
                printf("// %s\n", arrSecNames[i]);
                flash.FlashBss( fw, elfSection );
                break;
            }
            case SecDynStr:
            {
                _ASSERT( elfSection.size < sizeof(strSymbolNames) );
                fw.Seek( elfSection.offset );
                fw >> CStream( strSymbolNames, elfSection.size );
                BIOS::DBG::Print("saving buffer: ");
                for (int i=0; i<elfSection.size; i++)
                    if (strSymbolNames[i] != 0)
                        BIOS::DBG::Print("%c", strSymbolNames[i]);
                    else
                        BIOS::DBG::Print("/");
                break;
            }
            case SecRelPlt:
            {
                BIOS::DBG::Print("Matching imports...\n");
                printf("// Relocation table\n");
                _ASSERT( arrSectionOffset[SecDynSym] != -1 );
                _ASSERT( arrSectionOffset[SecDynStr] != -1 );
                
                int nSymbolCount = elfSection.size/sizeof(Elf32_Rel);
                for (int i=0; i<nSymbolCount; i++)
                {
                    Elf32_Rel elfRelocation;
                    Elf32_Sym elfSymbol;
                    
                    fw.Seek( elfSection.offset + i * sizeof(Elf32_Rel) );
                    fw >> CStream(&elfRelocation, sizeof(Elf32_Rel));
                    
                    _ASSERT( (elfRelocation.r_info & 0xff) == 0x16 );
                    int nIndex = elfRelocation.r_info >> 8;
                    
                    fw.Seek( arrSectionOffset[SecDynSym] + nIndex*sizeof(Elf32_Sym) );
                    fw >> CStream(&elfSymbol, sizeof(Elf32_Sym));
                    char* strSymbolName = strSymbolNames + elfSymbol.st_name;
                    ui32 dwProcAddr = BIOS::SYS::GetProcAddress( strSymbolName );

                    BIOS::DBG::Print("%s: GOT:%08x <- %08x", strSymbolName, elfRelocation.r_offset, dwProcAddr);
                    printf("// %s\n", strSymbolName);
                    if ( i < nSymbolCount-1 )
                    {
                        BIOS::DBG::Print("\n");
                    }
                    _ASSERT(dwProcAddr);
                    
//                    ui32 dwOld = flash.ReadDword(elfRelocation.r_offset);
                    flash.WriteDword(elfRelocation.r_offset, dwProcAddr);
//                    ui32 dwNew = flash.ReadDword(elfRelocation.r_offset);
                    
                    //printf("// %08x: old %08x new %08x\n", elfRelocation.r_offset, dwOld, dwNew);
                }
                break;
            }
            case SecInit:
            {
                // last processed section before jumping to entry
                int nCount = elfSection.size/sizeof(ui32);
                fw.Seek( elfSection.offset );
                for (int i=0; i<nCount; i++)
                {
                    ui32 dwInitPtr;
                    fw >> dwInitPtr;
                    BIOS::DBG::Print("0x%08x", dwInitPtr);
                    
                    flash.ExecuteInit(dwInitPtr);
                    BIOS::DBG::Print(", ");
                }
                BIOS::DBG::Print("Done.");
                break;
            }
            case SecStrTab:
            {
                _ASSERT( elfSection.size < sizeof(strSymbolNames) );
                fw.Seek( elfSection.offset );
                fw >> CStream( strSymbolNames, elfSection.size );
                
                BIOS::DBG::Print("saving buffer: ");
                for (int i=0; i<elfSection.size; i++)
                    if (strSymbolNames[i] != 0)
                        BIOS::DBG::Print("%c", strSymbolNames[i]);
                    else
                        BIOS::DBG::Print("/");
                break;
            }
            case SecSymTab:
            {
                _ASSERT( arrSectionOffset[SecStrTab] != -1 );
                int nSymbolCount = elfSection.size/sizeof(Elf32_Sym);
                for (int i=0; i<nSymbolCount; i++)
                {
                    Elf32_Sym elfSymbol;
                    fw.Seek( elfSection.offset + i * sizeof(Elf32_Sym) );
                    fw >> CStream(&elfSymbol, sizeof(Elf32_Sym));
                    char* strExportName = strSymbolNames + elfSymbol.st_name;
                    BIOS::DBG::Print("%s = %08x ", strExportName, elfSymbol.st_value);
                }
                break;
            }
            default:
                BIOS::DBG::Print("skipping %d bytes, at %d, type %d", elfSection.size, elfSection.offset, elfSection.type);
        }
        BIOS::DBG::Print("\n");
    }
    
    BIOS::DBG::Print("Load ok. Jumping to entry %08x \n", elfHeader.entry);
    int nRet = flash.Execute(elfHeader.entry);
    BIOS::DBG::Print("Return code=%d.\n", nRet);
}




int main(int argc, const char * argv[]) {
    if (argc < 2)
      return 1;
    CFlasherCode flasher;
    char *src = (char*)argv[1]; //"/Users/gabrielvalky/Documents/git/Work-in-progress/LA104/dynamic/advancedapp/client/build/mainss.elf";
    CWndUserManager_ElfExecute(src, flasher);
    return 0;
}


