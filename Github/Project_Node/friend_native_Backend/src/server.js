const express = require('express')
const mongoose = require('mongoose')
const bodyParser = require('body-parser')
const routers = require('./routers/routers') 


const app = express();

mongoose.connect('mongodb+srv://cloneinsta:cloneinsta@mario-alfredo-jorge.xvi1u.mongodb.net/job-in?retryWrites=true&w=majority', {
    useNewUrlParser: true,
    useUnifiedTopology: true,
}).then(() => {console.log('Connected')}).catch((err) => {console.log('Desconnected: ' +err)});

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true}));

app.use(routers);

app.listen(3232);