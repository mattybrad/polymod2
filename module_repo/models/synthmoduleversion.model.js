const mongoose = require('mongoose');
const Schema = mongoose.Schema;

var ModuleVersionSchema = new Schema({
  test: {type: String, default: 'hello'}
});

module.exports = mongoose.model('ModuleVersion', ModuleVersionSchema);