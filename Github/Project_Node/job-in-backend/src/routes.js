const express = require('express');
const SessionCompanyController = require('./app/Controllers/SessionConpanyController');
const CompanyResetController = require('./app/Controllers/CompanyResetController');

const router = express.Router();

router.post('/userCompanies', SessionCompanyController.store);
router.post('/Companyresets', CompanyResetController.store);

module.exports = router;