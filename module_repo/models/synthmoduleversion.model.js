const mongoose = require('mongoose');
const Schema = mongoose.Schema;

var SynthModuleVersionSchema = new Schema({
	versionNumber: String,
	headerCode: String,
	mainCode: String,
	sockets: [{
		isOutput: Boolean,
		name: String,
		channel: Number
	}],
	controls: [{
		name: String
	}],
	binaryOutputs: [{
		name: String
	}]
});

module.exports = mongoose.model('SynthModuleVersion', SynthModuleVersionSchema);
