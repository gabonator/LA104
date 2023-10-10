class InputSignal extends Rete.Component {
    constructor(){
        super("Input signal");
    }
    builder(node) {
        var out = new Rete.Output('out', "Signal", arrSocket);
//        node.data.out = new Signal([300,600,280,600,320,560,320,600,320,600,320,580,320,600,280,600,320,580,320,580,300,640,280,600,320,580,280,340,580,320,600,580,300,620,300,600,300,600,300,320,600,580,320,580,320,600,320,300,560,22580,300,580,280,600,320,620,300,580,320,580,300,600,320,600,300,600,300,600,300,600,300,620,300,600,300,600,300,300,620,280,640,560,300,620,280,620,280,620,280,340,600,580,320,580,340,560,320,300,600,23000,280,580,300,620,260,640,280,600,300,600,300,600,320,600,300,600,320,580,320,600,280,620,280,600,300,640,280,320,580,320,580,600,300,620,260,620,300,600,300,340,580,600,300,600,320,580,320,320,580,22540,320,580,280,620,300,600,300,620,300,580,300,600,300,620,300,600,300,600,280,600,320,620,300,580,300,600,340,280,600,300,620,580,300,620,300,580,320,600,280,320,580,620,320,600,280,600,320,300,580,498500,280,620,280,580,340,600,300,600,300,600,300,600,320,580,340,580,320,560,300,620,280,620,300,600,300,580,340,300,600,320,580,580,320,600,300,600,300,620,300,300,580,620,320,560,320,320,580,600,320,22600,260,600,300,620,300,580,320,600,300,620,280,620,280,620,260,620,300,600,340,560,320,600,300,600,300,620,300,300,600,320,600,580,300,600,280,620,300,620,280,340,580,600,280,620,280,340,580,580,320,23060,280,600,280,600,300,600,320,560,320,600,320,580,320,580,300,620,300,600,300,600,320,580,320,600,280,620,280,340,600,280,600,600,320,560,320,600,320,580,320,300,600,600,320,560,320,340,580,600,280,22580,280,600,280,620,300,600,300,600,320,580,300,620,280,620,280,600,300,600,320,580,340,580,300,620,280,620,300,320,580,340,560,600,300,600,320,600,280,600,300,340,600,600,280,600,300,320,580,620,300]);
//        node.data.out = new Signal([2000,1000,960,2000,980,980,2000,1000,980,960,1020,1980,980,980,1980,980,1000,980,980,980,1000,980,980,1980,1000,960,1020,980,1980,1000,980,980,1000,1960,2020,1960,2000,980,1000,1980,1980,1980,2000,1980,1020,960,980,1000,980,980,2000,980,1000,960,980,980,1000,1980,1020,940,2020,1980,980,1000,1960,1000,1000,1980,1980,1980,2000,10020,2000,980,1000,1960,1000,960,2020,980,980,1000,980,1960,1020,960,2000,1000,980,980,1000,980,980,960,1020,1980,980,980,1000,980,1980,980,980,1000,980,1980,2000,1980,2000,960,1000,1980,2020,1960,2000,1960,1020,960,1020,960,1020,960,2000,960,980,1000,980,980,980,1980,1020,960,2020,1980,980,960,2000,1000,980,2000,1960,2020,1980,10020,2000,960,1000,1980,1020,960,2000,960,1000,980,980,1980,1020,960,2000,980,980,980,980,1000,980,980,980,1980,1020,960,1020,940,2000,1000,980,1000,980,1980,2000,1980,1980,1000,980,1980,2000,1980,1980,1980,1020,960,980,1000,980,1000,2000,940,1020,960,1020,960,980,1980,1020,960,2000,1980,980,1000,1980,1000,980,1980,2000,1960,2020]);
        //node.data.out = new Signal([540,460,480,460,500,440,500,440,520,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,460,500,440,500,440,520,440,500,440,500,440,500,460,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,520,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,460,500,440,500,440,500,440,520,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,440,520,440,1000,440,740,700,740,700,760,680,500,440,520,920,520,440,500,440,500,460,500,440,500,440,1000,440,500,460,500,440,500,940,1000,440,500,440,520,920,1000,460,500,940,500,440,500,440,500,460,980,460,500,440,500,940,500,440,520,440,500,440,1000,940,500,440,1000,940,1000,440,500,440,500,940,1000,440,500,460,500,440,500,940,500,440,1000,940,500,460,500,440,1000,440,500,440,500,460,500,440,500,440,500,940,520,440,500,440,1000,940,500,440,500,440,520,440,500,440,500,460,500,440,500,440,1000,440,500,460,500,940,980,940,520,440,1000,920,1000,940,1000,440,500])
        node.data.out = new Signal([]);
        var static1 = new StaticControl(this.editor, "static1")
        node.data.static1 = "Double click to set...";
        return node.addControl(static1).addOutput(out);
    }
    worker(node, inputs, outputs) {
        var _node = this.editor.nodes.find(n => n.id == node.id)
        outputs['out'] = node.data.out;
        _node.outputs.get("out").name = node.data.out.getInfo();
        _node.outputs.get("out").node.update();
    }
}

