const express = require('express');
const router = express.Router();

const test_controller = require('../controllers/test.controller');

router.get('/test', test_controller.test);

module.exports = router;