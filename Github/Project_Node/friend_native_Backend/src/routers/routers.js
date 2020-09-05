const express = require('express');
const UserController = require('../controller/UserController');
const singinController = require('../controller/singinController');
const forgotPasswordController = require('../controller/forgotPasswordController');
const ResetPasswordController = require('../controller/ResetPasswordController');

const router = express.Router();

router.post('/singup', UserController.store);
router.post('/singin', singinController.store);
router.post('/forgot_password', forgotPasswordController.store);
router.post('/reset_password', ResetPasswordController.store);

module.exports = router;
