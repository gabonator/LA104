function codeFlowDecoder(json)
{
  var flow = [];
  var getNode = (name) => {
    var nodes = Object.values(json.nodes).filter(n => n.name == name);
    if (nodes.length != 1)
    {
      console.log("Failed while looking for", name);
      throw "Single node not found"
    }
    return nodes[0];
  }
  var getChildren = (node) => {
    var ids = Object.values(node.outputs).map(o => o.connections).flat(1).map(x => x.node)
    return ids.map(i => json.nodes[i])
  }
  var findChild = (nodes, name) => {
    var nodes = nodes.filter(n => n.name == name);
    if (nodes.length != 1)
      return false;
    return nodes[0];
  }
  var hasLink = (children, link1, link2) =>
  {
    return true; // TODO: lazy
  }

  var curNode = getNode("Input signal", true);
  while (1)
  {
    var curChildren = getChildren(curNode);
    if (findChild(curChildren, "Assertion"))
    {
      flow.push(["Assert", findChild(curChildren, "Assertion").data.condition, flow[flow.length-1][0]])
      var assertChild = getChildren(findChild(curChildren, "Assertion"))
      var otherChild = curChildren.filter(n => n.name != "Assertion")
      if (assertChild.length == 1 && otherChild.length == 0)
      {
        curChildren = assertChild;
      } else
      if (assertChild.length == 0 && otherChild.length == 1)
      {
        curChildren = curChildren.filter(n => n.name != "Assertion");
      } else 
      {
        throw "Cant choose right path"
      }
    }
    if (findChild(curChildren, "Print") || findChild(curChildren, "Histogram") || 
       findChild(curChildren, "Display signal"))
      curChildren = curChildren.filter(n => n.name != "Print" && n.name != "Histogram" && 
        n.name != "Display signal");
    if (curChildren.length == 1 && curChildren[0].name == "Quantize")
    {
      flow.push(["Quantize", curChildren[0].data.len])
      curNode = curChildren[0];
      continue;
    }
    if (curChildren.length == 2 && 
        findChild(curChildren, "Count elements") && 
        findChild(curChildren, "SubArray") &&
        hasLink(curChildren, "Count elements.out", "SubArray._begin"))
    {
      flow.push(["Skip while", findChild(curChildren, "Count elements").data.condition])
      curNode = findChild(curChildren, "SubArray");
      continue;
    }
    if (curChildren.length == 1 && 
        curChildren[0].name == "SubArray" && 
        curChildren[0].data.tillEnd && 
        curChildren[0].inputs._begin.connections.length == 0 &&
        curChildren[0].inputs._len.connections.length == 0)
    {
      flow.push(["Skip first", curChildren[0].data.begin])
      curNode = curChildren[0];
      continue;
    }
    if (curChildren.length == 1 && curChildren[0].name == "Lookup table" && 
        getChildren(curChildren[0]).length == 1 &&
        getChildren(curChildren[0])[0].name == "Lookup table")
    {
      // Manchester coding?
      var lut1 = curChildren[0];
      var lut2 = getChildren(lut1)[0];
      if ([lut1.data.assign1, lut1.data.assign2, lut1.data.assign3, lut1.data.assign4].flat(1).join(";") ==
          "500L;L;500H;H;1000L;L, L;1000H;H, H" &&
          [lut2.data.assign1, lut2.data.assign2, lut2.data.assign3, lut2.data.assign4].flat(1).join(";") ==
          "H, L;0;L, H;1;H;0;;")
      {
        var lut2data = [1, 2, 3, 4].map(x => lut2.data["assign"+x])
          .filter(x=>x[0] != "").map(x => [x[0].split(",").map(y=>y.trim()), x[1]])
        flow.push(["Manchester", parseInt(lut1.data.assign1[0]), lut2data])
        curNode = lut2;
        continue;
      }
    }

    if (curChildren.length == 1 && curChildren[0].name == "Lookup table" )
    {
      var lut = curChildren[0];
      if (lut.data.assign1[0].split(",").length == 2 && lut.data.assign2[0].split(",").length == 2 &&
          lut.data.assign3[0] == "" && lut.data.assign3[0] == "")
      {
        flow.push(["Pair LUT", lut.data.assign1[0].split(",").map(x=>x.trim()),
          lut.data.assign1[1], lut.data.assign2[0].split(",").map(x=>x.trim()),
          lut.data.assign2[1]])
        curNode = curChildren[0]; // keep integer conversion as next element
        continue;
      }
      {
        var assign = ["assign1", "assign2", "assign3", "assign4"]
         .map(x => lut.data[x])
         .filter(x => x[0].length)
         .map(x => [x[0].split(",").map(y=>y.trim()), x[1]])
        flow.push(["LUT", assign])
        curNode = curChildren[0]; 
        continue;
      }
    }

    if (curChildren.length == 1 && curChildren[0].name == "Bits to integer")
    {
      flow.push(["Bits to integer", curChildren[0].data.datatype.value, curChildren[0].data.firstbit.value])
      curNode = curChildren[0];
      continue;
    }
    if (curChildren.length == 1 && curChildren[0].name == "Attributes")
    {
      var attrs = [0, 1, 2, 3, 4, 5, 6, 7, 8].map(x => curChildren[0].data["attribute"+x])
        .filter(x => x)
      flow.push(["Attributes", attrs])
      curNode = curChildren[0];
      continue;
    }
    if (curChildren.length == 2 && 
        findChild(curChildren, "If signal...") && 
        findChild(curChildren, "SubArray") &&
        hasLink(curChildren, "If signal....out", "SubArray._begin") &&
        findChild(curChildren, "SubArray").data.tillEnd)
    {
      flow.push(["Conditional skip first", 
        findChild(curChildren, "If signal...").data.condition, 
        ...findChild(curChildren, "If signal...").data.retValues])
      curNode = findChild(curChildren, "SubArray");
      continue;
    }
    if (curChildren.length == 1 && curChildren[0].name == "Split" /* && curChildren[0].data.break*/)
    {
      flow.push(["Split", curChildren[0].data.condition])
      curNode = curChildren[0];
      continue;
    }
    if (curChildren.length == 0)
      break;
    flow.push(["Error", curChildren]);
//    console.log("Dont know how to process", curChildren);
    break;
  }
  return flow;
}