class Quantize extends Rete.Component {
    constructor(){
        super("Quantize");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Signal", arrSocket);
        this.out = new Rete.Output('out', "Signal", arrSocket);
        var len = new NumControl(this.editor, 'len');
        node.data.len = 300;
        return node
            .addInput(inp)
            .addControl(len)
            .addOutput(this.out);
    }

    worker(node, inputs, outputs) {
        var _node = this.editor.nodes.find(n => n.id == node.id)
        var input = inputs['in'].length ? inputs['in'][0] : new Signal();
        outputs['out'] = input.makeQuantized(node.data.len);
        _node.outputs.get("out").name = outputs['out'].getInfo();
        _node.outputs.get("out").node.update();
    }
}

class Filter extends Rete.Component {
    constructor(){
        super("Filter only");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Signal", arrSocket);
        this.out = new Rete.Output('out', "Signal", arrSocket);
        var condition = new StrControl(this.editor, 'condition', false, 'Condition');
        node.data.condition = "x < 5000";
        return node
            .addInput(inp)
            .addControl(condition)
            .addOutput(this.out);
    }

    worker(node, inputs, outputs) {
        var input = inputs['in'].length ? inputs['in'][0] : new Signal();
        outputs['out'] = input.makeFilterByCondition(node.data.condition, false);
        this.out.name = outputs['out'].getInfo();
        this.out.node.update();
    }
}

class Split extends Rete.Component {
    constructor(){
        super("Split");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Signal", arrSocket);
        this.out = new Rete.Output('out', "Signal", arrSocket);
        var condition = new StrControl(this.editor, 'condition', false, 'Condition');
        node.data.condition = "x > 5000";
        return node
            .addInput(inp)
            .addControl(condition)
            .addOutput(this.out);
    }

    worker(node, inputs, outputs) {
        var input = inputs['in'].length ? inputs['in'][0] : new Signal();
        outputs['out'] = input.makeSplitByCondition(node.data.condition);
        this.out.name = outputs['out'].getInfo();
        this.out.node.update();
    }
}

class If extends Rete.Component {
    constructor(){
        super("If signal...");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Signal", arrSocket);
        var out = new Rete.Output('out', "Number", numSocket);
        var condition = new StrControl(this.editor, 'condition', false, 'If condition is true');
        var static1 = new StaticControl(this.editor, "static1")
        var retValues = new StrDoubleControl(this.editor, 'retValues', {width1: "70px", width2: "70px"});
        node.data.retValues = ["1", "0"]
        node.data.static1 = "Return&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;else return"
        node.data.condition = "arr[0] < 800 && arr[1] > 5000";

        var aux = node
            .addInput(inp)
            .addControl(condition)
            .addControl(static1)
            .addControl(retValues)
            .addOutput(out);
        return aux;
    }

    worker(node, inputs, outputs) {
        var _node = this.editor.nodes.find(n => n.id == node.id)
        var arr = inputs['in'].length ? inputs['in'][0] : new Signal();
        var q = node.data.len;
        var newarr = [];
        var result;
        try {
          result = eval(`${node.data.condition}`);
        } catch (e) {
          _node.outputs.get("out").name = `Error`;
          _node.outputs.get("out").node.update();
          return;
        }
        var resultValue = result ? node.data.retValues[0] : node.data.retValues[1];
        outputs['out'] = [resultValue];
        _node.outputs.get("out").name = `Number ${resultValue}`;
        _node.outputs.get("out").node.update();
    }
}

class Histogram extends Rete.Component {
    constructor(){
        super("Histogram");
    }

    builder(node) {
        var inp1 = new Rete.Input('in', "Signal", arrSocket);

        return node
            .addInput(inp1);
    }

    worker(node, inputs, outputs) {
        var n1 = inputs['in'].length ? inputs['in'][0] : new Signal();
        if (n1.type.substr(0, 5) == "multi")
          n1 = n1.multi[0];
        SignalHistogram(n1);
    }
    destroyed(node)
    {
        SignalHistogram(new Signal());
    }

}


class SubArray extends Rete.Component {
    constructor(){
        super("SubArray");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Signal", arrSocket);
        var _begin = new Rete.Input('_begin', "(Number: Index of first)", numSocket);
        var _length = new Rete.Input('_len', "(Number: Length)", numSocket);
        var out = new Rete.Output('out', "Signal", arrSocket);
        var begin = new NumControl(this.editor, 'begin', false, "Index of first");
        var len = new NumControl(this.editor, 'len', false, "Length");
        var tillEnd = new CheckControl(this.editor, 'tillEnd', false, "Everything till end");

        node.data.begin = 1;
        node.data.len = 10;
        node.data.tillEnd = true;
        return node
            .addInput(inp)
            .addInput(_begin)
            .addInput(_length)
            .addControl(begin)
            .addControl(len)
            .addControl(tillEnd)
            .addOutput(out);
    }

