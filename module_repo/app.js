const express = require('express');
const bodyParser = require('body-parser');

const test = require('./routes/test.route');
const app = express();

app.use('/tests', test);

var port = 1234;
app.listen(port, () => {
  console.log('server working on port ' + port);
});