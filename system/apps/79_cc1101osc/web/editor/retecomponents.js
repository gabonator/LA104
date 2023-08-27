class InputSignal extends Rete.Component {
    constructor(){
        super("Input signal");
    }
    builder(node) {
        var out = new Rete.Output('out', "Signal", arrSocket);
//        node.data.out = new Signal([300,600,280,600,320,560,320,600,320,600,320,580,320,600,280,600,320,580,320,580,300,640,280,600,320,580,280,340,580,320,600,580,300,620,300,600,300,600,300,320,600,580,320,580,320,600,320,300,560,22580,300,580,280,600,320,620,300,580,320,580,300,600,320,600,300,600,300,600,300,600,300,620,300,600,300,600,300,300,620,280,640,560,300,620,280,620,280,620,280,340,600,580,320,580,340,560,320,300,600,23000,280,580,300,620,260,640,280,600,300,600,300,600,320,600,300,600,320,580,320,600,280,620,280,600,300,640,280,320,580,320,580,600,300,620,260,620,300,600,300,340,580,600,300,600,320,580,320,320,580,22540,320,580,280,620,300,600,300,620,300,580,300,600,300,620,300,600,300,600,280,600,320,620,300,580,300,600,340,280,600,300,620,580,300,620,300,580,320,600,280,320,580,620,320,600,280,600,320,300,580,498500,280,620,280,580,340,600,300,600,300,600,300,600,320,580,340,580,320,560,300,620,280,620,300,600,300,580,340,300,600,320,580,580,320,600,300,600,300,620,300,300,580,620,320,560,320,320,580,600,320,22600,260,600,300,620,300,580,320,600,300,620,280,620,280,620,260,620,300,600,340,560,320,600,300,600,300,620,300,300,600,320,600,580,300,600,280,620,300,620,280,340,580,600,280,620,280,340,580,580,320,23060,280,600,280,600,300,600,320,560,320,600,320,580,320,580,300,620,300,600,300,600,320,580,320,600,280,620,280,340,600,280,600,600,320,560,320,600,320,580,320,300,600,600,320,560,320,340,580,600,280,22580,280,600,280,620,300,600,300,600,320,580,300,620,280,620,280,600,300,600,320,580,340,580,300,620,280,620,300,320,580,340,560,600,300,600,320,600,280,600,300,340,600,600,280,600,300,320,580,620,300]);
        node.data.out = new Signal([2000,1000,960,2000,980,980,2000,1000,980,960,1020,1980,980,980,1980,980,1000,980,980,980,1000,980,980,1980,1000,960,1020,980,1980,1000,980,980,1000,1960,2020,1960,2000,980,1000,1980,1980,1980,2000,1980,1020,960,980,1000,980,980,2000,980,1000,960,980,980,1000,1980,1020,940,2020,1980,980,1000,1960,1000,1000,1980,1980,1980,2000,10020,2000,980,1000,1960,1000,960,2020,980,980,1000,980,1960,1020,960,2000,1000,980,980,1000,980,980,960,1020,1980,980,980,1000,980,1980,980,980,1000,980,1980,2000,1980,2000,960,1000,1980,2020,1960,2000,1960,1020,960,1020,960,1020,960,2000,960,980,1000,980,980,980,1980,1020,960,2020,1980,980,960,2000,1000,980,2000,1960,2020,1980,10020,2000,960,1000,1980,1020,960,2000,960,1000,980,980,1980,1020,960,2000,980,980,980,980,1000,980,980,980,1980,1020,960,1020,940,2000,1000,980,1000,980,1980,2000,1980,1980,1000,980,1980,2000,1980,1980,1980,1020,960,980,1000,980,1000,2000,940,1020,960,1020,960,980,1980,1020,960,2000,1980,980,1000,1980,1000,980,1980,2000,1960,2020]);
        node.data.out = new Signal([540,460,480,460,500,440,500,440,520,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,460,500,440,500,440,520,440,500,440,500,440,500,460,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,520,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,460,500,440,500,440,500,440,520,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,460,500,440,500,440,500,460,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,440,520,440,500,440,500,440,520,440,1000,440,740,700,740,700,760,680,500,440,520,920,520,440,500,440,500,460,500,440,500,440,1000,440,500,460,500,440,500,940,1000,440,500,440,520,920,1000,460,500,940,500,440,500,440,500,460,980,460,500,440,500,940,500,440,520,440,500,440,1000,940,500,440,1000,940,1000,440,500,440,500,940,1000,440,500,460,500,440,500,940,500,440,1000,940,500,460,500,440,1000,440,500,440,500,460,500,440,500,440,500,940,520,440,500,440,1000,940,500,440,500,440,520,440,500,440,500,460,500,440,500,440,1000,440,500,460,500,940,980,940,520,440,1000,920,1000,940,1000,440,500])
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
        SignalHistogram(n1);
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
        node.data.datatype = {options: {"uint8_t": 0, "uint16_t": 1, "uint32_t": 2}, value:2, title:"Data type"}

        return node
            .addInput(inp)
            .addControl(firstbit)
            .addControl(datatype)
            .addOutput(this.out);
    }

    worker(node, inputs, outputs) {
        var arr = [];
        if (inputs['in'].length && inputs['in'][0]?.data)
          arr = [...inputs['in'][0].data];

        var uint = 0;
        var msb = node.data.firstbit.value == 0
        var databits = [8*1, 8*2, 8*4][node.data.datatype.value]
        var arrout = [];

        var bitsToInt = (a, count, msb) => {
          var uint = 0;
          for (var i=0; i<a.length; i++)
          {
            if (a[i] === "0" || a[i] === 0)
            {
            } else if (a[i] === "1" || a[i] === 1)
            {
              if (msb)
                uint |= 1<<(count-1-i)
              else
                uint |= 1<<i
            } else {
              outputs['out'] = {data:[]}
              this.out.name = `Error!`;
              this.out.node.update();
              throw "wrong data"
            }
          }

          var digits = (count+3)>>2;
          var hex = "0x"+(("00000000"+uint.toString(16)).substr(-digits));
          return hex;
        }

        if (arr.length < databits)
        {
          arrout.push(bitsToInt(arr, arr.length, msb));
        } else {
          while (arr.length % databits != 0)
            arr.unshift(0);

          while (arr.length >= databits)
          {
            var part = arr.splice(0, databits);
            arrout.push(bitsToInt(part, databits, msb));
          }
          if (arr.length)
            arrout.push(bitsToInt(arr, databits, msb));
        }
        if (!msb)
          arrout.reverse();
        outputs['out'] = {data:arrout}
        this.out.name = `uint32_t [${arrout.length}]`; // TODO: signal
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
        node.data.attribute1 = ["id", "\"Came 001unip\""]
        node.data.attribute2 = ["code", "arr[0]"]
        node.data.attribute3 = ["", ""]
        node.data.attribute4 = ["", ""]
        var static1 = new StaticControl(this.editor, "static1")
        node.data.static1 = "Atribute&nbsp;&nbsp;&nbsp;Value"

        return node
            .addInput(inp)
            .addControl(static1)
            .addControl(attribute1)
            .addControl(attribute2)
            .addControl(attribute3)
            .addControl(attribute4)
            .addOutput(this.out);
    }

    worker(node, inputs, outputs) {
        var _node = this.editor.nodes.find(n => n.id == node.id)
        var arr = inputs['in'].length ? [...inputs['in'][0].data] : [];

        _node.controls.get('attribute1').setVisible(true)
        _node.controls.get('attribute2').setVisible(node.data.attribute1.join("") != "")
        _node.controls.get('attribute3').setVisible(node.data.attribute2.join("") != "")
        _node.controls.get('attribute4').setVisible(node.data.attribute3.join("") != "")
        var attrs = [node.data.attribute1, node.data.attribute2, node.data.attribute3, node.data.attribute4]
        var arrout = [];
        for (var attr of attrs)
        {
          if (attr[0] == "")
            continue;
          if (attr[1][0] == "\"")
            arrout.push([attr[0], attr[1]]);
          else
            arrout.push([attr[0], eval(attr[1])]);
        }
        outputs['out'] = {data:arrout, type:"attributes"};

        this.out.name = `Array of ${arrout.length} attributes`;
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
        var input = inputs['in'].length ? inputs['in'][0] : {data:[]}
        var arr = input.data;
        var p = "";
        if (input?.type == "attributes")
          p = arr.map(pair => `${pair[0]}: ${pair[1]}`).join("\n")
        else
          p = arr.join(", ");
        console.log(arr);
        this.editor.nodes.find(n => n.id == node.id).controls.get('print').setValue(p);
    }         
}

class Assert extends Rete.Component {
    constructor(){
        super("Assertion");
    }

    builder(node) {
        var inp = new Rete.Input('in', "Signal", arrSocket);
        var condition = new StrControl(this.editor, 'condition', false, 'Condition');
        var static1 = new StaticControl(this.editor, "static1")
        node.data.condition = "arr.length == 24";
        node.data.static1 = ""
        return node
            .addInput(inp)
            .addControl(condition)
            .addControl(static1);
    }

    worker(node, inputs, outputs) {
        var _node = this.editor.nodes.find(n => n.id == node.id)
        var input = inputs['in'].length ? inputs['in'][0] : new Signal();
        var result;
        if (input.checkAssertion(node.data.condition))
          result = "OK";
        else
          result = "Error!";
        _node.controls.get('static1').setValue(result)
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
        outputs['out'] = [input.getCountElements(node.data.condition, node.data.breakOn)];

        var _node = this.editor.nodes.find(n => n.id == node.id)
        _node.outputs.get("out").name = outputs['out'][0]; // TODO: getInfo
        _node.outputs.get("out").node.update();
    }
}
