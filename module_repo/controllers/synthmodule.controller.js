const SynthModule = require('../models/synthmodule.model');

exports.test = function(req, res) {
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

exports.synthModule_details = function(req, res) {
	SynthModule.findById(req.params.id, function(err, synthModule) {
		if(err) return next(err);
		res.send(synthModule);
	})
};