console.log("hello!");

document.getElementById("mainConsole").innerHTML = "This is a test";

var actx = new AudioContext();
console.log(actx.state);
window.onclick = function() {
  actx.resume();
  console.log("resume");
}
var polyphony = 3;

class Module {
  constructor() {
    this.analogInputs = [];
    this.socketInputs = [];
    this.socketOutputs = [];
  }
}

class ModuleMaster extends Module {
  constructor() {
    super();
    this.socketInputs[0] = new SocketInput("main");
    var masterGainSet = new NodeSet();
    for(var i=0; i<polyphony; i++) {
      var g = masterGainSet.nodes[i] = actx.createGain();
      g.connect(actx.destination);
    }
    masterGainSet.nodes[0].connect(actx.destination);
    this.testSet = masterGainSet;
  }
}

class ModuleVCO extends Module {
  constructor() {
    super();
    this.analogInputs[0] = new AnalogInput("tuning");
    this.socketInputs[0] = new SocketInput("freq cv");
    this.socketOutputs[0] = new SocketOutput("saw out");
    var oscSawSet = new NodeSet();
    for(var i=0; i<polyphony; i++) {
      var o = oscSawSet.nodes[i] = actx.createOscillator();
      o.type = "sawtooth";
      o.frequency.value = 110 + (i * 30 + 50 * Math.random());
      o.start();
    }
    this.testSet = oscSawSet;
  }
}

class AnalogInput {
  constructor(label) {
    this.label = label;
  }
}

class SocketInput {
  constructor(label) {
    this.label = label;
  }
}

class SocketOutput {
  constructor(label) {
    this.label = label;
  }
}

class NodeSet {
  constructor() {
    this.nodes = [];
  }
  connect(nodeSet) {
    for(var i=0; i<polyphony; i++) {
      this.nodes[i].connect(nodeSet.nodes[i]);
    }
  }
  disconnect(nodeSet) {
    for(var i=0; i<polyphony; i++) {
      this.nodes[i].disconnect(nodeSet.nodes[i]);
    }
  }
}

var v = new ModuleVCO();
var m = new ModuleMaster();
