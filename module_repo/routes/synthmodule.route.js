const express = require('express');
const router = express.Router();

const synthModule_controller = require('../controllers/synthmodule.controller');

router.get('/synthmodule', synthModule_controller.synthModule);
router.post('/create', synthModule_controller.synthModule_create);

module.exports = router;