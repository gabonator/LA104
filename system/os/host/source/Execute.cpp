#include "Manager.h"
#include "library/elf.h"
#include "framework/BufferedIo.h"
#include "gui/Gui.h"
#include "Shared.h"

bool CWndUserManager::ElfGetInterpreter( char* strName, char* strInterpreter )
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

//LINKERSECTION(".gbios")
void /*CWndUserManager::*/FlashData( CBufferedReader2& f, Elf32_Shdr& elfSection )
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
void /*CWndUserManager::*/FlashBss( CBufferedReader2& f, Elf32_Shdr& elfSection )
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

//LINKERSECTION(".gbios")
uint32_t CWndUserManager::ElfExecute( char* strName )
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
		return 0;

	Elf32_Ehdr elfHeader;
	Elf32_Shdr elfSection;

	fw >> CStream(&elfHeader, sizeof(Elf32_Ehdr));
	int nStringTableSectionOfs = elfHeader.shoff + elfHeader.shtrndx * sizeof(Elf32_Shdr);

	fw.Seek( nStringTableSectionOfs );
	fw >> CStream(&elfSection, sizeof(Elf32_Shdr));
	int nStringTableOfs = elfSection.offset;
	int nStringTableLen = elfSection.size;

	int nSymbolNamesOffset = -1;
//	char strSymbolNames[600]; // too small!

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
		SecRoData = 15
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

//		char* strSectionName = strSectionNames + elfSection.name;

		if ( strncmp( strSectionName, ".text", 5 ) == 0 )
		{
			BIOS::DBG::Print("%s>", strSectionName);
			// flash code
			FlashData( fw, elfSection );
			BIOS::DBG::Print("\n");
			continue;
		}

		if ( strncmp( strSectionName, ".data", 5 ) == 0 )
		{
			BIOS::DBG::Print("%s>", strSectionName);
			// flash data
			FlashData( fw, elfSection );
			BIOS::DBG::Print("\n");
			continue;
		}

		if ( strncmp( strSectionName, ".rodata", 7 ) == 0 )
		{
			BIOS::DBG::Print("%s>", strSectionName);
			// flash data
			FlashData( fw, elfSection );
			BIOS::DBG::Print("\n");
			continue;
		}

		if ( strncmp( strSectionName, ".bss", 4 ) == 0 )
		{
			BIOS::DBG::Print("%s>", strSectionName);
			// flash bss
			FlashBss( fw, elfSection );
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
				FlashData( fw, elfSection );
				break;	
			}
			case SecPlt:
			case SecGot:
			case SecDynamic:
			{
				FlashData( fw, elfSection );
				break;
			}
			case SecBss:
			{
				_ASSERT( 0 );
				FlashBss( fw, elfSection );
				break;
			}
			case SecDynStr:
			{
				nSymbolNamesOffset = elfSection.offset;
//				_ASSERT( elfSection.size < sizeof(strSymbolNames) );
//				fw.Seek( elfSection.offset );
//				fw >> CStream( strSymbolNames, elfSection.size );
				break;
			}
			case SecRelPlt:
			{
				BIOS::DBG::Print("Matching imports...\n");
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

					_ASSERT(nSymbolNamesOffset > -1);
					fw.Seek( nSymbolNamesOffset + elfSymbol.st_name);

					char strSymbolName[128];
					fw >> CStream(&strSymbolName, sizeof(strSymbolName)); 

					ui32 dwProcAddr = GetProcAddress( strSymbolName );
					BIOS::DBG::Print("%s, ", strSymbolName);
					if ( i == nSymbolCount-1 )
					{
						BIOS::DBG::Print("\n");
					}
					_ASSERT(dwProcAddr);
#ifndef _WIN32
					/*
					0x20000E84 (GOT[2]) -> 0x20000DAC (PLT[0])
					0x20000E84 (GOT[2]) <- new address
					*/
					ui32* pRelocation = (ui32*)elfRelocation.r_offset;
					*pRelocation = dwProcAddr;
#endif
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
#ifndef _WIN32
					typedef void (*TInitFunc)();
					TInitFunc InitFunc = (TInitFunc)dwInitPtr;
					InitFunc();
#endif
					BIOS::DBG::Print(", ");
				}			
				BIOS::DBG::Print("Done.");
				break;
			}
			case SecStrTab:
			{
//				_ASSERT( elfSection.size < sizeof(strSymbolNames) );
//				fw.Seek( elfSection.offset );
//				fw >> CStream( strSymbolNames, elfSection.size );
				break;
			}
			case SecSymTab:
			{
//				_ASSERT( arrSectionOffset[SecStrTab] != -1 );
//				int nSymbolCount = elfSection.size/sizeof(Elf32_Sym);
//				for (int i=0; i<nSymbolCount; i++)
//				{
//					Elf32_Sym elfSymbol;
//					fw.Seek( elfSection.offset + i * sizeof(Elf32_Sym) );
//					fw >> CStream(&elfSymbol, sizeof(Elf32_Sym));
//					char* strExportName = strSymbolNames + elfSymbol.st_name;
//					BIOS::DBG::Print("%s = %08x ", strExportName, elfSymbol.st_value);
//				}
			}
		}
		BIOS::DBG::Print("\n");
	}

	BIOS::DBG::Print("Load ok. Jumping to entry %08x \n", elfHeader.entry);
	return elfHeader.entry;
}
