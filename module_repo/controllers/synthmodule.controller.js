const SynthModule = require('../models/synthmodule.model');

exports.synthModule = function(req, res) {
  res.send('Greetings from the SynthModule controller');
};

exports.synthModule_create = function(req, res) {
  var synthModule = new SynthModule(
    {
      name: req.body.name,
	  description: req.body.description
    }
  );
  
  synthModule.save(function(err) {
    if(err) {
      return next(err);
    }
    res.send('CREATED');
  });
};