function codeSynthDecode(flow)
{
  var fullcode = [];
  var trailing = [];
  var synthElement = (c) => {
    switch (c[0])
    {
      case "Quantize":
        var q = c[1];
        var half = q >> 1;
        code.push(`assert(len > 0 && arr[0] > ${half});`);
        code.push(`for (int i=1, j=1; i<len-1; i++, j++)`);
        code.push(`  if (arr[i] <= ${half})`)
        code.push(`  {`);
        code.push(`    arr[j-1] += arr[i] + arr[i+1];`);
        code.push(`    i += 2;`);
        code.push(`    len -= 2;`);
        code.push(`  }`);
        code.push(`for (int i=0; i<len; i++)`);
        code.push(`  arr[i] = (arr[i] + ${half}) / ${q} * ${q};`);
      break;
      case "Skip while":
        var cond = c[1].split("x").join("arr[0]");
        code.push(`while (len && ${cond})`);
        code.push(`{`);
        code.push(`  arr++;`);
        code.push(`  len--;`);
        code.push(`}`);
      break;
      case "Skip first":
        code.push(`assert(len >= ${c[1]});`);
        code.push(`arr += ${c[1]};`);
        code.push(`len -= ${c[1]};`);
      break;
      case "Assert":
        var cond = c[1];
        if (c[2].includes("LUT"))
          cond = c[1].split("arr.length").join("datalen");
        else if (c[2] == "Attributes")
          cond = c[1].replace(new RegExp("arr\\.(\\w+)"), "pHandler->getIntegerAttribute(\"$1\")")//c[1].split("arr.").join("pHandler->getIntegerAttribute(...)");
        else
          cond = c[1].split("arr.length").join("len");
        code.push(`if (!(${cond}))`);
        code.push(`  return false;`);
      break;
      case "Manchester":
        code.push("// TODO");
        synthElement(["LUT", c[2]])
      break;
      case "Conditional skip first":
        code.push(`if (${c[1]})`);
        code.push(`{`);
        code.push(`  arr += ${c[2]};`);
        code.push(`  len -= ${c[2]};`);
        code.push(`} else`);
        code.push(`{`);
        code.push(`  arr += ${c[3]};`);
        code.push(`  len -= ${c[3]};`);
        code.push(`}`);
      break;
      case "Split":
        code.push("auto decoder = [&](uint16_t* arr, int len)");
        code.push("{");
        var cond = c[1].split("x").join("arr[i]");
        trailing.push(`};`);

        trailing.push(`int lastSplit = 0;`);
        trailing.push(`for (int i=0; i<len; i++)`);
        trailing.push(`  if (${cond})`);
        trailing.push(`  {`);
        trailing.push(`    if (decoder(arr+lastSplit, i-lastSplit))`);
        trailing.push(`      return true;`);
        trailing.push(`    lastSplit = ++i;`);
        trailing.push(`  }`); // TODO: add last part
        trailing.push(`if (decoder(arr+lastSplit, len-lastSplit))`);
        trailing.push(`  return true;`);
      break;
      case "Pair LUT":
        code.push(`uint8_t databits[32] = {0};`);
        code.push(`int datalen{0};`);
        code.push(``);
        code.push(`auto pushBit = [&databits, &datalen](bool b)`);
        code.push(`{`);
        code.push(`  if (b && datalen/8 < sizeof(databits))`);
        code.push(`    databits[datalen/8] |= 1 << (7-(datalen & 7));`);
        code.push(`  datalen++;`);
        code.push(`};`);
        code.push(``);

        code.push(`auto pushSymbol = [pushBit](char c)`);
        code.push(`{`);
        code.push(`  assert(c=='0' || c=='1');`);
        code.push(`  pushBit(c=='1');`);
        code.push(`};`);
        code.push(`assert(len%2 == 0);`);
        code.push(`for (int i=0; i<len; i+=2)`);
        code.push(`{`);
        for (var i=1; i<c.length; i+=2)
        {
          code.push(`  ${i>1 ? "else " : ""}if (arr[i] == ${c[i][0]} && arr[i+1] == ${c[i][1]})`);
          code.push(`    pushSymbol('${c[i+1]}');`);
        }
        code.push(`  else`);
        code.push(`    return false;`);
        code.push(`}`);
      break;
      case "LUT":
        code.push(`uint8_t databits[32] = {0};`);
        code.push(`int datalen{0};`);
        code.push(``);
        code.push(`auto pushBit = [&databits, &datalen](bool b)`);
        code.push(`{`);
        code.push(`  if (b && datalen/8 < sizeof(databits))`);
        code.push(`    databits[datalen/8] |= 1 << (7-(datalen & 7));`);
        code.push(`  datalen++;`);
        code.push(`};`);
        code.push(``);

        code.push(`auto pushSymbol = [pushBit](char c)`);
        code.push(`{`);
        code.push(`  assert(c=='0' || c=='1');`);
        code.push(`  pushBit(c=='1');`);
        code.push(`};`);
        code.push(`assert(len%2 == 0);`);
        code.push(`for (int i=0; i<len; i++)`);
        code.push(`{`);
        
        for (var i=0; i<c[1].length; i++)
        {
          var pulse = c[1][i][0];
          var symbol = c[1][i][1];
          if (pulse.length != 1)
            throw "only single pulse allowed"
          var pulseFlag = pulse[0].substr(-1);
          var pulseLen = parseInt(pulse[0]);
          if (pulseFlag != "L" && pulseFlag != "H")
            throw "needs pulse flag"
          var el = i>0 ? "else" : ""
          if (pulseFlag == "H")
            code.push(`${el} if (arr[i] == ${pulseLen} && i%2 == 0)`);
          else if (pulseFlag == "L")
            code.push(`${el} if (arr[i] == ${pulseLen} && i%2 == 1)`);
          if (symbol == "")
            code.push(";")
          else
            code.push(`pushSymbol('${symbol}');`)
        }
        code.push(`  else`);
        code.push(`    return false;`);
        code.push(`}`);
      break;
      case "Bits to integer":
        inject.push(`// Bits to integer`);
        inject.push(`uint8_t bitsData[32] = {0};`);
        inject.push(`int bitsLen = 0;`);
        inject.push(`auto pushBit = [&bitsData, &bitsLen](bool b)`);
        inject.push(`{`);
        inject.push(`  if (b && bitsLen/8 < sizeof(code))`);
        inject.push(`    code[bits/8] |= 1 << (7-(bits & 7));`);
        inject.push(`  bits++;`);
        inject.push(`};`);
        code.push("len = bits;");
        code.push("arr = nullptr;");
      break;
      case "Attributes":
        code.push(`auto extractBits = [&](int first, int last)`);
        code.push(`{`);
        code.push(`  uint32_t v=0;`);
        code.push(`  int len = last-first+1; // max min`); // TODO!
        code.push(`  assert(first/8 <= sizeof(databits));`);
        code.push(`  assert(last/8 <= sizeof(databits));`);
        code.push(`  if (last>=first)`);
        code.push(`    for (int i=first; i<=last; i++)`);
        code.push(`    {`);
        code.push(`      v >>= 1;`);
        code.push(`      if (databits[i/8] & (1<<(7-(i&7))))`);
        code.push(`        v |= 1 << (len-1);`);
        code.push(`    }`);
        code.push(`  else`);
        code.push(`    assert(0);`);
        code.push(`  return v;`);
        code.push(`};`);

        for (var i=0; i<c[1].length; i++)
        {
          var key = c[1][i][0];
          var val = c[1][i][1];
          if (!key || key == "")
            continue;

          if (val == "arr")
          {
            if (flow[fi-1][0] == "Assert" && flow[fi-1][2].includes("LUT"))
            {
              var m = flow[fi-1][1].match("arr.length\\s*==\\s*(\\d+)")
              if (!m)
                throw "Cant guess the data length"
              var bits = parseInt(m[1])
              val = `arr[${bits-1}:0]`
            } else
             throw "Cant guess the data length"
          }

          if (val[0] == "\"")
            code.push(`pHandler->addStringAttribute("${key}", ${val});`);
          else if (val.match("arr\\[(\\d+)\\s*(\\+?):\\s*(\\d+)]"))
          {
            var m = val.match("arr\\[(\\d+)\\s*(\\+?):\\s*(\\d+)]");
            var first = m[2] == "" ? parseInt(m[1]) : (parseInt(m[1]) + parseInt(m[3]) - 1);
            var last = m[2] == "" ? parseInt(m[3]) : parseInt(m[1]);
            var index = null;
            while (first-last >= 32)
            {
              index = (index === null) ? 0 : index;
              code.push(`pHandler->addIntegerAttribute("${key}${index === null ? "" : index}"), ${Math.min(first, last+32-1)}, ${last});`)
              last += 32;
              index++;
            }

            code.push(`pHandler->addIntegerAttribute("${key}${index === null ? "" : index}"), ${first}, ${last});`)
          } else
            throw "problem"

//            code.push(`pHandler->addIntegerAttribute("${key}", ${val});`);
        }
//        code.push(`return true;`);
      break;
      case "Error":
        code.push(`// Dont know how to process ${JSON.stringify(c[1])}`)
      break;
    }
    if (inject.length)
      fullcode.splice(fullcode.length-2, 0, inject);
    if (code.length)
    {
      if (flow[fi][0] == "Bits to integer" && flow[fi-1][0] == "Assert") 
        fullcode.splice(fullcode.length-1, 0, code);
      else
        fullcode.push(code);
    }
  }

  for (var fi=0; fi<flow.length; fi++)
  {
    var inject = false;
    var c = flow[fi];
    var code = [`// ${c}`];
    var inject = [];
    synthElement(c)
  }
  code.push(`return true;`);
  fullcode.push(trailing);
  fullcode.push([`return false;`]);
  return fullcode.map(c=>c.join("\n")).join("\n");
}

