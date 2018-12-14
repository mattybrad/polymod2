const express = require('express');
const router = express.Router();

const test_controller = require('../controllers/test.controller');

router.get('/test', test_controller.test);
router.post('/create', test_controller.test_create);

module.exports = router;