const mongoose = require('mongoose');
const Schema = mongoose.Schema;

var TestSchema = new Schema({
  name: {type: String, required: true, max: 100}
});

module.exports = mongoose.model('Test', TestSchema);