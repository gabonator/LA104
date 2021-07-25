"use strict";
//Object.defineProperty(exports, "__esModule", { value: true });
//exports.Parser = void 0;
//require("fast-text-encoding");
//var context_1 = require("./context");
var aliasRegistry = {};
var FUNCTION_PREFIX = '___parser_';
var PRIMITIVE_SIZES = {
    uint8: 1,
    uint16le: 2,
    uint16be: 2,
    uint32le: 4,
    uint32be: 4,
    int8: 1,
    int16le: 2,
    int16be: 2,
    int32le: 4,
    int32be: 4,
    int64be: 8,
    int64le: 8,
    uint64be: 8,
    uint64le: 8,
    floatle: 4,
    floatbe: 4,
    doublele: 8,
    doublebe: 8,
};
var PRIMITIVE_NAMES = {
    uint8: 'Uint8',
    uint16le: 'Uint16',
    uint16be: 'Uint16',
    uint32le: 'Uint32',
    uint32be: 'Uint32',
    int8: 'Int8',
    int16le: 'Int16',
    int16be: 'Int16',
    int32le: 'Int32',
    int32be: 'Int32',
    int64be: 'BigInt64',
    int64le: 'BigInt64',
    uint64be: 'BigUint64',
    uint64le: 'BigUint64',
    floatle: 'Float32',
    floatbe: 'Float32',
    doublele: 'Float64',
    doublebe: 'Float64',
};
var PRIMITIVE_LITTLE_ENDIANS = {
    uint8: false,
    uint16le: true,
    uint16be: false,
    uint32le: true,
    uint32be: false,
    int8: false,
    int16le: true,
    int16be: false,
    int32le: true,
    int32be: false,
    int64be: false,
    int64le: true,
    uint64be: false,
    uint64le: true,
    floatle: true,
    floatbe: false,
    doublele: true,
    doublebe: false,
};
var Parser = /** @class */ (function () {
    function Parser() {
        this.varName = '';
        this.type = '';
        this.options = {};
        this.next = null;
        this.head = null;
        this.compiled = null;
        this.endian = 'be';
        this.constructorFn = null;
        this.alias = null;
    }
    Parser.start = function () {
        return new Parser();
    };
    Parser.prototype.primitiveGenerateN = function (type, ctx) {
        var typeName = PRIMITIVE_NAMES[type];
        var littleEndian = PRIMITIVE_LITTLE_ENDIANS[type];
        ctx.pushCode(ctx.generateVariable(this.varName) + " = dataView.get" + typeName + "(offset, " + littleEndian + ");");
        ctx.pushCode("offset += " + PRIMITIVE_SIZES[type] + ";");
    };
    Parser.prototype.primitiveN = function (type, varName, options) {
        return this.setNextParser(type, varName, options);
    };
    Parser.prototype.useThisEndian = function (type) {
        return (type + this.endian.toLowerCase());
    };
    Parser.prototype.uint8 = function (varName, options) {
        return this.primitiveN('uint8', varName, options);
    };
    Parser.prototype.uint16 = function (varName, options) {
        return this.primitiveN(this.useThisEndian('uint16'), varName, options);
    };
    Parser.prototype.uint16le = function (varName, options) {
        return this.primitiveN('uint16le', varName, options);
    };
    Parser.prototype.uint16be = function (varName, options) {
        return this.primitiveN('uint16be', varName, options);
    };
    Parser.prototype.uint32 = function (varName, options) {
        return this.primitiveN(this.useThisEndian('uint32'), varName, options);
    };
    Parser.prototype.uint32le = function (varName, options) {
        return this.primitiveN('uint32le', varName, options);
    };
    Parser.prototype.uint32be = function (varName, options) {
        return this.primitiveN('uint32be', varName, options);
    };
    Parser.prototype.int8 = function (varName, options) {
        return this.primitiveN('int8', varName, options);
    };
    Parser.prototype.int16 = function (varName, options) {
        return this.primitiveN(this.useThisEndian('int16'), varName, options);
    };
    Parser.prototype.int16le = function (varName, options) {
        return this.primitiveN('int16le', varName, options);
    };
    Parser.prototype.int16be = function (varName, options) {
        return this.primitiveN('int16be', varName, options);
    };
    Parser.prototype.int32 = function (varName, options) {
        return this.primitiveN(this.useThisEndian('int32'), varName, options);
    };
    Parser.prototype.int32le = function (varName, options) {
        return this.primitiveN('int32le', varName, options);
    };
    Parser.prototype.int32be = function (varName, options) {
        return this.primitiveN('int32be', varName, options);
    };
    Parser.prototype.bigIntVersionCheck = function () {
        if (!DataView.prototype.getBigInt64)
            throw new Error('BigInt64 is unsupported in this runtime');
    };
    Parser.prototype.int64 = function (varName, options) {
        this.bigIntVersionCheck();
        return this.primitiveN(this.useThisEndian('int64'), varName, options);
    };
    Parser.prototype.int64be = function (varName, options) {
        this.bigIntVersionCheck();
        return this.primitiveN('int64be', varName, options);
    };
    Parser.prototype.int64le = function (varName, options) {
        this.bigIntVersionCheck();
        return this.primitiveN('int64le', varName, options);
    };
    Parser.prototype.uint64 = function (varName, options) {
        this.bigIntVersionCheck();
        return this.primitiveN(this.useThisEndian('uint64'), varName, options);
    };
    Parser.prototype.uint64be = function (varName, options) {
        this.bigIntVersionCheck();
        return this.primitiveN('uint64be', varName, options);
    };
    Parser.prototype.uint64le = function (varName, options) {
        this.bigIntVersionCheck();
        return this.primitiveN('uint64le', varName, options);
    };
    Parser.prototype.floatle = function (varName, options) {
        return this.primitiveN('floatle', varName, options);
    };
    Parser.prototype.floatbe = function (varName, options) {
        return this.primitiveN('floatbe', varName, options);
    };
    Parser.prototype.doublele = function (varName, options) {
        return this.primitiveN('doublele', varName, options);
    };
    Parser.prototype.doublebe = function (varName, options) {
        return this.primitiveN('doublebe', varName, options);
    };
    Parser.prototype.bitN = function (size, varName, options) {
        if (!options) {
            options = {};
        }
        options.length = size;
        return this.setNextParser('bit', varName, options);
    };
    Parser.prototype.bit1 = function (varName, options) {
        return this.bitN(1, varName, options);
    };
    Parser.prototype.bit2 = function (varName, options) {
        return this.bitN(2, varName, options);
    };
    Parser.prototype.bit3 = function (varName, options) {
        return this.bitN(3, varName, options);
    };
    Parser.prototype.bit4 = function (varName, options) {
        return this.bitN(4, varName, options);
    };
    Parser.prototype.bit5 = function (varName, options) {
        return this.bitN(5, varName, options);
    };
    Parser.prototype.bit6 = function (varName, options) {
        return this.bitN(6, varName, options);
    };
    Parser.prototype.bit7 = function (varName, options) {
        return this.bitN(7, varName, options);
    };
    Parser.prototype.bit8 = function (varName, options) {
        return this.bitN(8, varName, options);
    };
    Parser.prototype.bit9 = function (varName, options) {
        return this.bitN(9, varName, options);
    };
    Parser.prototype.bit10 = function (varName, options) {
        return this.bitN(10, varName, options);
    };
    Parser.prototype.bit11 = function (varName, options) {
        return this.bitN(11, varName, options);
    };
    Parser.prototype.bit12 = function (varName, options) {
        return this.bitN(12, varName, options);
    };
    Parser.prototype.bit13 = function (varName, options) {
        return this.bitN(13, varName, options);
    };
    Parser.prototype.bit14 = function (varName, options) {
        return this.bitN(14, varName, options);
    };
    Parser.prototype.bit15 = function (varName, options) {
        return this.bitN(15, varName, options);
    };
    Parser.prototype.bit16 = function (varName, options) {
        return this.bitN(16, varName, options);
    };
    Parser.prototype.bit17 = function (varName, options) {
        return this.bitN(17, varName, options);
    };
    Parser.prototype.bit18 = function (varName, options) {
        return this.bitN(18, varName, options);
    };
    Parser.prototype.bit19 = function (varName, options) {
        return this.bitN(19, varName, options);
    };
    Parser.prototype.bit20 = function (varName, options) {
        return this.bitN(20, varName, options);
    };
    Parser.prototype.bit21 = function (varName, options) {
        return this.bitN(21, varName, options);
    };
    Parser.prototype.bit22 = function (varName, options) {
        return this.bitN(22, varName, options);
    };
    Parser.prototype.bit23 = function (varName, options) {
        return this.bitN(23, varName, options);
    };
    Parser.prototype.bit24 = function (varName, options) {
        return this.bitN(24, varName, options);
    };
    Parser.prototype.bit25 = function (varName, options) {
        return this.bitN(25, varName, options);
    };
    Parser.prototype.bit26 = function (varName, options) {
        return this.bitN(26, varName, options);
    };
    Parser.prototype.bit27 = function (varName, options) {
        return this.bitN(27, varName, options);
    };
    Parser.prototype.bit28 = function (varName, options) {
        return this.bitN(28, varName, options);
    };
    Parser.prototype.bit29 = function (varName, options) {
        return this.bitN(29, varName, options);
    };
    Parser.prototype.bit30 = function (varName, options) {
        return this.bitN(30, varName, options);
    };
    Parser.prototype.bit31 = function (varName, options) {
        return this.bitN(31, varName, options);
    };
    Parser.prototype.bit32 = function (varName, options) {
        return this.bitN(32, varName, options);
    };
    Parser.prototype.namely = function (alias) {
        aliasRegistry[alias] = this;
        this.alias = alias;
        return this;
    };
    Parser.prototype.skip = function (length, options) {
        return this.seek(length, options);
    };
    Parser.prototype.seek = function (relOffset, options) {
        if (options && options.assert) {
            throw new Error('assert option on seek is not allowed.');
        }
        return this.setNextParser('seek', '', { length: relOffset });
    };
    Parser.prototype.string = function (varName, options) {
        if (!options.zeroTerminated && !options.length && !options.greedy) {
            throw new Error('Neither length, zeroTerminated, nor greedy is defined for string.');
        }
        if ((options.zeroTerminated || options.length) && options.greedy) {
            throw new Error('greedy is mutually exclusive with length and zeroTerminated for string.');
        }
        if (options.stripNull && !(options.length || options.greedy)) {
            throw new Error('Length or greedy must be defined if stripNull is defined.');
        }
        options.encoding = options.encoding || 'utf8';
        return this.setNextParser('string', varName, options);
    };
    Parser.prototype.buffer = function (varName, options) {
        if (!options.length && !options.readUntil) {
            throw new Error('Length nor readUntil is defined in buffer parser');
        }
        return this.setNextParser('buffer', varName, options);
    };
    Parser.prototype.wrapped = function (varName, options) {
        if (!options.length && !options.readUntil) {
            throw new Error('Length nor readUntil is defined in buffer parser');
        }
        if (!options.wrapper || !options.type) {
            throw new Error('Both wrapper and type must be defined in wrapper parser');
        }
        return this.setNextParser('wrapper', varName, options);
    };
    Parser.prototype.array = function (varName, options) {
        if (!options.readUntil && !options.length && !options.lengthInBytes) {
            throw new Error('Length option of array is not defined.');
        }
        if (!options.type) {
            throw new Error('Type option of array is not defined.');
        }
        if (typeof options.type === 'string' &&
            !aliasRegistry[options.type] &&
            Object.keys(PRIMITIVE_SIZES).indexOf(options.type) < 0) {
            throw new Error("Specified primitive type \"" + options.type + "\" is not supported.");
        }
        return this.setNextParser('array', varName, options);
    };
    Parser.prototype.choice = function (varName, options) {
        if (typeof options !== 'object' && typeof varName === 'object') {
            options = varName;
            varName = null;
        }
        if (!options.tag) {
            throw new Error('Tag option of array is not defined.');
        }
        if (!options.choices) {
            throw new Error('Choices option of array is not defined.');
        }
        Object.keys(options.choices).forEach(function (keyString) {
            var key = parseInt(keyString, 10);
            var value = options.choices[key];
            if (isNaN(key)) {
                throw new Error('Key of choices must be a number.');
            }
            if (!value) {
                throw new Error("Choice Case " + keyString + " of " + varName + " is not valid.");
            }
            if (typeof value === 'string' &&
                !aliasRegistry[value] &&
                Object.keys(PRIMITIVE_SIZES).indexOf(value) < 0) {
                throw new Error("Specified primitive type \"" + value + "\" is not supported.");
            }
        });
        return this.setNextParser('choice', varName, options);
    };
    Parser.prototype.nest = function (varName, options) {
        if (typeof options !== 'object' && typeof varName === 'object') {
            options = varName;
            varName = null;
        }
        if (!options.type) {
            throw new Error('Type option of nest is not defined.');
        }
        if (!(options.type instanceof Parser) && !aliasRegistry[options.type]) {
            throw new Error('Type option of nest must be a Parser object.');
        }
        if (!(options.type instanceof Parser) && !varName) {
            throw new Error('options.type must be a object if variable name is omitted.');
        }
        return this.setNextParser('nest', varName, options);
    };
    Parser.prototype.pointer = function (varName, options) {
        if (!options.offset) {
            throw new Error('Offset option of pointer is not defined.');
        }
        if (!options.type) {
            throw new Error('Type option of pointer is not defined.');
        }
        else if (typeof options.type === 'string') {
            if (Object.keys(PRIMITIVE_SIZES).indexOf(options.type) < 0 &&
                !aliasRegistry[options.type]) {
                throw new Error('Specified type "' + options.type + '" is not supported.');
            }
        }
        else if (options.type instanceof Parser) {
        }
        else {
            throw new Error('Type option of pointer must be a string or a Parser object.');
        }
        return this.setNextParser('pointer', varName, options);
    };
    Parser.prototype.saveOffset = function (varName, options) {
        return this.setNextParser('saveOffset', varName, options);
    };
    Parser.prototype.endianess = function (endianess) {
        switch (endianess.toLowerCase()) {
            case 'little':
                this.endian = 'le';
                break;
            case 'big':
                this.endian = 'be';
                break;
            default:
                throw new Error("Invalid endianess: " + endianess);
        }
        return this;
    };
    Parser.prototype.create = function (constructorFn) {
        if (!(constructorFn instanceof Function)) {
            throw new Error('Constructor must be a Function object.');
        }
        this.constructorFn = constructorFn;
        return this;
    };
    Parser.prototype.getContext = function (importPath) {
        var ctx = new context_1.Context(importPath);
        ctx.pushCode('var dataView = new DataView(buffer.buffer, buffer.byteOffset, buffer.length);');
        if (!this.alias) {
            this.addRawCode(ctx);
        }
        else {
            this.addAliasedCode(ctx);
            ctx.pushCode("return " + (FUNCTION_PREFIX + this.alias) + "(0).result;");
        }
        return ctx;
    };
    Parser.prototype.getCode = function () {
        return this.getContext().code;
    };
    Parser.prototype.addRawCode = function (ctx) {
        ctx.pushCode('var offset = 0;');
        ctx.pushCode("var vars = " + (this.constructorFn ? 'new constructorFn()' : '{}') + ";");
        ctx.pushCode('vars.$parent = null;');
        ctx.pushCode('vars.$root = vars;');
        this.generate(ctx);
        this.resolveReferences(ctx);
        ctx.pushCode('delete vars.$parent;');
        ctx.pushCode('delete vars.$root;');
        ctx.pushCode('return vars;');
    };
    Parser.prototype.addAliasedCode = function (ctx) {
        ctx.pushCode("function " + (FUNCTION_PREFIX + this.alias) + "(offset, context) {");
        ctx.pushCode("var vars = " + (this.constructorFn ? 'new constructorFn()' : '{}') + ";");
        ctx.pushCode("var ctx = Object.assign({$parent: null, $root: vars}, context || {});");
        ctx.pushCode("vars = Object.assign(vars, ctx);");
        this.generate(ctx);
        ctx.markResolved(this.alias);
        this.resolveReferences(ctx);
        ctx.pushCode('Object.keys(ctx).forEach(function (item) { delete vars[item]; });');
        ctx.pushCode('return { offset: offset, result: vars };');
        ctx.pushCode('}');
        return ctx;
    };
    Parser.prototype.resolveReferences = function (ctx) {
        var references = ctx.getUnresolvedReferences();
        ctx.markRequested(references);
        references.forEach(function (alias) {
            var parser = aliasRegistry[alias];
            parser.addAliasedCode(ctx);
        });
    };
    Parser.prototype.compile = function () {
        var importPath = 'imports';
        var ctx = this.getContext(importPath);
        this.compiled = new Function(importPath, 'TextDecoder', "return function (buffer, constructorFn) { " + ctx.code + " };")(ctx.imports, TextDecoder);
    };
    Parser.prototype.sizeOf = function () {
        var size = NaN;
        if (Object.keys(PRIMITIVE_SIZES).indexOf(this.type) >= 0) {
            size = PRIMITIVE_SIZES[this.type];
            // if this is a fixed length string
        }
        else if (this.type === 'string' &&
            typeof this.options.length === 'number') {
            size = this.options.length;
            // if this is a fixed length buffer
        }
        else if (this.type === 'buffer' &&
            typeof this.options.length === 'number') {
            size = this.options.length;
            // if this is a fixed length array
        }
        else if (this.type === 'array' &&
            typeof this.options.length === 'number') {
            var elementSize = NaN;
            if (typeof this.options.type === 'string') {
                elementSize = PRIMITIVE_SIZES[this.options.type];
            }
            else if (this.options.type instanceof Parser) {
                elementSize = this.options.type.sizeOf();
            }
            size = this.options.length * elementSize;
            // if this a skip
        }
        else if (this.type === 'seek') {
            size = this.options.length;
            // if this is a nested parser
        }
        else if (this.type === 'nest') {
            size = this.options.type.sizeOf();
        }
        else if (!this.type) {
            size = 0;
        }
        if (this.next) {
            size += this.next.sizeOf();
        }
        return size;
    };
    // Follow the parser chain till the root and start parsing from there
    Parser.prototype.parse = function (buffer) {
        if (!this.compiled) {
            this.compile();
        }
        return this.compiled(buffer, this.constructorFn);
    };
    Parser.prototype.setNextParser = function (type, varName, options) {
        var parser = new Parser();
        parser.type = type;
        parser.varName = varName;
        parser.options = options || parser.options;
        parser.endian = this.endian;
        if (this.head) {
            this.head.next = parser;
        }
        else {
            this.next = parser;
        }
        this.head = parser;
        return this;
    };
    // Call code generator for this parser
    Parser.prototype.generate = function (ctx) {
        if (this.type) {
            switch (this.type) {
                case 'uint8':
                case 'uint16le':
                case 'uint16be':
                case 'uint32le':
                case 'uint32be':
                case 'int8':
                case 'int16le':
                case 'int16be':
                case 'int32le':
                case 'int32be':
                case 'int64be':
                case 'int64le':
                case 'uint64be':
                case 'uint64le':
                case 'floatle':
                case 'floatbe':
                case 'doublele':
                case 'doublebe':
                    this.primitiveGenerateN(this.type, ctx);
                    break;
                case 'bit':
                    this.generateBit(ctx);
                    break;
                case 'string':
                    this.generateString(ctx);
                    break;
                case 'buffer':
                    this.generateBuffer(ctx);
                    break;
                case 'seek':
                    this.generateSeek(ctx);
                    break;
                case 'nest':
                    this.generateNest(ctx);
                    break;
                case 'array':
                    this.generateArray(ctx);
                    break;
                case 'choice':
                    this.generateChoice(ctx);
                    break;
                case 'pointer':
                    this.generatePointer(ctx);
                    break;
                case 'saveOffset':
                    this.generateSaveOffset(ctx);
                    break;
                case 'wrapper':
                    this.generateWrapper(ctx);
                    break;
            }
            this.generateAssert(ctx);
        }
        var varName = ctx.generateVariable(this.varName);
        if (this.options.formatter) {
            this.generateFormatter(ctx, varName, this.options.formatter);
        }
        return this.generateNext(ctx);
    };
    Parser.prototype.generateAssert = function (ctx) {
        if (!this.options.assert) {
            return;
        }
        var varName = ctx.generateVariable(this.varName);
        switch (typeof this.options.assert) {
            case 'function':
                var func = ctx.addImport(this.options.assert);
                ctx.pushCode("if (!" + func + ".call(vars, " + varName + ")) {");
                break;
            case 'number':
                ctx.pushCode("if (" + this.options.assert + " !== " + varName + ") {");
                break;
            case 'string':
                ctx.pushCode("if (\"" + this.options.assert + "\" !== " + varName + ") {");
                break;
            default:
                throw new Error('Assert option supports only strings, numbers and assert functions.');
        }
        ctx.generateError("\"Assert error: " + varName + " is \" + " + this.options.assert);
        ctx.pushCode('}');
    };
    // Recursively call code generators and append results
    Parser.prototype.generateNext = function (ctx) {
        if (this.next) {
            ctx = this.next.generate(ctx);
        }
        return ctx;
    };
    Parser.prototype.generateBit = function (ctx) {
        // TODO find better method to handle nested bit fields
        var parser = JSON.parse(JSON.stringify(this));
        parser.varName = ctx.generateVariable(parser.varName);
        ctx.bitFields.push(parser);
        if (!this.next ||
            (this.next && ['bit', 'nest'].indexOf(this.next.type) < 0)) {
            var sum_1 = 0;
            ctx.bitFields.forEach(function (parser) { return (sum_1 += parser.options.length); });
            var val_1 = ctx.generateTmpVariable();
            if (sum_1 <= 8) {
                ctx.pushCode("var " + val_1 + " = dataView.getUint8(offset);");
                sum_1 = 8;
            }
            else if (sum_1 <= 16) {
                ctx.pushCode("var " + val_1 + " = dataView.getUint16(offset);");
                sum_1 = 16;
            }
            else if (sum_1 <= 24) {
                var val1 = ctx.generateTmpVariable();
                var val2 = ctx.generateTmpVariable();
                ctx.pushCode("var " + val1 + " = dataView.getUint16(offset);");
                ctx.pushCode("var " + val2 + " = dataView.getUint8(offset + 2);");
                ctx.pushCode("var " + val_1 + " = (" + val1 + " << 8) | " + val2 + ";");
                sum_1 = 24;
            }
            else if (sum_1 <= 32) {
                ctx.pushCode("var " + val_1 + " = dataView.getUint32(offset);");
                sum_1 = 32;
            }
            else {
                throw new Error('Currently, bit field sequence longer than 4-bytes is not supported.');
            }
            ctx.pushCode("offset += " + sum_1 / 8 + ";");
            var bitOffset_1 = 0;
            var isBigEndian_1 = this.endian === 'be';
            ctx.bitFields.forEach(function (parser) {
                var length = parser.options.length;
                var offset = isBigEndian_1 ? sum_1 - bitOffset_1 - length : bitOffset_1;
                var mask = (1 << length) - 1;
                ctx.pushCode(parser.varName + " = " + val_1 + " >> " + offset + " & " + mask + ";");
                bitOffset_1 += length;
            });
            ctx.bitFields = [];
        }
    };
    Parser.prototype.generateSeek = function (ctx) {
        var length = ctx.generateOption(this.options.length);
        ctx.pushCode("offset += " + length + ";");
    };
    Parser.prototype.generateString = function (ctx) {
        var name = ctx.generateVariable(this.varName);
        var start = ctx.generateTmpVariable();
        var encoding = this.options.encoding;
        var isHex = encoding.toLowerCase() === 'hex';
        var toHex = 'b => b.toString(16).padStart(2, "0")';
        if (this.options.length && this.options.zeroTerminated) {
            var len = this.options.length;
            ctx.pushCode("var " + start + " = offset;");
            ctx.pushCode("while(dataView.getUint8(offset++) !== 0 && offset - " + start + " < " + len + ");");
            var end = "offset - " + start + " < " + len + " ? offset - 1 : offset";
            ctx.pushCode(isHex
                ? name + " = Array.from(buffer.subarray(" + start + ", " + end + "), " + toHex + ").join('');"
                : name + " = new TextDecoder('" + encoding + "').decode(buffer.subarray(" + start + ", " + end + "));");
        }
        else if (this.options.length) {
            var len = ctx.generateOption(this.options.length);
            ctx.pushCode(isHex
                ? name + " = Array.from(buffer.subarray(offset, offset + " + len + "), " + toHex + ").join('');"
                : name + " = new TextDecoder('" + encoding + "').decode(buffer.subarray(offset, offset + " + len + "));");
            ctx.pushCode("offset += " + len + ";");
        }
        else if (this.options.zeroTerminated) {
            ctx.pushCode("var " + start + " = offset;");
            ctx.pushCode('while(dataView.getUint8(offset++) !== 0);');
            ctx.pushCode(isHex
                ? name + " = Array.from(buffer.subarray(" + start + ", offset - 1), " + toHex + ").join('');"
                : name + " = new TextDecoder('" + encoding + "').decode(buffer.subarray(" + start + ", offset - 1));");
        }
        else if (this.options.greedy) {
            ctx.pushCode("var " + start + " = offset;");
            ctx.pushCode('while(buffer.length > offset++);');
            ctx.pushCode(isHex
                ? name + " = Array.from(buffer.subarray(" + start + ", offset), " + toHex + ").join('');"
                : name + " = new TextDecoder('" + encoding + "').decode(buffer.subarray(" + start + ", offset));");
        }
        if (this.options.stripNull) {
            ctx.pushCode(name + " = " + name + ".replace(/\\x00+$/g, '')");
        }
    };
    Parser.prototype.generateBuffer = function (ctx) {
        var varName = ctx.generateVariable(this.varName);
        if (typeof this.options.readUntil === 'function') {
            var pred = this.options.readUntil;
            var start = ctx.generateTmpVariable();
            var cur = ctx.generateTmpVariable();
            ctx.pushCode("var " + start + " = offset;");
            ctx.pushCode("var " + cur + " = 0;");
            ctx.pushCode("while (offset < buffer.length) {");
            ctx.pushCode(cur + " = dataView.getUint8(offset);");
            var func = ctx.addImport(pred);
            ctx.pushCode("if (" + func + ".call(" + ctx.generateVariable() + ", " + cur + ", buffer.subarray(offset))) break;");
            ctx.pushCode("offset += 1;");
            ctx.pushCode("}");
            ctx.pushCode(varName + " = buffer.subarray(" + start + ", offset);");
        }
        else if (this.options.readUntil === 'eof') {
            ctx.pushCode(varName + " = buffer.subarray(offset);");
        }
        else {
            var len = ctx.generateOption(this.options.length);
            ctx.pushCode(varName + " = buffer.subarray(offset, offset + " + len + ");");
            ctx.pushCode("offset += " + len + ";");
        }
        if (this.options.clone) {
            ctx.pushCode(varName + " = buffer.constructor.from(" + varName + ");");
        }
    };
    Parser.prototype.generateArray = function (ctx) {
        var length = ctx.generateOption(this.options.length);
        var lengthInBytes = ctx.generateOption(this.options.lengthInBytes);
        var type = this.options.type;
        var counter = ctx.generateTmpVariable();
        var lhs = ctx.generateVariable(this.varName);
        var item = ctx.generateTmpVariable();
        var key = this.options.key;
        var isHash = typeof key === 'string';
        if (isHash) {
            ctx.pushCode(lhs + " = {};");
        }
        else {
            ctx.pushCode(lhs + " = [];");
        }
        if (typeof this.options.readUntil === 'function') {
            ctx.pushCode('do {');
        }
        else if (this.options.readUntil === 'eof') {
            ctx.pushCode("for (var " + counter + " = 0; offset < buffer.length; " + counter + "++) {");
        }
        else if (lengthInBytes !== undefined) {
            ctx.pushCode("for (var " + counter + " = offset + " + lengthInBytes + "; offset < " + counter + "; ) {");
        }
        else {
            ctx.pushCode("for (var " + counter + " = " + length + "; " + counter + " > 0; " + counter + "--) {");
        }
        if (typeof type === 'string') {
            if (!aliasRegistry[type]) {
                var typeName = PRIMITIVE_NAMES[type];
                var littleEndian = PRIMITIVE_LITTLE_ENDIANS[type];
                ctx.pushCode("var " + item + " = dataView.get" + typeName + "(offset, " + littleEndian + ");");
                ctx.pushCode("offset += " + PRIMITIVE_SIZES[type] + ";");
            }
            else {
                var parentVar = ctx.generateVariable();
                var tempVar = ctx.generateTmpVariable();
                ctx.pushCode("var " + tempVar + " = " + (FUNCTION_PREFIX + type) + "(offset, {");
                ctx.pushCode("$parent: " + parentVar + ",");
                ctx.pushCode("$root: " + parentVar + ".$root,");
                if (!this.options.readUntil && lengthInBytes === undefined) {
                    ctx.pushCode("$index: " + length + " - " + counter + ",");
                }
                ctx.pushCode("});");
                ctx.pushCode("var " + item + " = " + tempVar + ".result; offset = " + tempVar + ".offset;");
                if (type !== this.alias)
                    ctx.addReference(type);
            }
        }
        else if (type instanceof Parser) {
            var parentVar = ctx.generateVariable();
            ctx.pushCode("var " + item + " = {};");
            ctx.pushScope(item);
            ctx.pushCode(item + ".$parent = " + parentVar + ";");
            ctx.pushCode(item + ".$root = " + parentVar + ".$root;");
            if (!this.options.readUntil && lengthInBytes === undefined) {
                ctx.pushCode(item + ".$index = " + length + " - " + counter + ";");
            }
            type.generate(ctx);
            ctx.pushCode("delete " + item + ".$parent;");
            ctx.pushCode("delete " + item + ".$root;");
            ctx.pushCode("delete " + item + ".$index;");
            ctx.popScope();
        }
        if (isHash) {
            ctx.pushCode(lhs + "[" + item + "." + key + "] = " + item + ";");
        }
        else {
            ctx.pushCode(lhs + ".push(" + item + ");");
        }
        ctx.pushCode('}');
        if (typeof this.options.readUntil === 'function') {
            var pred = this.options.readUntil;
            var func = ctx.addImport(pred);
            ctx.pushCode("while (!" + func + ".call(" + ctx.generateVariable() + ", " + item + ", buffer.subarray(offset)));");
        }
    };
    Parser.prototype.generateChoiceCase = function (ctx, varName, type) {
        if (typeof type === 'string') {
            var varName_1 = ctx.generateVariable(this.varName);
            if (!aliasRegistry[type]) {
                var typeName = PRIMITIVE_NAMES[type];
                var littleEndian = PRIMITIVE_LITTLE_ENDIANS[type];
                ctx.pushCode(varName_1 + " = dataView.get" + typeName + "(offset, " + littleEndian + ");");
                ctx.pushCode("offset += " + PRIMITIVE_SIZES[type]);
            }
            else {
                var tempVar = ctx.generateTmpVariable();
                ctx.pushCode("var " + tempVar + " = " + (FUNCTION_PREFIX + type) + "(offset, {");
                ctx.pushCode("$parent: " + varName_1 + ".$parent,");
                ctx.pushCode("$root: " + varName_1 + ".$root,");
                ctx.pushCode("});");
                ctx.pushCode(varName_1 + " = " + tempVar + ".result; offset = " + tempVar + ".offset;");
                if (type !== this.alias)
                    ctx.addReference(type);
            }
        }
        else if (type instanceof Parser) {
            ctx.pushPath(varName);
            type.generate(ctx);
            ctx.popPath(varName);
        }
    };
    Parser.prototype.generateChoice = function (ctx) {
        var _this = this;
        var tag = ctx.generateOption(this.options.tag);
        var nestVar = ctx.generateVariable(this.varName);
        if (this.varName) {
            ctx.pushCode(nestVar + " = {};");
            var parentVar = ctx.generateVariable();
            ctx.pushCode(nestVar + ".$parent = " + parentVar + ";");
            ctx.pushCode(nestVar + ".$root = " + parentVar + ".$root;");
        }
        ctx.pushCode("switch(" + tag + ") {");
        Object.keys(this.options.choices).forEach(function (tag) {
            var type = _this.options.choices[parseInt(tag, 10)];
            ctx.pushCode("case " + tag + ":");
            _this.generateChoiceCase(ctx, _this.varName, type);
            ctx.pushCode('break;');
        });
        ctx.pushCode('default:');
        if (this.options.defaultChoice) {
            this.generateChoiceCase(ctx, this.varName, this.options.defaultChoice);
        }
        else {
            ctx.generateError("\"Met undefined tag value \" + " + tag + " + \" at choice\"");
        }
        ctx.pushCode('}');
        if (this.varName) {
            ctx.pushCode("delete " + nestVar + ".$parent;");
            ctx.pushCode("delete " + nestVar + ".$root;");
        }
    };
    Parser.prototype.generateNest = function (ctx) {
        var nestVar = ctx.generateVariable(this.varName);
        if (this.options.type instanceof Parser) {
            if (this.varName) {
                var parentVar = ctx.generateVariable();
                ctx.pushCode(nestVar + " = {};");
                ctx.pushCode(nestVar + ".$parent = " + parentVar + ";");
                ctx.pushCode(nestVar + ".$root = " + parentVar + ".$root;");
            }
            ctx.pushPath(this.varName);
            this.options.type.generate(ctx);
            ctx.popPath(this.varName);
            if (this.varName) {
                ctx.pushCode("delete " + nestVar + ".$parent;");
                ctx.pushCode("delete " + nestVar + ".$root;");
            }
        }
        else if (aliasRegistry[this.options.type]) {
            var parentVar = ctx.generateVariable();
            var tempVar = ctx.generateTmpVariable();
            ctx.pushCode("var " + tempVar + " = " + (FUNCTION_PREFIX + this.options.type) + "(offset, {");
            ctx.pushCode("$parent: " + parentVar + ",");
            ctx.pushCode("$root: " + parentVar + ".$root,");
            ctx.pushCode("});");
            ctx.pushCode(nestVar + " = " + tempVar + ".result; offset = " + tempVar + ".offset;");
            if (this.options.type !== this.alias)
                ctx.addReference(this.options.type);
        }
    };
    Parser.prototype.generateWrapper = function (ctx) {
        var wrapperVar = ctx.generateVariable(this.varName);
        var wrappedBuf = ctx.generateTmpVariable();
        if (typeof this.options.readUntil === 'function') {
            var pred = this.options.readUntil;
            var start = ctx.generateTmpVariable();
            var cur = ctx.generateTmpVariable();
            ctx.pushCode("var " + start + " = offset;");
            ctx.pushCode("var " + cur + " = 0;");
            ctx.pushCode("while (offset < buffer.length) {");
            ctx.pushCode(cur + " = dataView.getUint8(offset);");
            var func_1 = ctx.addImport(pred);
            ctx.pushCode("if (" + func_1 + ".call(" + ctx.generateVariable() + ", " + cur + ", buffer.subarray(offset))) break;");
            ctx.pushCode("offset += 1;");
            ctx.pushCode("}");
            ctx.pushCode(wrappedBuf + " = buffer.subarray(" + start + ", offset);");
        }
        else if (this.options.readUntil === 'eof') {
            ctx.pushCode(wrappedBuf + " = buffer.subarray(offset);");
        }
        else {
            var len = ctx.generateOption(this.options.length);
            ctx.pushCode(wrappedBuf + " = buffer.subarray(offset, offset + " + len + ");");
            ctx.pushCode("offset += " + len + ";");
        }
        if (this.options.clone) {
            ctx.pushCode(wrappedBuf + " = buffer.constructor.from(" + wrappedBuf + ");");
        }
        var tempBuf = ctx.generateTmpVariable();
        var tempOff = ctx.generateTmpVariable();
        var tempView = ctx.generateTmpVariable();
        var func = ctx.addImport(this.options.wrapper);
        ctx.pushCode(wrappedBuf + " = " + func + ".call(this, " + wrappedBuf + ").subarray(0);");
        ctx.pushCode("var " + tempBuf + " = buffer;");
        ctx.pushCode("var " + tempOff + " = offset;");
        ctx.pushCode("var " + tempView + " = dataView;");
        ctx.pushCode("buffer = " + wrappedBuf + ";");
        ctx.pushCode("offset = 0;");
        ctx.pushCode("dataView = new DataView(buffer.buffer, buffer.byteOffset, buffer.length);");
        if (this.options.type instanceof Parser) {
            if (this.varName) {
                ctx.pushCode(wrapperVar + " = {};");
            }
            ctx.pushPath(this.varName);
            this.options.type.generate(ctx);
            ctx.popPath(this.varName);
        }
        else if (aliasRegistry[this.options.type]) {
            var tempVar = ctx.generateTmpVariable();
            ctx.pushCode("var " + tempVar + " = " + (FUNCTION_PREFIX + this.options.type) + "(0);");
            ctx.pushCode(wrapperVar + " = " + tempVar + ".result;");
            if (this.options.type !== this.alias)
                ctx.addReference(this.options.type);
        }
        ctx.pushCode("buffer = " + tempBuf + ";");
        ctx.pushCode("dataView = " + tempView + ";");
        ctx.pushCode("offset = " + tempOff + ";");
    };
    Parser.prototype.generateFormatter = function (ctx, varName, formatter) {
        if (typeof formatter === 'function') {
            var func = ctx.addImport(formatter);
            ctx.pushCode(varName + " = " + func + ".call(" + ctx.generateVariable() + ", " + varName + ");");
        }
    };
    Parser.prototype.generatePointer = function (ctx) {
        var type = this.options.type;
        var offset = ctx.generateOption(this.options.offset);
        var tempVar = ctx.generateTmpVariable();
        var nestVar = ctx.generateVariable(this.varName);
        // Save current offset
        ctx.pushCode("var " + tempVar + " = offset;");
        // Move offset
        ctx.pushCode("offset = " + offset + ";");
        if (this.options.type instanceof Parser) {
            var parentVar = ctx.generateVariable();
            ctx.pushCode(nestVar + " = {};");
            ctx.pushCode(nestVar + ".$parent = " + parentVar + ";");
            ctx.pushCode(nestVar + ".$root = " + parentVar + ".$root;");
            ctx.pushPath(this.varName);
            this.options.type.generate(ctx);
            ctx.popPath(this.varName);
            ctx.pushCode("delete " + nestVar + ".$parent;");
            ctx.pushCode("delete " + nestVar + ".$root;");
        }
        else if (aliasRegistry[this.options.type]) {
            var parentVar = ctx.generateVariable();
            var tempVar_1 = ctx.generateTmpVariable();
            ctx.pushCode("var " + tempVar_1 + " = " + (FUNCTION_PREFIX + this.options.type) + "(offset, {");
            ctx.pushCode("$parent: " + parentVar + ",");
            ctx.pushCode("$root: " + parentVar + ".$root,");
            ctx.pushCode("});");
            ctx.pushCode(nestVar + " = " + tempVar_1 + ".result; offset = " + tempVar_1 + ".offset;");
            if (this.options.type !== this.alias)
                ctx.addReference(this.options.type);
        }
        else if (Object.keys(PRIMITIVE_SIZES).indexOf(this.options.type) >= 0) {
            var typeName = PRIMITIVE_NAMES[type];
            var littleEndian = PRIMITIVE_LITTLE_ENDIANS[type];
            ctx.pushCode(nestVar + " = dataView.get" + typeName + "(offset, " + littleEndian + ");");
            ctx.pushCode("offset += " + PRIMITIVE_SIZES[type] + ";");
        }
        // Restore offset
        ctx.pushCode("offset = " + tempVar + ";");
    };
    Parser.prototype.generateSaveOffset = function (ctx) {
        var varName = ctx.generateVariable(this.varName);
        ctx.pushCode(varName + " = offset");
    };
    return Parser;
}());

