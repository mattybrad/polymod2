console.log("hello!");

document.getElementById("mainConsole").innerHTML = "This is a test";

var actx = new AudioContext();
console.log(actx.state);
window.onclick = function() {
  actx.resume();
  console.log("resume");
}
var polyphony = 3;
var moduleSlots = [];

function sendMessage(msg) {
  var splitMsg = msg.split("/");
  splitMsg.shift();
  var isValid = false;
  var command = splitMsg[0];
  switch(command) {
    case "addmodule":
    var moduleType = splitMsg[1];
    if(moduleTypes.hasOwnProperty(moduleType)) {
      var moduleNum = parseInt(splitMsg[2]);
      moduleSlots[moduleNum] = new moduleTypes[moduleType];
    }
    break;
  }
}

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
    this.socketInputs[0].nodeSet.connect(masterGainSet);
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
    oscSawSet.connect(this.socketOutputs[0].nodeSet);
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
    this.nodeSet = new NodeSet();
    for(var i=0; i<polyphony; i++) {
      var g = this.nodeSet.nodes[i] = actx.createGain();
    }
  }
}

class SocketOutput {
  constructor(label) {
    this.label = label;
    this.nodeSet = new NodeSet();
    for(var i=0; i<polyphony; i++) {
      var g = this.nodeSet.nodes[i] = actx.createGain();
    }
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

var moduleTypes = {
  master: ModuleMaster,
  vco: ModuleVCO
}

sendMessage("/addmodule/master/0");
sendMessage("/addmodule/vco/1");
