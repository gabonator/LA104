#include <stdint.h>
#include "elf.h"
#include <vector>
#include <iostream>
#include <fstream>

std::vector<uint8_t> generateElf(uint8_t* buffer, int size, int address)
{
	std::vector<uint8_t> f;
	auto write = [&](void*p, int n)
	{       
		std::vector<uint8_t> buf((uint8_t*)p, (uint8_t*)p+n);
		f.insert(f.end(), buf.begin(), buf.end());
	};

	char findStringTable[] = ".text" "_" ".shstrtab" "_";
	char stringTable[] = ".text" "\0" ".shstrtab" "\0";

	Elf32_Ehdr elfHeader = {0};
	Elf32_Shdr elfSection = {0};

        strcpy((char*)elfHeader.ident, "\u007fELF\u0001\u0001");
        elfHeader.type = 0x02;
        elfHeader.machine = 0x28;
        elfHeader.version = 0x01;

        elfHeader.shoff = sizeof(elfHeader); // 
        elfHeader.shtrndx = 1;
        elfHeader.shnum = 1; /// data + shstrtab 
        write(&elfHeader, sizeof(elfHeader));

	Elf32_Shdr elfSectionBinary = {0};
	elfSectionBinary.name = strstr(findStringTable, ".text") - findStringTable;
        elfSectionBinary.addr = address;
	elfSectionBinary.offset = sizeof(Elf32_Ehdr) + 2*sizeof(Elf32_Shdr) + sizeof(stringTable);
	elfSectionBinary.size = size;
        write(&elfSectionBinary, sizeof(elfSectionBinary));

	Elf32_Shdr elfSectionStrings = {0};
        elfSectionStrings.offset = sizeof(Elf32_Ehdr) + 2*sizeof(Elf32_Shdr);
        elfSectionStrings.size = sizeof(stringTable);
	elfSectionStrings.name = strstr(findStringTable, ".shstrtab") - findStringTable;

        write(&elfSectionStrings, sizeof(elfSectionStrings));

        write(stringTable, sizeof(stringTable));
        write(buffer, size);

	return std::move(f);
}

int main()
{
  std::cout << "Elf generator\n";
  std::ifstream file("ds213v20.bin", std::ios::binary | std::ios::ate);
  if (!file.is_open())
  {
    std::cout << "error - cant open!\n";
    return 1;
  }

  std::streamsize size = file.tellg();
  std::vector<uint8_t> buffer(size);

  file.seekg(0, std::ios::beg);
  if (!file.read((char*)buffer.data(), size))
  {
    std::cout << "error!\n";
    return 1;
  }

  std::vector<uint8_t> output = generateElf(buffer.data(), (int)buffer.size(), 0x08070000);
  std::ofstream fout("ds213v20.elf", std::ios::out | std::ios::binary);
  fout.write((char*)output.data(), output.size());
  fout.close();
  return 0;
}
