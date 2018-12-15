const express = require('express');
const router = express.Router();

const synthModule_controller = require('../controllers/synthmodule.controller');

router.get('/test', synthModule_controller.test);
router.post('/create', synthModule_controller.synthModule_create);
router.get('/:id', synthModule_controller.synthModule_details);

module.exports = router;