    worker(node, inputs, outputs) {
        var _node = this.editor.nodes.find(n => n.id == node.id)
        var arr = inputs['in'].length ? inputs['in'][0] : new Signal();
        if (inputs['_begin'].length == 1 && inputs['_begin'][0] !== undefined)
        {
          node.data.begin = inputs['_begin'][0][0]
          _node.controls.get('begin').setValue(node.data.begin)
          _node.controls.get('begin').setVisible(false)
        } else
          _node.controls.get('begin').setVisible(true)

        if (inputs['_len'].length == 1 && inputs['_len'][0] !== undefined)
        {
          node.data.len = inputs['_len'][0][0]
          _node.controls.get('len').setValue(inputs['_len'][0])
          _node.controls.get('len').setVisible(false)
        } else
          _node.controls.get('len').setVisible(true && !node.data.tillEnd)
        arr = arr.makeSlice(node.data.begin, node.data.len, node.data.tillEnd)

        outputs['out'] = arr;
        _node.outputs.get("out").name = arr.getInfo();
        _node.outputs.get("out").node.update();
    }
}

class Pairs extends Rete.Component {
    constructor(){
        super("Pairs");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Signal", arrSocket);
        this.out = new Rete.Output('out', "Signal", arrSocket);
        return node
            .addInput(inp)
            .addOutput(this.out);
    }

    worker(node, inputs, outputs) {
        var arr = inputs['in'].length ? inputs['in'][0] : new Signal();
        if (arr.data.length % 2 != 0)
        {
          outputs['out'] = new Signal([])
          this.out.name = `Error!`;
          this.out.node.update();
          throw "not pairs!"
        }

        outputs['out'] = arr.makePairs();

        this.out.name = outputs['out'].getInfo()
        this.out.node.update();
    }
}

class LookupTable extends Rete.Component {
    constructor(){
        super("Lookup table");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Signal", arrSocket);
        this.out = new Rete.Output('out', "Signal", arrSocket);
        var assignment1 = new StrDoubleControl(this.editor, 'assign1', {width1: 120, width2: 60});
        var assignment2 = new StrDoubleControl(this.editor, 'assign2', {width1: 120, width2: 60});
        var assignment3 = new StrDoubleControl(this.editor, 'assign3', {width1: 120, width2: 60});
        var assignment4 = new StrDoubleControl(this.editor, 'assign4', {width1: 120, width2: 60});
        var assignment5 = new StrDoubleControl(this.editor, 'assign5', {width1: 120, width2: 60, disabled1: true, disabled2: false});
        node.data.assign1 = ["300 600", "1"]
        node.data.assign2 = ["600 300", "0"]
        node.data.assign3 = ["", ""]
        node.data.assign4 = ["", ""]
        node.data.assign5 = ["Other", "?"]
        var static1 = new StaticControl(this.editor, "static1")
        node.data.static1 = "Input&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Output"

        return node
            .addInput(inp)
            .addControl(static1)
            .addControl(assignment1)
            .addControl(assignment2)
            .addControl(assignment3)
            .addControl(assignment4)
            .addControl(assignment5)
            .addOutput(this.out);
    }

    worker(node, inputs, outputs) {
        var _node = this.editor.nodes.find(n => n.id == node.id)
        var arr = inputs['in'].length ? inputs['in'][0] : new Signal();

        _node.controls.get('assign1').setVisible(true) // first
        _node.controls.get('assign2').setVisible(node.data.assign1.join("") != "")
        _node.controls.get('assign3').setVisible(node.data.assign2.join("") != "")
        _node.controls.get('assign4').setVisible(node.data.assign3.join("") != "")
        _node.controls.get('assign5').setVisible(true) // default

        outputs['out'] = arr.makeLookupTable([node.data.assign1,
          node.data.assign2, node.data.assign3, node.data.assign4],
          node.data.assign5[1]);
        this.out.name = outputs['out'].getInfo()
        this.out.node.update();
    }
}

class ToInteger extends Rete.Component {
    constructor(){
        super("Bits to integer");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Signal", arrSocket);
        this.out = new Rete.Output('out', "Signal", arrSocket);

        var firstbit = new OptionControl(this.editor, 'firstbit');
        node.data.firstbit = {options: {"MSB": 0, "LSB": 1}, value:0, title:"First bit"}
        var datatype = new OptionControl(this.editor, 'datatype');
        node.data.datatype = {options: {"uint4_t:": 0, "uint8_t": 1, "uint16_t": 2, "uint32_t": 3}, value:3, title:"Data type"}

        return node
            .addInput(inp)
            .addControl(firstbit)
            .addControl(datatype)
            .addOutput(this.out);
    }

