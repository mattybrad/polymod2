const mongoose = require('mongoose');
const Schema = mongoose.Schema;

var SynthModuleVersionSchema = require('./synthmoduleversion.model.js').schema;

var SynthModuleSchema = new Schema({
	name: {type: String, required: true, maxlength: 100},
	description: String,
	versions: [SynthModuleVersionSchema]
});

module.exports = mongoose.model('SynthModule', SynthModuleSchema);
