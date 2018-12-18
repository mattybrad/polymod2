const mongoose = require('mongoose');
const Schema = mongoose.Schema;

var SynthModuleVersionSchema = new Schema({
	versionNumber: String,
	headerCode: String,
	mainCode: String,
	sockets: [{
		isOutput: Boolean,
		name: String,
		channel: {
			type: Number,
			required: true,
			unique: true,
			validate: {
				validator: Number.isInteger,
				message: '{VALUE} is not an integer value'
			}
		}
	}],
	controls: [{
		name: String
	}],
	binaryOutputs: [{
		name: String
	}]
});

module.exports = mongoose.model('SynthModuleVersion', SynthModuleVersionSchema);