    worker(node, inputs, outputs) {
        var inp = new Signal();
        if (inputs['in'].length && inputs['in'][0])
          inp = inputs['in'][0];

        var databits = [4, 8, 16, 32][node.data.datatype.value]
        var datatype = ["uint4_t", "uint8_t", "uint16_t", "uint32_t"][node.data.datatype.value]
        var msb = node.data.firstbit.value == 0
        var convert = arr => new Signal(arr, "bitstream").getBitstreamToHex(msb, databits);
        var copy = new Signal();
        if (inp.type.substr(0, 5) == "multi")
        {
          copy.type = "multi"+datatype;
          copy.multi = [];
          for (var i=0; i<inp.multi.length; i++)
          {
            var conv = convert(inp.multi[i].data, msb, databits);
            if (!conv)
              continue;
            copy.multi.push({data: conv.values, length: conv.bits, levels: []});
          }
          if (copy.multi.length == 0)
          {
            outputs['out'] = new Signal();
            this.out.name = `Error!`;
            this.out.node.update();
            throw "wrong bit value";
            return;
          }
        } else {
          var conv = convert(inp.data, msb, databits);
          if (conv)
          {
            copy.data = conv.values;
            copy.level = [];
            copy.length = conv.bits;
            copy.type = datatype;
          } else {
            outputs['out'] = new Signal();
            this.out.name = `Error!`;
            this.out.node.update();
            throw "wrong bit value";
            return;
          }
        }
        outputs['out'] = copy;
        this.out.name = copy.getInfo();
        this.out.node.update();
    }
}

class Attributes extends Rete.Component {
    constructor(){
        super("Attributes");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Array", arrSocket);
        this.out = new Rete.Output('out', "Attributes", arrSocket);
        var attribute1 = new StrDoubleControl(this.editor, 'attribute1', {width1: 60, width2: 140});
        var attribute2 = new StrDoubleControl(this.editor, 'attribute2', {width1: 60, width2: 140});
        var attribute3 = new StrDoubleControl(this.editor, 'attribute3', {width1: 60, width2: 140});
        var attribute4 = new StrDoubleControl(this.editor, 'attribute4', {width1: 60, width2: 140});
        var attribute5 = new StrDoubleControl(this.editor, 'attribute5', {width1: 60, width2: 140});
        var attribute6 = new StrDoubleControl(this.editor, 'attribute6', {width1: 60, width2: 140});
        var attribute7 = new StrDoubleControl(this.editor, 'attribute7', {width1: 60, width2: 140});
        var attribute8 = new StrDoubleControl(this.editor, 'attribute8', {width1: 60, width2: 140});
        node.data.attribute1 = ["id", "\"Came 001unip\""]
        node.data.attribute2 = ["code", "arr[0]"]
        node.data.attribute3 = ["", ""]
        node.data.attribute4 = ["", ""]
        node.data.attribute5 = ["", ""]
        node.data.attribute6 = ["", ""]
        node.data.attribute7 = ["", ""]
        node.data.attribute8 = ["", ""]
        var static1 = new StaticControl(this.editor, "static1")
        node.data.static1 = "Atribute&nbsp;&nbsp;&nbsp;Value"

        return node
            .addInput(inp)
            .addControl(static1)
            .addControl(attribute1)
            .addControl(attribute2)
            .addControl(attribute3)
            .addControl(attribute4)
            .addControl(attribute5)
            .addControl(attribute6)
            .addControl(attribute7)
            .addControl(attribute8)
            .addOutput(this.out);
    }

