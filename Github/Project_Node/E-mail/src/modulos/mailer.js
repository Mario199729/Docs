const nodemailer = require('nodemailer');
const hbs = require('nodemailer-express-handlebars');
const path = require('path')


const { host, port, user, pass , secure} = require('../config/mial.json');

const transporter = nodemailer.createTransport({
    host,
    port,
    secure,
    auth: { user, pass}
});

transporter.use('compile', hbs({
    viewEngine: {
        defaultLayout: undefined,
        partialsDir: path.resolve('./src/resources/main')
    },
    viewPath: path.resolve('./src/resources/mail'),
    extName: '.html'
}));

module.exports = transporter;