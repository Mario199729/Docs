const mongoose = require('mongoose');

mongoose.Promise = global.Promise;
mongoose.connect('mongodb+srv://Mario:Mario@nodeproject.h1acz.mongodb.net/serie_node?retryWrites=true&w=majority', {
    useNewUrlParser: true,
    useUnifiedTopology: true  
}).then(() => { console.log('Conneted') }).catch(( err ) => { console.log('Desconnected') })


module.exports = mongoose;