    worker(node, inputs, outputs) {
        var _node = this.editor.nodes.find(n => n.id == node.id)
        var inp = inputs['in'].length ? inputs['in'][0] : new Signal();

        _node.controls.get('attribute1').setVisible(true)
        _node.controls.get('attribute2').setVisible(node.data.attribute1.join("") != "")
        _node.controls.get('attribute3').setVisible(node.data.attribute2.join("") != "")
        _node.controls.get('attribute4').setVisible(node.data.attribute3.join("") != "")
        _node.controls.get('attribute5').setVisible(node.data.attribute4.join("") != "")
        _node.controls.get('attribute6').setVisible(node.data.attribute5.join("") != "")
        _node.controls.get('attribute7').setVisible(node.data.attribute6.join("") != "")
        _node.controls.get('attribute8').setVisible(node.data.attribute7.join("") != "")
        var attrs = [node.data.attribute1, node.data.attribute2, node.data.attribute3, node.data.attribute4,
          node.data.attribute5, node.data.attribute6, node.data.attribute7, node.data.attribute8]
        var arrout = [];
        var arr = null;
        if (inp.type.substr(0, 5) == "multi")
        {
          for (var i=0; i<inp.multi.length; i++)
          {
            var val = inp.multi[i].data;
            if (arr === null)
              arr = val;
            else if (arr.toString() != val.toString())
            {
              outputs['out'] = new Signal(`Inconsistent input!`, "error");
              this.out.name = `Inconsistent input!`;
              this.out.node.update();
              //throw "different multi values"
              return;
            }
          }
        } else {
          arr = inp.data;
        }

        for (var attr of attrs)
        {
          var key = attr[0];
          var rexp = attr[1];
          var rval = "?";
          if (key == "")
            continue;
          if (rexp[0] == "\"")
            rval = rexp;
          else
          {
            var subStream = null;
            if (rexp.match("arr\\[(\\d+)\\s*:\\s*(\\d+)]"))
            {
              var tokens = rexp.match("arr\\[(\\d+)\\s*:\\s*(\\d+)]");
              rexp = rexp.split(tokens[0]).join(`hexrange(${tokens[1]},${tokens[2]})`)
            }
            if (rexp.match("arr\\[(\\d+)\\s*\\+:\\s*(\\d+)]"))
            {
              // verilog indexed part select
              var tokens = rexp.match("arr\\[(\\d+)\\s*\\+:\\s*(\\d+)]");
              tokens = [tokens[0], parseInt(tokens[1]), parseInt(tokens[2])]
              rexp = rexp.split(tokens[0]).join(`hexrange(${tokens[1]+tokens[2]-1},${tokens[1]})`)
            }
            if (rexp == "arr")
            {
              rexp = `hexrange(0,${arr.length-1})`
            }
            var hexrange = (first, last) => {
              var subStream = new Signal(arr, "bitstream").makeRange(first, last);
              // encode bitstream as LSB first
              // MSB first looks better (nicely matches the nibbles when displayed as 4 bits per row)
              // 0, 0, 0, 0; 0, 1, 1, 0; 0, 0, 0, 1 -> 0x061
              // but we would need to encode bitstream length attribute
              // (or we would store it as 0x06100000),
              // to make sure that the very first bit is the bit at 31. position
              // storing it as LSB makes it easy to extract the bite range in encoder - first bitstream
              // bit is the LSB of first DWORD
              var hex = subStream.getBitstreamToHex(false, 32);
              if (hex.values.length == 1)
                return hex.values[0];
              return hex.values;
            }
            try {
              rval = eval(rexp);
            } catch (e) {
              outputs['out'] = new Signal("Cant convert to integer", "error");
              this.out.name = `Conversion failure`;
              this.out.node.update();
              return;
            }
            if (Array.isArray(rval))
            {
              for (var i=0; i<rval.length; i++)
                arrout.push([key+i, rval[i]]);
              continue;
            }
/*
            if (subStream)
            {
              rval = subStream.getBitstreamToHex(true, 32);
              if (!rval || rval.values.length == 0)
              {
                outputs['out'] = new Signal("Cant convert to integer", "error");
                this.out.name = `Conversion failure`;
                this.out.node.update();
                return;
              } else if (rval.values.length == 1)
              {
                rval = rval.values[0];
              } else {
                for (var i=0; i<rval.values.length; i++)
                  arrout.push([key+i, rval.values[i]]);
                continue;
              }
            }
*/
          }
          arrout.push([key, rval]);
        }
        outputs['out'] = new Signal(arrout, "attributes");

        this.out.name = outputs['out'].getInfo();
        this.out.node.update();
    }
}

class DisplaySignal extends Rete.Component {
    constructor(){
        super("Display signal");
    }

    builder(node) {
        var inp1 = new Rete.Input('in', "Signal", arrSocket);
        return node.addInput(inp1);
    }

    worker(node, inputs, outputs) {
        var _node = this.editor.nodes.find(n => n.id == node.id)
        var n1 = inputs['in'].length ? inputs['in'][0] : new Signal();
        if (n1.type.substr(0, 5) == "multi")
          n1 = n1.multi[0];

        if (n1.data.length == 0)
          _node.window.hide();
        else
          _node.window.show(n1, 0.12);
    }

    created(node)
    {
      node.window = new SignalCanvas(900, 100);
    }
    destroyed(node)
    {
      node.window.remove();
    }
}

class Print extends Rete.Component {
    constructor(){
        super("Print");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Signal", arrSocket);
        this.print = new TextControl(this.editor, 'print');
        return node.addInput(inp).addControl(this.print);
    }
    worker(node, inputs, outputs) {
        var description = (input) => {
          if (input?.type == "attributes")
            return input.data.map(pair => `${pair[0]}: ${pair[1]}`).join("\n")
          else        
            return input.data.join(", ");
        }

        var input = inputs['in'].length ? inputs['in'][0] : new Signal("No input", "message");
        var p = "";
        if (input.type == "message")
          p = `${input.message}`
        else if (input.type == "error")
          p = `Error: ${input.message}`
        else
        {
          if (input.type.substr(0, 5) == "multi")
          {
            for (var i=0; i<input.multi.length; i++)
            {
              p += input.type + "[" + i + "]:\n"
              p += description(input.multi[i]) + "\n";
            }
          } else {
            p = description(input);
          }
        }
        this.editor.nodes.find(n => n.id == node.id).controls.get('print').setValue(p);
    }         
}

