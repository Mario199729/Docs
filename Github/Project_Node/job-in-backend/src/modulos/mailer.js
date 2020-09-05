const nodemailer = require('nodemailer');
const hb = require('nodemailer-express-handlebars');
const path = require('path');

const { host, port, secure, user, pass } = require('../Configs/mail.json');
const { extname } = require('path');

const transporter = nodemailer.createTransport({
    host,
    port,
    secure,
    auth: { user, pass },
});

transporter.use('compile', hb({
    viewEngine: {
        defaultLayout: undefined,
        partialsDir: path.resolve('./src/resources/mail/main/')
    },
    viewPath: path.resolve('./src/resources/mail/'),
    extName: '.html',
}));

module.exports = transporter;