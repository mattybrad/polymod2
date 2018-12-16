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
    res.send('Synth module created');
  });
};

exports.synthModule_details = function(req, res) {
	SynthModule.findOne({_id:req.params.id}, function(err, synthModule) {
		if(err) return next(err);
		res.send(synthModule);
	})
};

exports.synthModule_update = function(req, res) {
	SynthModule.findOneAndUpdate({_id:req.params.id}, {$set: req.body}, function(err, synthModule) {
		if(err) return next(err);
		res.send('Synth module updated');
	})
};

exports.synthModule_delete = function(req, res) {
	SynthModule.findOneAndDelete({_id:req.params.id}, function(err) {
		if(err) return next(err);
		res.send('Synth module deleted');
	})
};

exports.synthModule_list = function(req, res) {
	SynthModule.find({}, function(err, synthModule) {
		if(err) return next(err);
		res.send(synthModule);
	})
};