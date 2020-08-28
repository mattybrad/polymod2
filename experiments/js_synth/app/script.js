document.getElementById("mainConsole").innerHTML = "This is a test";

var actx = new AudioContext();
console.log(actx.state);
window.onclick = function() {
  actx.resume();
  console.log("resume");
}
var polyphony = 5;
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
    case "connect":
    var sourceModuleNum = parseInt(splitMsg[1]);
    var sourceSocketNum = parseInt(splitMsg[2]);
    var destModuleNum = parseInt(splitMsg[3]);
    var destSocketNum = parseInt(splitMsg[4]);
    break;
  }
}

function calculatePolyStatus() {
  var checkNum = 0;
  function checkSet(set) {
    //console.log("checking...");
    var prevConfirmed = set.confirmed;
    set.checkNum ++;
    if(set.hardcodedPoly) {
  		set.poly = true;
  		set.confirmed = true;
  	}
  	var allMono = true;
  	var allInputsConfirmed = true;
  	for(var i=0;i<set.inputs.length;i++) {
  		var inputSet = set.inputs[i];
  		if(inputSet.checkNum == checkNum && inputSet != set) checkSet(inputSet);
  		if(inputSet.poly) allMono = false;
  		if(!inputSet.confirmed) allInputsConfirmed = false;
  	}
  	if(!allMono) {
  		set.poly = true;
  		set.confirmed = true;
  	}
  	if(allInputsConfirmed) {
      set.confirmed = true;
    }
    if(!prevConfirmed && set.confirmed) {
      console.log(set.poly ? "poly" : "mono");
    }
  }

  function resetSet(set) {
    set.checkNum = 0;
    set.poly = false;
    set.confirmed = false;
    for(var i=0;i<set.inputs.length;i++) {
  		var inputSet = set.inputs[i];
      if(inputSet.confirmed) resetSet(inputSet);
    }
  }

  var masterModule;
  for(var i=0; i<moduleSlots.length; i++) {
    if(moduleSlots[i].moduleType == "master") masterModule = moduleSlots[i];
  }
  if(masterModule) {
    console.log("master module found");
    resetSet(masterModule.socketInputs[0].nodeSet);
    while(checkNum<2) {
      checkSet(masterModule.socketInputs[0].nodeSet);
      checkNum ++;
    }
  } else {
    console.log("no master module found");
  }
  console.log("done calculating poly status");
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
    this.moduleType = "master";
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
    this.moduleType = "vco";
    this.analogInputs[0] = new AnalogInput("tuning");
    this.socketInputs[0] = new SocketInput("freq cv");
    this.socketOutputs[0] = new SocketOutput("saw out");
    var oscSawSet = new NodeSet();
    var oscSawFreqSet = new NodeSet();
    for(var i=0; i<polyphony; i++) {
      var o = oscSawSet.nodes[i] = actx.createOscillator();
      oscSawFreqSet.nodes[i] = o.frequency;
      o.type = "sawtooth";
      o.frequency.value = 110;
      o.start();
    }
    oscSawSet.connect(this.socketOutputs[0].nodeSet);
    oscSawSet.inputs.push(oscSawFreqSet);
    this.socketInputs[0].nodeSet.connect(oscSawFreqSet);
  }
}

class ModuleRandomPolySource extends Module {
  constructor() {
    super();
    this.moduleType = "rps";
    this.socketOutputs[0] = new SocketOutput("random poly out");
    var dcSet = new NodeSet();
    dcSet.hardcodedPoly = true;
    for(var i=0; i<polyphony; i++) {
      var c = dcSet.nodes[i] = actx.createConstantSource();
      c.start();
      c.offset.value = 400+10*Math.random();
    }
    dcSet.connect(this.socketOutputs[0].nodeSet);
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
    this.inputs = [];
    this.hardcodedPoly = false;
    this.poly = false;
    this.checkNum = 0;
    this.confirmed = false;
  }
  connect(nodeSet) {
    nodeSet.inputs.push(this);
    for(var i=0; i<polyphony; i++) {
      this.nodes[i].connect(nodeSet.nodes[i]);
    }
    //calculatePolyStatus();
  }
  disconnect(nodeSet) {
    for(var i=0; i<nodeSet.inputs.length; i++) {
      if(nodeSet.inputs[i]==this) {
        nodeSet.inputs.splice(i, 1);
        i--;
      }
    }
    for(var i=0; i<polyphony; i++) {
      this.nodes[i].disconnect(nodeSet.nodes[i]);
    }
    //calculatePolyStatus();
  }
}

var moduleTypes = {
  master: ModuleMaster,
  vco: ModuleVCO,
  rps: ModuleRandomPolySource
}

sendMessage("/addmodule/master/0");
sendMessage("/addmodule/vco/1");
sendMessage("/addmodule/rps/2");
sendMessage("/addmodule/vco/3");

moduleSlots[1].socketOutputs[0].nodeSet.connect(moduleSlots[0].socketInputs[0].nodeSet);
moduleSlots[2].socketOutputs[0].nodeSet.connect(moduleSlots[1].socketInputs[0].nodeSet);
moduleSlots[1].socketOutputs[0].nodeSet.connect(moduleSlots[1].socketInputs[0].nodeSet);
moduleSlots[3].socketOutputs[0].nodeSet.connect(moduleSlots[0].socketInputs[0].nodeSet);
moduleSlots[3].socketOutputs[0].nodeSet.connect(moduleSlots[3].socketInputs[0].nodeSet);
