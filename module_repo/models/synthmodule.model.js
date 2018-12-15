const mongoose = require('mongoose');
const Schema = mongoose.Schema;

var SynthModuleSchema = new Schema({
  name: {type: String, required: true, maxlength: 100},
  description: String,
  versions: [{
	headerCode: String,
	mainCode: String,
	versionNumber: String
  }]
});

module.exports = mongoose.model('SynthModule', SynthModuleSchema);