const express = require('express');
const mongoose = require('mongoose');
const bodyParser = require('body-parser');

mongoose.connect('mongodb+srv://Mario:Mario@nodeproject.h1acz.mongodb.net/job-in-backend?retryWrites=true&w=majority',{
    useNewUrlParser: true,
    useUnifiedTopology: true
}).then(( ) => { console.log('connected') }).catch(( err ) => { console.log('Note connected') })

const app = express();

app.use(bodyParser.json());

app.use(require('./routes'));

app.listen(3232);