function codeFlowEncoder(json)
{
  var flow = [];
  var getNode = (name, needed) => {
    var nodes = Object.values(json.nodes).filter(n => n.name == name);
    if (nodes.length != 1)
    {
      if (needed)
      {
        console.log("Failed while looking for", name);
        throw "Single node not found"
      }
    }
    return nodes[0];
  }
  var getChildren = (node) => {
    var ids = Object.values(node.outputs).map(o => o.connections).flat(1).map(x => x.node)
    return ids.map(i => json.nodes[i])
  }
  var findChild = (nodes, name) => {
    var nodes = nodes.filter(n => n.name == name);
    if (nodes.length != 1)
      return false;
    return nodes[0];
  }

  var curNode = getNode("Reverse Attributes", true);
  var attr = [curNode.data.attribute1, curNode.data.attribute2, curNode.data.attribute3, curNode.data.attribute4,
    curNode.data.attribute5, curNode.data.attribute6, curNode.data.attribute7, curNode.data.attribute8]
    .filter(x=>x[0] != "")
  flow.push(["Reverse Attributes", {attr:attr}])
  while (1)
  {
    var curChildren = getChildren(curNode);
    if (curChildren.length == 1 && curChildren[0].name == "Lookup table" && 
        getChildren(curChildren[0]).length == 1 &&
        getChildren(curChildren[0])[0].name == "Sequence")
    {
      var lut = curChildren[0];
      var lutdata = [lut.data.assign1, lut.data.assign2, lut.data.assign3, lut.data.assign4]
        .filter(l => l[0] != "")
      var seq = getChildren(curChildren[0])[0]
      var seqdata = [seq.data.sequence0, seq.data.sequence1, seq.data.sequence2, seq.data.sequence3]
        .filter(s => s[1] != "")
        .map(s => [s[0] == "" ? 1 : parseInt(s[0]), s[1]])
      var simpleRepeat = seqdata.length == 1 && seqdata[0][1].split(",").pop().trim().substr(-1) == "L";
      var singleInstance = seqdata.filter(s=>s[1].includes("arr")).length == 1 && 
        seqdata.filter(s=>s[1].includes("arr") && s[0] == 1).length == 1 &&
        lutdata.length == 2 && lutdata.find(l=>l[0] == "0") && lutdata.find(l=>l[0] == "1");

      if (simpleRepeat)
      {
        var pattern = seqdata[0][1].split(",").map(x=>x.trim());
        var repeat = seqdata[0][0] || 1;
        var prefix = [];
        var suffix = [];
        var pause = parseInt(pattern.pop());
        var i;
        for (i=0; i<pattern.length; i++)
          if (pattern[i] == "arr")
            break;
          else
            prefix.push(pattern[i]);
        for (i++; i<pattern.length; i++)
            suffix.push(pattern[i]);

        var zero = [];
        var one = [];
        for (var i=0; i<lutdata.length; i++)
          if (lutdata[i][0] == "0")
            zero = lutdata[i][1].split(",").map(x=>x.trim())
          else if (lutdata[i][0] == "1")
            one = lutdata[i][1].split(",").map(x=>x.trim())
          else
            throw "error"
        flow.push(["Lookup pause", {prefix:prefix, suffix:suffix, pause:pause, zero:zero, one:one, repeat:repeat}])

        curNode = getChildren(curChildren[0])[0];
        continue;
      } else if (singleInstance) {
        var lutzero = lutdata.find(l=>l[0] == "0")[1].split(",").map(x=>x.trim())
        var lutone = lutdata.find(l=>l[0] == "1")[1].split(",").map(x=>x.trim())
        flow.push(["Lookup simple sequence", {lut:{zero:lutzero, one:lutone}, sequence:seqdata}])
        curNode = getChildren(curChildren[0])[0];
        continue;
      } else
        throw "unsupported lookup combination"
    }

    if (curChildren.length == 1 && curChildren[0].name == "Display signal")
    {
        break;
    }
    throw "error " + curChildren.map(x => x.name).join(", ")
    break;
  }
  return flow;
}

