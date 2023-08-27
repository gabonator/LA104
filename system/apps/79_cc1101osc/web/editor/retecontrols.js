class StaticControl extends Rete.Control {
  constructor(emitter, key, value) {
    super(key);
    this.component = VueStaticControl;
    this.props = { emitter, ikey: key };
  }
  setValue(val) {
    this.vueContext.value = val;
  }
}

class NumControl extends Rete.Control {
  constructor(emitter, key, readonly, title) {
    super(key);
    var isVisible = true;
    this.component = VueNumControl;
    this.props = { emitter, ikey: key, readonly, isVisible, title };
  }
  setValue(val) {
    this.vueContext.value = val;
  }
  setVisible(vis)
  {
    this.vueContext.isVisible = vis;
  }
}

class StrControl extends Rete.Control {
  constructor(emitter, key, readonly, title) {
    super(key);
    this.component = VueStrControl;
    this.props = { emitter, ikey: key, readonly, title };
  }

  setValue(val) {
    this.vueContext.value = val;
  }
  setVisible(vis)
  {
    this.vueContext.isVisible = vis;
  }
}

class StrDoubleControl extends Rete.Control {
  constructor(emitter, key, attrs) {
    super(key);
    this.component = VueStrDoubleControl;
    this.props = { emitter, ikey: key, readonly:false, isVisible: true, ...attrs };
  }
  setValue(val) {
    this.vueContext.value = val;
  }
  setVisible(vis) {
    this.vueContext.isVisible = vis;
  }
}

class CheckControl extends Rete.Control {
  constructor(emitter, key, readonly, title) {
    super(key);
    this.component = VueCheckControl;
    this.props = { emitter, ikey: key, readonly, title };
  }
  setValue(val) {
    this.vueContext.value = val;
  }
  setVisible(vis)
  {
    this.vueContext.isVisible = vis;
  }
}

class OptionControl extends Rete.Control {
  constructor(emitter, key, readonly, options) {
    super(key);
    this.component = VueOptionControl;
    this.props = { emitter, ikey: key, readonly, options };
  }
  setValue(val) {
    this.vueContext.value = val;
  }
  setVisible(vis)
  {
    this.vueContext.isVisible = vis;
  }
}

class TextControl extends Rete.Control {

  constructor(emitter, key, readonly) {
    super(key);
    this.component = VueTextControl;
    this.props = { emitter, ikey: key, readonly};
  }

  setValue(val) {
    this.vueContext.value = val;
  }
}
