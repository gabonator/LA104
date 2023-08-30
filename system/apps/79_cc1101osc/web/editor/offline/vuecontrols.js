var VueNumControl = {
  props: ['readonly', 'defaultVal', 'emitter', 'ikey', 'getData', 'putData', 'isVisible', 'title'],
  template: '<div v-if="isVisible" style="font-family: sans-serif; font-size:14px; color:#444;">{{title}}<br><input type="number" :readonly="readonly" :value="value" @input="change($event)" @dblclick.stop="" @pointerdown.stop="" @pointermove.stop=""/></div>',
  data() {
    return {
      value: 0,
      isVisible: true,
      title: ""
    }
  },
  methods: {
    change(e){
      this.value = +e.target.value;
      this.update();
    },
    update() {
      if (this.ikey)
        this.putData(this.ikey, this.value)
      this.emitter.trigger('process');
    }
  },
  mounted() {
    this.value = this.getData(this.ikey);;
    this.update();
  }
}

var VueStaticControl = {
  props: ['readonly', 'emitter', 'ikey', 'getData', 'putData'],
  template: '<div style="font-family: sans-serif; font-size:14px; color:#444;" v-html="value"></div>',
  data() {
    return {
      value: ""
    }
  },
  methods: {
  },
  mounted() {
    this.value = this.getData(this.ikey);
  }
}

var VueStrControl = {
  props: ['readonly', /*'defaultVal',*/ 'emitter', 'ikey', 'getData', 'putData', 'title'],
  template: '<div style="font-family: sans-serif; font-size:14px; color:#444;">{{title}}<br><input :readonly="readonly" :value="value" @input="change($event)" @dblclick.stop="" @pointerdown.stop="" @pointermove.stop=""/></div>',
  data() {
    return {
      value: "",
      title: ""
    }
  },
  methods: {
    change(e){
      this.value = e.target.value;
      this.update();
    },
    update() {
      if (this.ikey)
        this.putData(this.ikey, this.value)
      this.emitter.trigger('process');
    }
  },
  mounted() {
    let val = this.getData(this.ikey);
    this.value = val;// val === undefined ? this.defaultVal : val;
    this.update();
  }
}

var VueStrDoubleControl = {
  props: ['readonly', /*'defaultVal',*/ 'emitter', 'ikey', 'getData', 'putData', 'isVisible', 'width1', 'width2', 'disabled1', 'disabled2'],
  template: '<div v-if="isVisible"><input :style="{width:width1}" :readonly="readonly" :disabled="disabled1" :value="value1" @input="change1($event)" @dblclick.stop="" @pointerdown.stop="" @pointermove.stop=""/><input :style="{width:width2}" :readonly="readonly" :disabled="disabled2" :value="value2" @input="change2($event)" @dblclick.stop="" @pointerdown.stop="" @pointermove.stop=""/></div>',
  data() {
    return {
      width1: "120px",
      width2: "60px",
      isVisible: true,
      value1: "",
      value2: "",
      disabled1: false,
      disabled2: false,
    }
  },
  methods: {
    change1(e){
      this.value1 = e.target.value;
      this.update();
    },
    change2(e){
      this.value2 = e.target.value;
      this.update();
    },
    update() {
      if (this.ikey)
        this.putData(this.ikey, [this.value1, this.value2])
      this.emitter.trigger('process');
    }
  },
  mounted() {
    let val = this.getData(this.ikey);
    if (val && val.length >= 2)
    {
      this.value1 = val[0];
      this.value2 = val[1];
    }
    this.update();
  }
}

var VueCheckControl = {
  props: ['readonly', 'title', 'label', 'emitter', 'ikey', 'getData', 'putData'],
  template: '<div style="font-family: sans-serif; font-size:14px; color:#444;"><input style="width:32px" type="checkbox" :checked="value" :readonly="readonly" @input="change($event)" @dblclick.stop="" @pointerdown.stop="" @pointermove.stop=""/>{{title}}</div>',
  data() {
    return {
      value: false,
      title: "",
    }
  },
  methods: {
    change(e){
      this.value = e.target.checked;
      this.update();
    },
    update() {
      if (this.	ikey)
        this.putData(this.ikey, this.value)
      this.emitter.trigger('process');
    }
  },
  mounted() {
    this.value = this.getData(this.ikey);
    this.update();
  }
}

var VueOptionControl = {
  props: ['readonly', 'emitter', 'ikey', 'getData', 'putData'],
  template: `<div>
    <div style="font-family: sans-serif; font-size:14px; color:#444;">{{title}}</div>
    <label v-for="(val, key) in options" :key="val" style="font-family: sans-serif; font-size:14px; color:#444;">
      <input style="width:24px" type="radio" 
        :name="name" 
        :value="val" 
        :checked="val == value" 
        @input="change($event)" @dblclick.stop="" @pointerdown.stop="" @pointermove.stop=""> 
      {{ key }}<br>
    </label>
  </div>`,
  data() {
    return {
      value: 0,
      options: {},
      title: ""
    }
  },
  methods: {
    change(e){
      this.value = e.target.value;
      this.update();
    },
    update() {
      if (this.ikey)
        this.putData(this.ikey, {value:this.value, options:this.options, title:this.title})
      this.emitter.trigger('process');
    }
  },
  mounted() {
    let val = this.getData(this.ikey);
    this.options = val.options;
    this.value = val.value;
    this.title = val.title;
    this.update();
  }
}

var VueTextControl = {
  props: ['readonly', 'emitter', 'ikey', 'getData', 'putData'],
  template: '<textarea style="width:100%; height:80px" :readonly="readonly" :value="value" @input="change($event)" @dblclick.stop="" @pointerdown.stop="" @pointermove.stop=""/>',
  data() {
    return {
      value: "init text",
    }
  },
  methods: {
    change(e){
      this.value = e.target.value;
      this.update();
    },
    update() {
      if (this.ikey)
        this.putData(this.ikey, this.value)
      this.emitter.trigger('process');
    }
  },
  mounted() {
    this.value = this.getData(this.ikey);
    this.update();
  }
}
