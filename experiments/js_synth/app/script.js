navigator.requestMIDIAccess({sysex:true})
    .then(onMIDISuccess, onMIDIFailure);

function onMIDISuccess(midiAccess) {
    console.log(midiAccess);

    var inputs = midiAccess.inputs;
    var outputs = midiAccess.outputs;
}

function onMIDIFailure() {
    console.log('Could not access your MIDI devices.');
}

var actx = new AudioContext();
console.log(actx.state);
window.onclick = function() {
  actx.resume();
  console.log("resume");
}
var polyphony = 2;
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
      var m = moduleSlots[moduleNum] = new moduleTypes[moduleType];
      var element = m.createDomElement();
      element.id = "module" + moduleNum;
      document.body.appendChild(element);
    }
    break;
    case "removemodule":
    var moduleNum = parseInt(splitMsg[1]);
    if(moduleSlots[moduleNum]) {
      console.log("attempt to destroy module");
      moduleSlots[moduleNum].destroy();
      moduleSlots[moduleNum] = null;
      calculatePolyStatus();
      var element = document.getElementById("module"+moduleNum);
      document.body.removeChild(element);
    }
    break;
    case "connect":
    var sourceModuleNum = parseInt(splitMsg[1]);
    var sourceSocketNum = parseInt(splitMsg[2]);
    var destModuleNum = parseInt(splitMsg[3]);
    var destSocketNum = parseInt(splitMsg[4]);
    moduleSlots[sourceModuleNum].socketOutputs[sourceSocketNum].nodeSet.connect(moduleSlots[destModuleNum].socketInputs[destSocketNum].nodeSet);
    break;
    case "disconnect":
    var sourceModuleNum = parseInt(splitMsg[1]);
    var sourceSocketNum = parseInt(splitMsg[2]);
    var destModuleNum = parseInt(splitMsg[3]);
    var destSocketNum = parseInt(splitMsg[4]);
    moduleSlots[sourceModuleNum].socketOutputs[sourceSocketNum].nodeSet.disconnect(moduleSlots[destModuleNum].socketInputs[destSocketNum].nodeSet);
    break;
    case "analog":
    var moduleNum = parseInt(splitMsg[1]);
    var inputNum = parseInt(splitMsg[2]);
    var analogValue = parseFloat(splitMsg[3]);
    moduleSlots[moduleNum].analogInputs[inputNum].update(analogValue);
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
      if(set.socket) set.socket.update();
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
    if(moduleSlots[i] && moduleSlots[i].moduleType == "master") masterModule = moduleSlots[i];
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
  destroy() {
    for(var i=0; i<this.socketInputs.length; i++) {
      this.socketInputs[i].nodeSet.destroy();
    }
    for(var i=0; i<this.socketOutputs.length; i++) {
      this.socketOutputs[i].nodeSet.destroy();
    }
  }
  createDomElement() {
    var outer = document.createElement("div");
    outer.classList.add("module");
    outer.innerHTML = this.moduleType;
    var siList = document.createElement("ul");
    siList.classList.add("socketInputs");
    outer.appendChild(siList);
    var soList = document.createElement("ul");
    soList.classList.add("socketOutputs");
    outer.appendChild(soList);
    for(var i=0; i<this.socketInputs.length; i++) {
      var s = document.createElement("li");
      s.innerHTML = this.socketInputs[i].label;
      siList.appendChild(s);
    }
    for(var i=0; i<this.socketOutputs.length; i++) {
      var s = document.createElement("li");
      s.innerHTML = this.socketOutputs[i].label;
      siList.appendChild(s);
    }
    return outer;
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
    this.analogInputs[1] = new AnalogInput("tuning 2");
    this.socketInputs[0] = new SocketInput("freq cv");
    this.socketOutputs[0] = new SocketOutput("saw out");
    var oscSawSet = new NodeSet();
    var oscSawFreqGain = new NodeSet();
    var oscSawFreqSet = new NodeSet();
    var wsAtten = new NodeSet();
    var waveshapers = new NodeSet();
    var numSamples = 1000;
    var curve = new Float32Array(numSamples);
    for(var i=0; i<numSamples; i++) {
      var x = i * 2 / numSamples - 1;
      curve[i] = Math.pow(2,x*100-3)/100;
      //console.log(i,x,curve[i]);
    }
    //console.log(curve);
    //console.log(curve[54]);
    for(var i=0; i<polyphony; i++) {
      var o = oscSawSet.nodes[i] = actx.createOscillator();
      oscSawFreqSet.nodes[i] = o.frequency;
      var fg = oscSawFreqGain.nodes[i] = actx.createGain();
      fg.gain.value = 440 * 100;
      var wa = wsAtten.nodes[i] = actx.createGain();
      wa.gain.value = 0.1;
      var w = waveshapers.nodes[i] = actx.createWaveShaper();
      w.curve = curve;
      o.type = "sawtooth";
      o.frequency.value = 0;
      o.start();
    }
    oscSawSet.connect(this.socketOutputs[0].nodeSet);
    oscSawSet.inputs.push(oscSawFreqSet);
    this.socketInputs[0].nodeSet.connect(wsAtten);
    wsAtten.connect(waveshapers);
    waveshapers.connect(oscSawFreqGain);
    oscSawFreqGain.connect(oscSawFreqSet);
    this.analogInputs[0].nodeSet.connect(wsAtten);
    this.analogInputs[1].nodeSet.connect(wsAtten);
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
      c.offset.value = -0.1+0.2*Math.random();
    }
    dcSet.connect(this.socketOutputs[0].nodeSet);
  }
}

