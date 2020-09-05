const nodemailer = require('nodemailer');
const hbs = require('nodemailer-express-handlebars');
const path = require('path')

const { host, port, user, pass } = require('../configs/mail.json');


const transport = nodemailer.createTransport({
  host,
  port,
  auth: { user, pass }
});

transport.use('compile', hbs({
  viewEngine:{
    defaultLayout: undefined,
    partialsDir: path.resolve('./src/resources/main'),
  },
  viewPath: path.resolve('./src/resources/mail/'),
  extName: '.html'
}));

module.exports = transport;