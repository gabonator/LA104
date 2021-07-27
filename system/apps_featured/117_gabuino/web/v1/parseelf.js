var Elf32_Ehdr = new Parser()
  .endianess("little")
  .array("ident", {type: "uint8", length:16})
  .uint16("type")
  .uint16("machine")
  .uint32("version")
  .uint32("entry")
  .uint32("phoff")
  .uint32("shoff")
  .uint32("flags")
  .uint16("ehsize")
  .uint16("phentsize")
  .uint16("phnum")
  .uint16("shentsize")
  .uint16("shnum")
  .uint16("shtrndx");

var Elf32_Shdr = new Parser()
  .endianess("little")
  .uint32("name")
  .uint32("type")
  .uint32("flags")
  .uint32("addr")
  .uint32("offset")
  .uint32("size")
  .uint32("link")
  .uint32("info")
  .uint32("addralign")
  .uint32("entsize");

var Elf32_Rel = new Parser()
  .endianess("little")
  .uint32("r_offset")
  .uint32("r_info")

var Elf32_Sym = new Parser()
  .endianess("little")
  .uint32("st_name")
  .uint32("st_value")
  .uint32("st_size")
  .uint8("st_info")
  .uint8("st_other")
  .uint16("st_shndx")

class Moja 
{
  constructor(array, begin, len)
  {
    this.buffer = typeof(array.buffer) != "undefined" ? array.buffer : array;
    this.byteOffset = begin ? begin : 0;
    this.length = len ? len : array.length;
  }

  subbuffer (ofs, len) {
    return new Moja(this.buffer, ofs, len);
  }

  copyTo(buf, ofs)
  {
    var bytes = new Uint8Array(this.buffer);
    for (var i=0; i<this.length; i++)
      buf[ofs+i] = bytes[this.byteOffset+i];
  }

  stringAt(ofs) {
    var aux = "";
    var bytes = new Uint8Array(this.buffer);
    for (var i=ofs; i<this.length && bytes[i] != 0; i++)
      aux += String.fromCharCode(bytes[i]);
    return aux;
  }
}

function parseElf(elf)
{
  var elf = new Moja(elf);

  if (typeof(elf.subbuffer) == "undefined")
    elf.subbuffer = (ofs, len) => elf.slice(ofs, ofs+len);

  var elfHeader = Elf32_Ehdr.parse(elf);
  var nStringTableSectionOfs = elfHeader.shoff + elfHeader.shtrndx * 10 * 4;
  var shdrBuf = elf.subbuffer(nStringTableSectionOfs, 10 * 4);
  var stringTable = Elf32_Shdr.parse(shdrBuf);

//  console.log(elfHeader);
//  console.log(stringTable);

  var sections = {all:[]};
  for (var i=0; i<elfHeader.shnum; i++)
  {
    var shdrBuf = elf.subbuffer(elfHeader.shoff + i * 10 * 4, 10*4);
    var elfSection = Elf32_Shdr.parse(shdrBuf);

    var sectionName = elf.stringAt(stringTable.offset + elfSection.name);
    console.log({name:(sectionName+"         ").substr(0,12), //*fofs:elfSection.offset, */
      range:("000"+elfSection.addr.toString(16)).substr(-8) + "-" + ("000"+(elfSection.addr + elfSection.size).toString(16)).substr(-8), size:elfSection.size});
    if (sectionName==".rel.plt")
      sections.relplt = {...elfSection};
    if (sectionName==".dynsym")
      sections.dynsym = {...elfSection};
    if (sectionName==".dynstr")
      sections.dynstr = {...elfSection};
    if (sectionName==".bss")
      sections.bss = {...elfSection};

//console.log(sectionName, elfSection);
    if (elfSection.addr)
    {
      elfSection.name = sectionName;
      elfSection.data = elf.subbuffer(elfSection.offset, elfSection.size);
      sections.all.push({...elfSection});
    }
  }

  sections.all.sort((a, b) => a.addr - b.addr);
//  console.log(sections.all.map(s => s.name + " - " + s.addr.toString(16) + " - " + (s.addr + s.size).toString(16) ));
  //console.log(sections);

  var relocations = [];
  if (typeof(sections.relplt) != "undefined")
  {
    for (var i=0; i<sections.relplt.size; i+=8)
    {
      var elfRelocationBuf = elf.subbuffer(sections.relplt.offset + i, 8);
      var elfRelocation = Elf32_Rel.parse(elfRelocationBuf);

      if ( (elfRelocation.r_info & 0xff) != 0x16 )
        throw "error";

      var nIndex = elfRelocation.r_info >> 8;

      var elfSymbolBuf = elf.subbuffer(sections.dynsym.offset + nIndex * 4 * 4, 4 * 4);
      var elfSymbol = Elf32_Sym.parse(elfSymbolBuf);

      var symbolName = elf.stringAt(sections.dynstr.offset + elfSymbol.st_name);
      relocations.push({name:symbolName, addr:elfRelocation.r_offset});

    }
  }
  return {sections:sections.all, relocations:relocations, entry:elfHeader.entry};
//    console.log(symbolName + " -> " + elfRelocation.r_offset.toString(16));
//  console.log(sections.all.map(s => s.name + " - " + s.addr.toString(16) + " - " + (s.addr + s.size).toString(16) ));
}