class AnalogInput {
  constructor(label) {
    this.label = label;
    this.value = 0;
    this.nodeSet = new NodeSet();
    this.constantSource = actx.createConstantSource();
    this.constantSource.start();
    this.update(this.value);
    for(var i=0; i<polyphony; i++) {
      var c = this.nodeSet.nodes[i] = this.constantSource;
    }
  }
  update(value) {
    this.value = value;
    this.constantSource.offset.value = this.value;
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
    this.nodeSet.socket = this;
    for(var i=0; i<polyphony; i++) {
      var g = this.nodeSet.nodes[i] = actx.createGain();
    }
  }
  update() {
    var poly = this.nodeSet.poly;
    for(var i=0; i<polyphony; i++) {
      this.nodeSet.nodes[i].gain.value = (poly || i==0) ? 1 : 0;
    }
  }
}

class NodeSet {
  constructor() {
    this.nodes = [];
    this.inputs = [];
    this.outputs = [];
    this.hardcodedPoly = false;
    this.poly = false;
    this.checkNum = 0;
    this.confirmed = false;
    this.socket = null;
  }
  connect(nodeSet) {
    var found = false;
    for(var i=0; i<nodeSet.inputs.length; i++) {
      if(nodeSet.inputs[i]==this) {
        found = true;
      }
    }
    if(!found) {
      nodeSet.inputs.push(this);
      this.outputs.push(nodeSet);
      for(var i=0; i<polyphony; i++) {
        this.nodes[i].connect(nodeSet.nodes[i]);
      }
      calculatePolyStatus();
    }
  }
  disconnect(nodeSet) {
    var found = false;
    for(var i=0; i<nodeSet.inputs.length; i++) {
      if(nodeSet.inputs[i]==this) {
        found = true;
        nodeSet.inputs.splice(i, 1);
        i--;
      }
    }
    for(var i=0; i<polyphony && found; i++) {
      this.nodes[i].disconnect(nodeSet.nodes[i]);
    }
    for(var i=0; i<this.outputs.length; i++) {
      if(this.outputs[i]==nodeSet) {
        this.outputs.splice(i, 1);
        i--;
      }
    }
    calculatePolyStatus();
  }
  destroy(nodeSet) {
    var safety = 0;
    while(this.inputs.length > 0 && safety < 100) {
      this.inputs[0].disconnect(this);
      safety ++;
      console.log(1, safety);
    }
    safety = 0;
    while(this.inputs.length > 0 && safety < 100) {
      this.disconnect(this.inputs[0]);
      safety ++;
      console.log(2, safety);
    }
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
sendMessage("/connect/1/0/0/0");
//sendMessage("/connect/3/0/0/0");
//sendMessage("/connect/3/0/1/0");
//sendMessage("/connect/2/0/1/0");
var octave = 0;
setInterval(function(){
  var a = moduleSlots[1].analogInputs[0];
  a.update(octave/10);
  //console.log(octave);
  octave += 1;
  if(octave >= 4) octave = 0;
}, 200);
