const SynthModuleVersion = require('../models/synthmoduleversion.model');
const SynthModule = require('../models/synthmodule.model');

exports.synthModuleVersion_create = function(req, res, next) {
  var synthModuleVersion = new SynthModuleVersion(req.body);

  SynthModule.findOneAndUpdate({_id:req.params.id}, {$push: {versions: synthModuleVersion}}, function(err, synthModule) {
    if(err) return next(err);
    res.send('Synth module version created');
  });

};
