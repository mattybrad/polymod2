const mongoose = require('mongoose');
const Schema = mongoose.Schema;
//const SynthModuleVersionSchema = require('./synthmoduleversion.model.js');

var SynthModuleSchema = new Schema({
  name: {type: String, required: true, maxlength: 100},
  description: {type: String, maxlength: 100000}
});

module.exports = mongoose.model('SynthModule', SynthModuleSchema);