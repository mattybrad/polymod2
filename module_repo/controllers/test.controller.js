const Test = require('../models/test.model');

exports.test = function(req, res) {
  res.send('Greetings from the Test controller');
};

exports.test_create = function(req, res) {
  var test = new Test(
    {
      name: req.body.name
    }
  );
  
  test.save(function(err) {
    if(err) {
      return next(err);
    }
    res.send('CREATED');
  });
};