class Assert extends Rete.Component {
    constructor(){
        super("Assertion");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Signal", arrSocket);
        var out = new Rete.Output('out', "Signal", arrSocket);
        var condition = new StrControl(this.editor, 'condition', false, 'Condition');
        var static1 = new StaticControl(this.editor, "static1")
        node.data.condition = "arr.length == 24";
        node.data.static1 = ""
        return node
            .addInput(inp)
            .addControl(condition)
            .addControl(static1)
            .addOutput(out)
    }

    worker(node, inputs, outputs) {
        var _node = this.editor.nodes.find(n => n.id == node.id)
        var input = inputs['in'].length ? inputs['in'][0] : new Signal();
        outputs['out'] = input;
        var result = "";
        if (input.type.substr(0, 5) == "multi")
        {
          var bad = 0;
          for (var i=0; i<input.multi.length; i++)
          {
            if (!input.checkAssertion(node.data.condition, i))
            {
              bad++;
              input.multi.splice(i--, 1);
            }
          }
          if (bad == 0)
            result = `${input.multi.length} x OK`
          else
            result = `${bad} x Error! ${input.multi.length} x OK`
/*
          var r = input.checkAssertion(node.data.condition);
          if (r > 0)
            result = r + " x OK "
          if (input.multi.length - r > 0)
            result += (input.multi.length - r) + " x Error!"
*/
        } else
        {
          if (input.checkAssertion(node.data.condition))
          {
            result = "OK";
          }
          else
          {
            result = "Error!";
            outputs['out'] = new Signal();
          }
        }
        _node.controls.get('static1').setValue(result);

        var _node = this.editor.nodes.find(n => n.id == node.id);
        _node.outputs.get("out").name = outputs['out'].getInfo();
        _node.outputs.get("out").node.update();

    }
}


class Count extends Rete.Component {
    constructor(){
        super("Count elements");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Signal", arrSocket);
        var out = new Rete.Output('out', "Number", numSocket);
        var condition = new StrControl(this.editor, 'condition', false, 'Condition');
        var breakOn = new CheckControl(this.editor, 'breakOn', false, "Break on first");
        node.data.condition = "x == 500";
        node.data.breakOn = true;
        return node
            .addInput(inp)
            .addControl(condition)
            .addControl(breakOn)
            .addOutput(out);
    }

    worker(node, inputs, outputs) {
        var input = inputs['in'].length ? inputs['in'][0] : new Signal();
        var result = input.getCountElements(node.data.condition, node.data.breakOn)
        outputs['out'] = [result];

        var _node = this.editor.nodes.find(n => n.id == node.id)
        _node.outputs.get("out").name = `Number ${result}`; //outputs['out'][0]; // TODO: getInfo
        _node.outputs.get("out").node.update();
    }
}

class Denoise extends Rete.Component {
    constructor(){
        super("Denoise");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Signal", arrSocket);
        this.out = new Rete.Output('out', "Signal", arrSocket);
        var len = new NumControl(this.editor, 'len');
        node.data.len = 100;
        return node
            .addInput(inp)
            .addControl(len)
            .addOutput(this.out);
    }

    worker(node, inputs, outputs) {
        var _node = this.editor.nodes.find(n => n.id == node.id)
        var input = inputs['in'].length ? inputs['in'][0] : new Signal();
        outputs['out'] = input.makeDenoised(node.data.len);
        _node.outputs.get("out").name = outputs['out'].getInfo();
        _node.outputs.get("out").node.update();
    }
}

class Select extends Rete.Component {
    constructor(){
        super("Select");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Signal", arrSocket);
        this.out = new Rete.Output('out', "Signal", arrSocket);
        var index = new NumControl(this.editor, 'index');
        node.data.index = 0;
        return node
            .addInput(inp)
            .addControl(index)
            .addOutput(this.out);
    }

    worker(node, inputs, outputs) {
        var _node = this.editor.nodes.find(n => n.id == node.id)
        var input = inputs['in'].length ? inputs['in'][0] : new Signal();
        outputs['out'] = input.makeSelect(node.data.index);
        _node.outputs.get("out").name = outputs['out'].getInfo();
        _node.outputs.get("out").node.update();
    }
}

class Code extends Rete.Component {
    constructor(){
        super("Code");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Signal", arrSocket);
        var out = new Rete.Output('out', "Signal", arrSocket);
        var code = new TextControl(this.editor, 'code', false, {textWidth:"400px"});
        return node.addInput(inp).addControl(code).addOutput(out);
    }
    worker(node, inputs, outputs) {
        var input = inputs['in'].length ? inputs['in'][0] : new Signal();
        var arr = [...input.data];

        var code = this.editor.nodes.find(n => n.id == node.id).controls.get('code')
        code = code.getValue();
        eval(code);

        outputs['out'] = new Signal(arr, "symbols")
        var _node = this.editor.nodes.find(n => n.id == node.id)
        _node.outputs.get("out").name = outputs['out'].getInfo();
        _node.outputs.get("out").node.update();
    }         
}

