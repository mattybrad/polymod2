const express = require('express');
const bodyParser = require('body-parser');

const test = require('./routes/test.route');
const synthModule = require('./routes/synthmodule.route');
const app = express();

const mongoose = require('mongoose');
var dev_db_url = process.env.POLYMOD_TEST_URI;
mongoose.connect(dev_db_url, { useNewUrlParser: true });
mongoose.promise = global.Promise;
var db = mongoose.connection;
db.on('error', console.error.bind(console, 'ERROR!'));

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended:false}));
app.use('/tests', test);
app.use('/synthmodules', synthModule);

var port = 1234;
app.listen(port, () => {
  console.log('server working on port ' + port);
});