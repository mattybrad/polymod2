const express = require('express');
const router = express.Router();

const synthModule_controller = require('../controllers/synthmodule.controller');

router.get('/test', synthModule_controller.test);
router.get('/', synthModule_controller.synthModule_list);
router.post('/', synthModule_controller.synthModule_create);
router.get('/:id', synthModule_controller.synthModule_details);
router.put('/:id', synthModule_controller.synthModule_update);
router.delete('/:id', synthModule_controller.synthModule_delete);

module.exports = router;