const express = require('express');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');

mongoose.connect('mongodb+srv://Mario:Mario@nodeproject.h1acz.mongodb.net/emailTest?retryWrites=true&w=majority', {
    useNewUrlParser: true,
    useUnifiedTopology: true,
}).then( () => { console.log('Connected') }).catch(( err ) => { console.log('Desconnected '+err) });

const app = express();
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(require('./routers/routers'));

app.listen(3232);