// reverse

class RevAttributes extends Rete.Component {
    constructor(){
        super("Reverse Attributes");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Attributes", arrSocket);
        var out = new Rete.Output('out', "Bitstream", arrSocket);
        var attribute1 = new StrDoubleControl(this.editor, 'attribute1', {width1: 60, width2: 140});
        var attribute2 = new StrDoubleControl(this.editor, 'attribute2', {width1: 60, width2: 140});
        var attribute3 = new StrDoubleControl(this.editor, 'attribute3', {width1: 60, width2: 140});
        var attribute4 = new StrDoubleControl(this.editor, 'attribute4', {width1: 60, width2: 140});
        var attribute5 = new StrDoubleControl(this.editor, 'attribute5', {width1: 60, width2: 140});
        var attribute6 = new StrDoubleControl(this.editor, 'attribute6', {width1: 60, width2: 140});
        var attribute7 = new StrDoubleControl(this.editor, 'attribute7', {width1: 60, width2: 140});
        var attribute8 = new StrDoubleControl(this.editor, 'attribute8', {width1: 60, width2: 140});
        node.data.attribute1 = ["", ""]
        node.data.attribute2 = ["", ""]
        node.data.attribute3 = ["", ""]
        node.data.attribute4 = ["", ""]
        node.data.attribute5 = ["", ""]
        node.data.attribute6 = ["", ""]
        node.data.attribute7 = ["", ""]
        node.data.attribute8 = ["", ""]
        var static1 = new StaticControl(this.editor, "static1")
        node.data.static1 = "Atribute&nbsp;&nbsp;&nbsp;Value"

        return node
            .addInput(inp)
            .addControl(static1)
            .addControl(attribute1)
            .addControl(attribute2)
            .addControl(attribute3)
            .addControl(attribute4)
            .addControl(attribute5)
            .addControl(attribute6)
            .addControl(attribute7)
            .addControl(attribute8)
            .addOutput(out);
    }

    worker(node, inputs, outputs) {
        var _node = this.editor.nodes.find(n => n.id == node.id)
        var inp = inputs['in'].length ? inputs['in'][0] : new Signal();

        _node.controls.get('attribute1').setVisible(true)
        _node.controls.get('attribute2').setVisible(node.data.attribute1.join("") != "")
        _node.controls.get('attribute3').setVisible(node.data.attribute2.join("") != "")
        _node.controls.get('attribute4').setVisible(node.data.attribute3.join("") != "")
        _node.controls.get('attribute5').setVisible(node.data.attribute4.join("") != "")
        _node.controls.get('attribute6').setVisible(node.data.attribute5.join("") != "")
        _node.controls.get('attribute7').setVisible(node.data.attribute6.join("") != "")
        _node.controls.get('attribute8').setVisible(node.data.attribute7.join("") != "")
        if (inp.type != "attributes")
          throw "wrong input"

        var inattr = {}
        for (var a of inp.data)
          if (a[0])
            inattr[a[0]] = a[1];

        var attrs = [node.data.attribute1, node.data.attribute2, node.data.attribute3, node.data.attribute4,
          node.data.attribute5, node.data.attribute6, node.data.attribute7, node.data.attribute8]

        var outattr = {}
        for (var a of attrs)
          if (a[0])
            outattr[a[0]] = a[1];

        var bitstream = [];

        for (var a in outattr)
        {
          var key = a;
          if (outattr[key][0] == "\"")
          {
            if (inattr[key] != outattr[key])
              throw "String mismatch"
            continue;
          }

          if (outattr[key].match("arr\\[(\\d+)\\s*(\\+?):\\s*(\\d+)]"))
          {
            var tokens = outattr[key].match("arr\\[(\\d+)\\s*(\\+?):\\s*(\\d+)]");
            tokens = [tokens[0], parseInt(tokens[1]), tokens[2], parseInt(tokens[3])]
            var vals = []
            if (key+"0" in inattr)
            {
              for (var i=0; i<10; i++)
                if (key + i in inattr)
                  vals.push(inattr[key+i]) 
            } else
              vals.push(inattr[key])
            var ev;
            if (tokens[2] == "")
              ev = outattr[key].split(tokens[0]).join(`setrange(${tokens[1]},${tokens[3]},[${vals}])`)
            else if (tokens[2] == "+")
              ev = outattr[key].split(tokens[0]).join(`setrange(${tokens[1]+tokens[3]-1},${tokens[1]},[${vals}])`)
            else
              throw "error"
            var setrange = (first, last, data) =>
            {
              var remain = Math.max(first, last) - Math.min(first, last) + 1;
              var bits = 0;
              var check = 1; //1<<((remain - 1)%32); TODO: CHECK!
              for (var i=bitstream.len; i <= Math.max(first, last); i++) 
                bitstream[i] = '?';
              if (last >= first)
                for (var i=first; i <= last; i++)
                {
                  bitstream[i] = data[bits>>5] & check ? '1' : '0'
                  bits++;
                  check = (check >>> 1) | (check << 31)
                }
              else
                for (var i=first; i >= last; i--)
                {
                  bitstream[i] = data[bits>>5] & check ? '1' : '0'
                  bits++;
                  check = (check << 1) | (check >>> 31)
                }
            }
            eval(ev);
          } else
            throw "dont know what to do"
        }
        outputs['out'] = new Signal(bitstream, "bitstream");

        var _node = this.editor.nodes.find(n => n.id == node.id);
        _node.outputs.get("out").name = outputs['out'].getInfo();
        _node.outputs.get("out").node.update();
    }
}