function codeSynthEncode(flow)
{
  var code = []
  var encodeElement = (attr) =>
  {
    code.push(`// ${attr[0]}`);
    switch (attr[0])
    {
      case "Reverse Attributes":
        code.push("uint8_t data[32] = {};");
        code.push("int bits = 0;");
        code.push("auto writeBits = [&](uint32_t value, int first, int last) {");
        code.push("  bits = max(bits, max(first, last));");
        code.push("  if (first <= last)");
        code.push("    for (int i=first; i<=last; i++)");
        code.push("    {");
        code.push("      if (i >= 0 && i/8 < sizeof(data))");
        code.push("        data[i/8] |= (!!(value & (1<<(i-first)))) << (i&7);");
        code.push("    }");
        code.push("  else");
        code.push("    for (int i=first; i>=last; i--)");
        code.push("    {");
        code.push("      if (i >= 0 && i/8 < sizeof(data))");
        code.push("        data[i/8] |= (!!(value & (1<<(first-i)))) << (i&7);");
        code.push("    }");
        code.push("};");
        code.push("");
        for (var assign of attr[1].attr)
        {
          if (assign[1][0] == "\"" && assign[1].substr(-1) == "\"")
          {
            code.push(`if (strcmp(pHandler->getStringAttribute("${assign[0]}"), ${assign[1]}) != 0)`);
            code.push(`  return false;`);
          } else if (assign[1].match("arr\\[(\\d+)\\s*(\\+?):\\s*(\\d+)]"))
          {
            var m = assign[1].match("arr\\[(\\d+)\\s*(\\+?):\\s*(\\d+)]");
            var first = m[2] == "" ? parseInt(m[1]) : (parseInt(m[1]) + parseInt(m[3]) - 1);
            var last = m[2] == "" ? parseInt(m[3]) : parseInt(m[1]);
            var index = null;
            while (first-last >= 32)
            {
              index = (index === null) ? 0 : index;
              code.push(`writeBits(pHandler->getIntegerAttribute("${assign[0]}${index === null ? "" : index}"), ${Math.min(first, last+32-1)}, ${last});`)
              last += 32;
              index++;
            }

            code.push(`writeBits(pHandler->getIntegerAttribute("${assign[0]}${index === null ? "" : index}"), ${first}, ${last});`)
          }
        }
      break;
      case "Lookup simple sequence":
        encodeElement(["PushPulse-begin"])
        for (var pair of attr[1].sequence)
        {
          if (pair[0] == 1 && pair[1] == "arr")
          {
            // do full synth
            encodeElement(["Lookup", {lut:attr[1].lut}])
          } else {
            var symbols = pair[1].split(",").map(x=>x.trim());
            if (pair[0] > 1)
            {
              code.push(`for (int i=0; i<${pair[0]}; i++)`);
              code.push(`{`);
            }
            for (var s of symbols)
              code.push(`  pushPulse(${parseInt(s)}, '${s.substr(-1)}');`);
            if (pair[0] > 1)
              code.push(`}`);
          }
        }
        encodeElement(["PushPulse-end"])
      break;
      case "Lookup pause":
        encodeElement(["PushPulse-begin"])
        // do full synth
        for (var s of attr[1].prefix)
          code.push(`    pushPulse(${parseInt(s)}, '${s.substr(-1)}');`);
        encodeElement(["Lookup", {lut:attr[1]}])
        for (var s of attr[1].suffix)
          code.push(`    pushPulse(${parseInt(s)}, '${s.substr(-1)}');`);
        encodeElement(["PushPulse-end"])
        code.push(`pHandler->repeatWithPause(${attr[1].repeat}, ${attr[1].pause});`)
      break;
      case "Lookup":
        code.push(`for (int i=0; i<bits; i++)`);
        code.push(`  if (data[i/8] & (1<<(i&7)))`);
        code.push(`  {`);
        for (var s of attr[1].lut.one)
          code.push(`    pushPulse(${parseInt(s)}, '${s.substr(-1)}');`);
        code.push(`  } else {`);
        for (var s of attr[1].lut.zero)
          code.push(`    pushPulse(${parseInt(s)}, '${s.substr(-1)}');`);
        code.push(`  }`);
      break;
      case "PushPulse-begin":
        code.push(`  int lastPulseUs = 0;`);
        code.push(`  char lastPulseLevel = 0;`);
        code.push(`  auto pushPulse = [&](int us, char level)`);
        code.push(`  {`);
        code.push(`    if (lastPulseLevel == level)`);
        code.push(`    {`);
        code.push(`      lastPulseUs += us;`);
        code.push(`      return;`);
        code.push(`    }`);
        code.push(`    if (lastPulseLevel)`);
        code.push(`      pHandler->pushPulse(lastPulseUs);`);
        code.push(`    lastPulseUs = us;`);
        code.push(`    lastPulseLevel = level;`);
        code.push(`  };`);
      break;
      case "PushPulse-end":
        code.push(`  if (lastPulseLevel)`);
        code.push(`    pHandler->pushPulse(lastPulseUs);`);
      break;
      default:
        code.push("// don't know how to do this: " + attr[0]);

    }
  }

  for (var i=0; i<flow.length; i++)
  {
    encodeElement([...flow[i]]);
  }
  code.push("return true;")
  return code.join("\n");
}

