console.log("hello!");

document.getElementById("mainConsole").innerHTML = "This is a test";

var actx = new AudioContext();
window.onclick = function() {
  actx.resume();
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
    var tempOsc = actx.createOscillator();
    tempOsc.type = "sawtooth";
    tempOsc.frequency.value = 110;
    tempOsc.connect(actx.destination);
    tempOsc.start();
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
      o.frequency.value = 110 + (i * 30);
      o.start();
    }
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
  connect(outParam, nodeSet, inParam) {
    for(var i=0; i<polyphony; i++) {
      this.nodes[i][outParam].connect(nodeSet.nodes[i][inParam]);
    }
  }
}