class Sequence extends Rete.Component {
    constructor(){
        super("Sequence");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Array", arrSocket);
        var out = new Rete.Output('out', "Signal", arrSocket);
        var seq0 = new StrDoubleControl(this.editor, 'sequence0', {width1: 60, width2: 140});
        var seq1 = new StrDoubleControl(this.editor, 'sequence1', {width1: 60, width2: 140});
        var seq2 = new StrDoubleControl(this.editor, 'sequence2', {width1: 60, width2: 140});
        var seq3 = new StrDoubleControl(this.editor, 'sequence3', {width1: 60, width2: 140});
        node.data.sequence0 = ["", ""]
        node.data.sequence1 = ["", ""]
        node.data.sequence2 = ["", ""]
        node.data.sequence3 = ["", ""]
        var static1 = new StaticControl(this.editor, "static1")
        node.data.static1 = "Repeat&nbsp;&nbsp;&nbsp;&nbsp;Sequence"

        return node
            .addInput(inp)
            .addControl(static1)
            .addControl(seq0)
            .addControl(seq1)
            .addControl(seq2)
            .addControl(seq3)
            .addOutput(out);
    }

    worker(node, inputs, outputs) {
        var _node = this.editor.nodes.find(n => n.id == node.id)
        var inp = inputs['in'].length ? inputs['in'][0] : new Signal();

        _node.controls.get('sequence0').setVisible(true)
        _node.controls.get('sequence1').setVisible(node.data.sequence0.join("") != "")
        _node.controls.get('sequence2').setVisible(node.data.sequence1.join("") != "")
        _node.controls.get('sequence3').setVisible(node.data.sequence2.join("") != "")

        var out = new Signal([], "signal");
        var seq = [node.data.sequence0, node.data.sequence1, node.data.sequence2, node.data.sequence3]
        for (var s of seq)
          if (s[1])
          {
            var pulses = s[1].split(",").map(x=>x.trim());
            var repeat = s[0] ? parseInt(s[0]) : 1;
            for (var r=0; r<repeat; r++)
              for (var p of pulses)
              {
                if (p.substr(-1) == "H")
                {
                  out.data.push(parseInt(p));
                  out.level.push("h");
                }
                else if (p.substr(-1) == "L")
                {
                  out.data.push(parseInt(p));
                  out.level.push("l");
                }
                else if (p=="arr")
                {
                  out.data = out.data.concat(inp.data);
                  out.level = out.level.concat(inp.level);
                }
              }
          }
        outputs['out'] = out;

        var _node = this.editor.nodes.find(n => n.id == node.id);
        _node.outputs.get("out").name = outputs['out'].getInfo();
        _node.outputs.get("out").node.update();
    }
}

class Rtl433 extends Rete.Component {
    constructor(){
        super("Rtl433 Decoder");
        this.wasm = new WasmRtl433();
    }

    builder(node) {
        var inp = new Rete.Input('in', "Signal", arrSocket);
        var out = new Rete.Output('out', "Attributes", arrSocket);
        var static1 = new StaticControl(this.editor, "static1")
        node.data.static1 = "Unknown signal"

        return node
            .addInput(inp)
            .addControl(static1)
            .addOutput(out);
    }

    worker(node, inputs, outputs) {
        var _node = this.editor.nodes.find(n => n.id == node.id)
        var inp = inputs['in'].length ? inputs['in'][0] : new Signal();
        var signals = [];
        if (inp.type.substr(0, 5) == "multi")
        {
          for (var i=0; i<inp.multi.length; i++)
            signals.push(inp.multi[i].data);
        } else if (inp.type == "signal")
          signals.push(inp.data);

        _node.controls.get('static1').setValue(signals.length ? "Unknown" : "No data");

        // check level alternation?
        var kv = [];
        for (var i=0; i<signals.length; i++)
        {
          var m = this.wasm.decode(inp.data);
          if (m.length)
          {
            for (var attr in m[0])
              kv.push([attr, m[0][attr]]);
            _node.controls.get('static1').setValue(m[0].model + ((m.length > 1) ? "+" : ""));
          }
          if (kv.length)
            break;
        }
        outputs['out'] = new Signal(kv, "attributes");
    }
}