function codeSynth(json)
{
  console.log("trying decoder");
  try {
    var flow = codeFlowDecoder(json)
    return codeSynthDecode(flow);    
  } catch (e) {
    console.log("Decoder synth error:", e)
  }
  console.log("trying encoder");
  try {
    var flow = codeFlowEncoder(json)
    return codeSynthEncode(flow);    
  } catch (e) {
    console.log("Encoder synth error:", e)
  }
  return "// unsupported design"
}

function codeIndent(lines)
{
  var indent = 0;
  var shortindent = 0;
  var longindent = 0;
  for (var i=0; i<lines.length; i++)
  {
    var l = lines[i];
    var minindent = indent;
    for (var c of l)
    {
      if (c == "}")
      {
        minindent = Math.min(minindent, --indent)
        if (longindent > 0)
        {
          longindent--;
          indent--;
        }
      }
      if (c == "{")
        indent++;
    }
    if (lines[i].substr(-1) == ":")
      lines[i] = lines[i].trim();
    else
      lines[i] = "  ".repeat(minindent) + lines[i].trim();
    if (shortindent)
    {
      shortindent--;
      indent--;
    }
    if (l.includes("if ") || l.includes("for ") || l.includes("else"))
    {
      if (!l.includes("{") && !lines[i+1].includes("{"))
      {
        shortindent++;
        indent++;
      }
      else if (!l.includes("{") && lines[i+1].includes("{"))
      {
        longindent++;
        indent++;
        i++;
        lines[i] = "  ".repeat(indent) + lines[i].trim();
        indent++;
      }
    }
  }
  return lines;
}

