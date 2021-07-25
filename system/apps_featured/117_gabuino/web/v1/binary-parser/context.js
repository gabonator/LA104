"use strict";
//Object.defineProperty(exports, "__esModule", { value: true });
//exports.Context = void 0;
var Context = /** @class */ (function () {
    function Context(importPath) {
        this.code = '';
        this.scopes = [['vars']];
        this.bitFields = [];
        this.tmpVariableCount = 0;
        this.references = {};
        this.imports = [];
        this.reverseImports = new Map();
        this.importPath = importPath;
    }
    Context.prototype.generateVariable = function (name) {
        var arr = [];
        var scopes = this.scopes[this.scopes.length - 1];
        arr.push.apply(arr, scopes);
        if (name) {
            arr.push(name);
        }
        return arr.join('.');
    };
    Context.prototype.generateOption = function (val) {
        switch (typeof val) {
            case 'number':
                return val.toString();
            case 'string':
                return this.generateVariable(val);
            case 'function':
                return this.addImport(val) + ".call(" + this.generateVariable() + ", vars)";
        }
    };
    Context.prototype.generateError = function (err) {
        this.pushCode('throw new Error(' + err + ');');
    };
    Context.prototype.generateTmpVariable = function () {
        return '$tmp' + this.tmpVariableCount++;
    };
    Context.prototype.pushCode = function (code) {
        this.code += code + '\n';
    };
    Context.prototype.pushPath = function (name) {
        if (name) {
            this.scopes[this.scopes.length - 1].push(name);
        }
    };
    Context.prototype.popPath = function (name) {
        if (name) {
            this.scopes[this.scopes.length - 1].pop();
        }
    };
    Context.prototype.pushScope = function (name) {
        this.scopes.push([name]);
    };
    Context.prototype.popScope = function () {
        this.scopes.pop();
    };
    Context.prototype.addImport = function (im) {
        if (!this.importPath)
            return "(" + im + ")";
        var id = this.reverseImports.get(im);
        if (!id) {
            id = this.imports.push(im) - 1;
            this.reverseImports.set(im, id);
        }
        return this.importPath + "[" + id + "]";
    };
    Context.prototype.addReference = function (alias) {
        if (this.references[alias])
            return;
        this.references[alias] = { resolved: false, requested: false };
    };
    Context.prototype.markResolved = function (alias) {
        this.references[alias].resolved = true;
    };
    Context.prototype.markRequested = function (aliasList) {
        var _this = this;
        aliasList.forEach(function (alias) {
            _this.references[alias].requested = true;
        });
    };
    Context.prototype.getUnresolvedReferences = function () {
        var references = this.references;
        return Object.keys(this.references).filter(function (alias) { return !references[alias].resolved && !references[alias].requested; });
    };
    return Context;
}());

var context_1 = {Context:Context};