function generateCode(json)
{
  var getNode = (name) => {
    var nodes = Object.values(json.nodes).filter(n => n.name == name);
    if (nodes.length != 1)
      return;
    return nodes[0];
  }

  var inputNodeData = getNode("Input signal", false)?.data?.out?.data;
  var inputNodeAttributes = [];
  if (!inputNodeData)
  {
    inputNodeData = [];
    inputNodeAttributes = getNode("Attributes", true)?.data
    inputNodeAttributes = ["attribute1", "attribute2", "attribute3", "attribute4",
      "attribute5", "attribute6", "attribute7", "attribute8"].map(a => inputNodeAttributes[a])
      .filter(a => a[0] != "")
  }

  var coderDecoder = inputNodeData.length == 0;

  var fullcode = `
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
`;
if (!coderDecoder)
  fullcode += `uint16_t data[] = {${inputNodeData.join(", ")}};
`
fullcode += `
template <typename T>T max(T a, T b)
{
    return (a > b) ? a : b;
}

class CCodecHandler
{
public:
  void addStringAttribute(const char* key, const char* value)
  {
      printf("addStringAttribute('%s', '%s');\\n", key, value);
  }
  void addIntegerAttribute(const char* key, uint32_t value)
  {
      printf("addIntegerAttribute('%s', 0x%x);\\n", key, value);
  }
  const char* getStringAttribute(const char* key)
  {
`
    inputNodeAttributes.filter(x=>x[1][0] == "\"").forEach(x => 
      fullcode += `    if (strcmp(key, "${x[0]}") == 0)
        return ${x[1]};
`)
fullcode += `  assert(0);
  }
  const uint32_t getIntegerAttribute(const char* key)
  {
`
    inputNodeAttributes.filter(x=>x[1][0] != "\"").forEach(x =>
      fullcode += `    if (strcmp(key, "${x[0]}") == 0)
        return ${x[1]};
`)
fullcode += `    assert(0);
  }          
  void pushPulse(int n)
  {
    printf("%d, ", n);
  }
  void repeatWithPause(int repeat, int pause)
  {
    printf("Repeat sequence %d times with %d us pauses in between\\n", repeat, pause);
  }
};

`
if (coderDecoder)
  fullcode += `bool test(CCodecHandler* pHandler)`;
else
  fullcode += `bool test(uint16_t* arr, int len, CCodecHandler* pHandler)`;

fullcode += `
{
  ${codeSynth(json)}
}

int main(void)
{
  CCodecHandler handler;
`
if (coderDecoder)
  fullcode += `  printf("decoding result: %d\\n", test(&handler));
`;
else
  fullcode += `  printf("decoding result: %d\\n", test(data, sizeof(data), &handler));
`;
fullcode += `  return 0;
}
`
  return codeIndent(fullcode.split("\n")).join("\n");
}

if (typeof(module) != "undefined" && typeof(module.exports) != "undefined")
  module.exports = generateCode;